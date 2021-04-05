
#ifndef _skeletonStrokeNode_H
#define _skeletonStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeCreator.h>
#include "stroke.h"
#include "skGraph.h"
#include "skChain.h"
#include "brush.h"

class skeletonStrokeNode : public strokeCreator
{
public:
    skeletonStrokeNode();
    virtual ~skeletonStrokeNode();
    static void *creator();
    static MStatus initialize();
    virtual void postConstructor();
    virtual bool isAbstractClass() const { return false; }

    static MTypeId id;

private:
    MStatus collectBrushes(
        MDataBlock &data,
        std::vector<std::pair<int, Brush> > &brushes) const;

    unsigned createStrokesForChain(
        const skChain &current_chain,
        const std::vector<std::pair<int, Brush> > &brushes,
        const MFloatMatrix &canvasMatrix,
        unsigned parentIndex,
        int minimumPoints,
        bool followStroke,
        float pointDensity,
        float entryTransitionLength,
        float exitTransitionLength,
        float extendEntry,
        float extendExit,
        float strokeLength,
        float overlap,
        float splitAngle,
        float splitTestInterval,
        std::vector<Stroke> *pOutStrokes);

    unsigned createStrokeData(
        const MObject &dCurve,
        const MFloatArray &radii,
        float startDist,
        float endDist,
        float density,
        int minimumPoints,
        MDoubleArray &curveParams,
        MFloatArray &strokeRadii,
        float &maxRadius) const;

    Stroke createStroke(
        const MObject &dCurve,
        const std::pair<int, Brush> &brushPair,
        const MFloatMatrix &canvasMatrix,
        const MDoubleArray &curveParams,
        const MFloatArray &strokeRadii,
        bool followStroke,
        float entryTransitionLength,
        float exitTransitionLength) const;

    void getPointsAndRadii(
        const skChain &chain,
        float extendEntry,
        float extendExit,
        MPointArray &points,
        MFloatArray &radii) const;

    const std::pair<int, Brush> selectBrush(
        float radius,
        const std::vector<std::pair<int, Brush> > &brushes) const;

    float findEndDist(
        const MObject &dCurve,
        const MFloatVector &canvasNormal,
        float startDist,
        float endDist,
        double splitAngle,
        float splitTestInterval);

    unsigned int getStrokeBoundaries(
        const MObject &dCurve,
        const MFloatVector &canvasNormal,
        float strokeLength,
        float overlap,
        float extendEntry,
        float extendExit,
        double splitAngle,
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
        double splitAngle,
        float splitTestInterval,
        MFloatVector &result);

    void setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                     double approachStart, double approachMid, double approachEnd) const;

    MStatus generateStrokeGeometry(MDataBlock &data,
                                   std::vector<Stroke> *geoms);

    std::vector<std::pair<int, float> > getBrushRadii(
        MDataBlock &data) const;

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





    static MObject aCanvasMatrix;
    static MObject aChains;
    static MObject aStrokeLength;
    static MObject aOverlap;
    static MObject aBrushes;
    static MObject aPaintId;
    // static MObject aLayerId;
    static MObject aBrushFollowStroke;
    static MObject aSplitAngle;
    static MObject aSplitTestInterval;
    static MObject aEntryTransitionLength;
    static MObject aExitTransitionLength;
    static MObject aExtendEntry;
    static MObject aExtendExit;
    static MObject aMinimumPoints;
    static MObject aStrokeDirectionMap;

    float m_maxCoil; //> The maximum value of coils
};

inline float skeletonStrokeNode::calculateTargetWeight(
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

    return fmin(transitionWeight, ((2.0f * radius) / brushWidth));
}

#endif
