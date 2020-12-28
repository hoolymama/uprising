
#ifndef textureSampling_H
#define textureSampling_H

#include <maya/MObject.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MIntArray.h>
#include <maya/MStatus.h>

class TexUtils
{

public:
	static bool hasTexture(const MObject &node, const MObject &attribute);

	static MStatus getTextureName(const MObject &node, const MObject &attribute,
								  MString &name);



	static MStatus  sample3dTexture(
		const MObject &node, 
		const MObject &attribute,
		float scale,
		MFloatPointArray &points, 
		MFloatVectorArray &result);

	static MStatus  sample3dTexture(
		const MObject &node, 
		const MObject &attribute,
		float scale,
		MFloatPointArray &points, 
		MFloatArray &result);






	static MStatus sampleUVTexture(const MObject &node, const MObject &attribute,
								   MFloatArray &uVals,
								   MFloatArray &vVals, MFloatVectorArray &result);

	static MStatus sampleUVTexture(
		const MObject &node,
		const MObject &textureAttribute,
		MFloatArray &uVals,
		MFloatArray &vVals,
		int range,
		MIntArray &result);

	static MStatus sampleUVTexture(
		const MObject &node,
		const MObject &textureAttribute,
		MFloatArray &uVals,
		MFloatArray &vVals,
		const MObject &rampAttribute,
		int low, int high,
		MIntArray &result);

	static MStatus sampleUVTexture(
		const MObject &node,
		const MObject &textureAttribute,
		MFloatArray &uVals,
		MFloatArray &vVals,
		float low,
		float high,
		MFloatArray &result);

	static MStatus sampleUVGradient(
		const MObject &node,
		const MObject &textureAttribute,
		float sampleDistance,
		float scale,
		MFloatArray &uVals,
		MFloatArray &vVals,
		MFloatVectorArray &result,
		float boundaryWidth = 0.0f);
};

#endif