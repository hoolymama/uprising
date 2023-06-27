
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnDoubleArrayData.h>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MGlobal.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <maya/MArrayDataBuilder.h>

#include <maya/MFloatArray.h>
#include "errorMacros.h"
#include "attrUtils.h"
#include "stepRamp.h"
#include "jMayaIds.h"

MTypeId stepRamp::id(k_stepRamp);

MObject stepRamp::aNumSteps;
MObject stepRamp::aRange;
MObject stepRamp::aRamp;


MObject stepRamp::aRandomOffset;
MObject stepRamp::aRandomSeed;

MObject stepRamp::aOutput;

stepRamp::stepRamp() {}
stepRamp::~stepRamp() {}

void *stepRamp::creator()
{
	return new stepRamp;
}

void stepRamp::postConstructor() {}

MStatus stepRamp::initialize()
{

	MStatus st;
	MFnTypedAttribute tAttr;

	MRampAttribute rAttr;
	MFnNumericAttribute nAttr;

	aNumSteps = nAttr.create("numSteps", "ng", MFnNumericData::kInt);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(2);
	nAttr.setMin(2);

	st = addAttribute(aNumSteps);
	mser;

	aRange = nAttr.create("range", "rng", MFnNumericData::k2Float);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	st = addAttribute(aRange);
	mser;

	aRamp = rAttr.createCurveRamp("ramp", "rmp");
	st = addAttribute(aRamp);
	mser;


	aRandomOffset = nAttr.create("randomOffset", "ro", MFnNumericData::k2Float);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	st = addAttribute(aRandomOffset);

	aRandomSeed = nAttr.create("randomSeed", "rs", MFnNumericData::kInt);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(0);
	st = addAttribute(aRandomSeed);


	aOutput = nAttr.create("output", "out", MFnNumericData::kFloat);
	nAttr.setReadable(true);
	nAttr.setStorable(false);
	nAttr.setArray(true);
	nAttr.setUsesArrayDataBuilder(true);
	st = addAttribute(aOutput);
	mser;

	attributeAffects(aNumSteps, aOutput);
	attributeAffects(aRange, aOutput);
	attributeAffects(aRamp, aOutput);
	attributeAffects(aRandomOffset, aOutput);
	attributeAffects(aRandomSeed, aOutput);

	return MS::kSuccess;
}

MStatus stepRamp::compute(const MPlug &plug, MDataBlock &data)
{

	if (plug != aOutput)
		return MS::kUnknownParameter;

	MStatus st;

	const float2 &range = data.inputValue(aRange).asFloat2();
	float rangeMin = range[0];
	float rangeMax = range[1];
	int num = std::max(data.inputValue(aNumSteps).asInt(), 2);

	float2 &randomOffset = data.inputValue(aRandomOffset).asFloat2();
	float randomOffsetMin = randomOffset[0];
	float randomOffsetMax = randomOffset[1];
	int randomSeed = data.inputValue(aRandomSeed).asInt();
	srand48(randomSeed);

	MFloatArray sampleVals;
	float gap = 1.0 / (num - 1);
	for (int i = 0; i < num; ++i)
	{
		sampleVals.append(gap * i);
	}

	MFloatArray out;
	doRampLookup(
		thisMObject(),
		aRamp,
		sampleVals,
		out,
		0.0,
		1.0,
		rangeMin,
		rangeMax);

	for (unsigned i = 0; i < num; i++)
	{
		out[i] += (randomOffsetMin + (randomOffsetMax - randomOffsetMin) * drand48());
	}
	

	MArrayDataHandle hOutput = data.outputArrayValue(aOutput, &st);
	mser;
	MArrayDataBuilder bOutput = hOutput.builder();

	for (unsigned i = 0; i < num; i++)
	{
		MDataHandle hOut = bOutput.addElement(i);
		hOut.set(out[i]);
	}

	hOutput.setAllClean();

	return MS::kSuccess;
}
