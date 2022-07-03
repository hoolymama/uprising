#ifndef _BrushStrokeSpec_
#define _BrushStrokeSpec_

#include <maya/MObject.h>
#include <maya/MAngle.h>
#include "enums.h"
#include "brush.h"

class BrushStrokeSpec
{
public:
    BrushStrokeSpec();
    ~BrushStrokeSpec();

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
    Brush::Shape shape;
};
#endif
