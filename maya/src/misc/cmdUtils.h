

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

class CmdUtils {

public:

	static void flatten(const MFloatMatrix &mat, MDoubleArray &result) ;

	static void flatten(const MVectorArray &arr, MDoubleArray &result) ;

	static void flatten(const MFloatVectorArray &arr, MDoubleArray &result) ;

	static void flatten(const MColorArray &arr, MDoubleArray &result) ;

	static void flatten(const MPointArray &arr, MDoubleArray &result) ;

	static void flatten(const MFloatPointArray &arr, MDoubleArray &result) ;

	static void flatten(const MVector &v, MDoubleArray &result) ;

	static void flatten(const MFloatArray &arr, MDoubleArray &result) ;
};


#endif

