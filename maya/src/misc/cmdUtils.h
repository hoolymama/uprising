/**
 * @file cmdUtils.h
 * @brief Defines utility functions for working with Maya command data.
 * 
 * This file contains the CmdUtils class which provides utility functions for
 * working with Maya command data, particularly for flattening various Maya data
 * types into MDoubleArray objects for use in commands.
 */

#ifndef cmdUtils_H
#define cmdUtils_H

// #include <maya/MObject.h>
// #include <maya/MStatus.h>
#include <maya/MFloatMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MColorArray.h>
#include <maya/MFloatArray.h>

#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>

#include <maya/MVector.h>

/**
 * @class CmdUtils
 * @brief Utility class for working with Maya command data.
 * 
 * The CmdUtils class provides static utility functions for working with Maya command data,
 * particularly for flattening various Maya data types into MDoubleArray objects for use in commands.
 * These functions are used to convert complex data structures into a format that can be easily
 * passed to and from Maya commands.
 */
class CmdUtils {

public:
	/**
	 * @brief Flattens a MFloatMatrix into an MDoubleArray.
	 * @param mat The matrix to flatten.
	 * @param result The output array to store the flattened data.
	 */
	static void flatten(const MFloatMatrix &mat, MDoubleArray &result) ;

	/**
	 * @brief Flattens an MVectorArray into an MDoubleArray.
	 * @param arr The vector array to flatten.
	 * @param result The output array to store the flattened data.
	 */
	static void flatten(const MVectorArray &arr, MDoubleArray &result) ;

	/**
	 * @brief Flattens an MFloatVectorArray into an MDoubleArray.
	 * @param arr The float vector array to flatten.
	 * @param result The output array to store the flattened data.
	 */
	static void flatten(const MFloatVectorArray &arr, MDoubleArray &result) ;

	/**
	 * @brief Flattens an MColorArray into an MDoubleArray.
	 * @param arr The color array to flatten.
	 * @param result The output array to store the flattened data.
	 */
	static void flatten(const MColorArray &arr, MDoubleArray &result) ;

	/**
	 * @brief Flattens an MPointArray into an MDoubleArray.
	 * @param arr The point array to flatten.
	 * @param result The output array to store the flattened data.
	 */
	static void flatten(const MPointArray &arr, MDoubleArray &result) ;

	/**
	 * @brief Flattens an MFloatPointArray into an MDoubleArray.
	 * @param arr The float point array to flatten.
	 * @param result The output array to store the flattened data.
	 */
	static void flatten(const MFloatPointArray &arr, MDoubleArray &result) ;

	/**
	 * @brief Flattens an MVector into an MDoubleArray.
	 * @param v The vector to flatten.
	 * @param result The output array to store the flattened data.
	 */
	static void flatten(const MVector &v, MDoubleArray &result) ;

	/**
	 * @brief Flattens an MFloatArray into an MDoubleArray.
	 * @param arr The float array to flatten.
	 * @param result The output array to store the flattened data.
	 */
	static void flatten(const MFloatArray &arr, MDoubleArray &result) ;
};

#endif

