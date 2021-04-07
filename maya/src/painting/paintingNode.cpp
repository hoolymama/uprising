#include <maya/MIOStream.h>

#include <math.h>
#include <algorithm>
#include <map>
#include <utility>
#include <tuple>

#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>
#include <maya/MFnPluginData.h>
#include <maya/MRenderUtil.h>
#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MAngle.h>
#include <maya/MPlugArray.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include "paintingGeom.h"
#include "strokeData.h"
#include "paintingNode.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

#include "brushData.h"
#include "enums.h"

MTypeId painting::id(k_painting);
MString painting::drawDbClassification("drawdb/geometry/painting");
MString painting::drawRegistrantId("PaintingPlugin");

painting::painting()
{
  m_pd = new paintingData;
}

painting::~painting()
{
  if (m_pd)
  {
    delete m_pd;
    m_pd = 0;
  }
}

void *painting::creator()
{
  return new painting();
}

MObject painting::aInMatrix;

MObject painting::aViewMatrix;
MObject painting::aStrokes;

// MObject painting::aDisplacementMesh;

MObject painting::aLinearSpeed;           // cm/sec
MObject painting::aAngularSpeed;          // per sec
MObject painting::aApproximationDistance; // cm
MObject painting::aMaxPointToPointDistance;

MObject painting::aApproachDistanceStart;
MObject painting::aApproachDistanceMid;
MObject painting::aApproachDistanceEnd;
MObject painting::aApproachDistance;

// MObject painting::aApplyBiases;
// MObject painting::aBiasMult;

MObject painting::aBrushes;

MObject painting::aPaintColorR;
MObject painting::aPaintColorG;
MObject painting::aPaintColorB;
MObject painting::aPaintColor;
MObject painting::aPaintOpacity;
MObject painting::aPaintTravel;
MObject painting::aPaintCustomId;
MObject painting::aPaints;

MObject painting::aReassignParentId;

MObject painting::aPointSize;
MObject painting::aLineLength;
MObject painting::aLineThickness;

MObject painting::aDisplayTargets;
MObject painting::aDisplayTargetColors;
MObject painting::aDisplayApproachTargets;
MObject painting::aDisplayClusterPath;

MObject painting::aDisplayPivots;

MObject painting::aDisplayContactWidth;

MObject painting::aDisplayIds;
MObject painting::aDisplayParentIds;
MObject painting::aDisplayLayerIds;
MObject painting::aDisplayBrushIds;
MObject painting::aDisplayPaintIds;
MObject painting::aDisplayRepeatIds;

MObject painting::aIdDisplayOffset;

MObject painting::aArrowheadSize;

MObject painting::aStackGap;
MObject painting::aDrawParam;

MObject painting::aOutput;

MStatus painting::initialize()
{
  MStatus st;
  MString method("painting::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
  MFnCompoundAttribute cAttr;
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;
  MFnMessageAttribute msgAttr;

  MMatrix identity;
  identity.setToIdentity();

  aInMatrix = mAttr.create("inMatrix", "imat", MFnMatrixAttribute::kDouble);
  mAttr.setStorable(false);
  mAttr.setHidden(true);
  mAttr.setDefault(identity);
  addAttribute(aInMatrix);

  aViewMatrix= mAttr.create("viewMatrix", "vmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  addAttribute(aViewMatrix);
  // aDisplacementMesh = tAttr.create("displacementMesh", "dmsh", MFnData::kMesh, &st);
  // mser
  //     tAttr.setReadable(false);
  // tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  // st = addAttribute(aDisplacementMesh);
  // mser;

  // aLinearSpeed = nAttr.create("linearSpeed", "lnsp", MFnNumericData::kDouble);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setMin(0.00);
  // nAttr.setSoftMax(200);
  // nAttr.setDefault(100.00);
  // nAttr.setKeyable(true);
  // addAttribute(aLinearSpeed);

  // const double minAngSpeed = mayaMath::single_pi / 900.00;
  // const double defaultAngSpeed = mayaMath::single_pi / 9.00;
  // aAngularSpeed = uAttr.create("angularSpeed", "agsp", MFnUnitAttribute::kAngle);
  // uAttr.setStorable(true);
  // uAttr.setReadable(true);
  // uAttr.setMin(minAngSpeed);
  // uAttr.setMax((mayaMath::single_pi));
  // addAttribute(aAngularSpeed);

  // aApproximationDistance = nAttr.create("approximationDistance", "apxd",
  //                                       MFnNumericData::kDouble);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setMin(0.00);
  // nAttr.setSoftMax(20);
  // nAttr.setDefault(5.0);
  // nAttr.setKeyable(true);
  // addAttribute(aApproximationDistance);

  aMaxPointToPointDistance = nAttr.create("maxPointToPointDistance", "mxptp",
                                          MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(3.0);
  nAttr.setSoftMax(100.00);
  nAttr.setDefault(25.00);
  nAttr.setKeyable(true);
  addAttribute(aMaxPointToPointDistance);

  aApproachDistanceStart = nAttr.create("approachDistanceStart",
                                        "apds", MFnNumericData::kFloat);
  nAttr.setDefault(5.0);
  aApproachDistanceMid = nAttr.create("approachDistanceMid",
                                      "apdm", MFnNumericData::kFloat);
  nAttr.setDefault(3.0);
  aApproachDistanceEnd = nAttr.create("approachDistanceEnd",
                                      "apde", MFnNumericData::kFloat);
  nAttr.setDefault(5.0);
  aApproachDistance = nAttr.create("approachDistance",
                                   "apd", aApproachDistanceStart, aApproachDistanceMid, aApproachDistanceEnd);

  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  addAttribute(aApproachDistance);

  aStrokes = tAttr.create("strokes", "stks", strokeData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setIndexMatters(true);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aStrokes);

  aBrushes = tAttr.create("brushes", "bsh", brushData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setKeyable(true);
  tAttr.setIndexMatters(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aBrushes);

  aPaintColorR = nAttr.create("paintColorR", "pcr", MFnNumericData::kFloat);
  aPaintColorG = nAttr.create("paintColorG", "pcg", MFnNumericData::kFloat);
  aPaintColorB = nAttr.create("paintColorB", "pcb", MFnNumericData::kFloat);
  aPaintColor = nAttr.create("paintColor", "pc", aPaintColorR, aPaintColorG,
                             aPaintColorB);
  nAttr.setStorable(true);
  nAttr.setHidden(false);
  nAttr.setWritable(true);
  nAttr.setUsedAsColor(true);
  addAttribute(aPaintColor);

  aPaintOpacity = nAttr.create("paintOpacity", "pvis", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.5);

  aPaintTravel = nAttr.create("paintTravel", "ptvl", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aPaintTravel);

  aPaintCustomId = nAttr.create("paintCustomId", "pcid", MFnNumericData::kShort);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(-1);

  aPaints = cAttr.create("paints", "pts");
  cAttr.addChild(aPaintColor);
  cAttr.addChild(aPaintOpacity);
  cAttr.addChild(aPaintTravel);
  cAttr.addChild(aPaintCustomId);
  cAttr.setArray(true);
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute(aPaints);
  mser;

  aOutput = tAttr.create("output", "out", paintingData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  aReassignParentId = nAttr.create("reassignParentId", "rpi",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(false);
  addAttribute(aReassignParentId);

  aPointSize = nAttr.create("pointSize", "psi", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  addAttribute(aPointSize);

  aLineLength = nAttr.create("lineLength", "lln", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  addAttribute(aLineLength);

  aLineThickness = nAttr.create("lineThickness", "ltk", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  addAttribute(aLineThickness);

  aArrowheadSize = nAttr.create("arrowheadSize", "arsz", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  addAttribute(aArrowheadSize);

  aDisplayTargets = eAttr.create("displayTargets", "dtg");
  eAttr.addField("none", PaintingEnums::kTargetsNone);
  eAttr.addField("point", PaintingEnums::kTargetsPoint);
  eAttr.addField("line", PaintingEnums::kTargetsLine);
  eAttr.addField("matrix", PaintingEnums::kTargetsMatrix);
  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setDefault(PaintingEnums::kTargetsNone);
  addAttribute(aDisplayTargets);

  aDisplayTargetColors = eAttr.create("displayTargetColors", "dtcl");
  eAttr.addField("off", PaintingEnums::kTargetColorsOff);
  eAttr.addField("white", PaintingEnums::kTargetColorsWhite);
  eAttr.addField("rgb", PaintingEnums::kTargetColorsRGB);
  eAttr.addField("blend", PaintingEnums::kTargetColorsBlend);
  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setDefault(PaintingEnums::kTargetColorsOff);
  addAttribute(aDisplayTargetColors);

  aDisplayApproachTargets = nAttr.create("displayApproachTargets", "dapt",
                                         MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayApproachTargets);

  aDisplayContactWidth = nAttr.create("displayContactWidth", "dcwd",
                                      MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayContactWidth);

  aDisplayClusterPath = nAttr.create("displayClusterPath", "dcpt",
                                     MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayClusterPath);

  aDisplayPivots = nAttr.create("displayPivots", "dsp",
                                MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayPivots);

  aDisplayIds = nAttr.create("displayIds", "did",
                             MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayIds);

  aDisplayParentIds = nAttr.create("displayParentIds", "dprid",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayParentIds);

  aDisplayLayerIds = nAttr.create("displayLayerIds", "dlyid",
                                  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayLayerIds);

  aDisplayBrushIds = nAttr.create("displayBrushIds", "dbid",
                                  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayBrushIds);

  aDisplayPaintIds = nAttr.create("displayPaintIds", "dptid",
                                  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayPaintIds);

  aDisplayRepeatIds = nAttr.create("displayRepeatIds", "drpid",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayRepeatIds);

  aIdDisplayOffset = nAttr.create("idDisplayOffset", "iddo", MFnNumericData::k3Float);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.0f, 0.0f, 1.0f);
  addAttribute(aIdDisplayOffset);

  aStackGap = nAttr.create("stackGap", "sgap", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1);
  nAttr.setDefault(0);
  addAttribute(aStackGap);

  aDrawParam = nAttr.create("drawParam", "dprm", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.0f);
  nAttr.setSoftMax(1.0f);
  nAttr.setDefault(1.0f);
  addAttribute(aDrawParam);


  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aInMatrix, aOutput);
  st = attributeAffects(aViewMatrix, aOutput);


  st = attributeAffects(aLinearSpeed, aOutput);
  st = attributeAffects(aAngularSpeed, aOutput);
  st = attributeAffects(aApproximationDistance, aOutput);
  st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aPaints, aOutput);
  st = attributeAffects(aMaxPointToPointDistance, aOutput);
  st = attributeAffects(aApproachDistance, aOutput);

  // st = attributeAffects(aDisplacementMesh, aOutput);
  // st = attributeAffects(aApplyBiases, aOutput);
  // st = attributeAffects(aBiasMult, aOutput);

  st = attributeAffects(aReassignParentId, aOutput);

  return (MS::kSuccess);
}

MStatus painting::collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes)
{
  MStatus st;
  MArrayDataHandle ha = data.inputArrayValue(aBrushes, &st);
  msert;

  brushes[-1] = Brush();

  unsigned nPlugs = ha.elementCount();
  for (unsigned i = 0; i < nPlugs; i++, ha.next())
  {
    int index = ha.elementIndex(&st);
    if (st.error())
    {
      continue;
    }
    MDataHandle h = ha.inputValue(&st);
    if (st.error())
    {
      continue;
    }

    MObject d = h.data();
    MFnPluginData fnP(d, &st);
    if (st.error())
    {
      continue;
    }
    brushData *bData = (brushData *)fnP.data();

    brushes[index] = *(bData->fGeometry);
  }
  return MS::kSuccess;
}

MStatus painting::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  MString method("painting::compute");
  if (plug != aOutput)
  {
    return (MS::kUnknownParameter);
  }

  MDataHandle mh = data.inputValue(aInMatrix, &st);
  mser;
  MMatrix wm = mh.asMatrix();

  float ptpThresh = data.inputValue(aMaxPointToPointDistance).asFloat();
  if (ptpThresh < 3.0)
  {
    ptpThresh = 3.0;
  }

  std::map<int, Brush> brushes;
  collectBrushes(data, brushes);

  MArrayDataHandle hPaints = data.inputArrayValue(aPaints, &st);
  msert;
  std::map<int, Paint> paints = Paint::factory(
      hPaints,
      painting::aPaintColor,
      painting::aPaintOpacity,
      painting::aPaintTravel);

  m_pd->create();
  paintingGeom *pGeom = m_pd->geometry();

  pGeom->setBrushes(brushes);
  pGeom->setPaints(paints);

  // std::vector<Stroke> strokes;

  addStrokes(data, pGeom);

  if (pGeom->clusters().size())
  {

    MDataHandle hApproachDistance = data.inputValue(aApproachDistance);
    float approachStart = hApproachDistance.child(aApproachDistanceStart).asFloat();
    float approachMid = hApproachDistance.child(aApproachDistanceMid).asFloat();
    float approachEnd = hApproachDistance.child(aApproachDistanceEnd).asFloat();
    pGeom->setApproaches(approachStart, approachMid, approachEnd, ptpThresh);

    //   MObject dMesh = data.inputValue(aDisplacementMesh).asMeshTransformed();
    //   MFnMesh meshFn(dMesh, &st);
    //   if (!st.error())
    //   {
    //     MMeshIsectAccelParams ap = meshFn.autoUniformGridParams();
    //     pGeom->displace(meshFn, ap);
    //   }

    //   // lift up the start and end of each stroke according to
    //   // brush tip
  }

  MFnPluginData fnOut;
  MTypeId kdid(paintingData::id);
  MObject dOut = fnOut.create(kdid, &st);
  mser;
  paintingData *outGeometryData = (paintingData *)fnOut.data(&st);
  mser;
  if (m_pd)
  {
    *outGeometryData = (*m_pd);
  }

  MDataHandle outputHandle = data.outputValue(aOutput, &st);
  mser;
  st = outputHandle.set(outGeometryData);
  mser;
  data.setClean(plug);

  return MS::kSuccess;
}

MStatus painting::addStrokes(MDataBlock &data, paintingGeom *pGeom)
{
  MStatus st;
  MArrayDataHandle hStrokes = data.inputValue(aStrokes, &st);
  msert;
  unsigned nInputs = hStrokes.elementCount();
  // int gid = 0;
  for (unsigned i = 0; i < nInputs; i++, hStrokes.next())
  {

    bool reassignParentId = data.inputValue(aReassignParentId).asBool();
    int index = reassignParentId ? hStrokes.elementIndex() : -1;
    MDataHandle hStrokeInput = hStrokes.inputValue(&st);
    if (st.error())
    {
      continue;
    }
    MObject dStrokeInput = hStrokeInput.data();
    MFnPluginData fnStrokeInput(dStrokeInput, &st);
    if (st.error())
    {
      continue;
    }
    strokeData *sData = (strokeData *)fnStrokeInput.data();
    // strokeCurveGeom *strokeGeom = strokeData->fGeometry;
    const std::vector<Stroke> *strokeGeom = sData->fGeometry;

    // const std::vector<Stroke> &strokes = strokeGeom->strokes();
    std::vector<Stroke>::const_iterator citer;
    for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++)
    {

      pGeom->addStroke(*citer, index);
    }
  }
  return MS::kSuccess;
}

void painting::draw(M3dView &view,
                    const MDagPath &path,
                    M3dView::DisplayStyle style,
                    M3dView::DisplayStatus status)
{
  return;
}

bool painting::isBounded() const
{
  return false;
}

MBoundingBox painting::boundingBox() const
{
  return MBoundingBox();
}

void painting::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());
  nodeFn.setName("paintingShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}
