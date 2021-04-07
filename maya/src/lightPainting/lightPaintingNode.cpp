
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include "strokeData.h"
#include "lightPaintingNode.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "brushData.h"

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
 
MObject lightPainting::aBrush;
MObject lightPainting::aDisplayTargetColors;
MObject lightPainting::aOutput;

MStatus lightPainting::initialize()
{
  MStatus st;
  MString method("lightPainting::initialize");

  inheritAttributesFrom("paintingBase");

  MFnTypedAttribute tAttr;
  MFnEnumAttribute eAttr;

  aBrush = tAttr.create("brush", "bsh", brushData::id);
  tAttr.setHidden(false);
  tAttr.setStorable(false);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aBrush);

  aOutput = tAttr.create("output", "out", lightPaintingData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  aDisplayTargetColors = eAttr.create("displayTargetColors", "dtcl");
  eAttr.addField("white", PaintingEnums::kTargetColorsWhite);
  eAttr.addField("rgb", PaintingEnums::kTargetColorsRGB);
  eAttr.addField("blend", PaintingEnums::kTargetColorsBlend);
  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setDefault(PaintingEnums::kTargetColorsRGB);
  addAttribute(aDisplayTargetColors);

  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aInMatrix, aOutput);
  st = attributeAffects(aViewMatrix, aOutput);

  st = attributeAffects(aBrush, aOutput);
  st = attributeAffects(aLinearSpeed, aOutput);
  st = attributeAffects(aAngularSpeed, aOutput);
  st = attributeAffects(aApproximationDistance, aOutput);

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

  MDataHandle mh = data.inputValue(aInMatrix, &st);
  mser;
  MMatrix wm = mh.asMatrix();

  m_pd->create();

  MDataHandle hBrush = data.inputValue(aBrush, &st); msert;


  MObject dBrush = hBrush.data();
  MFnPluginData fnBrush(dBrush, &st); msert;


  brushData *bData = (brushData *)fnBrush.data();

  Brush *outBrush = m_pd->brush();

  *outBrush = *(bData->fGeometry);
   

  std::vector<Stroke> *outStrokeGeom = m_pd->strokes();

  addStrokes(data, outStrokeGeom);

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
