
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

const float PI = 3.14159265359;

MObject hatchStrokes::aTargetRotationMatrix;
MObject hatchStrokes::aFlowImage;
// MObject hatchStrokes::aPointDensity;
MObject hatchStrokes::aNumPoints;
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
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aFlowImage);

  aNumPoints = nAttr.create("numPoints", "den", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(3);
  nAttr.setMin(2);
  addAttribute(aNumPoints);

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

  aHatch = cAttr.create("hatches", "ht");
  cAttr.setKeyable(true);
  cAttr.setStorable(true);
  cAttr.addChild(aHatchLength);
  cAttr.addChild(aHatchAngle);
  cAttr.addChild(aTangentBlend);
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
  attributeAffects(aNumPoints, aOutput);
  attributeAffects(aFlowProjection, aOutput);
  attributeAffects(aFlowImage, aOutput);

  attributeAffects(aHatchLengthMax, aOutput);
  attributeAffects(aHatchLengthMin, aOutput);
  attributeAffects(aHatchLength, aOutput);

  attributeAffects(aHatchAngleMin, aOutput);
  attributeAffects(aHatchAngleMax, aOutput);
  attributeAffects(aHatchAngle, aOutput);

  attributeAffects(aTangentBlend, aOutput);

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

  MFloatMatrix targetRotationMatrix = data.inputValue(aTargetRotationMatrix).asFloatMatrix();
  targetRotationMatrix = mayaMath::rotationOnly(targetRotationMatrix);
  int numPoints = data.inputValue(aNumPoints).asInt();


  MFloatMatrix projection = data.inputValue(aFlowProjection).asFloatMatrix();
  MVector planeNormal = MVector(MFloatVector::zAxis * projection);


  int seed = data.inputValue(aSeed).asInt();

  MFloatPointArray points;
  MColorArray colors;
  MFloatVectorArray flow;
  MFloatVectorArray tangents;

  srand48(seed);
  getTargetPoints(strokes, points);
  getFlow(points, data, flow);
  getTangents(strokes, tangents);
  getTargetColors(strokes, colors);


  MObject thisObj = thisMObject();
  MRampAttribute rampAttr( thisObj, aColorRamp, &st );
  MFloatArray positions;
  MColorArray rampColors;

  rampAttr.sampleColorRamp	(numPoints,rampColors,&st);
  if (st != MS::kSuccess) {
    rampColors= MColorArray(numPoints, MColor(1.0f, 1.0f, 1.0f, 1.0f));
  }

  // std::vector<Stroke> sourceStrokes(*strokes);
  strokes->clear();

  MArrayDataHandle hHatches = data.inputArrayValue(aHatch);

  unsigned nInputs = hHatches.elementCount();
  // cerr << "nInputs" << nInputs << endl;
  for (unsigned i = 0; i < nInputs; i++, hHatches.next())
  {
    int index = hHatches.elementIndex(&st);
    MDataHandle hHatch = hHatches.inputValue(&st);
    if (st.error())
    {
      // cerr << "Error getting input hatch" << endl;
      continue;
    }
    // JPMDBG;
    addHatchSet(hHatch, planeNormal , numPoints, targetRotationMatrix, points, flow, tangents, colors,rampColors, strokes);
  }

  // float hatchRadius = data.inputValue(aHatchLength).asFloat() * 0.5f;

  // const unsigned count = points.length();

  // MPointArray editPoints(2);
  // MFloatArray weights(2);
  // MColorArray hcolors(2);

  // for (unsigned i = 0; i < count; i++)
  // {
  //   if (!flow[i].isEquivalent(MFloatVector::zero))
  //   {
  //     MFloatVector normal = flow[i].normal();

  //     MFloatVector p1 = points[i] + (normal * hatchRadius);
  //     MFloatVector p2 = points[i] - (normal * hatchRadius);

  //     editPoints[0] = p1;
  //     editPoints[1] = p2;

  //     weights[0] = 1.0f;
  //     weights[1] = 1.0f;

  //     hcolors[0] = colors[i];
  //     hcolors[1] = colors[i];
  //     // cerr << "--------" << endl;
  //     // cerr << "Stroke p:" << p1 << " " << p2 << " Colors:" << hcolors[0] << " " << hcolors[1] << endl;
  //     Stroke stroke(editPoints, weights, hcolors, pointDensity, 2, targetRotationMatrix);
  //     // cerr << "--------" << endl;
  //     if (stroke.valid())
  //     {
  //       strokes->push_back(stroke);
  //     }
  //   }
  // }

  return MS::kSuccess;
}

void hatchStrokes::addHatchSet(
    MDataHandle &h,
    const MVector &planeNormal,
    int numPoints,
    const MFloatMatrix &targetRotationMatrix,
    const MFloatPointArray &points,
    const MFloatVectorArray &flow,
    const MFloatVectorArray &tangents,
    const MColorArray &colors,
    const MColorArray &rampColors,
    std::vector<Stroke> *strokes) const
{
  // MStatus st;

  MDataHandle hHatchLength = h.child(aHatchLength);
  MDataHandle hHatchAngle = h.child(aHatchAngle);
  float tangentBlend = h.child(aTangentBlend).asFloat();
  float oneMinusTangentBlend = 1.0f - tangentBlend;

  float hatchLengthMin = hHatchLength.child(aHatchLengthMin).asFloat();
  float hatchLengthMax = hHatchLength.child(aHatchLengthMax).asFloat();
  float hatchAngleMin = hHatchAngle.child(aHatchAngleMin).asAngle().asRadians();
  float hatchAngleMax = hHatchAngle.child(aHatchAngleMax).asAngle().asRadians();

  const unsigned count = points.length();

  MPointArray editPoints(2);
 
  MFloatVector direction;
  float flowMagnitude = 0.0f; // used for mapping
  MFloatVector flowNormalized(MFloatVector::zero);

 

  for (unsigned i = 0; i < count; i++)
  {
    // cerr << "point iter:" << i << endl;
    if (flow[i].isEquivalent(MFloatVector::zero))
    {
      direction = tangents[i];
    }
    else
    {
      flowMagnitude = flow[i].length();
      flowNormalized = flow[i].normal();

      float hatchAngle = hatchAngleMin + (hatchAngleMax - hatchAngleMin) * drand48();
      MQuaternion q(double(hatchAngle), planeNormal);
      flowNormalized = MFloatVector(MVector(flowNormalized).rotateBy(q));



      if (tangentBlend == 0.0f)
      {
        direction = flowNormalized;
      }
      else if (tangentBlend == 1.0f)
      {
        direction = tangents[i];
      }
      else
      {
        direction = flowNormalized * oneMinusTangentBlend + tangents[i] * tangentBlend;
        if (!direction.isEquivalent(MFloatVector::zero))
        {
          direction.normalize();
        }
        else
        {
          // They cancel each other out, so just use the flow
          direction = flowNormalized;
        }
      }
    }

    float hatchLength = hatchLengthMin + (hatchLengthMax - hatchLengthMin) * drand48();
    float hatchRadius = hatchLength * 0.5f;

    // cerr << "hatchRadius:" << hatchRadius << endl;

    editPoints[0] = points[i] + (direction * hatchRadius);
    editPoints[1] = points[i] - (direction * hatchRadius);
    // cerr << "hatch direction:" << direction << endl;

    MColorArray hcolors(numPoints, colors[i] );
    for (int j = 0; j < numPoints; j++)
    {
      hcolors[j] *= rampColors[j];
    }

    // JPMDBG
    Stroke stroke(editPoints, hcolors, numPoints, targetRotationMatrix);
    // JPMDBG
    if (stroke.valid())
    {
      // JPMDBG;
      strokes->push_back(stroke);
    }
  }
}

void hatchStrokes::getFlow(
    const MFloatPointArray &points,
    MDataBlock &data,
    MFloatVectorArray &flow) const
{

  CImg<float> *pImage = cImgUtils::getFloatImage(data, aFlowImage);
  if (!pImage)
  {
    flow = MFloatVectorArray(points.length(), MFloatVector::zero);
    return;
  }
  // JPMDBG;
  int w = pImage->width();
  int h = pImage->height();

  if (!(w && h))
  {
    flow = MFloatVectorArray(points.length(), MFloatVector::zero);
    return;
  }

  MFloatMatrix norm;
  norm.setToIdentity();
  norm[0][0] = 0.5;
  norm[1][1] = 0.5;
  norm[3][0] = 0.5;
  norm[3][1] = 0.5;

  MFloatMatrix flop;
  flop.setToIdentity();
  flop[1][1] = -1.0;

  MFloatMatrix projection = data.inputValue(aFlowProjection).asFloatMatrix();

  MFloatMatrix projectionWorldToImage = projection.inverse() * norm;
  MFloatMatrix projectionImageToWorld = projectionWorldToImage.inverse();

  unsigned count = points.length();
  for (unsigned i = 0; i < count; i++)
  {
    MFloatPoint p = points[i] * projectionWorldToImage;

    float u = p.x;
    float v = p.y;

    MFloatVector f(MFloatVector::zero);
    if (u >= 0 && u < 1 && v >= 0 && v < 1)
    {

      float x, y;
      cImgUtils::toImageCoords(u, v, w, h, x, y);

      float dx = pImage->linear_atXY(x, y, 0, 0);
      float dy = -(pImage->linear_atXY(x, y, 0, 1));

      f = MFloatVector(dx, dy, 0.0f);
    }
    f = f * projectionImageToWorld;
    flow.append(f);
  }
}


      // if (u < 0.5f && v < 0.5f)
      // {
      //   color = MColor(1, 0, 0); // Bottom Left red
      // }
      // else if (u > 0.5f && v < 0.5f)
      // {
      //   color = MColor(0, 1, 0); // Bottom  Right green
      // }
      // else if (u > 0.5f && v > 0.5f)
      // {
      //   color = MColor(0, 0, 1); // Top  Right blue
      // }
      // else if (u < 0.5f && v > 0.5f)
      // {
      //   color = MColor(1, 1, 1); // Top  Left white
      // }
