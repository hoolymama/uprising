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

// #include "paintingGeom.h"
#include "strokeData.h"
#include "lightPaintingNode.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

#include "brushData.h"
#include "enums.h"


const double rad_to_deg = (180 / 3.1415927);

const int LEAD_COLOR = 18;
const int ACTIVE_COLOR = 15;
const int ACTIVE_AFFECTED_COLOR = 8;
const int DORMANT_COLOR = 4;
const int HILITE_COLOR = 17;
const int RED_COLOR = 12;

int clamp(int n, int lower, int upper)
{
  return std::max(lower, std::min(n, upper));
}

MTypeId lightPainting::id(k_lightPainting);
MString lightPainting::drawDbClassification("drawdb/geometry/painting");
MString lightPainting::drawRegistrantId("LightPaintingPlugin");

lightPainting::lightPainting()
{
  m_pd = new lightPaintingData;
}

lightPainting::~lightPainting()
{
  if (m_pd)
  {
    delete m_pd;
    m_pd = 0;
  }
}

void *lightPainting::creator()
{
  return new lightPainting();
}

const double epsilon = 0.0001;

MObject lightPainting::aInMatrix;

MObject lightPainting::aStrokes;

MObject lightPainting::aLinearSpeed;           // cm/sec
MObject lightPainting::aAngularSpeed;          // per sec
MObject lightPainting::aApproximationDistance; // cm
MObject lightPainting::aBrush;

MObject lightPainting::aReassignParentId;


MObject lightPainting::aPointSize;
MObject lightPainting::aLineLength;
MObject lightPainting::aLineThickness;

MObject lightPainting::aDisplayTargets;
MObject lightPainting::aDisplayTargetColors;

MObject lightPainting::aDisplayIds;
MObject lightPainting::aDisplayParentIds;
MObject lightPainting::aDisplayLayerIds;

MObject lightPainting::aIdDisplayOffset;
MObject lightPainting::aArrowheadSize;

MObject lightPainting::aDrawParam;

MObject lightPainting::aOutput;

MStatus lightPainting::initialize()
{
  MStatus st;
  MString method("lightPainting::initialize");

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

  aLinearSpeed = nAttr.create("linearSpeed", "lnsp", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(200.0f);
  nAttr.setDefault(100.0f);
  nAttr.setKeyable(true);
  addAttribute(aLinearSpeed);

  const float minAngSpeed = mayaMath::single_pi / 900.00;
  const float defaultAngSpeed = mayaMath::single_pi / 9.00;

  aAngularSpeed = uAttr.create("angularSpeed", "agsp", MFnUnitAttribute::kAngle);
  uAttr.setStorable(true);
  uAttr.setReadable(true);
  uAttr.setMin(minAngSpeed);
  uAttr.setMax((mayaMath::single_pi));
  addAttribute(aAngularSpeed);

  aApproximationDistance = nAttr.create("approximationDistance", "apxd",
                                        MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  nAttr.setKeyable(true);
  addAttribute(aApproximationDistance);
 
 
  aStrokes = tAttr.create("strokes", "stks", strokeData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setIndexMatters(true);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aStrokes);

  aBrush = tAttr.create("brush", "bsh", brushData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aBrush);

  aOutput = tAttr.create("output", "out", lightPaintingData::id);
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


  // LOCATOR DISPLAY
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


  // aDisplayApproachTargets = nAttr.create("displayApproachTargets", "dapt",
  //                                        MFnNumericData::kBoolean);
  // nAttr.setHidden(false);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setDefault(true);
  // addAttribute(aDisplayApproachTargets);

  // aDisplayContactWidth = nAttr.create("displayContactWidth", "dcwd",
  //                                     MFnNumericData::kBoolean);
  // nAttr.setHidden(false);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setDefault(true);
  // addAttribute(aDisplayContactWidth);

  // aDisplayClusterPath = nAttr.create("displayClusterPath", "dcpt",
  //                                    MFnNumericData::kBoolean);
  // nAttr.setHidden(false);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setDefault(true);
  // addAttribute(aDisplayClusterPath);

  // aDisplayPivots = nAttr.create("displayPivots", "dsp",
  //                               MFnNumericData::kBoolean);
  // nAttr.setHidden(false);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setDefault(true);
  // addAttribute(aDisplayPivots);

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

  // aDisplayBrushIds = nAttr.create("displayBrushIds", "dbid",
  //                                 MFnNumericData::kBoolean);
  // nAttr.setHidden(false);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setDefault(true);
  // addAttribute(aDisplayBrushIds);

  // aDisplayPaintIds = nAttr.create("displayPaintIds", "dptid",
  //                                 MFnNumericData::kBoolean);
  // nAttr.setHidden(false);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setDefault(true);
  // addAttribute(aDisplayPaintIds);

  // aDisplayRepeatIds = nAttr.create("displayRepeatIds", "drpid",
  //                                  MFnNumericData::kBoolean);
  // nAttr.setHidden(false);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setDefault(true);
  // addAttribute(aDisplayRepeatIds);

  aIdDisplayOffset = nAttr.create("idDisplayOffset", "iddo", MFnNumericData::k3Float);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.0f, 0.0f, 1.0f);
  addAttribute(aIdDisplayOffset);

  // aStackGap = nAttr.create("stackGap", "sgap", MFnNumericData::kDouble);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setMin(0.00);
  // nAttr.setSoftMax(1);
  // nAttr.setDefault(0);
  // addAttribute(aStackGap);

  aDrawParam = nAttr.create("drawParam", "dprm", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.0f);
  nAttr.setSoftMax(1.0f);
  nAttr.setDefault(1.0f);
  addAttribute(aDrawParam);

  
  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aLinearSpeed, aOutput);
  st = attributeAffects(aAngularSpeed, aOutput);
  st = attributeAffects(aApproximationDistance, aOutput);
  // st = attributeAffects(aBrushes, aOutput);
  // st = attributeAffects(aPaints, aOutput);
  // st = attributeAffects(aDisplacementMesh, aOutput);
  // st = attributeAffects(aMaxPointToPointDistance, aOutput);
  // st = attributeAffects(aApproachDistance, aOutput);
  // st = attributeAffects(aApplyBiases, aOutput);
  // st = attributeAffects(aBiasMult, aOutput);

  st = attributeAffects(aReassignParentId, aOutput);

  return (MS::kSuccess);
}

// MStatus lightPainting::collectBrushes(
//   MDataBlock &data, std::map<int, Brush> &brushes)
// {
//   MStatus st;
//   MArrayDataHandle ha = data.inputArrayValue(aBrushes, &st);
//   msert;

//   brushes[-1] = Brush();

//   unsigned nPlugs = ha.elementCount();
//   for (unsigned i = 0; i < nPlugs; i++, ha.next())
//   {
//     int index = ha.elementIndex(&st);
//     if (st.error())
//     {
//       continue;
//     }
//     MDataHandle h = ha.inputValue(&st);
//     if (st.error())
//     {
//       continue;
//     }

//     MObject d = h.data();
//     MFnPluginData fnP(d, &st);
//     if (st.error())
//     {
//       continue;
//     }
//     brushData *bData = (brushData *)fnP.data();

//     brushes[index] = *(bData->fGeometry);
//   }
//   return MS::kSuccess;
// }

MStatus lightPainting::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  MString method("lightPainting::compute");
  if (plug != aOutput)
  {
    return (MS::kUnknownParameter);
  }

  MDataHandle mh = data.inputValue(aInMatrix, &st);
  mser;
  MMatrix wm = mh.asMatrix();


  m_pd->create();

  MDataHandle hBrush = data.inputValue(aBrush, &st);
  MObject dBrush = hBrush.data();
  MFnPluginData fnBrush(dBrush, &st);mser;
  brushData *bData = (brushData *)fnBrush.data();
  Brush *outBrush = m_pd->brush();

  *outBrush= *(bData->fGeometry);
 
  std::vector<Stroke> *outStrokeGeom = m_pd->geometry();


  addStrokes(data, outStrokeGeom);



  MFnPluginData fnOut;
  MTypeId kdid(lightPaintingData::id);
  MObject dOut = fnOut.create(kdid, &st);
  mser;
  lightPaintingData *outGeometryData = (lightPaintingData *)fnOut.data(&st);
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

MStatus lightPainting::addStrokes(
  MDataBlock &data, 
  std::vector<Stroke> *outStrokeGeom 
) {
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

    std::vector<Stroke>::const_iterator citer;
    for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++)
    {
      outStrokeGeom->push_back(*citer);
      if (index > -1) {
        outStrokeGeom->back().setParentId(index);
      }
    }
  }
  return MS::kSuccess;
}

void lightPainting::draw(M3dView &view,
                    const MDagPath &path,
                    M3dView::DisplayStyle style,
                    M3dView::DisplayStatus status)
{
  return;
}

bool lightPainting::isBounded() const
{
  return false;
}

MBoundingBox lightPainting::boundingBox() const
{
  return MBoundingBox();
}

void lightPainting::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());
  nodeFn.setName("lightPaintingShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}
