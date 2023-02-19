

#ifndef StrokeUtils_H
#define StrokeUtils_H

#include <maya/MObject.h>
#include "stroke.h"

class StrokeUtils
{

public:
	static MStatus createCurve(const Stroke *stroke, MObject &curveData, bool normalizeKnots=true);

	static MStatus createCurve(const MPointArray &editPoints, MObject &curveData, bool normalizeKnots=true);

	static MStatus normalizeKnots(MObject &curveData);

	static float interpFloat(const MFloatArray &values, float param);

	static MColor interpColor(const MColorArray &colors, float param);

	static MStatus resampleCurve(
	const MPointArray &editPoints,
	float density, // pts per cm
	int minimumPoints,
	MFloatPointArray &resultPoints,
	MFloatVectorArray &resultTangents,
	MFloatArray &resultParams
	);


};

#endif
