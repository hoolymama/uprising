#include "texUtils.h"

#include <maya/MString.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MFloatMatrix.h>
#include <maya/MRenderUtil.h>
#include <maya/MRampAttribute.h>


#include "errorMacros.h"






bool TexUtils::hasTexture(const MObject &node, const MObject &attribute)
{
  MString name;
  MStatus st = getTextureName(node, attribute, name);
  if (st.error()) {return false; }
  return true;
}

MStatus TexUtils::getTextureName(const MObject &node, const MObject &attribute,
                                 MString &name)  {
  MStatus st;

  MPlugArray plugArray;
  MPlug plug(node, attribute);
  bool hasConnection = plug.connectedTo(plugArray, 1, 0, &st);
  if (st.error() or (! hasConnection)) {
    return  MS::kUnknownParameter;
  }
  name = plugArray[0].name(&st);
  return MS::kSuccess;
}


MStatus TexUtils::sampleUVTexture(const MObject &node, const MObject &attribute,
                                  MFloatArray &uVals,
                                  MFloatArray &vVals, MFloatVectorArray &result)  {

  MStatus st;
  MString plugName;
  st = getTextureName(node, attribute, plugName);
  if (st.error()) {return MS::kFailure; }
  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();
  MFloatVectorArray transparencies;
  // MFloatVectorArray colors;

  int n = uVals.length();

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat,
        0, &uVals, &vVals, 0, 0, 0, 0, 0, result, transparencies);
  if (st.error()) {
    return  MS::kUnknownParameter;
  }
  return MS::kSuccess;
}

MStatus TexUtils::sampleUVTexture(
  const MObject &node,
  const MObject &textureAttribute,
  MFloatArray &uVals,
  MFloatArray &vVals,
  const MObject &rampAttribute,
  int low, int high,
  MIntArray &result)
{

  MStatus st;
  MString plugName;
  st = getTextureName(node, textureAttribute, plugName);
  if (st.error()) {return MS::kFailure; }

  MRampAttribute remapRamp( node, rampAttribute );
  if (st.error()) {return MS::kFailure; }

  int span = high - low;

  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();
  MFloatVectorArray transparencies;
  MFloatVectorArray colors;

  int n = uVals.length();

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat,
        0, &uVals, &vVals, 0, 0, 0, 0, 0, colors, transparencies);
  if (st.error()) {
    return  MS::kUnknownParameter;
  }

  result.setLength(n);
  for (int i = 0; i < n; ++i)
  {
    float remappedVal;
    remapRamp.getValueAtPosition( colors[i].x, remappedVal);
    result.set(int((remappedVal * span) + low), i);
  }
  return MS::kSuccess;
}



MStatus TexUtils::sampleUVTexture(
  const MObject &node,
  const MObject &textureAttribute,
  MFloatArray &uVals,
  MFloatArray &vVals,
  int range,
  MIntArray &result)
{
  MStatus st;
  MString plugName;
  st = getTextureName(node, textureAttribute, plugName);
  if (st.error()) {return MS::kFailure; }

  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();
  MFloatVectorArray transparencies;
  MFloatVectorArray colors;

  int n = uVals.length();

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat,
        0, &uVals, &vVals, 0, 0, 0, 0, 0, colors, transparencies);
  if (st.error()) {
    return  MS::kUnknownParameter;
  }

  result.setLength(n);
  for (int i = 0; i < n; ++i)
  {
    result.set(int(colors[i].x * range), i);
  }
  return MS::kSuccess;
}


MStatus TexUtils::sampleUVTexture(
  const MObject &node,
  const MObject &textureAttribute,
  MFloatArray &uVals,
  MFloatArray &vVals,
  float low,
  float high,
  MFloatArray &result)
{
  float range = high - low;
  MStatus st;
  MString plugName;
  st = getTextureName(node, textureAttribute, plugName);
  if (st.error()) {return MS::kFailure; }

  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();
  MFloatVectorArray transparencies;
  MFloatVectorArray colors;

  int n = uVals.length();

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat,
        0, &uVals, &vVals, 0, 0, 0, 0, 0, colors, transparencies);
  if (st.error()) {
    return  MS::kUnknownParameter;
  }

  result.setLength(n);
  for (int i = 0; i < n; ++i)
  {
    result.set((colors[i].x * range) + low , i);
  }
  return MS::kSuccess;
}


MStatus TexUtils::sampleUVGradient(
  const MObject &node,
  const MObject &textureAttribute,
  float sampleDistance,
  float scale,
  MFloatArray &uVals,
  MFloatArray &vVals,
  MFloatVectorArray &result)
{
  MStatus st;
  MString plugName;
  st = getTextureName(node, textureAttribute, plugName);
  if (st.error()) {   return MS::kFailure; }

  unsigned n = uVals.length();
  result.setLength(n);

  if (sampleDistance < 0.00001) {
    sampleDistance = 0.00001;
  }
  scale = scale / sampleDistance;

  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();
  MFloatVectorArray transparencies;


  MFloatArray uOffsetVals(n);
  MFloatArray vOffsetVals(n);

  for (unsigned i = 0; i < n; i++) {
    uOffsetVals.set( (uVals[i] + sampleDistance) , i);
    vOffsetVals.set( (vVals[i] + sampleDistance) , i);
  }

  MFloatVectorArray colors;
  MFloatVectorArray uOffsetColors;
  MFloatVectorArray vOffsetColors;

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat, 0, &uVals,
        &vVals, 0, 0, 0, 0, 0, colors, transparencies); msert;
  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat, 0,
        &uOffsetVals, &vVals, 0, 0, 0, 0, 0, uOffsetColors, transparencies); msert;
  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat, 0, &uVals,
        &vOffsetVals, 0, 0, 0, 0, 0, vOffsetColors, transparencies); msert;

  for (unsigned i = 0; i < n; i++) {
    MFloatVector grad(
      (uOffsetColors[i].x - colors[i].x) ,
      (vOffsetColors[i].x - colors[i].x) ,
      0.0);

    result.set(grad * scale, i);
  }
  return MS::kSuccess;
}

