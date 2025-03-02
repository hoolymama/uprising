/**
 * @file brushStrokeSpec.h
 * @brief Defines the specification for brush strokes.
 *
 * This file implements the specification for brush strokes:
 * - Defines parameters for brush orientation during strokes
 * - Controls brush tilt, twist, and bank angles
 * - Manages entry and exit transitions
 * - Specifies brush shape for the stroke
 */

#ifndef _BrushStrokeSpec_
#define _BrushStrokeSpec_

#include <maya/MObject.h>
#include <maya/MAngle.h>
#include "enums.h"
#include "brush.h"

/**
 * @class BrushStrokeSpec
 * @brief Specification for brush strokes.
 *
 * BrushStrokeSpec defines parameters for brush strokes:
 * - Controls brush orientation (tilt, twist, bank)
 * - Manages start and end angles
 * - Defines entry and exit transitions
 * - Specifies brush shape and rotation order
 */
class BrushStrokeSpec
{
public:
    /**
     * @brief Default constructor.
     */
    BrushStrokeSpec();
    
    /**
     * @brief Destructor.
     */
    ~BrushStrokeSpec();

    float tiltStart;                   ///< Starting tilt angle
    float tiltEnd;                     ///< Ending tilt angle
    float twistStart;                  ///< Starting twist angle
    float twistEnd;                    ///< Ending twist angle
    float bankStart;                   ///< Starting bank angle
    float bankEnd;                     ///< Ending bank angle
    PaintingEnums::BrushRotateOrder order; ///< Rotation order for brush orientation
    bool follow;                       ///< Whether brush follows stroke direction
    float entryTransition;             ///< Entry transition length
    float exitTransition;              ///< Exit transition length
    Brush::Shape shape;                ///< Brush shape for the stroke
};
#endif
