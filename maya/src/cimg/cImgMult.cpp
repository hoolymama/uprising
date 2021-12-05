
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgMult.h"
#include "jMayaIds.h"

MTypeId cImgMult::id(k_cImgMult);

cImgMult::cImgMult() {}

cImgMult::~cImgMult() {}

void *cImgMult::creator()
{
	return new cImgMult();
}

MStatus cImgMult::initialize()
{
	MStatus st;

	inheritAttributesFrom("cImgMerge");

	return MS::kSuccess;
}

MStatus cImgMult::process(MDataBlock &data,
						  const CImgList<unsigned char> &images,
						  CImg<unsigned char> &result)

{
	if (!images.size())
	{
		return MS::kUnknownParameter;
	}

	int w = images.front().width();
	int h = images.front().height();
	int s = images.front().spectrum();

	float recip = 1.0f / 255;
	CImg<float> fimg = CImg<>(w, h, 1, s, 1);

	CImgList<unsigned char>::const_iterator citer;
	for (citer = images.begin(); citer != images.end(); citer++)
	{
		fimg.mul(CImg<float>(citer->get_resize(w, h)) * recip);
	}

	result.assign((fimg * 255).min(255));

	return MS::kSuccess;
}
