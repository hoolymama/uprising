
#ifndef _skeletonStrokeNode_H
#define _skeletonStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <paintStrokeCreator.h>
#include "stroke.h"
// #include "skGraph.h"
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
        const MFloatMatrix &canvasMatrix,
        const MDoubleArray &curveParams,
        const MFloatArray &strokeRadii,
        bool followStroke,
        float entryTransitionLength,
        float exitTransitionLength) const;






    // float findEndDist(
    //     const MObject &dCurve,
    //     const MFloatVector &canvasNormal,
    //     float startDist,
    //     float endDist,
    //     double splitAngle,
    //     float splitTestInterval);

    // unsigned int getStrokeBoundaries(
    //     const MObject &dCurve,
    //     const MFloatVector &canvasNormal,
    //     float strokeLength,
    //     float overlap,
    //     float extendEntry,
    //     float extendExit,
    //     double splitAngle,
    //     float splitTestInterval,
    //     MFloatVectorArray &result);

    // bool getBoundary(
    //     const MObject &dCurve,
    //     double curveLength,
    //     const MFloatVector &canvasNormal,
    //     float lastEndDist,
    //     float strokeLength,
    //     float overlap,
    //     float extendEntry,
    //     float extendExit,
    //     double splitAngle,
    //     float splitTestInterval,
    //     MFloatVector &result);


    // static float calculateTargetWeight(
    //     const double &distanceOnCurve,
    //     const double &entryDistance,
    //     const double &exitDistance,
    //     const double &entryTransitionDistance,
    //     const double &exitTransitionDistance,
    //     const float &entryTransitionLength,
    //     const float &exitTransitionLength,
    //     const float &radius,
    //     const float &brushWidth);

    static MObject aChains;
    static MObject aBrushes;

};


#endif
