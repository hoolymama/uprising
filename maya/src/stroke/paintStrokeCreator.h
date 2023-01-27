
#ifndef _paintStrokeCreator_H
#define _paintStrokeCreator_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeCreator.h>
#include "stroke.h"
#include "skGraph.h"
#include "skChain.h"
#include "brush.h"

class paintStrokeCreator : public strokeCreator
{
public:
    paintStrokeCreator();
    virtual ~paintStrokeCreator();
    static void *creator();
    static MStatus initialize();
    virtual void postConstructor();
    virtual bool isAbstractClass() const { return true; }

  virtual MStatus generateStrokeGeometry(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

    static MTypeId id;


void applyBrushStrokeSpec(
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes) const;

protected:



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

    static float  calculateTargetWeight(
        const double &distanceOnCurve,
        const double &lowDistance,
        const double &highDistance,
        const double &lowTransitionDistance,
        const double &highTransitionDistance,
        const float &lowTransitionLength,
        const float &highTransitionLength
        );



    static MObject aCanvasMatrix;
    static MObject aStrokeLength;
    static MObject aMinimumStrokeAdvance;
    static MObject aOverlap;
    
    static MObject aBrushId;
    static MObject aPaintId;

    static MObject aBrushFollowStroke;
    static MObject aSplitAngle;
    static MObject aSplitTestInterval;
    static MObject aEntryTransitionLength;
    static MObject aExitTransitionLength;
    static MObject aMinimumPoints;
    static MObject aApplyBrushBias;
    static MObject aBrushShape;
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

    return  transitionWeight;
}

#endif
