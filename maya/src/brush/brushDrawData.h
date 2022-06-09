#ifndef brushDrawData_H
#define brushDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>

#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MColorArray.h>
#include <maya/MUintArray.h>

class brushDrawData : public MUserData
{
public:
	brushDrawData();
	virtual ~brushDrawData();
 
	MFloatPointArray points;
	MFloatVectorArray normals;
	MColorArray colors;
	MUintArray indices;

	MPoint coneBase;
	double coneScale;
	
};

#endif

