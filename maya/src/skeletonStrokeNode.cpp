
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNurbsCurveData.h>

#include "skeletonStrokeNode.h"
#include "stroke.h"
#include "strokeRotationSpec.h"
#include "strokeRepeatSpec.h"

#include "cImgFloatData.h"
#include "cImgData.h"

#include "brushData.h"
#include "cImgUtils.h"

#include <jMayaIds.h>
#include "errorMacros.h"

#include "skChainData.h"

const double rad_to_deg = (180 / 3.1415927);

MObject skeletonStrokeNode::aChains;
MObject skeletonStrokeNode::aBrushFilter;
MObject skeletonStrokeNode::aBrushes;
MObject skeletonStrokeNode::aStrokeLength;
MObject skeletonStrokeNode::aOverlap;
MObject skeletonStrokeNode::aBrushRampScope;
MObject skeletonStrokeNode::aSplitAngle;
MObject skeletonStrokeNode::aSplitTestInterval;


MTypeId skeletonStrokeNode:: id(k_skeletonStrokeNode );

skeletonStrokeNode:: skeletonStrokeNode() {}

skeletonStrokeNode:: ~skeletonStrokeNode() {}

void *skeletonStrokeNode::creator() {
    return new skeletonStrokeNode();
}



MStatus skeletonStrokeNode:: initialize()
{
    MStatus st;

    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnEnumAttribute eAttr;
    MFnCompoundAttribute cAttr;
    MFnMatrixAttribute mAttr;
    MFnUnitAttribute uAttr;

    inheritAttributesFrom("strokeNode");

    /// skeleton generation
    //////////////////
    aChains = tAttr.create("chains", "chn", skChainData::id ) ;
    tAttr.setStorable(false);
    tAttr.setKeyable(true);

    tAttr.setDisconnectBehavior(MFnAttribute::kReset);
    st = addAttribute( aChains ); mser;

    aStrokeLength = nAttr.create( "strokeLength", "stl", MFnNumericData::kFloat);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setMin(0.01f);
    nAttr.setDefault(10.0f);
    st = addAttribute(aStrokeLength); mser;

    aOverlap  = nAttr.create("overlap", "ovlp", MFnNumericData:: kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    st = addAttribute(aOverlap);
    mser;

    aBrushes = tAttr.create( "brushes", "bsh", brushData::id );
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    tAttr.setArray(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    addAttribute(aBrushes);

    aBrushRampScope = eAttr.create("brushRampScope", "brsc",
                                   StrokeRotationSpec::kStroke);
    eAttr.addField("stroke", StrokeRotationSpec::kStroke);
    eAttr.addField("travelStroke", StrokeRotationSpec::kTravelStroke);
    eAttr.addField("curve", StrokeRotationSpec::kCurve);
    eAttr.setKeyable(true);
    eAttr.setHidden(false);
    st = addAttribute(aBrushRampScope);
    mser;

    aBrushFilter = eAttr.create("brushFilter", "bflt", Brush::kRound);
    eAttr.addField("round", Brush::kRound);
    eAttr.addField("flat", Brush::kFlat);
    eAttr.addField("all", Brush::kAll);
    eAttr.setKeyable(true);
    eAttr.setHidden(false);
    st = addAttribute(aBrushFilter);
    mser;

    ;


    aSplitAngle = uAttr.create( "splitAngle", "span",
                                MFnUnitAttribute::kAngle );
    uAttr.setHidden( false );
    uAttr.setKeyable( true );
    uAttr.setStorable(true);
    st = addAttribute(aSplitAngle); mser;

    aSplitTestInterval = nAttr.create("splitTestInterval", "spti", MFnNumericData:: kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    st = addAttribute(aSplitTestInterval);
    mser;




    attributeAffects(aChains, aOutput);
    attributeAffects(aBrushFilter, aOutput);
    attributeAffects(aBrushes, aOutput);
    attributeAffects(aStrokeLength, aOutput);
    attributeAffects(aOverlap, aOutput);
    attributeAffects(aBrushRampScope, aOutput);
    attributeAffects(aSplitAngle, aOutput);
    attributeAffects(aSplitTestInterval, aOutput);

    return (MS::kSuccess );
}

/*
void skeletonStrokeNode::splitOnAngle(const MPointArray &points, double splitThreshold,
                                      const MVector &planeNormal, std::vector<MPointArray> &result) const
{
    MPointArray curr;
    int len = points.length();
    MVector lastVec; // start as zero

    double leftExtent  = 0;
    double rightExtent = 0;
    double accumAngle = 0;
    curr.append(points[0]);

    for (int i = 1; i < len; i++) {
        MPoint lastPt = points[(i - 1)];
        MPoint thisPt = points[i];
        MVector thisVec = (thisPt - lastPt).normal();

        if (lastVec != MVector::zero) {
            // there is a last vec and a new vec, so test to see if we exceeded
            MQuaternion q(lastVec, thisVec);
            double angle;
            MVector axis(MVector::zAxis);
            bool rotated = q.getAxisAngle(axis, angle);
            if (rotated) {
                double dir = (axis * planeNormal  < 0) ? -1.0 : 1.0 ;
                accumAngle += (dir * angle);
                if (accumAngle < leftExtent) {leftExtent = accumAngle;}
                if (accumAngle > rightExtent) {rightExtent = accumAngle;}

                if ((rightExtent - leftExtent) > splitThreshold)
                {   // need to start a new array
                    result.push_back(curr);
                    curr = MPointArray();
                    curr.append(points[i - 1]);

                    leftExtent  = 0;
                    rightExtent = 0;
                    accumAngle = 0;
                }
            }
        }

        curr.append(points[i]);
        lastVec = thisVec;
    }
    flowPointsList.push_back(curr);
}
*/
double skeletonStrokeNode::findEndDist(
    const MObject  &dCurve,
    double startDist,
    double endDist,
    double splitAngle,
    double splitTestInterval ) const
{

    double leftExtent  = 0;
    double rightExtent = 0;
    double accumAngle = 0;
    double currDist = startDist;
    MFnNurbsCurve curveFn(dCurve);
    double param = curveFn.findParamFromLength(startDist);
    MVector lastTangent = curveFn.tangent(param);
    double angle;
    MVector axis(MVector::zAxis);
    bool foundEnd = false;
    do
    {
        currDist += splitTestInterval;
        if (currDist > endDist) {
            return endDist;
        }
        param = curveFn.findParamFromLength(currDist);
        MVector tangent =  curveFn.tangent(param);

        MQuaternion q(lastTangent, tangent);
        lastTangent = tangent;

        bool rotated = q.getAxisAngle(axis, angle);

        if (! rotated) { continue; }

        double dir = (axis * MVector::zAxis  < 0) ? -1.0 : 1.0 ;
        accumAngle += (dir * angle);
        if (accumAngle < leftExtent) {leftExtent = accumAngle;}
        if (accumAngle > rightExtent) {rightExtent = accumAngle;}
        foundEnd = ( (rightExtent - leftExtent) > splitAngle);
    }
    while (! foundEnd);
    return currDist;
}


unsigned int skeletonStrokeNode::getStrokeBoundaries(
    const MObject   &dCurve,
    float strokeLength,
    float overlap,
    double splitAngle,
    double splitTestInterval,
    MVectorArray &result
) const  {
    const double epsilon = 0.0001;

    MStatus st = MS::kSuccess;

    MFnNurbsCurve curveFn(dCurve, &st);
    if (st.error()) { return 0; }

    double curveLen = curveFn.length(epsilon);
    double startDist = 0;
    double endDist;

    if (strokeLength < 0.1) // 1mm
    {
        strokeLength = 0.1;
    }
    if (overlap >= strokeLength) {
        overlap = 0.0;
    }




    do {
        endDist = fmin((startDist + strokeLength), curveLen);

        if (splitAngle > 0.0001 && splitTestInterval > 0.01) {
            endDist = findEndDist(dCurve, startDist, endDist, splitAngle, splitTestInterval);
        }

        result.append(MVector(startDist, endDist));
        /* last time around hit the end */
        if (endDist >= curveLen) { break; }

        double halfLastLength = (endDist - startDist) / 2;
        if (overlap > halfLastLength) {
            startDist = endDist - halfLastLength;
        }
        else {
            startDist = endDist - overlap;
        }
        if (startDist >= curveLen) { break; }
    }
    while ( true );

    return result.length();
}



unsigned int skeletonStrokeNode::getStrokeBoundaries(
    const MObject   &dCurve,
    float strokeLength,
    float overlap,
    MVectorArray &result
) const  {
    const double epsilon = 0.0001;

    MStatus st = MS::kSuccess;

    MFnNurbsCurve curveFn(dCurve, &st);
    if (st.error()) { return 0; }
    double curveLen = curveFn.length(epsilon);

    double startDist = 0;
    double endDist;

    if (strokeLength < 0.1)
    {
        strokeLength = 0.1;
    }
    if (overlap >= strokeLength) {
        overlap = 0.0;
    }

    do {
        endDist = fmin((startDist + strokeLength), curveLen);
        result.append(MVector(startDist, endDist));
        if (endDist >= curveLen) { break; }
        startDist = endDist - overlap;
        if (startDist >= curveLen) { break; }
    }
    while ( true );

    return result.length();
}

MStatus skeletonStrokeNode::collectBrushes(
    MDataBlock &data,
    std::vector< std::pair<int, Brush> > &brushes,
    Brush::Shape filter) const
{

    /*
        Brushes, sorted by width, starting with largest.
    */


    MStatus st;
    MArrayDataHandle ha = data.inputArrayValue(aBrushes, &st); msert;
    unsigned nPlugs = ha.elementCount();

    for (unsigned i = 0; i < nPlugs; i++, ha.next()) {
        int index = ha.elementIndex(&st);
        if (st.error()) {
            continue;
        }
        MDataHandle h = ha.inputValue(&st);
        if (st.error()) {
            continue;
        }
        MObject d = h.data();
        MFnPluginData fnP( d , &st);
        if (st.error()) {
            continue;
        }
        brushData *bData = (brushData *)fnP.data();

        const Brush *pBrush = bData->fGeometry;
        if (pBrush->matches(filter))
        {
            brushes.push_back(std::make_pair(index, *pBrush));
        }
    }

    /*
    Sort from largest to smallest. This is to help us select the best available brush.


    */
    std::sort(brushes.begin(), brushes.end(),
              [](const std::pair<int, Brush> &a, const std::pair<int, Brush> &b) -> bool
    {
        return a.second.width() > b.second.width() ;
    });
    return MS::kSuccess;
}

const std::pair<int, Brush> skeletonStrokeNode::selectBrush(
    const skChain &chain,
    const std::vector< std::pair<int, Brush> > &brushes) const
{
    /*
    The brushes are already sorted widest to finest. We test each brush in turn to
    see if it is big enough for the stroke. When we come across the first brush that
    is too small, we select the previous brush. It will in theory be the best suited.
    If the first brush (the biggest brush) is too small for the stroke, we're just
    going to have to use it.

    If there are no brushes (should never happen!!),
    then we return a pair with key -1.
    */

    std::pair<int, Brush> result = std::make_pair(-1, Brush());

    std::vector< std::pair<int, Brush> >::const_iterator brushIter;
    for (brushIter = brushes.begin(); brushIter != brushes.end(); brushIter++)
    {
        float brushRad = brushIter->second.width() * 0.5;
        if (brushRad <= chain.maxRadius()) {
            if (brushIter == brushes.begin() ) { // use this
                result = *brushIter;
            }
            break;
        }
        result = *brushIter;
    }
    return result;
}



// void skeletonStrokeNode::getContacts(
//     const skChain &chain,
//     const std::pair<int, Brush> &indexedBrush,
//     MDoubleArray &contacts) const
// {
//     // MFloatArray radii;
//     // chain.appendRadiiTo(radii);


//     contacts.clear();
//     if ( indexedBrush.second.isFlat())
//     {
//         contacts = MDoubleArray( chain.size(), 1.0);
//     }
//     else {
//         float brushRadius = indexedBrush.second.width() * 0.5;
//         for (int i = 0; i < chain.size(); ++i)
//         {
//             contacts.append(
//                 fmin( (chain[i].radius / brushRadius) , 1.0)
//             );
//         }
//     }
// }


void skeletonStrokeNode::getPointsAndContacts(
    const skChain &chain,
    const std::pair<int, Brush> &indexedBrush,
    float entryLength,
    float exitLength,
    MDoubleArray &contacts,
    MPointArray &points) const
{
    const double epsilon = 0.0001;

    contacts.clear();
    points.clear();

    const int hasEntry = (entryLength > epsilon) ? 1 : 0;
    const int hasExit = (exitLength > epsilon) ? 1 : 0;


    std::vector< skPoint >::const_iterator p0, p1;
    skPoint entryPoint, exitPoint;
    if (hasEntry) {
        p1 = chain.points().begin(); // 0
        p0 = std::next(p1); // 1
        entryPoint = skPoint::extrapolate(*p0, *p1, entryLength);
        points.append(entryPoint.x, entryPoint.y, 0.0);
    }

    chain.appendPointsTo(points);

    if (hasExit) {
        p1 = std::prev(chain.points().end()); // -1
        p0 =  std::prev(p1); // -2
        exitPoint = skPoint::extrapolate(*p0, *p1, exitLength);
        points.append(exitPoint.x, exitPoint.y, 0.0);
    }


    if ( indexedBrush.second.isFlat())
    {
        contacts = MDoubleArray( (chain.size() + hasEntry + hasExit), 1.0);
        return;
    }

    float brushWidth = indexedBrush.second.width();
    if (brushWidth < epsilon) {
        brushWidth = 1.0;
    }
    float brushRadiusRecip  = 2.0 / brushWidth;

    if (hasEntry) {
        contacts.append(fmin( (entryPoint.radius * brushRadiusRecip) , 1.0));
    }

    for (int  i = 0; i < chain.size(); ++i)
    {
        contacts.append(
            fmin( (chain[i].radius * brushRadiusRecip) , 1.0)
        );
    }

    if (hasExit) {
        contacts.append(fmin( (exitPoint.radius * brushRadiusRecip) , 1.0));
    }
}



MStatus skeletonStrokeNode::generateStrokeGeometry(MDataBlock &data,
        std::vector < Stroke > *pStrokes) const
{


    MStatus st;
    if (! data.inputValue(aActive).asBool()) {
        return MS:: kSuccess;
    }

    MObject thisObj = thisMObject();
    MFnDependencyNode fnNode(thisObj);
    MString nodeName = fnNode.name();

    //////////////////////////////////////////////////////////////
    double pointDensity = data.inputValue(aPointDensity).asDouble();
    if (pointDensity < 0.001) {
        pointDensity = 0.001;
    }
    int minimumPoints = data.inputValue(aMinimumPoints).asInt();

    double entryLength = data.inputValue(aEntryLength).asDouble();
    double exitLength = data.inputValue(aExitLength).asDouble();
    bool localContact = data.inputValue(aLocalContact).asBool();


    Stroke::TransitionBlendMethod transitionBlendMethod = Stroke::TransitionBlendMethod(
                data.inputValue(
                    aTransitionBlendMethod).asShort());


    Stroke::DirectionMethod strokeDirection = Stroke::DirectionMethod(data.inputValue(
                aStrokeDirection).asShort());

    double pivotParam = data.inputValue(aPivotFraction).asDouble();

    StrokeRotationSpec rotSpec;
    rotSpec.tiltRampAtt =  strokeNode::aBrushTiltRamp;
    rotSpec.bankRampAtt =  strokeNode::aBrushBankRamp;
    rotSpec.twistRampAtt =  strokeNode::aBrushTwistRamp;

    MDataHandle hRangeHandle = data.inputValue(aBrushTiltRange);
    rotSpec.tiltRampMin = hRangeHandle.child(aBrushTiltRangeMin).asAngle().asRadians();
    rotSpec.tiltRampMax = hRangeHandle.child(aBrushTiltRangeMax).asAngle().asRadians();

    hRangeHandle = data.inputValue(aBrushBankRange);
    rotSpec.bankRampMin = hRangeHandle.child(aBrushBankRangeMin).asAngle().asRadians();
    rotSpec.bankRampMax = hRangeHandle.child(aBrushBankRangeMax).asAngle().asRadians();

    hRangeHandle = data.inputValue(aBrushTwistRange);
    rotSpec.twistRampMin = hRangeHandle.child(aBrushTwistRangeMin).asAngle().asRadians();
    rotSpec.twistRampMax = hRangeHandle.child(aBrushTwistRangeMax).asAngle().asRadians();

    rotSpec.rampScope = StrokeRotationSpec::Scope(data.inputValue(aBrushRampScope).asShort());
    rotSpec.followStroke =  data.inputValue(aBrushFollowStroke).asBool();

    StrokeRepeatSpec repeatSpec;
    repeatSpec.repeats = data.inputValue(aRepeats).asShort();
    repeatSpec.offset = data.inputValue(aRepeatOffset).asDouble();
    repeatSpec.mirror = data.inputValue(aRepeatMirror).asBool();
    repeatSpec.oscillate = data.inputValue(aRepeatOscillate).asBool();
    repeatSpec.fan = data.inputValue(aRepeatFan).asAngle().asRadians();

    short brushId = data.inputValue(aBrushId).asShort();
    short paintId = data.inputValue(aPaintId).asShort();
    int layerId = data.inputValue(aLayerId).asInt();
    //////////////////////////////////////////////////////////////


    MDataHandle hChains = data.inputValue(aChains, &st); msert;
    MObject dChains = hChains.data();
    MFnPluginData fnChains( dChains , &st); msert;
    skChainData *scData = (skChainData *)fnChains.data( &st); msert;
    const std::vector<skChain> *geom = scData->fGeometry;
    if ((! geom) || geom->size() == 0 ) {
        return MS::kUnknownParameter;
    }




    double splitAngle =  data.inputValue(aSplitAngle).asAngle().asRadians();
    double splitTestInterval = data.inputValue(aSplitTestInterval).asFloat();





    float strokeLength = data.inputValue(aStrokeLength).asFloat();
    float overlap = data.inputValue(aOverlap).asFloat();
    Brush::Shape filter = Brush::Shape(data.inputValue(aBrushFilter).asShort());

    std::vector< std::pair<int, Brush> > brushes;
    st = collectBrushes(data, brushes  , filter);
    if (! brushes.size()) {
        MGlobal::displayWarning("No brushes: " + nodeName);
        return MS::kUnknownParameter;
    }


    std::vector<skChain>::const_iterator citer;
    for (citer = geom->begin(); citer != geom->end(); citer++)
    {
        MPointArray editPoints;
        MDoubleArray contacts;

        const std::pair<int, Brush> selectedBrush = selectBrush(*citer, brushes);

        getPointsAndContacts(*citer, selectedBrush, entryLength, exitLength, contacts,
                             editPoints);
        int selectedBrushId = selectedBrush.first;
        int customBrushId = selectedBrush.second.customId();

        MFnNurbsCurve curveFn;
        MFnNurbsCurveData dataCreator;
        MObject curveData = dataCreator.create( &st ); mser;
        // cerr << "editPoints" << editPoints << endl;
        MObject dCurve = curveFn.createWithEditPoints(   editPoints, 3, MFnNurbsCurve::kOpen,
                         true, false, false, curveData, &st);

        MDoubleArray knotVals;
        st = curveFn.getKnots(knotVals);
        int numKnots =  knotVals.length();
        // cerr << "numKnots:" <<  numKnots << " --- " << knotVals[(numKnots - 1)] << endl;
        double maxValRecip = 1.0 / knotVals[(numKnots - 1)];

        for (int i = 0; i < numKnots; ++i)
        {
            knotVals[i] =  knotVals[i] * maxValRecip;
        }

        curveFn.setKnots(knotVals, 0, (numKnots - 1));
        double curveLength = curveFn.length(epsilon);
        MVectorArray boundaries;
        if (! getStrokeBoundaries(dCurve, strokeLength, overlap, splitAngle, splitTestInterval,
                                  boundaries)) {
            continue;
        }
        for (int strokeId = 0; strokeId < boundaries.length(); ++strokeId) {
            const double &startDist = boundaries[strokeId].x;
            const double &endDist = boundaries[strokeId].y;

            unsigned strokeGroupSize = Stroke::create(
                                           thisObj,
                                           dCurve,
                                           contacts,
                                           localContact,
                                           curveLength,
                                           startDist,
                                           endDist,
                                           entryLength,
                                           exitLength,
                                           transitionBlendMethod,
                                           pointDensity,
                                           minimumPoints,
                                           rotSpec,
                                           repeatSpec,
                                           strokeDirection,
                                           pivotParam,
                                           strokeId,
                                           selectedBrushId,
                                           paintId,
                                           layerId,
                                           customBrushId,
                                           pStrokes);
        }
    }

    return MS::kSuccess;
}

void skeletonStrokeNode:: postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
