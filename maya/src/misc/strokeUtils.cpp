#include <maya/MFnNurbsCurveData.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDoubleArray.h>
#include <maya/MPointArray.h>

#include "strokeUtils.h"

 
MStatus StrokeUtils::createCurve(const Stroke *stroke, MObject &curveData)
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
