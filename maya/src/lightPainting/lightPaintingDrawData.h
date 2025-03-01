

#ifndef lightPaintingDrawData_H
#define lightPaintingDrawData_H

// #include <maya/MPxDrawOverride.h>
#include <maya/MFloatVector.h>
#include <maya/MUserData.h>
#include <vector>
#include <map>
// #include <maya/MDrawRegistry.h>
// #include <maya/MViewport2Renderer.h>
// #include <maya/MMatrix.h>
// #include <maya/MColor.h>
// #include <maya/MStringArray.h>
// #include "lightPaintingGeom.h"
#include "enums.h"
#include "brush.h"
#include "stroke.h"

class LightPaintingDrawData : public MUserData
{
public:
	LightPaintingDrawData();
	virtual ~LightPaintingDrawData();

	bool displayClusterPath;
	bool displayPivots;
	bool displayIds;
	bool displayParentIds;
	bool displayBrushIds;
	bool displayLayerIds;

	float pointSize;
	float lineLength;
	float lineThickness;

	float drawParam;
	float arrowheadSize;
	short displayTargets;

	PaintingEnums::TargetColorsDisplay targetDisplayMode; ///< RGB, white, or blend

	MFloatVector drawingNormal;

	MFloatVector idDisplayOffset;

	std::vector<Stroke> *strokes;
	std::map<int, Brush> *brushes;
	
	Brush *brush; 
};

#endif
