

#ifndef skGraphNodeDrawData_H
#define skGraphNodeDrawData_H



#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MMatrix.h>
#include <maya/MColor.h>
#include <maya/MPointArray.h>
#include <maya/MColorArray.h>
#include <maya/MIntArray.h>

class skGraphNodeDrawData : public MUserData {
public:

	skGraphNodeDrawData();
	virtual ~skGraphNodeDrawData();

	float pointSize       ;
	MColorArray colors     ;
	MPointArray points;
	MDoubleArray radius;
	MIntArray counts   ;
	bool drawEdges        ;
	bool drawPoints       ;
	bool drawCircles      ;

};



#endif
