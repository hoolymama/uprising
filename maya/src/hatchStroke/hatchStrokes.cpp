
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MRampAttribute.h>

#include "cImgUtils.h"
#include "cImgFloatData.h"

#include "strokeData.h"
#include "hatchStrokes.h"
#include "stroke.h"
#include "mayaMath.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"
#include "enums.h"
#include "vec2d.h"

const float PI = 3.14159265359;

MObject hatchStrokes::aTargetRotationMatrix;
MObject hatchStrokes::aFlowImage;
MObject hatchStrokes::aFlowAttraction;
// MObject hatchStrokes::aPointDensity;
MObject hatchStrokes::aNumPointsSide;
MObject hatchStrokes::aFlowProjection;

MObject hatchStrokes::aHatchLengthMax;
MObject hatchStrokes::aHatchLengthMin;
MObject hatchStrokes::aHatchLength;

MObject hatchStrokes::aHatchAngleMin;
MObject hatchStrokes::aHatchAngleMax;
MObject hatchStrokes::aHatchAngle;

MObject hatchStrokes::aTangentBlend;
MObject hatchStrokes::aHatch;

MObject hatchStrokes::aSeed;

MObject hatchStrokes::aColorRamp;

MTypeId hatchStrokes::id(k_hatchStrokes);

hatchStrokes::hatchStrokes() {}

hatchStrokes::~hatchStrokes() {}

void *hatchStrokes::creator()
{
  return new hatchStrokes();
}

/// Post constructor
void hatchStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus hatchStrokes::initialize()
{
  MStatus st;
  MString method("hatchStrokes::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;
  MFnMatrixAttribute mAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
  MFnCompoundAttribute cAttr;
  MRampAttribute rAttr;

  aFlowImage = tAttr.create("flowImage", "fim", cImgFloatData::id);
  tAttr.setStorable(false);
  tAttr.setConnectable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aFlowImage);

  aNumPointsSide = nAttr.create("numPointsPerSide", "nps", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(2);
  nAttr.setMin(1);
  addAttribute(aNumPointsSide);

  aFlowProjection = mAttr.create("flowProjection", "fprj", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  addAttribute(aFlowProjection);

  aTargetRotationMatrix = mAttr.create("targetRotationMatrix", "tmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  addAttribute(aTargetRotationMatrix);

  aHatchLengthMin = nAttr.create("hatchLengthMin", "hlnn", MFnNumericData::kFloat);
  nAttr.setDefault(0.5);
  aHatchLengthMax = nAttr.create("hatchLengthMax", "hlnx", MFnNumericData::kFloat);
  nAttr.setDefault(1.0);
  aHatchLength = nAttr.create("hatchLength", "hln", aHatchLengthMin, aHatchLengthMax);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  // st = addAttribute(aHatchLength);
  mser;

  aHatchAngleMin = uAttr.create("hatchAngleMin", "hann", MFnUnitAttribute::kAngle);
  uAttr.setDefault(0.0);
  aHatchAngleMax = uAttr.create("hatchAngleMax", "hanx", MFnUnitAttribute::kAngle);
  uAttr.setDefault(0.0);
  aHatchAngle = nAttr.create("hatchAngle", "han", aHatchAngleMin, aHatchAngleMax);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  // st = addAttribute(aHatchAngle);
  mser;

  aTangentBlend = nAttr.create("tangentBlend", "tanb", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(0.0f);

  aFlowAttraction = nAttr.create("flowAttraction", "fatt", MFnNumericData::kFloat);
  nAttr.setStorable(false);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);

  aHatch = cAttr.create("hatches", "ht");
  cAttr.setKeyable(true);
  cAttr.setStorable(true);
  cAttr.addChild(aHatchLength);
  cAttr.addChild(aHatchAngle);
  cAttr.addChild(aTangentBlend);
  cAttr.addChild(aFlowAttraction);
  cAttr.setArray(true);
  addAttribute(aHatch);

  aSeed = nAttr.create("seed", "sd", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  st = addAttribute(aSeed);

  aColorRamp = rAttr.createColorRamp("colorRamp", "crmp");
  st = addAttribute(aColorRamp);
  mser;

  attributeAffects(aTargetRotationMatrix, aOutput);
  attributeAffects(aNumPointsSide, aOutput);
  attributeAffects(aFlowProjection, aOutput);
  attributeAffects(aFlowImage, aOutput);

  attributeAffects(aHatchLengthMax, aOutput);
  attributeAffects(aHatchLengthMin, aOutput);
  attributeAffects(aHatchLength, aOutput);

  attributeAffects(aHatchAngleMin, aOutput);
  attributeAffects(aHatchAngleMax, aOutput);
  attributeAffects(aHatchAngle, aOutput);

  attributeAffects(aTangentBlend, aOutput);

  attributeAffects(aFlowAttraction, aOutput);

  attributeAffects(aHatch, aOutput);

  attributeAffects(aSeed, aOutput);

  attributeAffects(aColorRamp, aOutput);

  return (MS::kSuccess);
}

MStatus hatchStrokes::mutate(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{
  MStatus st;

  const CImg<float> *pImage = cImgUtils::getFloatImage(data, aFlowImage);
  if (!pImage)
  {
    // flow = MFloatVectorArray(points.length(), MFloatVector::zero);
    return MS::kUnknownParameter;
  }

  int w = pImage->width();
  int h = pImage->height();

  if (!(w && h))
  {
    return MS::kUnknownParameter;
  }
  MFloatMatrix projectionUvToImage;
  cImgUtils::transformUvToImage(w, h, projectionUvToImage);
  MFloatMatrix projectionImageToUv = projectionUvToImage.inverse();

  MFloatMatrix norm;
  norm.setToIdentity();
  norm[0][0] = 0.5;
  norm[1][1] = 0.5;
  norm[3][0] = 0.5;
  norm[3][1] = 0.5;
  MFloatMatrix projection = data.inputValue(aFlowProjection).asFloatMatrix();
  MFloatMatrix projectionWorldToUv = projection.inverse() * norm;
  MFloatMatrix projectionUvToWorld = projectionWorldToUv.inverse();

  MFloatMatrix projectionWorldToImage = projection.inverse() * norm * projectionUvToImage;
  MFloatMatrix projectionImageToWorld = projectionWorldToImage.inverse() ;


  MFloatMatrix targetRotationMatrix = data.inputValue(aTargetRotationMatrix).asFloatMatrix();
  targetRotationMatrix = mayaMath::rotationOnly(targetRotationMatrix);
  unsigned numPointsSide = data.inputValue(aNumPointsSide).asInt();

  int totalNumPoints = numPointsSide*2 + 1;
 
  int seed = data.inputValue(aSeed).asInt();

  MFloatPointArray points;
  MColorArray colors;
  MFloatVectorArray tangents;

  srand48(seed);
  // Each point, tangent, and color of input strokes
  getTargetPoints(strokes, points);
  // getTangents(strokes, tangents);
  getTargetColors(strokes, colors);

  // MFloatVectorArray flow;
  // getFlow(points, data, flow);

  // Ramp colors are multipliers for the points along the hatch
  MObject thisObj = thisMObject();
  MRampAttribute rampAttr(thisObj, aColorRamp, &st);
  MFloatArray positions;
  MColorArray rampColors;

  rampAttr.sampleColorRamp(totalNumPoints, rampColors, &st);
  if (st != MS::kSuccess)
  {
    rampColors = MColorArray(totalNumPoints, MColor(1.0f, 1.0f, 1.0f, 1.0f));
  }

  // std::vector<Stroke> sourceStrokes(*strokes);

  MArrayDataHandle hHatches = data.inputArrayValue(aHatch);
  unsigned nInputs = hHatches.elementCount();

  if (nInputs == 0)
  {
    return MS::kUnknownParameter;
  }

  strokes->clear();
  // cerr << "nInputs" << nInputs << endl;
  for (unsigned i = 0; i < nInputs; i++, hHatches.next())
  {
    int index = hHatches.elementIndex(&st);
    MDataHandle hHatch = hHatches.inputValue(&st);

    if (st.error())
    {
      continue;
    }
    addHatchSet(
        hHatch,
        numPointsSide,
        targetRotationMatrix,
        points,
        // tangents,
        // flow,
        colors,
        rampColors,
        projectionWorldToUv,
        projectionUvToWorld,
        projectionUvToImage,
        projectionImageToUv,
        pImage,
        strokes);
  }
  return MS::kSuccess;
}

void hatchStrokes::addHatchSet(
    MDataHandle &handle,
    unsigned numPointsSide,
    const MFloatMatrix &targetRotationMatrix,
    const MFloatPointArray &points,
    const MColorArray &colors,
    const MColorArray &rampColors,
    const MFloatMatrix &projectionWorldToUv,
    const MFloatMatrix &projectionUvToWorld,
    const MFloatMatrix &projectionUvToImage,
    const MFloatMatrix &projectionImageToUv,
    const CImg<float> *pImage,
    std::vector<Stroke> *strokes) const
{



  MDataHandle hHatchLength = handle.child(aHatchLength);
  MDataHandle hHatchAngle = handle.child(aHatchAngle);
  float tangentBlend = handle.child(aTangentBlend).asFloat();
  float oneMinusTangentBlend = 1.0f - tangentBlend;
  float flowAttraction = handle.child(aFlowAttraction).asFloat();

  float hatchLengthMin = hHatchLength.child(aHatchLengthMin).asFloat();
  float hatchLengthMax = hHatchLength.child(aHatchLengthMax).asFloat();
  float hatchAngleMin = hHatchAngle.child(aHatchAngleMin).asAngle().asRadians();
  float hatchAngleMax = hHatchAngle.child(aHatchAngleMax).asAngle().asRadians();

  const unsigned count = points.length();

  

  MFloatPointArray flowPoints;
  for (unsigned i = 0; i < count; i++)
  {
    flowPoints.clear();

    float hatchLength = hatchLengthMin + (hatchLengthMax - hatchLengthMin) * drand48();
    // float hatchAngle = hatchAngleMin + (hatchAngleMax - hatchAngleMin) * drand48();
    getFlowPoints(
        points[i],
        numPointsSide,
        hatchLength,
        //  hatchAngle,
        flowAttraction,
        projectionWorldToUv,
        projectionUvToWorld,
        projectionUvToImage,
        projectionImageToUv,
        pImage,
        flowPoints);

 
    int len = flowPoints.length();
    if ( len != rampColors.length() )
    {
      continue;
    }

    MColorArray hcolors(len, colors[i]);
    for (int j = 0; j < len; j++)
    {
      hcolors[j] *= rampColors[j];
    }

 
    Stroke stroke(flowPoints, hcolors, targetRotationMatrix);
 
    if (stroke.valid())
    {
      strokes->push_back(stroke);
    }
  }
}

void hatchStrokes::getFlowPoints(
    const MFloatPoint &point,
    unsigned numPointsSide,
    float hatchLength,
    // float hatchAngle,
    float flowAttraction,
    const MFloatMatrix &projectionWorldToUv,
    const MFloatMatrix &projectionUvToWorld,
    const MFloatMatrix &projectionUvToImage,
    const MFloatMatrix &projectionImageToUv,
    const CImg<float> *pImage,
    MFloatPointArray &flowPoints) const
{

  unsigned totalNumPoints = (numPointsSide) + 1;

  int w = pImage->width();
  int h = pImage->height();

  flowPoints.clear();
  MFloatPointArray flowPointsA;
  flowPointsA.clear();
 
  float spanCm = hatchLength / (numPointsSide);
 
  float spanPixels = (MFloatVector(spanCm, 0, 0) * projectionWorldToUv).length() * w * 0.5;

  MFloatPoint uv = point * projectionWorldToUv;
  uv.z=0.0f;
  float xOrig, yOrig;
  cImgUtils::toImageCoords(uv.x, uv.y, w, h, xOrig, yOrig);

 
  JVector2D p0(xOrig, yOrig);
 
  float dx;
  float dy;
  for (int j = 0; j < numPointsSide; j++)
  {

    dx = pImage->linear_atXY(p0.x, p0.y, 0, 0);
    dy = pImage->linear_atXY(p0.x, p0.y, 0, 1);

    JVector2D dxdy0(dx, dy);
    if (dxdy0.isZero())
    {
      // dont make points
      return;
    }
    JVector2D flowVector = dxdy0.normal();
    flowVector.rotateBy90(); // flow
    flowVector *= spanPixels;

    JVector2D p1 = p0 + flowVector; // new sample point
    dx = pImage->linear_atXY(p1.x, p1.y, 0, 0);
    dy = pImage->linear_atXY(p1.x, p1.y, 0, 1);
    JVector2D dxdy1(dx, dy);

    JVector2D p2 = p1 + dxdy1.projection(-flowVector);

    MFloatPoint f = MFloatPoint(p2.x, p2.y, 0.0f) * projectionImageToUv * projectionUvToWorld;
    flowPointsA.append(f);

    p0 = p2;
  }

  // cerr << "flowPointsA.length() " << flowPointsA.length() << endl;
  flowPoints.append(point);
  for (int j = 0; j < numPointsSide; j++)
  {
    flowPoints.append(flowPointsA[j]);
  }
}
