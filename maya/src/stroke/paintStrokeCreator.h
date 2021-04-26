
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


protected:


    
    float findEndDist(
        const MObject &dCurve,
        const MFloatVector &canvasNormal,
        float startDist,
        float endDist,
        float splitAngle,
        float splitTestInterval,
        float &outMaxCoil);

/**
 * @brief 
 * 
 * @param dCurve 
 * @param canvasNormal 
 * @param strokeLength 
 * @param overlap 
 * @param extendEntry 
 * @param extendExit 
 * @param splitAngle 
 * @param splitTestInterval 
 * @param result [(startDist, endDist, maxCoil), ..]
 * @return unsigned int 
 */
    unsigned int getStrokeBoundaries(
        const MObject &dCurve,
        const MFloatVector &canvasNormal,
        float strokeLength,
        float overlap,
        float extendEntry,
        float extendExit,
        float splitAngle,
        float splitTestInterval,
        MFloatVectorArray &result);

    bool getBoundary(
        const MObject &dCurve,
        double curveLength,
        const MFloatVector &canvasNormal,
        float lastEndDist,
        float strokeLength,
        float overlap,
        float extendEntry,
        float extendExit,
        float splitAngle,
        float splitTestInterval,
        MFloatVector &result);



    // std::vector<std::pair<int, float> > getBrushRadii(
    //     MDataBlock &data) const;

    static float calculateTargetWeight(
        const double &distanceOnCurve,
        const double &entryDistance,
        const double &exitDistance,
        const double &entryTransitionDistance,
        const double &exitTransitionDistance,
        const float &entryTransitionLength,
        const float &exitTransitionLength,
        const float &radius,
        const float &brushWidth);

    static float  calculateTargetWeight(
        const double &distanceOnCurve,
        const double &entryDistance,
        const double &exitDistance,
        const double &entryTransitionDistance,
        const double &exitTransitionDistance,
        const float &entryTransitionLength,
        const float &exitTransitionLength
        );



    static MObject aCanvasMatrix;
    static MObject aStrokeLength;
    static MObject aOverlap;
    static MObject aPaintId;
    static MObject aBrushFollowStroke;
    static MObject aSplitAngle;
    static MObject aSplitTestInterval;
    static MObject aEntryTransitionLength;
    static MObject aExitTransitionLength;
    static MObject aExtendEntry;
    static MObject aExtendExit;
    static MObject aMinimumPoints;
    static MObject aApplyBrushBias;
    // static MObject aOutCoil;
    

};

inline float paintStrokeCreator::calculateTargetWeight(
    const double &distanceOnCurve,
    const double &entryDistance,
    const double &exitDistance,
    const double &entryTransitionDistance,
    const double &exitTransitionDistance,
    const float &entryTransitionLength,
    const float &exitTransitionLength,
    const float &radius,
    const float &brushWidth

)
{
    float transitionWeight = calculateTargetWeight(
        distanceOnCurve,
        entryDistance,
        exitDistance,
        entryTransitionDistance,
        exitTransitionDistance,
        entryTransitionLength,
        exitTransitionLength
    ); 
   
    return fmin(transitionWeight, ((2.0f * radius) / brushWidth));
}

inline float paintStrokeCreator::calculateTargetWeight(
    const double &distanceOnCurve,
    const double &entryDistance,
    const double &exitDistance,
    const double &entryTransitionDistance,
    const double &exitTransitionDistance,
    const float &entryTransitionLength,
    const float &exitTransitionLength
)
{
    const float epsilon = 0.0001;

    float transitionWeight = 1.0;
    if ((distanceOnCurve+epsilon) < entryTransitionDistance)
    {
        transitionWeight = float((distanceOnCurve - entryDistance) / entryTransitionLength);
    }
    else if (distanceOnCurve > (exitTransitionDistance+epsilon))
    {
        transitionWeight = float((exitDistance - distanceOnCurve) / exitTransitionLength);
    }

    return  transitionWeight;
}

#endif
