
#ifndef _skeletonStrokeNode_H
#define _skeletonStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <paintStrokeCreator.h>
#include "stroke.h"

#include "skChain.h"
#include "brush.h"

class skeletonStrokeNode : public paintStrokeCreator
{
public:
    skeletonStrokeNode();
    virtual ~skeletonStrokeNode();
    static void *creator();
    static MStatus initialize();
    virtual void postConstructor();
    virtual bool isAbstractClass() const { return false; }

    virtual MStatus generateStrokeGeometry(
        const MPlug &plug,
        MDataBlock &data,
        std::vector<Stroke> *pStrokes);

    static MTypeId id;

private:
    unsigned createStrokesForChain(
        const skChain &current_chain,

        const MFloatVector &canvasNormal,
        const MFloatPointArray &goalPoints,
        bool awayFromGoal,
        unsigned parentIndex,
        int minimumPoints,

        float pointDensity,

        float extendEntry,
        float extendExit,
        float strokeLength,
        float minimumStrokeAdvance,
        float overlap,
        float splitAngle,
        float splitTestInterval,
        std::vector<Stroke> *pOutStrokes);

    bool shouldReverse(
        const MPoint &startPoint,
        const MPoint &endPoint,
        const MFloatPointArray &goalPoints,
        bool awayFromGoal) const;

    void getChainPointsAndRadii(
        const skChain &chain,
        float extendEntry,
        float extendExit,
        MPointArray &points,
        MFloatArray &radii) const;

    unsigned createStrokeData(
        const MObject &dCurve,
        const MFloatArray &radii,
        float startDist,
        float endDist,
        float density,
        int minimumPoints,
        MDoubleArray &curveParams,
        MFloatArray &strokeRadii) const;

    Stroke createStroke(
        const MObject &dCurve,
        const MDoubleArray &curveParams,
        const MFloatArray &strokeRadii

    ) const;

    Stroke createReverseStroke(
        const MObject &dCurve,

        const MDoubleArray &curveParams,
        const MFloatArray &strokeRadii) const;

    static MObject aChains;
    static MObject aActive;

    static MObject aExtendEntry;
    static MObject aExtendExit;

    static MObject aInputData;
    static MObject aSplitAngle;
    static MObject aSelector;
    static MObject aGoalPoint;
    static MObject aGoalPoints;

    static MObject aAwayFromGoal;
    static MObject aSmoothNeighbors;

    static MObject aSmoothPositions;
    static MObject aSmoothWeights;
};

#endif
