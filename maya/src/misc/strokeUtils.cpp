#include <maya/MFnNurbsCurveData.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDoubleArray.h>
#include <maya/MPointArray.h>

#include "strokeUtils.h"

const double epsilon = 0.0001;

 
MStatus StrokeUtils::createCurve(const Stroke *stroke, MObject &curveData, bool normalizeKnots)
{
  MStatus st;

  MPointArray editPoints;
  Stroke::const_target_iterator ctiter = stroke->targets_begin();
  for (; ctiter != stroke->targets_end(); ctiter++)
  {
    editPoints.append(ctiter->position());
  }
return createCurve(editPoints, curveData, normalizeKnots);

}


MStatus StrokeUtils::createCurve(const MPointArray &editPoints, MObject &curveData, bool normalizeKnots)
{
  MStatus st;

  MFnNurbsCurveData dataCreator;
  curveData = dataCreator.create(&st);
  msert;
  MFnNurbsCurve curveFn;
  curveFn.createWithEditPoints(
	editPoints, 3, MFnNurbsCurve::kOpen, false, false, false, curveData, &st);
  msert;
  if (normalizeKnots)
  {
	st = StrokeUtils::normalizeKnots(curveData);
	msert;
  }

  return MS::kSuccess;
}



MStatus StrokeUtils::normalizeKnots(MObject &curveData)
{
MStatus st;
  MFnNurbsCurve curveFn(curveData, &st);
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




float StrokeUtils::interpFloat(const MFloatArray &values, float param)
{
	int len = values.length();
	if (len < 2)
	{
		return 1.0;
	}
	int last = (len - 1);
	if (param >= 1.0f)
	{
		return values[last];
	}
	else if (param <= 0.0f)
	{
		return values[0];
	}
	float t = param * last;
	float r = t - floor(t);
	int lindex = int(t);
	return (values[lindex] * (1 - r)) + (values[(lindex + 1)] * (r));
}

MColor StrokeUtils::interpColor(const MColorArray &colors, float param)
{
	int len = colors.length();
	if (len < 2)
	{
		return MColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	int last = (len - 1);
	if (param >= 1.0f)
	{
		return colors[last];
	}
	else if (param <= 0.0f)
	{
		return colors[0];
	}
	float t = param * last;
	float r = t - floor(t);
	int lindex = int(t);
	return (colors[lindex] * (1 - r)) + (colors[(lindex + 1)] * (r));
}

MStatus StrokeUtils::resampleCurve(
	const MPointArray &editPoints,
	float density,
	int minimumPoints,
	MFloatPointArray &resultPoints,
	MFloatVectorArray &resultTangents,
	MFloatArray &resultParams
	)
{

	MObject curveData;
	MStatus st = StrokeUtils::createCurve(editPoints, curveData);
	msert;
	MFnNurbsCurve curveFn(curveData, &st);
	msert;

	double curveLength = curveFn.length(epsilon);
	int numPoints = int(density * curveLength);
	numPoints = std::max(numPoints, minimumPoints);
	double gap = curveLength / (numPoints - 1);

	for (unsigned i = 0; i < numPoints; i++)
	{
		float curveDist = i * gap;
		double curveParam = curveFn.findParamFromLength(curveDist);
		resultTangents.append(curveFn.tangent(curveParam).normal());

		MPoint point;
		st = curveFn.getPointAtParam(curveParam, point, MSpace::kObject);
		msert;
		resultPoints.append(point);
		resultParams.append(curveParam);
	}
	return MS::kSuccess;
}
