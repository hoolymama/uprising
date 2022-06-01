

#ifndef diptychDrawData_H
#define diptychDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>

#include <maya/MColor.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>

class diptychDrawData : public MUserData
{

public:
	diptychDrawData();
	virtual ~diptychDrawData();

	float boardOffset;

	float boardSizeX;
	float boardSizeY;
	MColor boardColor;

	float squareOffsetX;
	float squareOffsetY;

	float squareSize;
	MColor squareColor;

	bool mirror;
};

#endif
