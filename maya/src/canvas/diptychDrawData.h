

#ifndef diptychDrawData_H
#define diptychDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>

#include <maya/MColor.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>

class diptychDrawData : public MUserData
{

public:
	diptychDrawData();
	virtual ~diptychDrawData();
 

	MColor boardColor;
	MColor squareColor;
	MColor pinColor;
	MColor probeColor;
	

	bool mirror;
	// float paintingLocatorSize;

	bool displaySquareMatrix;
	bool displayBoardMatrix;
	bool displayPinMatrix;
	bool displayPaintingMatrix;
	bool displayProbes;

	MPointArray probePoints;
	float probePointSize;

	MVector up;
	MVector normal;
	MPoint origin;

	MPoint squareCenter;
	MPoint squareCenterMirror;

	double squareScaleX;
	double squareScaleY;

	MPoint boardCenter;
	MPoint boardCenterMirror;

	double boardScaleX;
	double boardScaleY;
	

	double pinScaleX;
	double pinScaleY;

	MPoint paintingXAxis;
	MPoint paintingYAxis;
	MPoint paintingZAxis;
	
	


};

#endif

