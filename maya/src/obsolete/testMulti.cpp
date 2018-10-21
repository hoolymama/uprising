/*
 *  testMulti.cpp
 *  animal
 *
 *  Created by Julian Mann on 01/08/2006.
 *  Copyright 2006 HoolyMama. All rights reserved.
 *
 */

#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include "errorMacros.h"

#include "errorMacros.h"
#include "jMayaIds.h"

#include "testMulti.h"

MTypeId testMulti::id(k_testMulti);
MObject testMulti::aInput;

MObject testMulti::aOutput;


void *testMulti::creator () {
	return new testMulti;
}
/// Post constructor
void
testMulti::postConstructor()
{
}

MStatus testMulti::initialize () {

	MStatus st;

	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;

	aInput = tAttr.create("input", "in", MFnData::kVectorArray);
	tAttr.setWritable(true);
	tAttr.setStorable(false);
	tAttr.setReadable(false);


	aOutput = nAttr.create ("output", "out", MFnNumericData::k3Double);
	nAttr.setStorable (false);
	nAttr.setWritable (false);
	nAttr.setReadable (true);
	nAttr.setArray (true);
	nAttr.setUsesArrayDataBuilder(true);


	// add attributes and setup dependencies
	addAttribute(aInput);
	addAttribute (aOutput);
	attributeAffects (aInput, aOutput);



	return MS::kSuccess;
}

testMulti::testMulti () {}
testMulti::~testMulti () {}

MStatus testMulti::compute (const MPlug &plug, MDataBlock &data)
{
	if (! (plug == aOutput )) { return MS::kUnknownParameter; }
	MStatus st = MS::kSuccess;
	MDataHandle hIn = data.inputValue( aInput, &st );	msert;
	MObject objIn  = hIn.data();
	MVectorArray in = MFnVectorArrayData(objIn).array();
	const unsigned nLength = in.length();

	MArrayDataHandle  hOutput = data.outputArrayValue( aOutput, &st ); msert;
	MArrayDataBuilder bOutput = hOutput.builder();

	for (unsigned i = 0; i < nLength; i++) {
		MDataHandle h = bOutput.addElement(i);

		double3 &out = h.asDouble3();
		out[0] = in[i].x;
		out[1] = in[i].y;
		out[2] = in[i].z;
	}
	hOutput.setAllClean();
	return st;
}
