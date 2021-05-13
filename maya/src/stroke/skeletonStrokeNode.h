
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
	// virtual MStatus	connectionBroken ( const MPlug & plug, const MPlug & otherPlug, bool asSrc );
	
    virtual MStatus generateStrokeGeometry(
        const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

    static MTypeId id;

private:
    MStatus collectBrushes(
        MDataBlock &data,
        std::vector<std::pair<int, Brush> > &brushes) const;

    unsigned createStrokesForChain(
        const skChain &current_chain,
        const std::vector<std::pair<int, Brush> > &brushes,
        const MFloatVector &canvasNormal,
        const MFloatPoint &goalPoint,
        unsigned parentIndex,
        int minimumPoints,
        bool followStroke,
        bool applyBrushBias,
        float pointDensity,
        float entryTransitionLength,
        float exitTransitionLength,
        float extendEntry,
        float extendExit,
        float strokeLength,
        float minimumStrokeAdvance,
        float overlap,
        float splitAngle,
        float splitTestInterval,
        std::vector<Stroke> *pOutStrokes);

    void getChainPointsAndRadii(
        const skChain &chain,
        float extendEntry,
        float extendExit,
        MPointArray &points,
        MFloatArray &radii) const;

    const std::pair<int, Brush> selectBrush(
        float radius,
        const std::vector<std::pair<int, Brush> > &brushes) const;


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
        const MDoubleArray &curveParams,
        const MFloatArray &strokeRadii,
        bool followStroke,
        bool applyBrushBias,
        float entryTransitionLength,
        float exitTransitionLength) const;

    Stroke createReverseStroke(
        const MObject &dCurve,
        const std::pair<int, Brush> &brushPair,
        const MDoubleArray &curveParams,
        const MFloatArray &strokeRadii,
        bool followStroke,
        bool applyBrushBias,
        float entryTransitionLength,
        float exitTransitionLength) const;


    static MObject aChains;
    static MObject aActive;
    static MObject aBrushes;
    static MObject aInputData;
    static MObject aSplitAngle;
    static MObject aSelector;
    static MObject aGoalPoint;

};


#endif
