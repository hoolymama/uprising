
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>


#include "paintingGeom.h"
#include "strokeData.h"
#include "paintingNode.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "brushData.h"


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

MObject painting::aMaxPointToPointDistance;

MObject painting::aApproachDistanceStart;
MObject painting::aApproachDistanceMid;
MObject painting::aApproachDistanceEnd;
MObject painting::aApproachDistance;

MObject painting::aCanvasMatrix;
MObject painting::aBrushes;

MObject painting::aPaintColorR;
MObject painting::aPaintColorG;
MObject painting::aPaintColorB;
MObject painting::aPaintColor;
MObject painting::aPaintOpacity;
MObject painting::aPaintTravel;
MObject painting::aPaintCustomId;
MObject painting::aPaints;

MObject painting::aDisplayApproachTargets;
MObject painting::aDisplayClusterPath;

MObject painting::aDisplayPivots;

MObject painting::aDisplayContactWidth;

MObject painting::aDisplayBrushIds;
MObject painting::aDisplayPaintIds;
MObject painting::aDisplayRepeatIds;

MObject painting::aClusterPathColor;
MObject painting::aStackGap;

MObject painting::aOutput;

MStatus painting::initialize()
{
  MStatus st;
  MString method("painting::initialize");
  
  inheritAttributesFrom("paintingBase");


  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnCompoundAttribute cAttr;
  MFnEnumAttribute eAttr;
  MFnMatrixAttribute mAttr;

  aCanvasMatrix= mAttr.create("canvasMatrix", "cmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  addAttribute(aCanvasMatrix);


  aMaxPointToPointDistance = nAttr.create("maxPointToPointDistance", "mxptp",
                                          MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(3.0f);
  nAttr.setSoftMax(100.00f);
  nAttr.setDefault(25.00f);
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


	aClusterPathColor = nAttr.createColor("clusterPathColor", "cpcol");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(aClusterPathColor);


  aStackGap = nAttr.create("stackGap", "sgap", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1);
  nAttr.setDefault(0);
  addAttribute(aStackGap);


  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aInMatrix, aOutput);
  st = attributeAffects(aCanvasMatrix, aOutput);
  st = attributeAffects(aLinearSpeed, aOutput);
  st = attributeAffects(aAngularSpeed, aOutput);
  st = attributeAffects(aApproximationDistance, aOutput);
  st = attributeAffects(aMaxPointToPointDistance, aOutput);
  st = attributeAffects(aApproachDistance, aOutput);
  st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aPaints, aOutput);
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
  // cerr << "painting::compute ptpThresh: "<< ptpThresh << endl; 
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
    const std::vector<Stroke> *strokeGeom = sData->fGeometry;


    std::vector<Stroke>::const_iterator citer;
    for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++)
    {

      pGeom->addStroke(*citer, index);
    }
  }
  return MS::kSuccess;
}

void painting::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());
  nodeFn.setName("paintingShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}
 