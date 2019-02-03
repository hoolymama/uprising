
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



MTypeId skeletonStrokeNode:: id(k_skeletonStrokeNode );

skeletonStrokeNode:: skeletonStrokeNode() {}

skeletonStrokeNode:: ~skeletonStrokeNode() {}

void *skeletonStrokeNode::creator() {
    return new skeletonStrokeNode();
}

const double epsilon = 0.0001;


MStatus skeletonStrokeNode:: initialize()
{
    MStatus st;

    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnEnumAttribute eAttr;
    MFnCompoundAttribute cAttr;
    MFnMatrixAttribute mAttr;

    inheritAttributesFrom("strokeNode");

    /// skeleton generation
    //////////////////
    aChains = tAttr.create("chains", "chn", skChainData::id ) ;
    tAttr.setStorable(false);
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

    attributeAffects(aChains, aOutput);
    attributeAffects(aBrushFilter, aOutput);
    attributeAffects(aBrushes, aOutput);
    attributeAffects(aStrokeLength, aOutput);
    attributeAffects(aOverlap, aOutput);
    attributeAffects(aBrushRampScope, aOutput);

    return (MS::kSuccess );
}


unsigned int skeletonStrokeNode::getStrokeBoundaries(
    const MObject  &dCurve,
    float strokeLength,
    float overlap,
    MVectorArray &result
) const  {

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

    std::sort(brushes.begin(), brushes.end(),
              [](const std::pair<int, Brush> &a, const std::pair<int, Brush> &b) -> bool
    {
        return a.second.width() > b.second.width() ;
    });
    return MS::kSuccess;
}


int skeletonStrokeNode::getContacts(
    const skChain &chain,
    const std::vector< std::pair<int, Brush> > &brushes,
    MDoubleArray &contacts) const
{
    /*

    If the first brush (the biggest brush) is too small for the stroke
    then we return -1, meaning we cant actually paint the stroke.
    */

    int index = -1;
    float brushRadius =  1.0;
    bool isFlat = false;
    // std::vector< std::pair<int, Brush> >::const_iterator selected_brush = brushes.find(-1);

    std::vector< std::pair<int, Brush> >::const_iterator brushIter;
    for (brushIter = brushes.begin(); brushIter != brushes.end(); brushIter++)
    {
        float brushRad = brushIter->second.width() * 0.5;
        if (brushRad < chain.maxRadius() && (brushIter != brushes.begin() ))
        {
            break;
        }
        index = brushIter->first;
        brushRadius = brushRad;
        isFlat =  brushIter->second.isFlat();
    }
    /*

    */

    MFloatArray radii;
    chain.appendRadii(radii);

    if (isFlat)
    {
        for (int i = 0; i < radii.length(); ++i)
        {
            contacts.append( 1.0);
        }
    }
    else {
        for (int i = 0; i < radii.length(); ++i)
        {
            contacts.append(
                fmin( radii[i] / brushRadius, 1.0)
            );
        }
    }
    return index;
}


MStatus skeletonStrokeNode::generateStrokeGeometry(MDataBlock &data,
        std::vector < Stroke > *pStrokes) const
{


    MStatus st;
    if (! data.inputValue(aActive).asBool()) {
        return MS:: kSuccess;
    }
    //////////////////////////////////////////////////////////////
    double pointDensity = data.inputValue(aPointDensity).asDouble();
    if (pointDensity < 0.001) {
        pointDensity = 0.001;
    }

    double entryLength = data.inputValue(aEntryLength).asDouble();
    double exitLength = data.inputValue(aExitLength).asDouble();
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




    MObject thisObj = thisMObject();

    float strokeLength = data.inputValue(aStrokeLength).asFloat();
    float overlap = data.inputValue(aOverlap).asFloat();
    Brush::Shape filter = Brush::Shape(data.inputValue(aBrushFilter).asShort());

    std::vector< std::pair<int, Brush> > brushes;
    st = collectBrushes(data, brushes  , filter);


    std::vector<skChain>::const_iterator citer;
    for (citer = geom->begin(); citer != geom->end(); citer++)
    {
        MPointArray editPoints;
        MDoubleArray contacts;
        citer->appendPoints(editPoints );

        int selectedBrushId  = getContacts(*citer, brushes,  contacts);
        if (selectedBrushId == -1)
        {
            selectedBrushId = brushId;
        }


        MFnNurbsCurve curveFn;
        MFnNurbsCurveData dataCreator;
        MObject curveData = dataCreator.create( &st ); mser;
        MObject dCurve = curveFn.createWithEditPoints(   editPoints, 3, MFnNurbsCurve::kOpen,
                         true, false, true, curveData, &st);
        double curveLength = curveFn.length(epsilon);

        MVectorArray boundaries;
        if (! getStrokeBoundaries(dCurve, strokeLength, overlap, boundaries)) {
            continue;
        }

        for (int strokeId = 0; strokeId < boundaries.length(); ++strokeId) {
            const double &startDist = boundaries[strokeId].x;
            const double &endDist = boundaries[strokeId].y;

            unsigned strokeGroupSize = Stroke::create(
                                           thisObj,
                                           dCurve,
                                           contacts,
                                           curveLength,
                                           startDist,
                                           endDist,
                                           entryLength,
                                           exitLength,
                                           pointDensity,
                                           rotSpec,
                                           repeatSpec,
                                           strokeDirection,
                                           pivotParam,
                                           strokeId,
                                           selectedBrushId,
                                           paintId,
                                           layerId,
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
