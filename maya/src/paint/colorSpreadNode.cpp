
#include <maya/MIOStream.h>

#include <maya/MColor.h>
#include <maya/MFloatVector.h>


#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include <jMayaIds.h>

#include "errorMacros.h"

#include "colorSpreadNode.h"

MObject colorSpreadNode::aColor;

MObject colorSpreadNode::aHuePropagation;
MObject colorSpreadNode::aSaturationPropagation;
MObject colorSpreadNode::aValuePropagation;

MObject colorSpreadNode::aHueRange;
MObject colorSpreadNode::aSaturationRange;
MObject colorSpreadNode::aValueRange;
MObject colorSpreadNode::aSeed;

MObject colorSpreadNode::aOutColor;

MTypeId colorSpreadNode::id(k_colorSpreadNode);

colorSpreadNode::colorSpreadNode() {}

colorSpreadNode::~colorSpreadNode() {}

void *colorSpreadNode::creator()
{
  return new colorSpreadNode();
}

void colorSpreadNode::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus colorSpreadNode::initialize()
{
  MStatus st;
  MString method("colorSpreadNode::initialize");

  MFnNumericAttribute nAttr;
  MFnEnumAttribute eAttr;

  aColor = nAttr.createColor("color", "col");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aColor);

  aSeed = nAttr.create("seed", "sd", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aSeed);

  aHuePropagation = eAttr.create("huePropagation", "hppg", colorSpreadNode::kSpread);
  eAttr.addField("spread", colorSpreadNode::kSpread);
  eAttr.addField("forwardLinear", colorSpreadNode::kForwardLinear);
  eAttr.addField("forwardRandom", colorSpreadNode::kForwardRandom);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  addAttribute(aHuePropagation);

  aSaturationPropagation = eAttr.create("saturationPropagation", "sppg", colorSpreadNode::kSpread);
  eAttr.addField("spread", colorSpreadNode::kSpread);
  eAttr.addField("forwardLinear", colorSpreadNode::kForwardLinear);
  eAttr.addField("forwardRandom", colorSpreadNode::kForwardRandom);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  addAttribute(aSaturationPropagation);

  aValuePropagation = eAttr.create("valuePropagation", "vppg", colorSpreadNode::kSpread);
  eAttr.addField("spread", colorSpreadNode::kSpread);
  eAttr.addField("forwardLinear", colorSpreadNode::kForwardLinear);
  eAttr.addField("forwardRandom", colorSpreadNode::kForwardRandom);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  addAttribute(aValuePropagation);

  aHueRange = nAttr.create("hueRange", "hur", MFnNumericData::k2Float);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aHueRange);

  aSaturationRange = nAttr.create("saturationRange", "str", MFnNumericData::k2Float);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aSaturationRange);

  aValueRange = nAttr.create("valueRange", "vlr", MFnNumericData::k2Float);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aValueRange);

  aOutColor = nAttr.createColor("outColor", "ocol");
  nAttr.setReadable(true);
  nAttr.setStorable(false);
  nAttr.setArray(true);
  addAttribute(aOutColor);

  attributeAffects(aColor, aOutColor);

  attributeAffects(aHuePropagation, aOutColor);
  attributeAffects(aSaturationPropagation, aOutColor);
  attributeAffects(aValuePropagation, aOutColor);
  attributeAffects(aHueRange, aOutColor);
  attributeAffects(aSaturationRange, aOutColor);
  attributeAffects(aValueRange, aOutColor);
  attributeAffects(aSeed, aOutColor);

  return (MS::kSuccess);
}

MStatus colorSpreadNode::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;

  if (!((plug == aOutColor)))
  {
    return (MS::kUnknownParameter);
  }

  MFloatVector fColor = data.inputValue(aColor).asFloatVector();
  MColor color = MColor(fColor.x, fColor.y, fColor.z);

  int seed = data.inputValue(aSeed).asInt();

  colorSpreadNode::Propagation huePropagation = colorSpreadNode::Propagation(data.inputValue(aHuePropagation).asShort());
  colorSpreadNode::Propagation satPropagation = colorSpreadNode::Propagation(data.inputValue(aSaturationPropagation).asShort());
  colorSpreadNode::Propagation valPropagation = colorSpreadNode::Propagation(data.inputValue(aValuePropagation).asShort());

  const float2 &hueRange = data.inputValue(aHueRange).asFloat2();
  const float2 &satRange = data.inputValue(aSaturationRange).asFloat2();
  const float2 &valRange = data.inputValue(aValueRange).asFloat2();

  MArrayDataHandle hOutputs = data.outputArrayValue(aOutColor, &st);
  mser;

  float sourceHue, sourceSat, sourceVal;
  color.get(MColor::kHSV, sourceHue, sourceSat, sourceVal);

  float hueRand = float(drand48());
  float satRand = float(drand48());
  float valRand = float(drand48());

  unsigned numColors = hOutputs.elementCount();
  srand48(seed);
  for (int i = 0; i < numColors; i++, hOutputs.next())
  {

    float hue = sourceHue + (hueRand * (hueRange[1] - hueRange[0])) + hueRange[0];
    float sat = sourceSat + (satRand * (satRange[1] - satRange[0])) + satRange[0];
    float val = sourceVal + (valRand * (valRange[1] - valRange[0])) + valRange[0];

    hue = fmod(hue + 720, 360.0);
    sat = fmax(fmin(sat, 1.0), 0.0);
    val = fmax(fmin(val, 1.0), 0.0);

    MColor outColor(MColor::kHSV, hue, sat, val);
    MDataHandle hOutput = hOutputs.outputValue(&st);

    mser;
    float3 &out = hOutput.asFloat3();
    out[0] = outColor.r;
    out[1] = outColor.g;
    out[2] = outColor.b;

    if ((huePropagation == colorSpreadNode::kForwardLinear) || (huePropagation == colorSpreadNode::kForwardRandom))
    {
      sourceHue = hue;
    }
    if ((huePropagation == colorSpreadNode::kSpread) || (huePropagation == colorSpreadNode::kForwardRandom))
    {
      hueRand = float(drand48());
    }
    else
    {
      drand48();
    }

    if ((satPropagation == colorSpreadNode::kForwardLinear) || (satPropagation == colorSpreadNode::kForwardRandom))
    {
      sourceSat = sat;
    }
    if ((satPropagation == colorSpreadNode::kSpread) || (satPropagation == colorSpreadNode::kForwardRandom))
    {
      satRand = float(drand48());
    }
    else
    {
      drand48();
    }

    if ((valPropagation == colorSpreadNode::kForwardLinear) || (valPropagation == colorSpreadNode::kForwardRandom))
    {
      sourceVal = val;
    }
    if ((valPropagation == colorSpreadNode::kSpread) || (valPropagation == colorSpreadNode::kForwardRandom))
    {
      valRand = float(drand48());
    }
    else
    {
      drand48();
    }
  }

  hOutputs.setAllClean();
  return MS::kSuccess;
}