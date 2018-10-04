
#ifndef textureSampling_H
#define textureSampling_H

#include <maya/MObject.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MIntArray.h>
#include <maya/MStatus.h>


bool hasTexture(const MObject &node, const MObject &attribute);

MStatus getTextureName(const MObject &node, const MObject &attribute,
                       MString &name) ;

MStatus sampleUVTexture(const MObject &node, const MObject &attribute,
                        MFloatArray &uVals,
                        MFloatArray &vVals, MFloatVectorArray &result) ;


MStatus sampleUVTexture(const MObject &node, const MObject &attribute,
                        MFloatArray &uVals,
                        MFloatArray &vVals, MIntArray &result, short range) ;
#endif