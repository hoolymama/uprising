#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>

#include "errorMacros.h"
#include "projectToPlanePP.h"
#include "jMayaIds.h"

MTypeId projectToPlanePP::id(k_projectToPlanePP);

MObject projectToPlanePP::aInput;
MObject projectToPlanePP::aPlaneNormal;
MObject projectToPlanePP::aOutput;

void *projectToPlanePP::creator()
{
	return new projectToPlanePP;
}

/// Post constructor
void projectToPlanePP::postConstructor()
{
	MPxNode::postConstructor();

	setExistWithoutInConnections(true);
	setExistWithoutOutConnections(true);
}

MStatus projectToPlanePP::initialize()
{

	MStatus st;

	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;

	aInput = tAttr.create("input", "in", MFnData::kVectorArray);
	tAttr.setWritable(true);
	tAttr.setStorable(false);
	tAttr.setReadable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);

	st = addAttribute(aInput);
	mser;

	aPlaneNormal = nAttr.create("planeNormal", "pn", MFnNumericData::k3Double);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0, 0.0, 0.0);
	st = addAttribute(aPlaneNormal);
	mser;

	aOutput = tAttr.create("output", "out", MFnData::kVectorArray);
	tAttr.setStorable(false);
	tAttr.setWritable(false);
	tAttr.setReadable(true);
	st = addAttribute(aOutput);
	mser;

	st = attributeAffects(aInput, aOutput);
	mser;
	st = attributeAffects(aPlaneNormal, aOutput);
	mser;

	return MS::kSuccess;
}

projectToPlanePP::projectToPlanePP() {}
projectToPlanePP::~projectToPlanePP() {}

MStatus projectToPlanePP::compute(const MPlug &plug, MDataBlock &data)
{

	if (plug != aOutput)
		return MS::kUnknownParameter;

	MStatus st;

	const MVectorArray input = MFnVectorArrayData(data.inputValue(aInput).data()).array();

	const double3 &pn = data.inputValue(aPlaneNormal).asDouble3();
	MVector n(pn[0], pn[1], pn[2]);

	int len = input.length();

	MVectorArray out;
	out.copy(input);

	for (int i = 0; i < len; i++)
	{

		MVector bi = input[i] ^ n;
		MVector shadow = (n ^ bi).normal();
		double dot = input[i] * shadow;
		out.set(shadow * dot, i);
	}

	MDataHandle hOut = data.outputValue(aOutput);
	MFnVectorArrayData fnOut;
	MObject dOut = fnOut.create(out);
	hOut.set(dOut);
	data.setClean(plug);
	return MS::kSuccess;
}