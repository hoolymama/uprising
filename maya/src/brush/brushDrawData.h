#ifndef brushDrawData_H
#define brushDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>

#include <maya/MColor.h>
#include <maya/MFloatPointArray.h>
#include <maya/MUintArray.h>

class brushDrawData : public MUserData
{

public:
	brushDrawData();
	virtual ~brushDrawData();
 
	MFloatPointArray points;
	MUintArray indices;
	MColor color;
};

#endif

