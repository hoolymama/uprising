#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnEnumAttribute.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgBlur.h"
#include "jMayaIds.h"

MTypeId cImgBlur::id(k_cImgBlur);

MObject cImgBlur::aAmount;
MObject cImgBlur::aBoundary;
MObject cImgBlur::aGaussian;

cImgBlur::cImgBlur() {}

cImgBlur::~cImgBlur() {}

void *cImgBlur::creator()
{
	return new cImgBlur();
}

MStatus cImgBlur::initialize()
{
	MStatus st;

	inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;

	aAmount = nAttr.create("amount", "amt", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aAmount);

	aBoundary = eAttr.create("boundary", "bnd", cImgBlur::kBoundaryDirichlet);
	eAttr.addField("dirichlet", cImgBlur::kBoundaryDirichlet);
	eAttr.addField("neumann", cImgBlur::kBoundaryNeumann);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute(aBoundary);

	aGaussian = nAttr.create("gaussian", "gus", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aGaussian);

	attributeAffects(aAmount, aOutput);
	attributeAffects(aBoundary, aOutput);
	attributeAffects(aGaussian, aOutput);

	return MS::kSuccess;
}

MStatus cImgBlur::process(MDataBlock &data,
						  const CImg<unsigned char> &image,
						  CImg<unsigned char> &result)

{

	float amount = data.inputValue(aAmount).asFloat();
	bool gauss = data.inputValue(aGaussian).asBool();

	bool boundary = cImgBlur::kBoundaryNeumann == cImgBlur::Boundary(data.inputValue(
																			 aBoundary)
																		 .asShort());

	if (amount < 0)
	{
		amount = 0;
	}
	result = image.get_blur(amount, boundary, gauss);
	return MS::kSuccess;
}