/*
 *  rampLookup.cpp
 *  jtools
 *
 *  Created by Julian Mann on 8/24/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <vector>

#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MColorArray.h>
#include <maya/MArrayDataBuilder.h>


#include <maya/MFloatArray.h>
#include "errorMacros.h"
#include "attrUtils.h"
#include "paletteOrder.h"
#include "jMayaIds.h"

MTypeId paletteOrder::id(k_paletteOrder);

MObject paletteOrder::aInput;
MObject paletteOrder::aMethod;
MObject paletteOrder::aOffset;
MObject paletteOrder::aOutput;


paletteOrder::paletteOrder () {}
paletteOrder::~paletteOrder () {}

void *paletteOrder::creator () {
	return new paletteOrder;
}

void paletteOrder::postConstructor() {}

MStatus paletteOrder::initialize () {

	MStatus st;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;

	aInput = nAttr.createColor("input", "in");
	nAttr.setStorable(true);
	nAttr.setArray(true);
	st = addAttribute(aInput);

	aMethod = eAttr.create("method", "mth", paletteOrder::kNone);
	eAttr.addField("None", paletteOrder::kNone);
	eAttr.addField("HSPAscending", paletteOrder::kHSPAscending);
	eAttr.addField("HSPDescending", paletteOrder::kHSPDescending);
	eAttr.setKeyable(true);
	eAttr.setHidden(false);
	st = addAttribute(aMethod);


	aOffset = nAttr.create("offset", "off", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0);
	addAttribute(aOffset);

	aOutput = nAttr.create("output", "out", MFnNumericData::kInt);
	nAttr.setReadable(true);
	nAttr.setStorable(false);
	nAttr.setKeyable(false);
	nAttr.setArray(true);
	nAttr.setUsesArrayDataBuilder(true);
	addAttribute(aOutput);

	attributeAffects (aOffset, aOutput);
	attributeAffects (aInput, aOutput);
	attributeAffects (aMethod, aOutput);

	return MS::kSuccess;
}

bool compareHSP(std::tuple<int, MColor , int> c1, std::tuple<int, MColor , int> c2)
{
	/*
	 * Compare two colors based on their HSP value

	 Must be strict weak ordering, therefore, if brightnesses are the same we compare original index.
	 */
	float brightness1 = paletteOrder::calculateHSP(std::get<1>(c1));
	float brightness2 = paletteOrder::calculateHSP(std::get<1>(c2));
	if (brightness1 < brightness2) {
		return true;
	} else  if (brightness1 > brightness2) {
		return false;
	}
	return std::get<0>(c1) < std::get<0>(c2);
}

bool compareOrigIndex(std::tuple<int, MColor , int> c1, std::tuple<int, MColor , int> c2)
{
	/*
	 * Compare two colors based on their original index value
	 */
	return std::get<0>(c1) < std::get<0>(c2);
}


MStatus paletteOrder::compute (const MPlug &plug, MDataBlock &data)
{

	if (plug != aOutput ) { return MS::kUnknownParameter; }

	MStatus st;

	int offset = data.inputValue(aOffset).asInt();
	MArrayDataHandle ha = data.inputArrayValue(aInput);
	unsigned nInputs = ha.elementCount();

	paletteOrder::Method method = paletteOrder::Method(data.inputValue(aMethod).asShort());
	
	MArrayDataHandle hOutput = data.outputArrayValue(aOutput, &st);mser;
	MArrayDataBuilder bOutput = hOutput.builder();
	
	if (method == paletteOrder::kNone) {
		for (unsigned i = 0; i < nInputs; i++) {
			MDataHandle hOut = bOutput.addElement(i);
			hOut.set(int(i));
		}
		hOutput.set(bOutput);
		hOutput.setAllClean();
		return MS::kSuccess;
	}

	// create the vector of colors (index and color)) 
	std::vector<std::tuple<int, MColor , int> > colors;
	for (unsigned i = 0; i < nInputs; i++, ha.next())
	{
		const MFloatVector &color = ha.inputValue().asFloatVector();
		colors.push_back(std::make_tuple( i, MColor(color.x, color.y, color.z ),0 ));
	}

	// sort on brightness
	std::sort(colors.begin(), colors.end(), compareHSP);
	// reverse (maybe)
	if (method == paletteOrder::kHSPDescending) {
		std::reverse(colors.begin(), colors.end());
	}

	// Add the new order number (with offset) to the tuples
	std::vector<std::tuple<int, MColor , int> >::iterator it;
	it = colors.begin();
	for (unsigned i=offset; it != colors.end(); it++, i++)
	{
		std::get<2>(*it) = i;
	}

	// Re-sort on the original index
	std::sort(colors.begin(), colors.end(), compareOrigIndex);

	// Put the order into the output.
	it = colors.begin();
	for (unsigned i=0; it != colors.end(); it++, i++)
	{
		MDataHandle hOut = bOutput.addElement(i);
		hOut.set(std::get<2>(*it));
	}

  	hOutput.set(bOutput);
	hOutput.setAllClean();

	return MS::kSuccess;
}
 