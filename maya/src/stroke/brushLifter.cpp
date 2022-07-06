#include <map>
#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnNurbsCurveData.h>
#include <maya/MFnPluginData.h>
#include <jMayaIds.h>
#include "errorMacros.h"

#include "brushLifter.h"
#include "brushData.h"
#include "brushShopData.h"

MObject brushLifter::aBrushShop;
MObject brushLifter::aReassignBrushIds;
MObject brushLifter::aApplyBias;
MObject brushLifter::aApplyLift;

MTypeId brushLifter::id(k_brushLifter);

brushLifter::brushLifter() {}

brushLifter::~brushLifter() {}

void *brushLifter::creator()
{
  return new brushLifter();
}


void brushLifter::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus brushLifter::initialize()
{
  MStatus st;
  MString method("brushLifter::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnTypedAttribute tAttr;
  MFnNumericAttribute nAttr;
  MFnEnumAttribute eAttr;

  aReassignBrushIds = nAttr.create("reassignBrushIds", "rbi", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aReassignBrushIds);
  mser;

  aApplyBias = nAttr.create("applyBias", "abs", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aApplyBias);
  mser;

  aApplyLift = nAttr.create("applyLift", "alf", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aApplyLift);
  mser;

  aBrushShop = tAttr.create("brushShop", "shop", brushShopData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aBrushShop);

  // st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aBrushShop, aOutput);
  st = attributeAffects(aReassignBrushIds, aOutput);
  st = attributeAffects(aApplyBias, aOutput);
  st = attributeAffects(aApplyLift, aOutput);

  return (MS::kSuccess);
}

/**
 * Manage the mutation
 *
 * Start off by selecting the best brush for every stroke.
 * Then for each stroke:
 *    Set weights from radius
 *    Apply forward bias
 *    Apply rotation
 *    Apply lift
 *
 */
MStatus brushLifter::mutate(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{

  MStatus st;
  if (strokes->size() == 0)
  {
    return MS::kSuccess;
  }

  // std::map<int, Brush> brushes;
  // st = collectBrushes(data, brushes);
  BrushShop brushShop;

  st = getBrushShop(data, brushShop);
  std::map<int, Brush> brushes;
  brushShop.getBrushes(brushes);

  if (!brushes.size())
  {
    return MS::kFailure;
  }
  msert;

  bool shouldApplyLift = data.inputValue(aApplyLift).asBool();
  bool shouldApplyBias = data.inputValue(aApplyBias).asBool();
  bool shouldReassignBrushIds = data.inputValue(aReassignBrushIds).asBool();

  if (shouldReassignBrushIds)
  {
    assignBrushes(brushShop, strokes);
  }

  std::map<int, Brush>::const_iterator brushIter;
  std::vector<Stroke>::iterator strokeIter = strokes->begin();

  for (unsigned i = 0; strokeIter != strokes->end(); strokeIter++)
  {
    int brushId = strokeIter->brushId();
    brushIter = brushes.find(brushId);
    if (brushIter == brushes.end())
    {
      brushIter = brushes.find(-1);
    }
    const Brush &brush = brushIter->second;

    // get a pointer to the current stroke from the iterator.
    Stroke *stroke = &(*strokeIter);

    // Make a nurbs curve to serve as a utility.
    MObject curveObject;
    createCurve(stroke, curveObject);

    setWeights(brush, curveObject, stroke);

    MVectorArray tangents;
    if (shouldApplyBias)
    {
      applyBias(brush, curveObject, stroke, tangents);
    }
    else
    {
      getTangents(curveObject, tangents);
    }

    applyRotation(brush, curveObject, tangents, stroke);

    strokeIter->resetTangents();

    if (shouldApplyLift)
    {
      applyLift(brush, stroke);
    }
  }

  return MS::kSuccess;
}

void brushLifter::assignBrushes(BrushShop &brushShop, std::vector<Stroke> *strokes) const
{

  MStatus st;
  std::vector<Stroke>::iterator stroke = strokes->begin();
  for (; stroke != strokes->end(); stroke++)
  {
    // Get the atts we need.
    float width = stroke->maxRadius() * 2.0f;
    int paintId = stroke->paintId();
    Brush::Shape shape = stroke->brushStrokeSpec().shape;


    int brushId = brushShop.findBrushId(width, paintId, shape, &st);
    if (st.error())
    {
      continue;
    }
    stroke->setBrushId(brushId);
  }
}

void brushLifter::setWeights(const Brush &brush, const MObject &curveObject, Stroke *stroke) const
{
  /*
  Set the weights for each target of this stroke.

  The weights are based on the radius of the stroke.
  We also calculate the entry and exit weights (linear interpolation)
  The final weight for each target is the min value of the two.
  The maximum weight is 1.0=, since that represents the brush pushing as hard as it can.
  */
  const double epsilon = 0.001;

  MFnNurbsCurve curveFn(curveObject);
  double curveLength = curveFn.length();

  MDoubleArray knotVals;
  curveFn.getKnots(knotVals);

  float brushRadius = fmax(brush.width(), 0.01) * 0.5;

  const BrushStrokeSpec &spec = stroke->brushStrokeSpec();
  float entryTransition = spec.entryTransition;
  float exitTransition = spec.exitTransition;
  float totalTransition = entryTransition + exitTransition;
  if (totalTransition > curveLength)
  {
    float shrinkBy = curveLength / totalTransition;
    entryTransition *= shrinkBy;
    exitTransition *= shrinkBy;
  }

  Stroke::target_iterator target = stroke->targets_begin();
  for (int k = 2; target != stroke->targets_end(); target++, k++)
  {

    float transitionWeight = 1.0;
    const double &param = knotVals[k];
    float brushWeight = target->radius() / brushRadius;
    double distance = curveFn.findLengthFromParam(param);
    if (distance+epsilon < entryTransition)
    {
      transitionWeight = float(distance / entryTransition);
    }
    else if ((distance-epsilon) > (curveLength - exitTransition))
    {
      // cerr << "distance" << distance << " curveLength" << curveLength << " exitTransition" << exitTransition << endl;
      // cerr << "DOING TAPER = " <<  ((distance-epsilon) > (curveLength - exitTransition)) << endl;
      transitionWeight = float((curveLength - distance) / exitTransition);
    }

    target->setWeight(fmin(transitionWeight, brushWeight ));
  }
}

void brushLifter::applyBias(const Brush &brush, const MObject &curveObject, Stroke *stroke, MVectorArray &tangents) const
{
  /*
  Apply the forward bias to targets of this stroke.

  Forward bioas is the distance we push each target forward along the stroke's path in order to
  compensate for the fact that if a brush is leaning and pushing down, then the bristles bend, which
  puts the contact point further back along the stroke.

  The amount of bias is strongest when the weight is 1.0 because that's when the bristles are
  bending most. It is weakest if the brush tip is only just touching the stroke. The coefficuents
  are attributes of ewach brush and were attained by measuring the offset in a brush test.

  */

  MStatus st;
  MFnNurbsCurve curveFn(curveObject);
  double curveLength = curveFn.length();
  tangents.clear();
  MDoubleArray knotVals;
  curveFn.getKnots(knotVals);

  float forwardBias0 = fmax(0.0, brush.forwardBias0());
  float forwardBias1 = fmax(0.0, brush.forwardBias1());

  Stroke::target_iterator target = stroke->targets_begin();
  for (int k = 2; target != stroke->targets_end(); target++, k++)
  {
    const float &weight = target->weight();
    float forwardBias = (forwardBias1 * weight) + (forwardBias0 * (1.0f - weight));

    const double &param = knotVals[k];
    double distanceOnCurve = curveFn.findLengthFromParam(param, &st);
    mser;
    double biasedDist = distanceOnCurve + forwardBias;
    double biasedCurveParam = curveFn.findParamFromLength(biasedDist, &st);

    MPoint point;
    st = curveFn.getPointAtParam(biasedCurveParam, point, MSpace::kObject);

    MVector tangent = curveFn.tangent(biasedCurveParam).normal();
    tangents.append(tangent);

    float extraDist = biasedDist - curveLength;
    if (extraDist > 0.0)
    {
      point += tangent * extraDist;
    }
    target->setPosition(point);
  }
}

void brushLifter::getTangents(const MObject &curveObject, MVectorArray &tangents) const
{
  /*
  Get the tangents for each knot of the curve.
  */
  MStatus st;
  MFnNurbsCurve curveFn(curveObject);
  double curveLength = curveFn.length();
  tangents.clear();
  MDoubleArray knotVals;
  curveFn.getKnots(knotVals);

  for (int k = 2; k < knotVals.length() - 2; k++)
  {
    tangents.append(curveFn.tangent(knotVals[k]).normal());
  }
}

void brushLifter::applyRotation(const Brush &brush, const MObject &curveObject, const MVectorArray &tangents, Stroke *stroke) const
{
  MStatus st;

  const MFloatMatrix &brushMatrix = brush.matrix();

  const BrushStrokeSpec &spec = stroke->brushStrokeSpec();
  float tiltStart = spec.tiltStart;
  float tiltEnd = spec.tiltEnd;
  float twistStart = spec.twistStart;
  float twistEnd = spec.twistEnd;
  float bankStart = spec.bankStart;
  float bankEnd = spec.bankEnd;
  PaintingEnums::BrushRotateOrder order = spec.order;
  bool follow = spec.follow;

  MFnNurbsCurve curveFn(curveObject);
  MDoubleArray knotVals;
  curveFn.getKnots(knotVals);

  Stroke::target_iterator target = stroke->targets_begin();
  int k = 2;
  for (int i = 0; target != stroke->targets_end(); target++, i++, k++)
  {

    MFloatMatrix mat(brushMatrix);
    if (follow)
    {
      MFloatVector front = MFloatVector::yNegAxis * mat;
      mat *= MFloatMatrix(MQuaternion(front, tangents[i]).asMatrix().matrix);
    }

    const MFloatMatrix &target_mat = target->matrix();
    mat[3][0] = target_mat[3][0];
    mat[3][1] = target_mat[3][1];
    mat[3][2] = target_mat[3][2];

    target->setMatrix(mat);

    float param = knotVals[k];
    float rparam = 1.0 - param;
    float twistAngle = (twistStart * rparam) + (twistEnd * param);
    float tiltAngle = (tiltStart * rparam) + (tiltEnd * param);
    float bankAngle = (bankStart * rparam) + (bankEnd * param);
    target->applyTiltBankTwist(tiltAngle, bankAngle, twistAngle, order);
  }
}

void brushLifter::applyLift(const Brush &brush, Stroke *stroke) const
{
  const float epsilon = 0.0001f;
  float height = brush.transitionHeight();
  float power = brush.contactPower();

  Stroke::target_iterator target = stroke->targets_begin();
  for (; target != stroke->targets_end(); target++)
  {
    float lift = (1.0 - target->weight());
    if (lift > epsilon)
    {
      lift = pow(lift, power) * height;
      MFloatVector offset = target->zAxis() * -lift;
      target->offsetBy(offset);
    }
  }
}

MStatus brushLifter::getBrushShop(
    MDataBlock &data, BrushShop &brushShop) const
{
  MStatus st;
  MDataHandle h = data.inputValue(aBrushShop, &st);
  msert;
  MObject d = h.data();
  MFnPluginData fnP(d, &st);
  if (st.error())
  {
    return st;
  }
  brushShopData *bData = (brushShopData *)fnP.data(&st);
  msert;
  brushShop = *(bData->fGeometry);
  if (brushShop.racks.size() == 0)
  {
    return MS::kFailure;
  }

  return MS::kSuccess;
}

MStatus brushLifter::createCurve(const Stroke *stroke, MObject &curveData) const
{
  MStatus st;

  MPointArray editPoints;
  Stroke::const_target_iterator ctiter = stroke->targets_begin();
  for (; ctiter != stroke->targets_end(); ctiter++)
  {
    editPoints.append(ctiter->position());
  }

  MFnNurbsCurveData dataCreator;
  curveData = dataCreator.create(&st);
  msert;
  MFnNurbsCurve curveFn;
  curveFn.createWithEditPoints(editPoints, 3, MFnNurbsCurve::kOpen, false, false, false, curveData, &st);
  msert;

  MDoubleArray knotVals;
  st = curveFn.getKnots(knotVals);
  int numKnots = knotVals.length();
  double recip = 1.0 / knotVals[(numKnots - 1)];
  for (int i = 0; i < numKnots; ++i)
  {
    knotVals[i] = knotVals[i] * recip;
  }
  curveFn.setKnots(knotVals, 0, (numKnots - 1));
  return MS::kSuccess;
}
