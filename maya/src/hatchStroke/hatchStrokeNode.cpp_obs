
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

#include "hatchStrokeNode.h"
#include "stroke.h"
// #include "strokeRotationSpec.h"
// #include "strokeRepeatSpec.h"
#include "brushData.h"

#include <jMayaIds.h>
#include "errorMacros.h"
#include "mayaMath.h"
// #include "pearlChainData.h"
#include "texUtils.h"
const double rad_to_deg = (180 / 3.1415927);

MObject hatchStrokeNode::aPoints;
MObject hatchStrokeNode::aGradients;

MObject hatchStrokeNode::aScaleGradients;
  
MObject hatchStrokeNode::aLengthRamp;
MObject hatchStrokeNode::aLengthRangeMin;
MObject hatchStrokeNode::aLengthRangeMax;
MObject hatchStrokeNode::aLengthRange;

  
MObject hatchStrokeNode::aWidthRamp;
MObject hatchStrokeNode::aWidthRangeMin;
MObject hatchStrokeNode::aWidthRangeMax;
MObject hatchStrokeNode::aWidthRange;



MObject hatchStrokeNode::aTargetRotationMatrix;
MObject hatchStrokeNode::aInputMatrix;
MObject hatchStrokeNode::aSmoothNeighbors;
MObject hatchStrokeNode::aSmoothPositions;
MObject hatchStrokeNode::aSmoothWeights;
 

MObject hatchStrokeNode::aMinimumPoints;

MTypeId hatchStrokeNode::id(k_hatchStrokeNode);

hatchStrokeNode::hatchStrokeNode() {}

hatchStrokeNode::~hatchStrokeNode() {}

void *hatchStrokeNode::creator()
{
    return new hatchStrokeNode();
}

void hatchStrokeNode::postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}


MStatus hatchStrokeNode::initialize()
{
    MStatus st;

    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnEnumAttribute eAttr;
    MFnCompoundAttribute cAttr;
    MFnMatrixAttribute mAttr;
    MFnUnitAttribute uAttr;

    inheritAttributesFrom("strokeCreator");


	aPoints = tAttr.create("points", "pts", MFnData::kVectorArray);
	tAttr.setStorable(false);
	tAttr.setReadable(false);
	st = addAttribute(aPoints);
	mser;

	aGradients = tAttr.create("gradients", "grd", MFnData::kVectorArray);
	tAttr.setReadable(false);
	st = addAttribute(aGradients);
	mser;


    aScaleGradients = nAttr.create("scaleGradients", "sgd", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(1.0);
    st = addAttribute(aScaleGradients);



	aLengthRamp = MRampAttribute::createCurveRamp("lengthRamp", "lrmp");
	st = addAttribute(aLengthRamp);
	mser;

	aLengthRangeMin = nAttr.create("lengthRangeMin", "lnrn", MFnNumericData::kDouble);
	nAttr.setDefault(0.0);
	aLengthRangeMax = nAttr.create("lengthRangeMax", "lnrx", MFnNumericData::kDouble);
	nAttr.setDefault(0.0);
	aLengthRange = nAttr.create("lengthRange", "lnr", aLengthRangeMin, aLengthRangeMax);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	st = addAttribute(aLengthRange);
	mser;

	aWidthRamp = MRampAttribute::createCurveRamp("widthRamp", "wrmp");
	st = addAttribute(aWidthRamp);
	mser;

	aWidthRangeMin = nAttr.create("widthRangeMin", "wdrn", MFnNumericData::kDouble);
	nAttr.setDefault(0.0);
	aWidthRangeMax = nAttr.create("widthRangeMax", "wdrx", MFnNumericData::kDouble);
	nAttr.setDefault(0.0);
	aWidthRange = nAttr.create("widthRange", "wdr", aWidthRangeMin, aWidthRangeMax);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	st = addAttribute(aWidthRange);
	mser;




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

    // aPearlChains = tAttr.create("pearlChains", "pcn", pearlChainData::id);
    // tAttr.setStorable(false);
    // tAttr.setArray(true);
    // tAttr.setHidden(false);
    // tAttr.setKeyable(true);
    // tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    // st = addAttribute(aPearlChains);
    // mser;

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

    attributeAffects(aPoints, aOutput);
    attributeAffects(aGradients, aOutput);
    
    attributeAffects(aScaleGradients, aOutput);
    attributeAffects(aLengthRamp, aOutput);;
    attributeAffects(aLengthRangeMin, aOutput);;
    attributeAffects(aLengthRangeMax, aOutput);;
    attributeAffects(aLengthRange, aOutput);;

    attributeAffects(aWidthRamp, aOutput);;
    attributeAffects(aWidthRangeMin, aOutput);;
    attributeAffects(aWidthRangeMax, aOutput);;
    attributeAffects(aWidthRange, aOutput);;



    attributeAffects(aInputMatrix, aOutput);

    attributeAffects(aSmoothNeighbors, aOutput);
    attributeAffects(aSmoothPositions, aOutput);
    attributeAffects(aSmoothWeights, aOutput);
 
    return (MS::kSuccess);
}

MStatus hatchStrokeNode::generateStrokeGeometry(
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
 
    MFloatMatrix targetRotationMatrix = data.inputValue(aTargetRotationMatrix).asFloatMatrix();
    targetRotationMatrix = mayaMath::rotationOnly(targetRotationMatrix);

    MMatrix wm = data.inputValue(hatchStrokeNode::aInputMatrix).asMatrix();
    double scales[3];
    MTransformationMatrix(wm).getScale(scales, MSpace::kWorld);
    double scale = scales[0];

    const double epsilon = 0.0001;
    MPointArray editPoints;
    MFloatArray radii;



	MDataHandle hPoints = data.inputValue(aPoints);
	MObject dPoints = hPoints.data();
	MFnVectorArrayData fnPoints(dPoints);
	MVectorArray points = fnPoints.array(&st);

 	MDataHandle hGradients = data.inputValue(aGradients);
	MObject dGradients = hGradients.data();
	MFnVectorArrayData fnGradients(dGradients);
	MVectorArray gradients = fnGradients.array(&st);

    float scaleGradients = data.inputValue(aScaleGradients).asFloat();

	MRampAttribute lengthAttr(thisObj, aLengthRamp);
	mser;

	MDataHandle hLengthRange = data.inputValue(aLengthRange);
	double lengthRangeMin = hLengthRange.child(aLengthRangeMin).asDouble();
	double lengthRangeMax = hLengthRange.child(aLengthRangeMax).asDouble();
    double lengthRange = lengthRangeMax - lengthRangeMin;


	MRampAttribute widthAttr(thisObj, aLengthRamp);
	mser;

	MDataHandle hWidthRange = data.inputValue(aLengthRange);
	double widthRangeMin = hLengthRange.child(aLengthRangeMin).asDouble();
	double widthRangeMax = hLengthRange.child(aLengthRangeMax).asDouble();
    double widthRange = widthRangeMax - widthRangeMin;



    unsigned nPoints = points.length();
    if (nPoints < 2 || nPoints != gradients.length())
    {
        return MS::kUnknownParameter;
    }


    for (unsigned i = 0; i < nPoints; i++)
    {
        editPoints.clear();
        radii.clear();

        MVector p = points[i];
        MVector g = gradients[i] ;
        MVector gn = g.normal();
        float gMag = g.length() * scaleGradients;
        float strokeLength;
 
		lengthAttr.getValueAtPosition(gMag, strokeLength, &st);
        strokeLength = (strokeLength * lengthRange) + lengthRangeMin;

        MPoint start = p - gn * strokeLength * 0.5;
        MPoint end = p + gn * strokeLength * 0.5;

        editPoints.append(start);
        editPoints.append(end);

        float strokeRadius;
        widthAttr.getValueAtPosition(gMag, strokeRadius, &st);
        strokeRadius = ((strokeRadius * widthRange) + widthRangeMin) * 0.5;
 
        radii.append(strokeRadius);
        radii.append(strokeRadius);

        Stroke stroke(editPoints, radii, pointDensity, minimumPoints, targetRotationMatrix);
         
        if (stroke.valid())
        {
            Stroke::target_iterator titer = stroke.targets_begin();
            for (; titer != stroke.targets_end(); titer++)
            {
                float w = titer->weight();
                titer->setColor(MColor(w,w,w,w));
            }
            stroke.setParentId(0);
            pOutStrokes->push_back(stroke);
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


