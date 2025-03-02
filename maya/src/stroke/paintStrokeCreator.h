/**
 * @file paintStrokeCreator.h
 * @brief Defines the paintStrokeCreator class for creating paint strokes in Maya.
 * 
 * This file contains the paintStrokeCreator class which extends strokeCreator to provide
 * base functionality for nodes that generate paint strokes. It includes methods for
 * calculating target weights, applying brush stroke specifications, and other
 * common operations needed when creating paint strokes.
 */

#ifndef _paintStrokeCreator_H
#define _paintStrokeCreator_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeCreator.h>
#include "stroke.h"
#include "skGraph.h"
#include "skChain.h"
#include "brush.h"

/**
 * @class paintStrokeCreator
 * @brief A base class for nodes that generate paint strokes.
 * 
 * The paintStrokeCreator class extends strokeCreator to provide base functionality
 * for nodes that generate paint strokes. It includes methods for calculating target
 * weights, applying brush stroke specifications, and other common operations needed
 * when creating paint strokes. This class is abstract and meant to be subclassed by
 * specific paint stroke creator node types.
 */
class paintStrokeCreator : public strokeCreator
{
public:
    /**
     * @brief Default constructor.
     */
    paintStrokeCreator();
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~paintStrokeCreator();
    
    /**
     * @brief Creator function for Maya's plugin system.
     * @return Pointer to a new paintStrokeCreator object.
     */
    static void *creator();
    
    /**
     * @brief Initialize attributes and connections for the node.
     * @return MStatus indicating success or failure.
     */
    static MStatus initialize();
    
    /**
     * @brief Post-constructor initialization.
     */
    virtual void postConstructor();
    
    /**
     * @brief Indicates whether this class is abstract.
     * @return True, as this is an abstract base class.
     */
    virtual bool isAbstractClass() const { return true; }

    /**
     * @brief Generates stroke geometry based on the node's attributes.
     * 
     * This implementation handles common paint stroke generation tasks and
     * applies brush stroke specifications to the generated strokes.
     * 
     * @param plug The plug to compute.
     * @param data The data block containing the node's data.
     * @param pStrokes Pointer to a vector to store the generated strokes.
     * @return MStatus indicating success or failure.
     */
    virtual MStatus generateStrokeGeometry(
        const MPlug &plug,
        MDataBlock &data,
        std::vector<Stroke> *pStrokes);

    /** @brief Unique type ID for this node type. */
    static MTypeId id;

    /**
     * @brief Applies brush stroke specifications to the generated strokes.
     * @param data The data block containing the node's data.
     * @param pOutStrokes Pointer to a vector of strokes to modify.
     */
    void applyBrushStrokeSpec(
        MDataBlock &data,
        std::vector<Stroke> *pOutStrokes) const;

protected:
    /**
     * @brief Calculates the target weight based on distance and brush width.
     * 
     * This method calculates the target weight based on the distance along the curve,
     * transition distances, and brush width.
     * 
     * @param distanceOnCurve The distance along the curve.
     * @param lowDistance The lower distance bound.
     * @param highDistance The upper distance bound.
     * @param lowTransitionDistance The lower transition distance.
     * @param highTransitionDistance The upper transition distance.
     * @param lowTransitionLength The lower transition length.
     * @param highTransitionLength The upper transition length.
     * @param radius The radius value.
     * @param brushWidth The brush width.
     * @return The calculated target weight.
     */
    static float calculateTargetWeight(
        const double &distanceOnCurve,
        const double &lowDistance,
        const double &highDistance,
        const double &lowTransitionDistance,
        const double &highTransitionDistance,
        const float &lowTransitionLength,
        const float &highTransitionLength,
        const float &radius,
        const float &brushWidth);

    /**
     * @brief Calculates the target weight based on distance.
     * 
     * This method calculates the target weight based on the distance along the curve
     * and transition distances.
     * 
     * @param distanceOnCurve The distance along the curve.
     * @param lowDistance The lower distance bound.
     * @param highDistance The upper distance bound.
     * @param lowTransitionDistance The lower transition distance.
     * @param highTransitionDistance The upper transition distance.
     * @param lowTransitionLength The lower transition length.
     * @param highTransitionLength The upper transition length.
     * @return The calculated target weight.
     */
    static float calculateTargetWeight(
        const double &distanceOnCurve,
        const double &lowDistance,
        const double &highDistance,
        const double &lowTransitionDistance,
        const double &highTransitionDistance,
        const float &lowTransitionLength,
        const float &highTransitionLength
        );

    /** @brief Canvas matrix attribute for defining the stroke space. */
    static MObject aCanvasMatrix;
    
    /** @brief Stroke length attribute. */
    static MObject aStrokeLength;
    
    /** @brief Minimum stroke advance attribute. */
    static MObject aMinimumStrokeAdvance;
    
    /** @brief Overlap attribute. */
    static MObject aOverlap;
    
    /** @brief Brush ID attribute. */
    static MObject aBrushId;
    
    /** @brief Paint ID attribute. */
    static MObject aPaintId;

    /** @brief Brush follow stroke flag attribute. */
    static MObject aBrushFollowStroke;
    
    /** @brief Split angle attribute. */
    static MObject aSplitAngle;
    
    /** @brief Split test interval attribute. */
    static MObject aSplitTestInterval;
    
    /** @brief Entry transition length attribute. */
    static MObject aEntryTransitionLength;
    
    /** @brief Exit transition length attribute. */
    static MObject aExitTransitionLength;
    
    /** @brief Minimum points attribute. */
    static MObject aMinimumPoints;
    
    /** @brief Apply brush bias flag attribute. */
    static MObject aApplyBrushBias;
    
    /** @brief Brush shape attribute. */
    static MObject aBrushShape;
    
    /** @brief Dither probability attribute. */
    static MObject aDitherProbability;
};

inline float paintStrokeCreator::calculateTargetWeight(
    const double &distanceOnCurve,
    const double &lowDistance,
    const double &highDistance,
    const double &lowTransitionDistance,
    const double &highTransitionDistance,
    const float &lowTransitionLength,
    const float &highTransitionLength,
    const float &radius,
    const float &brushWidth

)
{
    float transitionWeight = calculateTargetWeight(
        distanceOnCurve,
        lowDistance,
        highDistance,
        lowTransitionDistance,
        highTransitionDistance,
        lowTransitionLength,
        highTransitionLength
    ); 
   
    return fmin(transitionWeight, ((2.0f * radius) / brushWidth));
}

inline float paintStrokeCreator::calculateTargetWeight(
    const double &distanceOnCurve,
    const double &lowDistance,
    const double &highDistance,
    const double &lowTransitionDistance,
    const double &highTransitionDistance,
    const float &lowTransitionLength,
    const float &highTransitionLength
)
{
    const float epsilon = 0.0001;

    float transitionWeight = 1.0;
    if ((distanceOnCurve+epsilon) < lowTransitionDistance)
    {
        transitionWeight = float((distanceOnCurve - lowDistance) / lowTransitionLength);
    }
    else if (distanceOnCurve > (highTransitionDistance+epsilon))
    {
        transitionWeight = float((highDistance - distanceOnCurve) / highTransitionLength);
    }

    return transitionWeight;
}

#endif
