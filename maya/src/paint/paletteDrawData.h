

#ifndef PaletteDrawData_H
#define PaletteDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MMatrix.h>
#include <maya/MColor.h>
#include <maya/MStringArray.h>
#include "paintingGeom.h"
#include "paint.h"

class PaletteDrawData : public MUserData
{
public:
	PaletteDrawData();
	virtual ~PaletteDrawData();

 
	std::map<int, Paint> palette;
	float xPosition;
	float yPosition;
	float width;
	float height;
	bool displayId;
	bool displayName;
	bool displayRGB;
	
	
 
};

#endif
 