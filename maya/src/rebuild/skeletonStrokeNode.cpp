
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

#include <jMayaIds.h>
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);

MObject skeletonStrokeNode::aSkeletonImage;

// MObject skeletonStrokeNode::aBrushFilter;



MObject skeletonStrokeNode::aMinPixels;
MObject skeletonStrokeNode::aSpanPixels;
MObject skeletonStrokeNode::aBrushes;
MObject skeletonStrokeNode::aBrushWidth;
MObject skeletonStrokeNode::aBrushActive;

MObject skeletonStrokeNode::aProjectionMatrix;

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

    MFloatMatrix identity;
    identity.setToIdentity();
    aProjectionMatrix = mAttr.create( "projectionMatrix", "pmat",
                                      MFnMatrixAttribute::kFloat );
    mAttr.setStorable( false );
    mAttr.setHidden( false );
    mAttr.setKeyable(true);
    mAttr.setDefault(identity);
    addAttribute(aProjectionMatrix);

    aSkeletonImage = tAttr.create("skeletonImage", "simg", cImgFloatData::id ) ;
    tAttr.setStorable(false);
    tAttr.setDisconnectBehavior(MFnAttribute::kReset);
    st = addAttribute( aSkeletonImage ); mser;


    aMinPixels = nAttr.create( "minPixels", "mnpx", MFnNumericData::kInt);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setMin(2);
    nAttr.setDefault(10);
    st = addAttribute(aMinPixels); mser;

    aSpanPixels = nAttr.create( "spanPixels", "spx", MFnNumericData::kInt);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setMin(1);
    nAttr.setDefault(10);
    st = addAttribute(aSpanPixels); mser;


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




    // static MObject aBrushes;
    // static MObject aBrushWidth;
    // static MObject aBrushActive;


    aBrushes = cAttr.create( "brushes", "bs");
    cAttr.setArray(true);
    cAttr.addChild(aBrushWidth);
    cAttr.addChild(aBrushActive);

    st = addAttribute(aBrushes); mser;



    aBrushRampScope = eAttr.create("brushRampScope", "brsc",
                                   StrokeRotationSpec::kStroke);
    eAttr.addField("stroke", StrokeRotationSpec::kStroke);
    eAttr.addField("travelStroke", StrokeRotationSpec::kTravelStroke);
    eAttr.addField("curve", StrokeRotationSpec::kCurve);
    eAttr.setKeyable(true);
    eAttr.setHidden(false);
    st = addAttribute(aBrushRampScope);
    mser;


    // aBrushFilter = eAttr.create("brushFilter", "bflt", skeletonStrokeNode::kRound);
    // eAttr.addField("round", skeletonStrokeNode::kRound);
    // eAttr.addField("flat", skeletonStrokeNode::kFlat);
    // eAttr.addField("all", skeletonStrokeNode::kAll);
    // eAttr.setKeyable(true);
    // eAttr.setHidden(false);
    // st = addAttribute(aBrushFilter);
    // mser;



    attributeAffects(aSkeletonImage, aOutput);
    attributeAffects(aBrushFilter, aOutput);
    attributeAffects(aMinPixels, aOutput);
    attributeAffects(aSpanPixels, aOutput);
    attributeAffects(aBrushWidths, aOutput);

    attributeAffects(aProjectionMatrix, aOutput);
    attributeAffects(aStrokeLength, aOutput);
    attributeAffects(aOverlap, aOutput);
    attributeAffects(aBrushRampScope, aOutput);
    return (MS::kSuccess );
}


CImg<float> *skeletonStrokeNode::getImage(MDataBlock &data, MObject &attribute ) const
{
    MStatus st;
    MDataHandle hImageData = data.inputValue(attribute, &st);
    if (st.error()) {   return 0;}
    MObject dImageData = hImageData.data();
    MFnPluginData fnImageData( dImageData , &st);
    if (st.error()) {   return 0;}
    cImgFloatData *imageData = (cImgFloatData *)fnImageData.data();
    return imageData->fImg;
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

std::vector< std::pair<int, float> > skeletonStrokeNode::getBrushRadii(
    MDataBlock &data) const
{
    MStatus st;
    std::vector< std::pair<int, float> > result;


    BrushFilter filter = BrushFilter(data.inputValue(aBrushFilter).asShort());



    MArrayDataHandle ha = data.inputArrayValue(aBrushWidths, &st);
    if (st.error()) {
        return result;
    }

    unsigned nPlugs = ha.elementCount();

    for (unsigned i = 0; i < nPlugs; i++, ha.next()) {
        int index = ha.elementIndex(&st);
        if (st.error()) {
            continue;
        }
        MDataHandle hComp = ha.inputValue(&st );
        if (st.error()) {
            continue;
        }

        double radius = ha.inputValue().asFloat() * 0.5;
        result.push_back(std::make_pair(index, radius));
    }

    std::sort(result.begin(), result.end(),
              [](const std::pair<int, float> &a, const std::pair<int, float> &b) -> bool
    {
        return a.second > b.second;
    });

    return result;
}


MStatus skeletonStrokeNode::generateStrokeGeometry(MDataBlock &data,
        std::vector < Stroke > *pStrokes) const
{

    MStatus st;

    if (! data.inputValue(aActive).asBool()) {
        return MS:: kSuccess;
    }

    MObject thisObj = thisMObject();

    CImg<float>  *pImage = getImage(data, aSkeletonImage );
    MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();

    // float pixelWidth = projection[0][0] / float(pImage->width());


    // MFloatMatrix imat = projection.inverse();
    int step =  data.inputValue(aSpanPixels).asInt();
    int minPixels =  data.inputValue(aMinPixels).asInt();
    float strokeLength = data.inputValue(aStrokeLength).asFloat();
    float overlap = data.inputValue(aOverlap).asFloat();


    // float maxRadius = data.inputValue(aBrushWidth).asFloat() * 0.5;

    std::vector<std::pair<int, float> > brushRadii = getBrushRadii(data);

    // for (auto iter = brushRadii.begin(); iter != brushRadii.end(); iter++ )
    // {
    //     cerr << "ID:" << iter->first << " W:" << iter->second << endl;
    // }




    if (! pImage) {
        return MS::kSuccess;
    }
    skGraph g(pImage);
    std::vector< skChain > chains;
    g.getChains(projection, chains, step, minPixels);

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

    std::vector<skChain>::const_iterator citer;
    for (citer = chains.begin(); citer != chains.end(); citer++)
    {
        MPointArray editPoints;
        MDoubleArray contacts;
        citer->appendPoints(editPoints );

        int selectedBrushId  = citer->getContacts(contacts, brushRadii);



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
