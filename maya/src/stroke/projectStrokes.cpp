
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MTransformationMatrix.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>

#include <maya/MMeshIntersector.h>

#include "strokeData.h"
#include "projectStrokes.h"
#include "stroke.h"
#include "mayaMath.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"
#include "strokeUtils.h"
#include "enums.h"

const float PI = 3.14159265359;
MObject projectStrokes::aTargetRotationMatrix;
MObject projectStrokes::aProjectionMatrix;
MObject projectStrokes::aProjectionPlaneDistance;
MObject projectStrokes::aMinimumTargetDistance;
MObject projectStrokes::aPointDensity;
MObject projectStrokes::aMinimumPoints;

MTypeId projectStrokes::id(k_projectStrokes);

projectStrokes::projectStrokes() {}

projectStrokes::~projectStrokes() {}

void *projectStrokes::creator()
{
  return new projectStrokes();
}

/// Post constructor
void projectStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus projectStrokes::initialize()
{
  MStatus st;
  MString method("projectStrokes::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;
  MFnUnitAttribute uAttr;
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;

  aTargetRotationMatrix = mAttr.create("targetRotationMatrix", "tmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  addAttribute(aTargetRotationMatrix);

  aProjectionMatrix = mAttr.create("projectionMatrix", "pmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  addAttribute(aProjectionMatrix);

  // aProjectionType = eAttr.create("projectionType", "ptyp", projectStrokes::kPerspective);
  // eAttr.addField("orthographic", projectStrokes::kOrthographic);
  // eAttr.addField("perspective", projectStrokes::kPerspective);
  // eAttr.setHidden(false);
  // eAttr.setKeyable(true);
  // st = addAttribute(aProjectionType);

  aProjectionPlaneDistance = nAttr.create("projectionPlaneDistance", "ppd", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(1.0f);
  addAttribute(aProjectionPlaneDistance);

  aMinimumTargetDistance = nAttr.create("minimumTargetDistance", "mtd", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(1.0f);
  addAttribute(aMinimumTargetDistance);

  aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(2.0);
  st = addAttribute(aPointDensity);

  aMinimumPoints = nAttr.create("minimumPoints", "mnpts", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(2);
  st = addAttribute(aMinimumPoints);

  attributeAffects(aTargetRotationMatrix, aOutput);
  attributeAffects(aProjectionMatrix, aOutput);
  // attributeAffects(aProjectionType, aOutput);
  attributeAffects(aProjectionPlaneDistance, aOutput);
  attributeAffects(aMinimumTargetDistance, aOutput);
  attributeAffects(aPointDensity, aOutput);
  attributeAffects(aMinimumPoints, aOutput);

  return (MS::kSuccess);
}

MStatus projectStrokes::mutate(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{
  MStatus st;

  float projectionPlaneDistance = data.inputValue(aProjectionPlaneDistance).asFloat();
  float minimumTargetDistance = data.inputValue(aMinimumTargetDistance).asFloat();


  MFloatMatrix projectionMatrix = data.inputValue(aProjectionMatrix).asFloatMatrix();
  MFloatMatrix projectionMatrixInverse = projectionMatrix.inverse();
  MFloatVector planeNormal = (MFloatVector::zNegAxis * projectionMatrix).normal();
  MFloatPoint viewPoint = MFloatPoint::origin * projectionMatrix;
  MFloatPoint planePoint = viewPoint + planeNormal * projectionPlaneDistance;

  MFloatMatrix targetRotationMatrix = data.inputValue(aTargetRotationMatrix).asFloatMatrix();
  targetRotationMatrix = mayaMath::rotationOnly(targetRotationMatrix);

  // projectStrokes::ProjectionType projectionType = projectStrokes::ProjectionType(data.inputValue(aProjectionType).asShort());


  float pointDensity = fmax(data.inputValue(aPointDensity).asFloat(), 0.001);
  int minimumPoints = data.inputValue(aMinimumPoints).asInt();

  std::vector<Stroke> sourceStrokes(*strokes);
  strokes->clear();

  std::vector<Stroke>::iterator siter = sourceStrokes.begin();
  std::vector<Stroke>::iterator senditer = sourceStrokes.end();

  for (; siter != senditer; siter++)
  {

    std::vector<Target>::const_iterator titer;
    std::vector<Target>::const_iterator tenditer = siter->targets().end();

    int len = siter->size();

    MPointArray editPoints;
    MFloatArray weights;
    MColorArray colors;

    unsigned t = 0;

    MPoint * lastPoint = nullptr;

    for (titer = siter->targets().begin(); titer != tenditer; titer++, t++)
    {

      // project the point to the plane
      MFloatPoint projectedPoint;

      MFloatVector viewDirection = (titer->position() - viewPoint).normal();

      bool hitPlane = mayaMath::rayIntersectsPlane(
          planePoint,
          planeNormal,
          viewPoint,
          viewDirection,
          projectedPoint);

      if (!hitPlane)
      {
        continue;
      }

      bool usePoint = true;

      if (lastPoint != nullptr)
      {
        float distance = (projectedPoint - MFloatPoint(*lastPoint)).length();
        if (distance < minimumTargetDistance)
        {
          usePoint = false;
        }
      }

      if (usePoint)
      {
        editPoints.append(projectedPoint);
        weights.append(titer->weight());
        colors.append(titer->color());
        lastPoint = &editPoints[editPoints.length() - 1];
      }
    }

    MFloatPointArray resultPoints;
    MFloatVectorArray resultTangents;
    MFloatArray resultParams;
    st = StrokeUtils::resampleCurve(
        editPoints,
        pointDensity,
        minimumPoints,
        resultPoints,
        resultTangents,
        resultParams);

    if (!st)
    {
      continue;
    }

    Stroke stroke(resultPoints, targetRotationMatrix);

    Stroke::target_iterator target = stroke.targets_begin();
    for (int i = 0; target != stroke.targets_end(); target++, i++)
    {
      float w = StrokeUtils::interpFloat(weights, resultParams[i]);
      MColor c = StrokeUtils::interpColor(colors, resultParams[i]);
      target->setWeight(w);
      target->setColor(c);
    }

    if (stroke.valid())
    {
      strokes->push_back(stroke);
    }
  }
  return MS::kSuccess;
}
