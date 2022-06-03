

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
 

	MMatrix squareMatrix;
	MMatrix boardMatrix;
	MMatrix pinMatrix;
	MMatrix paintingMatrix;

	MColor boardColor;
	MColor squareColor;
	MColor pinColor;

	bool mirror;
	float paintingLocatorSize;



	bool displaySquareMatrix;
	bool displayBoardMatrix;
	bool displayPinMatrix;
	bool displayPaintingMatrix;



};

#endif

