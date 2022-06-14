
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>

#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNurbsCurveData.h>

#include "pearlStrokeNode.h"
#include "stroke.h"
// #include "strokeRotationSpec.h"
// #include "strokeRepeatSpec.h"
#include "brushData.h"

#include <jMayaIds.h>
#include "errorMacros.h"
#include "mayaMath.h"
#include "pearlChainData.h"
#include "texUtils.h"
const double rad_to_deg = (180 / 3.1415927);

MObject pearlStrokeNode::aTargetRotationMatrix;

MObject pearlStrokeNode::aInputMatrix;
MObject pearlStrokeNode::aPearlChains;

MObject pearlStrokeNode::aSmoothNeighbors;
MObject pearlStrokeNode::aSmoothPositions;
MObject pearlStrokeNode::aSmoothWeights;
 

MObject pearlStrokeNode::aMinimumPoints;

MTypeId pearlStrokeNode::id(k_pearlStrokeNode);

pearlStrokeNode::pearlStrokeNode() {}

pearlStrokeNode::~pearlStrokeNode() {}

void *pearlStrokeNode::creator()
{
    return new pearlStrokeNode();
}

void pearlStrokeNode::postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}


MStatus pearlStrokeNode::initialize()
{
    MStatus st;

    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnEnumAttribute eAttr;
    MFnCompoundAttribute cAttr;
    MFnMatrixAttribute mAttr;
    MFnUnitAttribute uAttr;

    inheritAttributesFrom("strokeCreator");

    aTargetRotationMatrix = mAttr.create("targetRotationMatrix", "tmat", MFnMatrixAttribute::kFloat);
    mAttr.setStorable(false);
    mAttr.setHidden(false);
    mAttr.setKeyable(true);
    addAttribute(aTargetRotationMatrix);

    aInputMatrix = mAttr.create("inputMatrix", "inmat", MFnMatrixAttribute::kDouble);
    mAttr.setStorable(false);
    mAttr.setHidden(false);
    mAttr.setKeyable(true);
    addAttribute(aInputMatrix);

    aPearlChains = tAttr.create("pearlChains", "pcn", pearlChainData::id);
    tAttr.setStorable(false);
    tAttr.setArray(true);
    tAttr.setHidden(false);
    tAttr.setKeyable(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    st = addAttribute(aPearlChains);
    mser;

    aSmoothNeighbors = nAttr.create("neighborsToSmooth", "smn", MFnNumericData::kInt);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(-1);
    st = addAttribute(aSmoothNeighbors);

    aSmoothPositions = nAttr.create("smoothPositions", "smps", MFnNumericData::kBoolean);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(false);
    st = addAttribute(aSmoothPositions);

    aSmoothWeights = nAttr.create("smoothWeights", "swts", MFnNumericData::kBoolean);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(false);
    st = addAttribute(aSmoothWeights);
 
    aMinimumPoints = nAttr.create("minimumPoints", "mnpts", MFnNumericData::kInt);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setWritable(true);
    nAttr.setDefault(2);
    st = addAttribute(aMinimumPoints);
    mser;
 
    attributeAffects(aTargetRotationMatrix, aOutput);

    attributeAffects(aPearlChains, aOutput);
    attributeAffects(aInputMatrix, aOutput);

    attributeAffects(aSmoothNeighbors, aOutput);
    attributeAffects(aSmoothPositions, aOutput);
    attributeAffects(aSmoothWeights, aOutput);
 
    return (MS::kSuccess);
}

MStatus pearlStrokeNode::generateStrokeGeometry(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes)
{

    MStatus st;

    short int nodeState = data.inputValue(state).asShort();
    if (nodeState != 0)
    {
        return MS::kUnknownParameter;
    }

    MObject thisObj = thisMObject();
    MFnDependencyNode fnNode(thisObj);
    MString nodeName = fnNode.name();

    float pointDensity = fmax(data.inputValue(aPointDensity).asFloat(), 0.001);

    int minimumPoints = data.inputValue(aMinimumPoints).asInt();
    minimumPoints = std::max(minimumPoints, 2);
    int layerId = data.inputValue(aLayerId).asInt();

    int smoothNeighbors = data.inputValue(aSmoothNeighbors).asInt();
    bool smoothPositions = data.inputValue(aSmoothPositions).asBool();
    bool smoothWeights = data.inputValue(aSmoothWeights).asBool();
 
    MArrayDataHandle hChainsArray = data.inputArrayValue(aPearlChains, &st);
    unsigned nPearlChains = hChainsArray.elementCount();

    MFloatMatrix targetRotationMatrix = data.inputValue(aTargetRotationMatrix).asFloatMatrix();
    targetRotationMatrix = mayaMath::rotationOnly(targetRotationMatrix);

    MMatrix wm = data.inputValue(pearlStrokeNode::aInputMatrix).asMatrix();
    double scales[3];
    MTransformationMatrix(wm).getScale(scales, MSpace::kWorld);
    double scale = scales[0];

    const double epsilon = 0.0001;
    MPointArray editPoints;
    MFloatArray radii;


    MFloatPointArray points;
 


    for (unsigned i = 0; i < nPearlChains; i++, hChainsArray.next())
    {

        int elIndex = hChainsArray.elementIndex();

        MDataHandle hChains = hChainsArray.inputValue();

        MObject dChains = hChains.data();
        MFnPluginData fnChains(dChains, &st);
        if (st.error())
        {
            continue;
        }
        pearlChainData *pcData = (pearlChainData *)fnChains.data(&st);
        if (st.error())
        {
            continue;
        }

        const std::vector<pearlChain> *geom = pcData->fGeometry;

        std::vector<pearlChain>::const_iterator current_chain;
        for (current_chain = geom->begin(); current_chain != geom->end(); current_chain++)
        {


            editPoints.clear();
            radii.clear();

            current_chain->appendPointsTo(editPoints, wm);
            current_chain->appendRadiiTo(radii, float(scale));

            Stroke stroke(editPoints, radii, pointDensity, minimumPoints, targetRotationMatrix);
         
            if (stroke.valid())
            {
                Stroke::target_iterator titer = stroke.targets_begin();
                for (; titer != stroke.targets_end(); titer++)
                {
                    float w = titer->weight();
                    titer->setColor(MColor(w,w,w,w));
                }
                stroke.setParentId(elIndex);
                pOutStrokes->push_back(stroke);
            }
        }

    }

    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
        curr_stroke->setLayerId(layerId);
    }

    if ((smoothPositions || smoothWeights) && smoothNeighbors > 0)
    {
        for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
        {
            curr_stroke->smoothTargets(smoothNeighbors, smoothPositions, smoothWeights);
        }
    }



    strokeCreator::generateStrokeGeometry(plug, data, pOutStrokes);

    return MS::kSuccess;
}


