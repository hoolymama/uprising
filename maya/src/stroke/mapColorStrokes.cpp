
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MFloatVectorArray.h> 
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>

#include <maya/MMeshIntersector.h>

#include "strokeData.h"
#include "mapColorStrokes.h"
#include "stroke.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

MObject mapColorStrokes::aRGB;
MObject mapColorStrokes::aWhite;

MObject mapColorStrokes::aMesh;
MObject mapColorStrokes::aPoint;
MObject mapColorStrokes::aBias;
MObject mapColorStrokes::aDoOcclusion;

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
  MFnTypedAttribute tAttr;

  aRGB = nAttr.createColor("rgb", "rgb");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aRGB);

  MObject aR = nAttr.child(0);
  MObject aG = nAttr.child(1);
  MObject aB = nAttr.child(2);

  aWhite = nAttr.create("white", "wht", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aWhite);

  aPoint = nAttr.createPoint("occlusionPoint", "opt");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aPoint);

  aMesh = tAttr.create("occlusionMesh", "omsh", MFnData::kMesh);
  tAttr.setReadable(false);
  addAttribute(aMesh);

  aBias = nAttr.create("occlusionBias", "obs", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aBias);

  aDoOcclusion = nAttr.create("doOcclusion", "occ", MFnNumericData::kBoolean);
  nAttr.setReadable(false);
  nAttr.setDefault(false);
  nAttr.setKeyable(true);
  addAttribute(aDoOcclusion);

  attributeAffects(aPoint, aOutput);
  attributeAffects(aMesh, aOutput);
  attributeAffects(aBias, aOutput);
  attributeAffects(aDoOcclusion, aOutput);
  attributeAffects(aRGB, aOutput);
  attributeAffects(aR, aOutput);
  attributeAffects(aG, aOutput);
  attributeAffects(aB, aOutput);
  attributeAffects(aWhite, aOutput);

  return (MS::kSuccess);
}

MStatus mapColorStrokes::mutate( 
  MDataBlock &data,
  std::vector<Stroke> *strokes)  const
{
  MStatus st;

//  MObject thisObj = thisMObject();
  
  // MFloatPointArray points;
  // points.clear();


  // getTargetPoints(data, strokes, points);

  MFloatVectorArray colors;
  MFloatArray whites;

  MFloatPointArray points;
  getTargetPoints(strokes, points);

  getColors(data, points, colors, whites);

  if ((points.length() != colors.length()) || (points.length() != whites.length()))
  {
    return MS::kUnknownParameter;
  }

  bool doOcclusion = data.inputValue(aDoOcclusion).asBool();
  if (doOcclusion)
  {
    st = occludeColors(data, points, colors, whites);
  }

  applyColors(strokes, colors, whites);

  return MS::kSuccess;
}

void mapColorStrokes::getColors(
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatVectorArray &colors,
    MFloatArray &whites) const
{

  MStatus st;
  MObject thisObj = thisMObject();

  unsigned len = points.length();
  bool isColorMapped = false;
  bool isWhiteMapped = false;

  if (TexUtils::hasTexture(thisObj, mapColorStrokes::aRGB))
  {

    st = TexUtils::sampleSolidTexture( thisObj, mapColorStrokes::aRGB, 1.0, points, colors);

    if (!st.error())
    {
      isColorMapped = true;
    }
  }
  if (!isColorMapped)
  {
    MFloatVector color = data.inputValue(aRGB).asFloatVector();
    colors = MFloatVectorArray(len, color);
  }

  if (TexUtils::hasTexture(thisObj, mapColorStrokes::aWhite))
  {

    st = TexUtils::sampleSolidTexture(
        thisObj,
        mapColorStrokes::aRGB,
        1.0,
        points,
        whites);

    if (!st.error())
    {
      isWhiteMapped = true;
    }
  }
  if (!isWhiteMapped)
  {
    float white = data.inputValue(aWhite).asFloat();
    whites = MFloatArray(len, white);
  }
}

MStatus mapColorStrokes::occludeColors(
    MDataBlock &data,
    const MFloatPointArray &points,
    MFloatVectorArray &colors,
    MFloatArray &whites) const
{

  MStatus st;
  unsigned len = points.length();
  float3 &fpoint = data.inputValue(aPoint).asFloat3();
  MFloatPoint camPoint(fpoint[0], fpoint[1], fpoint[2]);
  const float &bias = data.inputValue(aBias).asFloat();

  MDataHandle hMesh = data.inputValue(aMesh, &st);
  msert;
  MObject inMesh = hMesh.asMeshTransformed();
  MFnMesh mFnIn(inMesh, &st);
  msert;

  MMeshIsectAccelParams ap = mFnIn.autoUniformGridParams();
  MFloatPoint fpSource;
  MFloatVector fvRayDir;
  float hitRayParam;
  MFloatPoint fHitPoint;

  for (unsigned i = 0; i < len; i++)
  {
    const MFloatVector &fvRayDir = MFloatVector(camPoint - points[i]).normal();

    const MFloatPoint &fpSource = points[i] + (fvRayDir * bias);

    bool hit = mFnIn.anyIntersection(
        fpSource, fvRayDir, NULL, NULL, false,
        MSpace::kWorld, 99999, false, &ap, fHitPoint,
        &hitRayParam, NULL, NULL, NULL, NULL, 0.000001f, &st);
    mser;
    if (hit)
    {
      colors[i] = MFloatVector(0, 0, 0);
      whites[i] = 0.0f;
    }
  }

  return MS::kSuccess;
}

void mapColorStrokes::applyColors(
    std::vector<Stroke> *geom,
    const MFloatVectorArray &colors,
    const MFloatArray &whites) const
{
  std::vector<Stroke>::iterator iter = geom->begin();
  unsigned index = 0;
  for (unsigned i = 0; iter != geom->end(); iter++, i++)
  {
    iter->setTargetColors(colors, whites, index);
    index += iter->size();
  }
}