#ifndef _BrushStrokeSpec_
#define _BrushStrokeSpec_

#include <maya/MObject.h>
#include <maya/MAngle.h>
#include "enums.h"

class BrushStrokeSpec {
public:

	BrushStrokeSpec() {};
	~BrushStrokeSpec() {};

    float tiltStart;
    float tiltEnd;
    float twistStart;
    float twistEnd;
    float bankStart;
    float bankEnd;
    PaintingEnums::BrushRotateOrder order;
	bool follow;
    float entryTransition;
    float exitTransition;
    
    

};
#endif
