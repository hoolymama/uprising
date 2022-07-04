#include <map>
#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <jMayaIds.h>
#include "errorMacros.h"

#include "brushShopNode.h"
#include "brushData.h"
#include "brushShopData.h"

MObject brushShopNode::aBrushes;
MObject brushShopNode::aOutput;

MTypeId brushShopNode::id(k_brushShopNode);

brushShopNode::brushShopNode() {}

brushShopNode::~brushShopNode() {}

void *brushShopNode::creator()
{
    return new brushShopNode();
}

void brushShopNode::postConstructor()
{
    MPxNode::postConstructor();
    setExistWithoutOutConnections(true);
}

MStatus brushShopNode::initialize()
{
    MStatus st;
    MString method("brushShopNode::initialize");

    inheritAttributesFrom("strokeMutator");

    MFnTypedAttribute tAttr;

    aBrushes = tAttr.create("brushes", "bsh", brushData::id);
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    tAttr.setArray(true);
    tAttr.setIndexMatters(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    addAttribute(aBrushes);

    aOutput = tAttr.create("output", "out", brushShopData::id);
    tAttr.setReadable(true);
    tAttr.setStorable(false);
    addAttribute(aOutput);

    st = attributeAffects(aBrushes, aOutput);

    return (MS::kSuccess);
}

MStatus brushShopNode::compute(const MPlug &plug, MDataBlock &data)
{
    MStatus st;

    if (plug != aOutput)
    {
        return (MS::kUnknownParameter);
    }


    std::map<int, Brush> brushes;
    st = collectBrushes(data, brushes);

    std::map<Brush::Shape, BrushRack>  brushShop;

    brushShop.insert(std::make_pair(Brush::kFlat,  BrushRack(brushes, Brush::kFlat)));
    brushShop.insert(std::make_pair(Brush::kRound, BrushRack(brushes, Brush::kRound)));

    MDataHandle hOutput = data.outputValue(aOutput);
    MFnPluginData fnOut;
    MTypeId kdid(brushShopData::id);
    MObject dOut = fnOut.create(kdid, &st);
    mser;
    brushShopData *newData = (brushShopData *)fnOut.data(&st);
    mser;
    *(newData->fGeometry) = brushShop;\
    hOutput.set(newData);
    hOutput.setClean();
    return (MS::kSuccess);
}


MStatus brushShopNode::collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const
{
  MStatus st;
  MArrayDataHandle ha = data.inputArrayValue(aBrushes, &st);
  msert;

  // brushes[-1] = Brush();

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

// MStatus brushShopNode::mutate(
//     const MPlug &plug,
//     MDataBlock &data,
//     std::vector<Stroke> *strokes) const
// {

//   MStatus st;
//   if (strokes->size() == 0)
//   {
//     return MS::kSuccess;
//   }
//   std::map<int, Brush> brushes;
//   st = collectBrushes(data, brushes);
//   msert;

//   bool shouldApplyLift = data.inputValue(aApplyLift).asBool();
//   bool shouldApplyBias = data.inputValue(aApplyBias).asBool();
//   bool shouldReassignBrushIds = data.inputValue(aReassignBrushIds).asBool();

//   if (shouldReassignBrushIds) {
//     assignBrushes(brushes, strokes );
//   }

//   std::map<int, Brush>::const_iterator brushIter;
//   std::vector<Stroke>::iterator currentStroke = strokes->begin();

//   for (unsigned i = 0; currentStroke != strokes->end(); currentStroke++)
//   {
//     int brushId = currentStroke->brushId();
//     brushIter = brushes.find(brushId);
//     if (brushIter == brushes.end())
//     {
//       brushIter = brushes.find(-1);
//     }
//     const Brush &brush = brushIter->second;

//     // get a pointer to the current stroke from the iterator.
//     Stroke *stroke = &(*currentStroke);

//     // Make a nurbs curve to serve as a utility.
//     MObject curveObject;
//     createCurve(stroke, curveObject);

//     setWeights(brush, curveObject, stroke);

//     MVectorArray tangents;
//     if (shouldApplyBias)
//     {
//       applyBias(brush, curveObject, stroke, tangents);
//     } else {
//       getTangents(curveObject, tangents);
//     }

//     applyRotation(brush, curveObject, tangents, stroke);

//     currentStroke->resetTangents();

//     if (shouldApplyLift)
//     {
//       applyLift(brush, stroke);
//     }
//   }

//   return MS::kSuccess;
// }

// void brushShopNode::assignBrushes(const std::map<int, Brush> &brushes, std::vector<Stroke> *strokes) const
// {
//  /*
//   Make a data structure that will allow us to arrange brushes by model.

//   A brushShop is a map of brushRacks. Currenly only two racks (flat brushes, round brushes)
//   Each brushRack is a map of brushModels. (davinci30, davinci22, etc)
//   Each brushModel contains a list of brushes of that model. (e.g. davinci30 has 3 brushes, davinci22 has 2 brushes)
//   */

//   std::map<Brush::Shape, BrushRack>  brushShop;
//   brushShop.insert(std::make_pair(Brush::kFlat,  BrushRack(brushes, Brush::kFlat)));
//   brushShop.insert(std::make_pair(Brush::kRound, BrushRack(brushes, Brush::kRound)));

//   // std::map<Brush::Shape, BrushRack>::const_iterator citer = brushShop.begin();
//   // for (;citer!=brushShop.end(); citer++)
//   // {
//   //   cerr << citer->second << endl;;
//   // }

//   std::vector<Stroke>::iterator stroke = strokes->begin();
//   for (; stroke != strokes->end(); stroke++)
//   {
//     // Get the atts we need.
//     float width = stroke->maxRadius()*2.0f;
//     int paintId = stroke->paintId();
//     Brush::Shape shape = stroke->brushStrokeSpec().shape;

//   // Loop over strokes and assign a brush to each.
//     std::map<Brush::Shape, BrushRack>::iterator iter = brushShop.find(shape);
//     if (iter == brushShop.end())
//     {
//       // Ignore the stroke with an invalid shape. (should be impossible anyway)
//       continue;
//     }
//     BrushRack &rack = iter->second;
//     int brushId = rack.getBrushId(width, paintId);
//     if (brushId == -1)
//     {
//       // No brush found for this stroke.
//       continue;
//     }
//     stroke->setBrushId(brushId);
//   }
// }

// void brushShopNode::setWeights(const Brush &brush, const MObject &curveObject, Stroke *stroke) const
// {
//   /*
//   Set the weights for each target of this stroke.

//   The weights are based on the radius of the stroke.
//   We also calculate the entry and exit weights (linear interpolation)
//   The final weight fdor each target is the min value of the two.
//   The maximum weight is 1.0=, since that represents the brush pushing as hard as it can.
//   */

//   MFnNurbsCurve curveFn(curveObject);
//   double curveLength = curveFn.length();

//   MDoubleArray knotVals;
//   curveFn.getKnots(knotVals);

//   float brushRadius = fmax(brush.width(), 0.01) * 0.5;

//   const BrushStrokeSpec &spec = stroke->brushStrokeSpec();
//   float entryTransition = spec.entryTransition;
//   float exitTransition = spec.exitTransition;
//   float totalTransition = entryTransition + exitTransition;
//   if (totalTransition > curveLength)
//   {
//     float shrinkBy = curveLength / totalTransition;
//     entryTransition *= shrinkBy;
//     exitTransition *= shrinkBy;
//   }

//   Stroke::target_iterator target = stroke->targets_begin();
//   for (int k = 2; target != stroke->targets_end(); target++, k++)
//   {

//     float transitionWeight = 1.0;
//     const double &param = knotVals[k];
//     float brushWeight = target->radius() / brushRadius;
//     double distance = curveFn.findLengthFromParam(param);
//     if (distance < entryTransition)
//     {
//       transitionWeight = float(distance / entryTransition);
//     }
//     else if (distance > (curveLength - exitTransition))
//     {
//       transitionWeight = float((curveLength - distance) / exitTransition);
//     }

//     target->setWeight(fmin(transitionWeight,brushWeight));
//   }
// }

// void brushShopNode::applyBias(const Brush &brush, const MObject& curveObject, Stroke *stroke, MVectorArray &tangents) const
// {
//   /*
//   Apply the forward bias to targets of this stroke.

//   Forward bioas is the distance we push each target forward along the stroke's path in order to
//   compensate for the fact that if a brush is leaning and pushing down, then the bristles bend, which
//   puts the contact point further back along the stroke.

//   The amount of bias is strongest when the weight is 1.0 because that's when the bristles are
//   bending most. It is weakest if the brush tip is only just touching the stroke. The coefficuents
//   are attributes of ewach brush and were attained by measuring the offset in a brush test.

//   */

//   MStatus st;
//   MFnNurbsCurve curveFn(curveObject);
//   double curveLength = curveFn.length();
//   tangents.clear();
//   MDoubleArray knotVals;
//   curveFn.getKnots(knotVals);

//   float forwardBias0 = fmax(0.0, brush.forwardBias0());
//   float forwardBias1 = fmax(0.0, brush.forwardBias1());

//   Stroke::target_iterator target = stroke->targets_begin();
//   for (int k = 2; target != stroke->targets_end(); target++, k++)
//   {
//     const float &weight = target->weight();
//     float forwardBias = (forwardBias1 * weight) + (forwardBias0 * (1.0f - weight));

//     const double &param = knotVals[k];
//     double distanceOnCurve = curveFn.findLengthFromParam(param, &st);
//     mser;
//     double biasedDist = distanceOnCurve + forwardBias;
//     double biasedCurveParam = curveFn.findParamFromLength(biasedDist, &st);

//     MPoint point;
//     st = curveFn.getPointAtParam(biasedCurveParam, point, MSpace::kObject);

//     MVector tangent = curveFn.tangent(biasedCurveParam).normal();
//     tangents.append(tangent);

//     float extraDist = biasedDist - curveLength;
//     if (extraDist > 0.0)
//     {
//       point += tangent * extraDist;
//     }
//     target->setPosition(point);
//   }
// }

// void brushShopNode::getTangents(const MObject& curveObject, MVectorArray &tangents) const
// {
//   /*
//   Get the tangents for each knot of the curve.
//   */
//   MStatus st;
//   MFnNurbsCurve curveFn(curveObject);
//   double curveLength = curveFn.length();
//   tangents.clear();
//   MDoubleArray knotVals;
//   curveFn.getKnots(knotVals);

//   for (int k = 2; k<knotVals.length() -2; k++)
//   {
//     tangents.append( curveFn.tangent(knotVals[k]).normal());
//   }
// }

// void brushShopNode::applyRotation(const Brush &brush, const MObject& curveObject, const MVectorArray &tangents, Stroke *stroke) const
// {
//   MStatus st;

//   const MFloatMatrix &brushMatrix = brush.matrix();

//   const BrushStrokeSpec &spec = stroke->brushStrokeSpec();
//   float tiltStart = spec.tiltStart;
//   float tiltEnd = spec.tiltEnd;
//   float twistStart = spec.twistStart;
//   float twistEnd = spec.twistEnd;
//   float bankStart = spec.bankStart;
//   float bankEnd = spec.bankEnd;
//   PaintingEnums::BrushRotateOrder order = spec.order;
//   bool follow = spec.follow;

//   MFnNurbsCurve curveFn(curveObject);
//   MDoubleArray knotVals;
//   curveFn.getKnots(knotVals);

//   Stroke::target_iterator target = stroke->targets_begin();
//   int k=2;
//   for (int i=0; target != stroke->targets_end(); target++, i++, k++)
//   {

//     MFloatMatrix mat(brushMatrix);
//     if (follow){
//       MFloatVector front = MFloatVector::yNegAxis * mat;
//       mat *= MFloatMatrix(MQuaternion(front, tangents[i]).asMatrix().matrix);
//     }

//     const MFloatMatrix & target_mat = target->matrix();
//     mat[3][0] = target_mat[3][0];
//     mat[3][1] = target_mat[3][1];
//     mat[3][2] = target_mat[3][2];

//     target->setMatrix(mat);

//     float param = knotVals[k];
//     float rparam = 1.0 - param;
//     float twistAngle = (twistStart * rparam) + (twistEnd * param);
//     float tiltAngle = (tiltStart * rparam) + (tiltEnd * param);
//     float bankAngle = (bankStart * rparam) + (bankEnd * param);
//     target->applyTiltBankTwist( tiltAngle,  bankAngle, twistAngle, order);
//   }
// }

// void brushShopNode::applyLift(const Brush &brush, Stroke *stroke) const
// {
//   const float epsilon = 0.0001f;
//   float height = brush.transitionHeight();
//   float power = brush.contactPower();

//   Stroke::target_iterator target = stroke->targets_begin();
//   for (; target != stroke->targets_end(); target++)
//   {
//     float lift = (1.0 - target->weight());
//     if (lift > epsilon)
//     {
//       lift = pow(lift, power) * height;
//       MFloatVector offset = target->zAxis() * -lift;
//       target->offsetBy(offset);
//     }
//   }
// }


// MStatus brushShopNode::createCurve(const Stroke *stroke, MObject &curveData) const
// {
//   MStatus st;

//   MPointArray editPoints;
//   Stroke::const_target_iterator ctiter = stroke->targets_begin();
//   for (; ctiter != stroke->targets_end(); ctiter++)
//   {
//     editPoints.append(ctiter->position());
//   }

//   MFnNurbsCurveData dataCreator;
//   curveData = dataCreator.create(&st);
//   msert;
//   MFnNurbsCurve curveFn;
//   curveFn.createWithEditPoints(editPoints, 3, MFnNurbsCurve::kOpen, false, false, false, curveData, &st);
//   msert;

//   MDoubleArray knotVals;
//   st = curveFn.getKnots(knotVals);
//   int numKnots = knotVals.length();
//   double recip = 1.0 / knotVals[(numKnots - 1)];
//   for (int i = 0; i < numKnots; ++i)
//   {
//     knotVals[i] = knotVals[i] * recip;
//   }
//   curveFn.setKnots(knotVals, 0, (numKnots - 1));
//   return MS::kSuccess;
// }
