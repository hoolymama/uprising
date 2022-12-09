
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include "strokeData.h"
#include "lightPaintingNode.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "brushData.h"
#include "nodeUtils.h"

MTypeId lightPainting::id(k_lightPainting);
MString lightPainting::drawDbClassification("drawdb/geometry/lightPainting");
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


MObject lightPainting::aDisplayTargetColors;
MObject lightPainting::aViewMatrix;

MObject lightPainting::aColorGain;
MObject lightPainting::aWhiteGain;

MObject lightPainting::aWaitGain;

MObject lightPainting::aOutput;

MStatus lightPainting::initialize()
{
  MStatus st;
  MString method("lightPainting::initialize");

  inheritAttributesFrom("paintingBase");

  MFnTypedAttribute tAttr;
  MFnEnumAttribute eAttr;
  MFnMatrixAttribute mAttr;
  MFnNumericAttribute nAttr;

  aViewMatrix = mAttr.create("viewMatrix", "vmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  addAttribute(aViewMatrix);

  aColorGain = nAttr.create("colorGain", "clg", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0);
  nAttr.setMax(10.0);
  st = addAttribute(aColorGain);

  aWhiteGain = nAttr.create("whiteGain", "whg", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0);
  nAttr.setMax(10.0);
  st = addAttribute(aWhiteGain);

  aWaitGain = nAttr.create("waitGain", "wag", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0);
  st = addAttribute(aWaitGain);

  aOutput = tAttr.create("output", "out", lightPaintingData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  aDisplayTargetColors = eAttr.create("displayTargetColors", "dtcl");
  eAttr.addField("white", PaintingEnums::kTargetColorsWhite);
  eAttr.addField("rgb", PaintingEnums::kTargetColorsRGB);
  eAttr.addField("blend", PaintingEnums::kTargetColorsBlend);
  eAttr.addField("wait", PaintingEnums::kTargetColorsWait);
  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setDefault(PaintingEnums::kTargetColorsRGB);
  addAttribute(aDisplayTargetColors);

  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aReassignParentId, aOutput);

  return (MS::kSuccess);
}


MStatus lightPainting::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  MString method("lightPainting::compute");
  if (plug != aOutput)
  {
    return (MS::kUnknownParameter);
  }

  MMatrix wm = NodeUtils::firstWorldMatrix(thisMObject());

  m_pd->create();

  std::vector<Stroke> *outStrokeGeom = m_pd->strokes();
  addStrokes(data, outStrokeGeom);


  std::map<int, Brush> *outBrushes = m_pd->brushes();
  collectBrushes(data, *outBrushes);



  MFnPluginData fnOut;
  MTypeId kdid(lightPaintingData::id);

  MObject dOut = fnOut.create(kdid, &st);
  msert;
  lightPaintingData *outGeometryData = (lightPaintingData *)fnOut.data(&st);
  msert;
  if (m_pd)
  {
    *outGeometryData = (*m_pd); // assignment
  }

  MDataHandle outputHandle = data.outputValue(aOutput, &st);
  msert;
  st = outputHandle.set(outGeometryData);
  msert;
  data.setClean(plug);

  return MS::kSuccess;
}

MStatus lightPainting::addStrokes(
    MDataBlock &data,
    std::vector<Stroke> *outStrokeGeom)
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
      outStrokeGeom->push_back(*citer);
      if (index > -1)
      {
        outStrokeGeom->back().setParentId(index);
      }
    }
  }
  return MS::kSuccess;
}

void lightPainting::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());
  nodeFn.setName("lightPaintingShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}
