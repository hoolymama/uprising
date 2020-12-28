
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MString.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include "strokeData.h"
#include "mapColorStrokes.h"
#include "stroke.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

MObject mapColorStrokes::aStrokes;
MObject mapColorStrokes::aRGB;
MObject mapColorStrokes::aWhite;
MObject mapColorStrokes::aOutput;

MTypeId mapColorStrokes::id(k_mapColorStrokes);

mapColorStrokes::mapColorStrokes() {}

mapColorStrokes::~mapColorStrokes() {}

void *mapColorStrokes::creator()
{
  return new mapColorStrokes();
}

/// Post constructor
void mapColorStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

// const double epsilon = 0.0001;

MStatus mapColorStrokes::initialize()
{
  MStatus st;
  MString method("mapColorStrokes::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnMatrixAttribute mAttr;

 


  aStrokes = tAttr.create("strokes", "stks", strokeData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aStrokes);

  aRGB = nAttr.createColor("rgb", "rgb");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aRGB);

  aWhite = nAttr.create("white", "wht", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aWhite);

  aOutput = tAttr.create("output", "out", strokeData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aRGB, aOutput);
  st = attributeAffects(aWhite, aOutput);

  return (MS::kSuccess);
}

MStatus mapColorStrokes::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  if (plug != aOutput)
  {
    return (MS::kUnknownParameter);
  }

  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(strokeData::id);

  MObject dOut = fnOut.create(kdid, &st);
  strokeData *newData = (strokeData *)fnOut.data(&st);
  mser;
  std::vector<Stroke> *geom = newData->fGeometry;
  geom->clear();

  copyStrokes(data, geom);

  MFloatPointArray points;
  points.clear();

  getPoints(data, geom, points);

  hOutput.set(newData);
  data.setClean(plug);

  return MS::kSuccess;
}

MStatus mapColorStrokes::copyStrokes(MDataBlock &data, std::vector<Stroke> *geom)
{
  MStatus st;
  MDataHandle hStrokes = data.inputValue(aStrokes, &st);
  msert;

  MObject dStrokes = hStrokes.data();
  MFnPluginData fnStrokes(dStrokes, &st);
  msert;

  strokeData *sData = (strokeData *)fnStrokes.data();
  const std::vector<Stroke> *strokeGeom = sData->fGeometry;

  std::vector<Stroke>::const_iterator citer;
  for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++)
  {
    geom->push_back(*citer);
  }

  return MS::kSuccess;
}

 
void mapColorStrokes::getPoints(MDataBlock &data, std::vector<Stroke> *geom, MFloatPointArray &points) const
{
  for (std::vector<Stroke>::const_iterator iter = geom->begin(); iter != geom->end(); iter++)
  {
    const std::vector<Target> targets = iter->targets();
    for (std::vector<Target>::const_iterator targetIter = targets.begin(); targetIter != targets.end(); targetIter++)
    {
      const MMatrix &mat = targetIter->matrix();
      points.append(MFloatPoint(mat[3][0],mat[3][1],mat[3][2]));
    }
  }
}

void mapColorStrokes::applyColor(
    MDataBlock &data,
    std::vector<Stroke> *geom,
    MFloatPointArray &points ) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  
  unsigned len = points.length();
  bool isColorMapped = false;
  bool isWhiteMapped = false;
  
  MFloatVectorArray colors;
  if (TexUtils::hasTexture(thisObj, mapColorStrokes::aRGB))
  {

    st = TexUtils::sample3dTexture(
        thisObj,
        mapColorStrokes::aRGB,
        10.0,
        points, 
        colors);

    if (!st.error()) {
      isColorMapped = true;
    }
  }
  if (!isColorMapped) {
    MFloatVector color = data.inputValue(aRGB).asFloatVector();
    colors = MFloatVectorArray(len, color );
  }

  MFloatArray whites;
  if (TexUtils::hasTexture(thisObj, mapColorStrokes::aWhite))
  {

    st = TexUtils::sample3dTexture(
        thisObj,
        mapColorStrokes::aRGB,
        10.0,
        points, 
        whites);

    if (!st.error()) {
      isWhiteMapped = true;
    }
  }
  if (!isWhiteMapped) {
    float white = data.inputValue(aWhite).asFloat();
    whites = MFloatArray( len,white);
  }
 
  std::vector<Stroke>::iterator iter = geom->begin();
  unsigned index = 0;
  for (unsigned i = 0; iter != geom->end(); iter++, i++)
  {
    iter->setTargetColors(colors, whites, index);
    index += iter->size();
  }

}
