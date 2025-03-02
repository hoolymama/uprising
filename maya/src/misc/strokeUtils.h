/**
 * @file strokeUtils.h
 * @brief Defines utility functions for working with strokes.
 * 
 * This file contains the StrokeUtils class which provides utility functions for
 * working with strokes, including curve creation, knot normalization, interpolation,
 * and resampling operations.
 */

#ifndef StrokeUtils_H
#define StrokeUtils_H

#include <maya/MObject.h>
#include "stroke.h"

/**
 * @class StrokeUtils
 * @brief Utility class for working with strokes.
 * 
 * The StrokeUtils class provides static utility functions for working with strokes,
 * including curve creation, knot normalization, interpolation, and resampling operations.
 * These functions are used throughout the painting system to manipulate and process strokes.
 */
class StrokeUtils
{

public:
	/**
	 * @brief Creates a curve from a stroke.
	 * @param stroke Pointer to the stroke to create a curve from.
	 * @param curveData Output object to store the created curve.
	 * @param normalizeKnots Whether to normalize the knots (default: true).
	 * @return MStatus indicating success or failure.
	 */
	static MStatus createCurve(const Stroke *stroke, MObject &curveData, bool normalizeKnots=true);

	/**
	 * @brief Creates a curve from an array of edit points.
	 * @param editPoints Array of points to create the curve from.
	 * @param curveData Output object to store the created curve.
	 * @param normalizeKnots Whether to normalize the knots (default: true).
	 * @return MStatus indicating success or failure.
	 */
	static MStatus createCurve(const MPointArray &editPoints, MObject &curveData, bool normalizeKnots=true);

	/**
	 * @brief Normalizes the knots of a curve.
	 * @param curveData The curve data to normalize.
	 * @return MStatus indicating success or failure.
	 */
	static MStatus normalizeKnots(MObject &curveData);

	/**
	 * @brief Interpolates a float value at a given parameter.
	 * @param values Array of float values to interpolate between.
	 * @param param Parameter value for interpolation (0.0 to 1.0).
	 * @return Interpolated float value.
	 */
	static float interpFloat(const MFloatArray &values, float param);

	/**
	 * @brief Interpolates a color value at a given parameter.
	 * @param colors Array of color values to interpolate between.
	 * @param param Parameter value for interpolation (0.0 to 1.0).
	 * @return Interpolated color value.
	 */
	static MColor interpColor(const MColorArray &colors, float param);

	/**
	 * @brief Resamples a curve with a specified density.
	 * @param editPoints Array of input edit points.
	 * @param density Points per centimeter.
	 * @param minimumPoints Minimum number of points to generate.
	 * @param resultPoints Output array to store the resampled points.
	 * @param resultTangents Output array to store the tangents at each point.
	 * @param resultParams Output array to store the parameter values.
	 * @return MStatus indicating success or failure.
	 */
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
