
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include "paintingGeom.h"
#include "paint.h"

#include "strokeData.h"
#include "paintingNode.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "paletteData.h"
#include "nodeUtils.h"

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

MObject painting::aPalette;

MObject painting::aDisplayApproachTargets;
MObject painting::aDisplayClusterPath;

MObject painting::aDisplayPivots;

MObject painting::aDisplayContactWidth;

MObject painting::aDisplayPaintIds;
MObject painting::aDisplayRepeatIds;
MObject painting::aDisplayBrushModelIds;

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

  aCanvasMatrix = mAttr.create("canvasMatrix", "cmat", MFnMatrixAttribute::kFloat);
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

  aPalette = tAttr.create("palette", "plt", paletteData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aPalette);

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
    nAttr.setKeyable(true);
  addAttribute(aDisplayApproachTargets);

  aDisplayContactWidth = nAttr.create("displayContactWidth", "dcwd",
                                      MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  addAttribute(aDisplayContactWidth);

  aDisplayClusterPath = nAttr.create("displayClusterPath", "dcpt",
                                     MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  addAttribute(aDisplayClusterPath);

  aDisplayPivots = nAttr.create("displayPivots", "dsp",
                                MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  addAttribute(aDisplayPivots);


  aDisplayPaintIds = nAttr.create("displayPaintIds", "dptid",
                                  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  addAttribute(aDisplayPaintIds);

  aDisplayRepeatIds = nAttr.create("displayRepeatIds", "drpid",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  addAttribute(aDisplayRepeatIds);

  aDisplayBrushModelIds = nAttr.create("displayBrushModelIds", "dbmid",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  addAttribute(aDisplayBrushModelIds);


  aClusterPathColor = nAttr.createColor("clusterPathColor", "cpcol");
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setKeyable(true);
  addAttribute(aClusterPathColor);

  aStackGap = nAttr.create("stackGap", "sgap", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1);
  nAttr.setDefault(0);
  nAttr.setKeyable(true);
  addAttribute(aStackGap);

  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aBrushShop, aOutput);
  
  
  st = attributeAffects(aCanvasMatrix, aOutput);

  st = attributeAffects(aMaxPointToPointDistance, aOutput);
  st = attributeAffects(aApproachDistance, aOutput);
  st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aPalette, aOutput);
  st = attributeAffects(aReassignParentId, aOutput);

  return (MS::kSuccess);
}

MStatus painting::getPalette(MDataBlock &data, std::map<int, Paint> &palette) const
{
  MStatus st;
  MDataHandle h = data.inputValue(aPalette, &st);
  msert;
  MObject d = h.data();
  MFnPluginData fnP(d, &st);
  if (st.error())
  {
    return st;
  }
  paletteData *pData = (paletteData *)fnP.data(&st);
  msert;
  palette = *(pData->fGeometry);
  if (palette.size() == 0)
  {
    return MS::kFailure;
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

  MMatrix wm = NodeUtils::firstWorldMatrix(thisMObject());

  float ptpThresh = data.inputValue(aMaxPointToPointDistance).asFloat();

  if (ptpThresh < 3.0)
  {
    ptpThresh = 3.0;
  }

  // paintingBase::collectBrushes(data, brushes);

  BrushShop brushShop;
  st = getBrushShop(data, brushShop); msert;
  std::map<int, Brush> brushes;
  brushShop.getBrushes(brushes);

  std::map<int, Paint> palette;
  st = getPalette(data, palette);
  if (st.error())
  {
    palette[-1] = Paint();
  }

  m_pd->create();
  paintingGeom *pGeom = m_pd->geometry();

  pGeom->setBrushes(brushes);
  pGeom->setPaints(palette);

  addStrokes(data, pGeom);

  if (pGeom->clusters().size())
  {
    MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();
    MFloatVector canvasNormal((MFloatVector::zAxis * canvasMatrix).normal());

    MDataHandle hApproachDistance = data.inputValue(aApproachDistance);
    MFloatVector approachStart = canvasNormal * hApproachDistance.child(aApproachDistanceStart).asFloat();
    MFloatVector approachMid = canvasNormal * hApproachDistance.child(aApproachDistanceMid).asFloat();
    MFloatVector approachEnd = canvasNormal * hApproachDistance.child(aApproachDistanceEnd).asFloat();

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
