

#ifndef imagePointsDrawData_H
#define imagePointsDrawData_H



#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>
 
 
#include <maya/MColor.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>

class imagePointsDrawData : public MUserData 
{


public:

	imagePointsDrawData();
	virtual ~imagePointsDrawData();

	MColor color;
	MPointArray points;
	MDoubleArray radii;
	
	int pointSize;
	int lineThickness;
	bool displayPoints;
	bool displayCircles;
 	float circleDisplaySize;
};

#endif
