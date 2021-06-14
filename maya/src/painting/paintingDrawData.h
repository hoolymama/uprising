

#ifndef PaintingDrawData_H
#define PaintingDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MMatrix.h>
#include <maya/MColor.h>
#include <maya/MStringArray.h>
#include "paintingGeom.h"
#include "enums.h"

class PaintingDrawData : public MUserData
{
public:
	PaintingDrawData();
	virtual ~PaintingDrawData();

	bool displayClusterPath;
	bool displayPivots;
	bool displayIds;
	bool displayParentIds;
	bool displayLayerIds;
	bool displayBrushIds;
	bool displayPaintIds;
	bool displayRepeatIds;
	bool displayApproachTargets;

	bool displayContactWidth;

	float pointSize;
	float lineLength;
	float lineThickness;
	float stackGap;
	float drawParam;
	MColor wireColor;
	MColor clusterPathColor;
	
	
	float arrowheadSize;

	short displayTargets;
	short displayTargetColors;

	MFloatVector drawingNormal;

	MFloatVector idDisplayOffset;

	MFloatVectorArray stackOffsets;

	paintingGeom *geom;
};

#endif
 