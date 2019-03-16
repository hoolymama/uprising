/*
 *  rampLookup.cpp
 *  jtools
 *
 *  Created by Julian Mann on 8/24/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */




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
#include "gateRamp.h"
#include "jMayaIds.h"

MTypeId gateRamp::id(k_gateRamp);



MObject gateRamp::aNumGates;
MObject gateRamp::aRange;
MObject gateRamp::aPadding;
MObject gateRamp::aRamp;
MObject gateRamp::aOutputMin;
MObject gateRamp::aOutputMax;
MObject gateRamp::aOutput;


gateRamp::gateRamp () {}
gateRamp::~gateRamp () {}

void *gateRamp::creator () {
	return new gateRamp;
}
/// Post constructor
void
gateRamp::postConstructor() {}

MStatus gateRamp::initialize () {

	MStatus st;
	MFnTypedAttribute tAttr;

	MRampAttribute rAttr;
	MFnNumericAttribute nAttr;



	aNumGates = nAttr.create("numGates", "ng", MFnNumericData::kInt);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	st = addAttribute(aNumGates); mser;

	aRange = nAttr.create("range", "rng", MFnNumericData::k2Float);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	st = addAttribute(aRange); mser;

	aPadding = nAttr.create("padding", "pad", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0);
	st = addAttribute(aPadding); mser;

	aRamp = rAttr.createCurveRamp("ramp", "rmp");
	st = addAttribute(aRamp ); mser;


	// Joint Rotation output
	///////////////////////////////////////////////////////////////////////

	aOutputMin = nAttr.create("outputMin", "omin", MFnNumericData::kFloat);
	aOutputMax = nAttr.create("outputMax", "omax", MFnNumericData::kFloat);
	aOutput = nAttr.create("output", "out", aOutputMin, aOutputMax);
	nAttr.setReadable( true );
	nAttr.setStorable( false );
	nAttr.setArray( true );
	nAttr.setUsesArrayDataBuilder( true );
	st = addAttribute( aOutput ); mser;


	attributeAffects (aNumGates, aOutputMax);
	attributeAffects (aRange, aOutputMax);
	attributeAffects (aPadding, aOutputMax);
	attributeAffects (aRamp, aOutputMax);


	attributeAffects (aNumGates, aOutputMin);
	attributeAffects (aRange, aOutputMin);
	attributeAffects (aPadding, aOutputMin);
	attributeAffects (aRamp, aOutputMin);



	attributeAffects (aNumGates, aOutput);
	attributeAffects (aRange, aOutput);
	attributeAffects (aPadding, aOutput);
	attributeAffects (aRamp, aOutput);


	return MS::kSuccess;
}


MStatus gateRamp::compute (const MPlug &plug, MDataBlock &data)
{

	if (!(
	      (plug == aOutput ) ||
	      (plug == aOutputMin ) ||
	      (plug == aOutputMax )

	    )) { return MS::kUnknownParameter; }

	MStatus st;


	const float2 &range = data.inputValue( aRange ).asFloat2();
	float rangeMin =   range[0];
	float rangeMax =   range[1];

	float padding = data.inputValue(aPadding).asFloat();
	int num =  data.inputValue(aNumGates).asInt();

	MFloatArray sampleVals;
	float gap = 1.0 / num;
	for (int i = 0; i < num; ++i)
	{
		sampleVals.append(gap * i);
	}
	sampleVals.append(1.0);
	// now have num+1 samples


	MFloatArray out;
	doRampLookup(thisMObject(), aRamp, sampleVals, out, 0.0, 1.0, rangeMin, rangeMax);



	// Set the rotations for the joints
	////////////////////////////////////////////////////////////////////
	MArrayDataHandle     hOutput = data.outputArrayValue( aOutput, &st ); mser;
	MArrayDataBuilder    bOutput = hOutput.builder();


	for  ( unsigned i = 0; i < num; i++) {
		MDataHandle hOut = bOutput.addElement(i);
		float2 &outval = hOut.asFloat2();
		outval[0] = out[i] - padding;
		outval[1] = out[(i + 1)] + padding;
	}

	hOutput.setAllClean();

	return MS::kSuccess;
}
