#include <map>
#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnNurbsCurveData.h>
#include <maya/MFnPluginData.h>
#include "brushLifter.h"
#include <jMayaIds.h>
#include "errorMacros.h"
// #include "texUtils.h"

#include "brushData.h"

const float epsilon = 0.0001f;

MObject brushLifter::aBrushes;
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

  aBrushes = tAttr.create("brushes", "bsh", brushData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setIndexMatters(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aBrushes);

  st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aApplyBias, aOutput);
  st = attributeAffects(aApplyLift, aOutput);

  return (MS::kSuccess);
}

/**
 * Manage the mutation
 *
 * Start off by assigning a brush to every stroke. 
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
  std::map<int, Brush> brushes;
  st = collectBrushes(data, brushes);
  msert;

  bool shouldApplyLift = data.inputValue(aApplyLift).asBool();
  bool shouldApplyBias = data.inputValue(aApplyBias).asBool();

  assignBrushes(brushes, strokes );



  std::map<int, Brush>::const_iterator brushIter;
  std::vector<Stroke>::iterator currentStroke = strokes->begin();

  for (unsigned i = 0; currentStroke != strokes->end(); currentStroke++)
  {
    int brushId = currentStroke->brushId();
    brushIter = brushes.find(brushId);
    if (brushIter == brushes.end())
    {
      brushIter = brushes.find(-1);
    }
    const Brush &brush = brushIter->second;
    
    // get a pointer to the current stroke from the iterator.
    Stroke *stroke = &(*currentStroke);

    // Make a nurbs curve to serve as a utility.
    MObject curveObject;
    createCurve(stroke, curveObject);

    setWeights(brush, curveObject, stroke);

    MVectorArray tangents;
    if (shouldApplyBias)
    {
      applyBias(brush, curveObject, stroke, tangents);
    } else {
      getTangents(curveObject, tangents);
    }

    applyRotation(brush, curveObject, tangents, stroke);

    currentStroke->resetTangents();

    if (shouldApplyLift)
    {
      applyLift(brush, stroke);
    }
  }
  
  return MS::kSuccess;
}

void brushLifter::assignBrushes(const std::map<int, Brush> &brushes, std::vector<Stroke> *strokes) const
{
  // std::vector<Stroke>::iterator currentStroke = strokes->begin();
  // for (; currentStroke != strokes->end(); currentStroke++)
  // {
  //   // currentStroke->setBrushId(currentStroke->brushId());
  // }
}
void brushLifter::setWeights(const Brush &brush, const MObject &curveObject, Stroke *stroke) const
{
  MFnNurbsCurve curveFn(curveObject);
  double curveLength = curveFn.length(epsilon);
  // cerr << "curve length: " << curveLength << endl;

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
    if (distance < entryTransition)
    {
      transitionWeight = float(distance / entryTransition);
    }
    else if (distance > (curveLength - exitTransition))
    {
      transitionWeight = float((curveLength - distance) / exitTransition);
    }

    target->setWeight(fmin(transitionWeight,brushWeight));
  }
}


void brushLifter::applyBias(const Brush &brush, const MObject& curveObject, Stroke *stroke, MVectorArray &tangents) const
{
  MStatus st;
  MFnNurbsCurve curveFn(curveObject);
  double curveLength = curveFn.length(epsilon);
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

void brushLifter::getTangents(const MObject& curveObject, MVectorArray &tangents) const
{
  MStatus st;
  MFnNurbsCurve curveFn(curveObject);
  double curveLength = curveFn.length(epsilon);
  tangents.clear();
  MDoubleArray knotVals;
  curveFn.getKnots(knotVals);
 
  for (int k = 2; k<knotVals.length() -2; k++)
  {
    tangents.append( curveFn.tangent(knotVals[k]).normal());
  }
}

void brushLifter::applyRotation(const Brush &brush, const MObject& curveObject, const MVectorArray &tangents, Stroke *stroke) const
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
  int k=2;
  for (int i=0; target != stroke->targets_end(); target++, i++, k++)
  {

    MFloatMatrix mat(brushMatrix);
    if (follow){
      MFloatVector front = MFloatVector::yNegAxis * mat;
      mat *= MFloatMatrix(MQuaternion(front, tangents[i]).asMatrix().matrix);
    }

    const MFloatMatrix & target_mat = target->matrix();
    mat[3][0] = target_mat[3][0];
    mat[3][1] = target_mat[3][1];
    mat[3][2] = target_mat[3][2];

    target->setMatrix(mat);

    float param = knotVals[k];
    float rparam = 1.0 - param;
    float twistAngle = (twistStart * rparam) + (twistEnd * param);
    float tiltAngle = (tiltStart * rparam) + (tiltEnd * param);
    float bankAngle = (bankStart * rparam) + (bankEnd * param);
    target->applyTiltBankTwist( tiltAngle,  bankAngle, twistAngle, order);
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

MStatus brushLifter::collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const
{
  MStatus st;
  MArrayDataHandle ha = data.inputArrayValue(aBrushes, &st);
  msert;

  brushes[-1] = Brush();

  unsigned nPlugs = ha.elementCount();
  for (unsigned i = 0; i < nPlugs; i++, ha.next())
  {
    int index = ha.elementIndex(&st);
    if (st.error())
    {
      continue;
    }
    MDataHandle h = ha.inputValue(&st);
    if (st.error())
    {
      continue;
    }

    MObject d = h.data();
    MFnPluginData fnP(d, &st);
    if (st.error())
    {
      continue;
    }
    brushData *bData = (brushData *)fnP.data();

    brushes[index] = *(bData->fGeometry);
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
