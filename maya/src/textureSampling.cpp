#include "textureSampling.h"

#include <maya/MString.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MFloatMatrix.h>
#include <maya/MRenderUtil.h>

// #include "errorMacros.h"



bool hasTexture(const MObject &node, const MObject &attribute)
{
  MString name;
  MStatus st = getTextureName(node, attribute, name);
  if (st.error()) {return false; }
  return true;
}

MStatus getTextureName(const MObject &node, const MObject &attribute,
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


MStatus sampleUVTexture(const MObject &node, const MObject &attribute,
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

MStatus sampleUVTexture(const MObject &node, const MObject &attribute,
                        MFloatArray &uVals,
                        MFloatArray &vVals, MIntArray &result, short range)  {

  MStatus st;
  MString plugName;
  st = getTextureName(node, attribute, plugName);
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
    result.set(  int(colors[i].x * float(range)), i);
  }
  return MS::kSuccess;
}




