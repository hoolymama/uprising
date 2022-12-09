

#ifndef imagePointsDrawData_H
#define imagePointsDrawData_H



#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>
 
 
#include <maya/MColor.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>

class imagePointsDrawData : public MUserData 
{


public:

	imagePointsDrawData();
	virtual ~imagePointsDrawData();

	MColor color;
	MColor gradientColor;
	
	MPointArray points;
	MPointArray lineList;
	MDoubleArray radii;
	
	int pointSize;
	int lineThickness;
	bool displayPoints;
	bool displayOrder;
	bool displayCircles;
 	float circleDisplaySize;
	bool displayGradient;
	float gradientLineLength;
};

#endif
