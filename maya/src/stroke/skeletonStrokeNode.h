
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

    MStatus createCurve(
        const MPointArray &editPoints,
        MObject &dCurve) const;

    unsigned createStrokesForChain(
        const skChain &current_chain,
        const std::vector<std::pair<int, Brush> > &brushes,
        const MFloatMatrix &targetRotationMatrix,
        const MFloatVector &canvasNormal,
        unsigned strokeIndex,
        unsigned parentIndex,
        int minimumPoints,
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

    unsigned  createStrokeData(
        const MObject &dCurve,
        // const Brush &brush,
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
        const Brush &brush,
        const MDoubleArray &curveParams,
        const MFloatArray &strokeRadii,
        const MFloatMatrix &targetRotationMatrix,
        float entryTransitionLength,
        float exitTransitionLength ) const;

    void getPointsAndRadii(
        const skChain &chain,
        float extendEntry,
        float extendExit,
        MPointArray &points,
        MFloatArray &radii) const;

    // void getPointsAndContacts(const skChain &chain,
    //                           const std::pair<int, Brush> &indexedBrush,
    //                           float entryLength,
    //                           float exitLength,
    //                           float extendEntry,
    //                           float extendExit,
    //                           MDoubleArray &contacts,
    //                           MPointArray &points) const;

    // const std::pair<int, Brush> selectBrush(
    //     const skChain &chain,
    //     const std::vector<std::pair<int, Brush> > &brushes) const;
const std::pair<int, Brush>  selectBrush(
    float radius,
    const std::vector<std::pair<int, Brush> > &brushes) const;


float findEndDist(
    const MObject &dCurve,
    const MFloatVector &canvasNormal,
    float startDist,
    float endDist,
    double splitAngle,
    float splitTestInterval) ;
    
    unsigned int getStrokeBoundaries(
        const MObject &dCurve,
        const MFloatVector &canvasNormal,
        float strokeLength,
        float overlap,
        float extendEntry,
        float extendExit,
        double splitAngle,
        float splitTestInterval,
        MFloatVectorArray &result) ;

    // unsigned int getStrokeBoundaries(
    //     const MObject &dCurve,
    //     float strokeLength,
    //     float overlap,
    //     MVectorArray &result) const;

    // CImg<float> *getImage(MDataBlock &data, MObject &attribute )const;

    void setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                     double approachStart, double approachMid, double approachEnd) const;

    MStatus generateStrokeGeometry(MDataBlock &data,
                                   std::vector<Stroke> *geoms);

    std::vector<std::pair<int, float> > getBrushRadii(
        MDataBlock &data) const;

    static MObject aCanvasMatrix;
    static MObject aChains;
    static MObject aStrokeLength;
    static MObject aOverlap;
    static MObject aBrushes;
    static MObject aPaintId;
    static MObject aLayerId;
    static MObject aBrushFollowStroke;
    static MObject aSplitAngle;
    static MObject aSplitTestInterval;
    static MObject aEntryTransitionLength;
    static MObject aExitTransitionLength;
    static MObject aExtendEntry;
    static MObject aExtendExit;
    static MObject aMinimumPoints;
    static MObject aStrokeDirectionMap;
    // static MObject aOutCoil;

    float m_maxCoil; //> The maximum value of coils
};

#endif
