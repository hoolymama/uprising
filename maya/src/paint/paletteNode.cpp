
#include <maya/MIOStream.h>
#include <map>
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <jMayaIds.h>
// #include "mayaMath.h"
#include "errorMacros.h"

#include "paletteData.h"
#include "paletteNode.h"
// #include "colorUtils.h"

MTypeId paletteNode::id(k_paletteNode);
MString paletteNode::drawDbClassification("drawdb/geometry/paletteNode");
MString paletteNode::drawRegistrantId("paletteNodePlugin");

paletteNode::paletteNode()
{
}

paletteNode::~paletteNode()
{
}

void *paletteNode::creator()
{
  return new paletteNode();
}

const double epsilon = 0.0001;

MObject paletteNode::aColor;
MObject paletteNode::aOpacity;
MObject paletteNode::aTravel;
MObject paletteNode::aName;
MObject paletteNode::aPotId;
 
MObject paletteNode::aInput;


// MObject paletteNode::aHueRotate;
// MObject paletteNode::aSaturationRangeRemap;
// MObject paletteNode::aValueRangeRemap;
  


MObject paletteNode::aWidth;
MObject paletteNode::aHeight;
MObject paletteNode::aXPos;
MObject paletteNode::aYPos;
MObject paletteNode::aDisplayId;
MObject paletteNode::aDisplayName;
MObject paletteNode::aDisplayRGB;
MObject paletteNode::aDisplayPotId;

MObject paletteNode::aOutput;
  

MStatus paletteNode::initialize()
{
  MStatus st;
  MString method("paletteNode::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnCompoundAttribute cAttr;

  aColor = nAttr.createColor("color", "col");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0f, 0.5f, 0.0f);
  nAttr.setConnectable(true);
  addAttribute(aColor);

  aOpacity = nAttr.create("opacity", "opa", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0f);
  addAttribute(aOpacity);

  aTravel = nAttr.create("travel", "tvl", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0f);
  addAttribute(aTravel);
  
  aName = tAttr.create("name", "nm", MFnData::kString);


  aPotId =  nAttr.create("potId", "pid", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
 
  aInput = cAttr.create("input", "in");
  cAttr.addChild(aColor);
  cAttr.addChild(aOpacity);
  cAttr.addChild(aTravel);
  cAttr.addChild(aName);
  cAttr.addChild(aPotId);
  cAttr.setArray(true);
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute(aInput);

  aWidth = nAttr.create("Width", "wid", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.05);
  addAttribute(aWidth);

  aHeight = nAttr.create("height", "hgt", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.05);
  addAttribute(aHeight);

  aXPos = nAttr.create("xpos", "xp", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.01);
  addAttribute(aXPos);

  aYPos = nAttr.create("ypos", "yp", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.01);
  addAttribute(aYPos);

  aDisplayId= nAttr.create("displayId", "did",
                                  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayId);

  aDisplayName= nAttr.create("displayName", "dnm",
                                  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayName);

  aDisplayRGB= nAttr.create("displayRGB", "drgb",
                                  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayRGB);


  aDisplayPotId = nAttr.create("displayPotId", "dpid",
                                  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayPotId);

  aOutput = tAttr.create("output", "owb", paletteData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);
 

  attributeAffects(aPotId, aOutput);
 
  attributeAffects(aColor,aOutput);
  attributeAffects(aName,aOutput);
  attributeAffects(aOpacity,aOutput);
  attributeAffects(aTravel,aOutput);
  attributeAffects(aInput,aOutput);

  return (MS::kSuccess);
}


MStatus paletteNode::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;

  if (plug != aOutput)
  {
    return (MS::kUnknownParameter);
  }

 
  MArrayDataHandle hInput = data.inputArrayValue(aInput, &st);
  
  /////////////
  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(paletteData::id);
  MObject dOut = fnOut.create(kdid, &st);
  mser;
  paletteData *newData = (paletteData *)fnOut.data(&st);
  mser;
  std::map<int, Paint> *palette = newData->fGeometry;

  //////////

 
 
  unsigned nInputs = hInput.elementCount();
  for (unsigned i = 0; i < nInputs; i++, hInput.next())
  {
    MDataHandle hPaintInput = hInput.inputValue(&st);
    int paintId = hInput.elementIndex();
    if (st.error())
    {
      continue;
    }

    MFloatVector fcolor = hPaintInput.child(aColor).asFloatVector();
 
    int pot =  hPaintInput.child(aPotId).asInt();
    float opacity = hPaintInput.child(aOpacity).asFloat();
    float travel = hPaintInput.child(aTravel).asFloat();
    MString name = hPaintInput.child(aName).asString();
 
    (*palette)[paintId] = Paint(fcolor, opacity ,pot, travel, name);
  }

  hOutput.set(newData);
  hOutput.setClean();
  return MS::kSuccess;
}

void paletteNode::draw(M3dView &view,
                     const MDagPath &path,
                     M3dView::DisplayStyle style,
                     M3dView::DisplayStatus status)
{
}

bool paletteNode::isBounded() const
{
  return false;
}

MBoundingBox paletteNode::boundingBox() const
{
  return MBoundingBox();
}

void paletteNode::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());
  nodeFn.setName("paletteShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}
