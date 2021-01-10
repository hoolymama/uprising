
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include "strokeData.h"
#include "mapColorStrokes.h"
#include "stroke.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

 
MObject mapColorStrokes::aRGB;
// MObject mapColorStrokes::aR;
// MObject mapColorStrokes::aG;
// MObject mapColorStrokes::aB;

MObject mapColorStrokes::aWhite;
 
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
 

MStatus mapColorStrokes::initialize()
{
  MStatus st;
  MString method("mapColorStrokes::initialize");

	inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;
 
  aRGB = nAttr.createColor("rgb", "rgb");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aRGB);

  MObject  aR = nAttr.child(0);
  MObject  aG = nAttr.child(1);
  MObject  aB = nAttr.child(2);


  aWhite = nAttr.create("white", "wht", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aWhite);

  


 
  st = attributeAffects(aRGB, aOutput);

  st = attributeAffects(  aR, aOutput);
  st = attributeAffects(  aG, aOutput);
  st = attributeAffects(  aB, aOutput);
 


  st = attributeAffects(aWhite, aOutput);

  return (MS::kSuccess);
}

MStatus mapColorStrokes::mutate(MDataBlock &data, std::vector<Stroke> *geom) const 
{

  MFloatPointArray points;
  points.clear();
  getPoints(data, geom, points);
  applyColor(data, geom, points );

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
        1.0,
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
        1.0,
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
