#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnEnumAttribute.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgDetail.h"
#include "jMayaIds.h"

const float pi = float(cimg::PI);
const float p8 = pi / 8.0;
const float p38 = 3.0 * p8;
const float p58 = 5.0 * p8;
const float p78 = 7.0 * p8;

MTypeId cImgDetail::id(k_cImgDetail);

MObject cImgDetail::aExpandBoundary;
MObject cImgDetail::aBlurGradient;
MObject cImgDetail::aLowThreshold;
MObject cImgDetail::aHighThreshold;
MObject cImgDetail::aMode;
MObject cImgDetail::aMedian;

cImgDetail::cImgDetail() {}

cImgDetail::~cImgDetail() {}

void *cImgDetail::creator()
{
	return new cImgDetail();
}

MStatus cImgDetail::initialize()
{
	MStatus st;

	inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;

	aExpandBoundary = nAttr.create("expandBoundary", "exb", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1);
	st = addAttribute(aExpandBoundary);

	aBlurGradient = nAttr.create("blurGradient", "bgr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aBlurGradient);

	aLowThreshold = nAttr.create("lowThreshold", "lt", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aLowThreshold);

	aHighThreshold = nAttr.create("highThreshold", "ht", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aHighThreshold);

	aMode = eAttr.create("mode", "mod", cImgDetail::kHysteresis);
	eAttr.addField("gradient", cImgDetail::kGradient);
	eAttr.addField("hysteresis", cImgDetail::kHysteresis);
	eAttr.addField("detail", cImgDetail::kDetailMagnitude);

	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute(aMode);

	aMedian = nAttr.create("median", "med", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1);
	st = addAttribute(aMedian);

	attributeAffects(aBlurGradient, aOutput);
	attributeAffects(aLowThreshold, aOutput);
	attributeAffects(aHighThreshold, aOutput);
	attributeAffects(aMode, aOutput);
	attributeAffects(aMedian, aOutput);

	return MS::kSuccess;
}

MStatus cImgDetail::process(MDataBlock &data,
							const CImg<unsigned char> &image,
							CImg<unsigned char> &result)

{

	float gradientBlur = data.inputValue(aBlurGradient).asFloat();
	float lowThreshold = data.inputValue(aLowThreshold).asFloat();
	float highThreshold = data.inputValue(aHighThreshold).asFloat();
	cImgDetail::Mode mode = cImgDetail::Mode(data.inputValue(aMode).asShort());
	int median = data.inputValue(aMedian).asInt();
	if (gradientBlur < 0)
	{
		gradientBlur = 0;
	}

	const unsigned char black[3] = {0, 0, 0};

	// CImg<unsigned char> img = image.get_norm().normalize(0, 255);
	int w = image.width();
	int h = image.height();

	result.assign(w, h, 1, 1, 255);

	if (mode == cImgDetail::kHysteresis)
	{
		findEdgesHysteresis(image, gradientBlur, lowThreshold, highThreshold, result);
	}
	else if (mode == kGradient)
	{
		findEdgesGradient(image, gradientBlur, result);
	}
	else
	{
		detailMagnitude(image, gradientBlur, lowThreshold, highThreshold, result);
	}
	if (median)
	{
		result.blur_median(median);
	}
	return MS::kSuccess;
}

void cImgDetail::detailMagnitude(
	const CImg<unsigned char> &image,
	float gradientBlur,
	float lowThreshold,
	float highThreshold,
	CImg<unsigned char> &result)

{
	int w = image.width();
	int h = image.height();
	result.assign(w, h, 1, 1, 255);
	// CImgList<> grad(img.get_norm().blur(gradientBlur).normalize(0, 255).get_gradient());
	CImgList<> grad(image.get_channel(0).blur(gradientBlur).get_gradient());


	double thresholdRange = highThreshold - lowThreshold;
	// const unsigned char black[3] = {0, 0, 0};
	const unsigned char black[1]= {0};
	for (int y = 1; y < h - 1; ++y)
	{
		for (int x = 1; x < w - 1; ++x)
		{

			double
				Gx = grad[0](x, y),
				Gy = grad[1](x, y),
				Gxy = sqrt(Gx * Gx + Gy * Gy);

			if (Gxy >= highThreshold)
			{
				result.draw_point(x, y, black);
				continue;
			}
			if (Gxy < lowThreshold)
			{
				continue;
			}
			unsigned char val(((1.0 - (Gxy - lowThreshold) / thresholdRange)) * 255);

			// inbetween
			// Neighbourhood of the actual pixel:
			const unsigned char gray[1] = {val};

			result.draw_point(x, y, gray);
		}
	}
}

void cImgDetail::findEdgesHysteresis(
	const CImg<unsigned char> &image,
	float gradientBlur,
	float lowThreshold,
	float highThreshold,
	CImg<unsigned char> &result)

{
	int w = image.width();
	int h = image.height();
	result.assign(w, h, 1, 1, 255);
	// CImgList<> grad(img.get_norm().blur(gradientBlur).normalize(0, 255).get_gradient());
	CImgList<> grad(image.get_norm().blur(gradientBlur).get_gradient());
	const unsigned char black[3] = {0, 0, 0};

	for (int y = 1; y < h - 1; ++y)
	{
		for (int x = 1; x < w - 1; ++x)
		{

			double
				Gx = grad[0](x, y),
				Gy = grad[1](x, y),
				Gxy = cimg::abs(Gx) + cimg::abs(Gy),
				// ^-- For efficient computation we observe that |Gx|+ |Gy| ~=~ sqrt( Gx^2 + Gy^2)
				Gr, Gur, Gu, Gul, Gl, Gdl, Gd, Gdr;
			// ^-- right, up right, up, up left, left, down left, down, down right.

			// theta is from the interv
			double theta = std::atan2(std::max(1e-8, Gy), Gx) + pi;

			if (Gxy >= highThreshold)
			{
				result.draw_point(x, y, black);
				continue;
			}

			// inbetween
			if (lowThreshold <= Gxy && Gxy < highThreshold)
			{
				// Neighbourhood of the actual pixel:

				Gr = cimg::abs(grad[0](x + 1, y)) + cimg::abs(grad[1](x + 1, y));		   // right
				Gl = cimg::abs(grad[0](x - 1, y)) + cimg::abs(grad[1](x - 1, y));		   // left
				Gur = cimg::abs(grad[0](x + 1, y + 1)) + cimg::abs(grad[1](x + 1, y + 1)); // up right
				Gdl = cimg::abs(grad[0](x - 1, y - 1)) + cimg::abs(grad[1](x - 1, y - 1)); // down left
				Gu = cimg::abs(grad[0](x, y + 1)) + cimg::abs(grad[1](x, y + 1));		   // up
				Gd = cimg::abs(grad[0](x, y - 1)) + cimg::abs(grad[1](x, y - 1));		   // down
				Gul = cimg::abs(grad[0](x - 1, y + 1)) + cimg::abs(grad[1](x - 1, y + 1)); // up left
				Gdr = cimg::abs(grad[0](x + 1, y - 1)) + cimg::abs(grad[1](x + 1, y - 1)); // down right

				if (Gr >= highThreshold ||
					Gur >= highThreshold ||
					Gu >= highThreshold ||
					Gul >= highThreshold ||
					Gl >= highThreshold ||
					Gdl >= highThreshold ||
					Gu >= highThreshold ||
					Gdr >= highThreshold)
				{
					result.draw_point(x, y, black);
				}
			}
		}
	}
}

void cImgDetail::findEdgesGradient(
	const CImg<unsigned char> &image,
	float gradientBlur,
	CImg<unsigned char> &result)

{
	int w = image.width();
	int h = image.height();
	result.assign(w, h, 1, 1, 255);
	// CImgList<> grad(img.get_norm().blur(gradientBlur).normalize(0, 255).get_gradient());
	CImgList<> grad(image.get_norm().blur(gradientBlur).get_gradient());
	const unsigned char black[3] = {0, 0, 0};

	for (int y = 1; y < h - 1; ++y)
	{
		for (int x = 1; x < w - 1; ++x)
		{

			double
				Gx = grad[0](x, y),					 //
				Gy = grad[1](x, y),					 //  The actual pixel is (s,t)
				Gxy = cimg::abs(Gx) + cimg::abs(Gy), //
				// ^-- For efficient computation we observe that |Gx|+ |Gy| ~=~ sqrt( Gx^2 + Gy^2)
				Gr, Gur, Gu, Gul, Gl, Gdl, Gd, Gdr;
			// ^-- right, up right, up, up left, left, down left, down, down right.
			double theta = std::atan2(std::max(1e-8, Gy),
									  Gx) +
						   pi; // theta is from the interv

			if (theta >= pi)
			{
				theta -= pi;
			}
			//rounding theta:
			if ((p8 < theta && theta <= p38) || (p78 < theta && theta <= pi))
			{
				// See (*) below for explanation of the vocabulary used.
				// Direction-pixel is (s + 1,t) with corresponding gradient value Gr.
				Gr = cimg::abs(grad[0](x + 1, y)) + cimg::abs(grad[1](x + 1, y)); // right
				// Contra-direction-pixel is (s - 1,t) with corresponding gradient value Gl.
				Gl = cimg::abs(grad[0](x - 1, y)) + cimg::abs(grad[1](x - 1, y)); // left
				if (Gr < Gxy && Gl < Gxy)
				{
					result.draw_point(x, y, black);
				}
			}
			else if (p8 < theta && theta <= p38)
			{
				// Direction-pixel is (x + 1,t + 1) with corresponding gradient value Gur.
				Gur = cimg::abs(grad[0](x + 1, y + 1)) + cimg::abs(grad[1](x + 1, y + 1)); // up right
				// Contra-direction-pixel is (x-1,t-1) with corresponding gradient value Gdl.
				Gdl = cimg::abs(grad[0](x - 1, y - 1)) + cimg::abs(grad[1](x - 1, y - 1)); // down left
				if (Gur < Gxy && Gdl < Gxy)
				{
					result.draw_point(x, y, black);
				}
			}
			else if (p38 < theta && theta <= p58)
			{
				// Direction-pixel is (x,t + 1) with corresponding gradient value Gu.
				Gu = cimg::abs(grad[0](x, y + 1)) + cimg::abs(grad[1](x, y + 1)); // up
				// Contra-direction-pixel is (x,t - 1) with corresponding gradient value Gd.
				Gd = cimg::abs(grad[0](x, y - 1)) + cimg::abs(grad[1](x, y - 1)); // down
				if (Gu < Gxy && Gd < Gxy)
				{
					result.draw_point(x, y, black);
				}
			}
			else if (p58 < theta && theta <= p78)
			{
				// Direction-pixel is (x - 1,t + 1) with corresponding gradient value Gul.
				Gul = cimg::abs(grad[0](x - 1, y + 1)) + cimg::abs(grad[1](x - 1, y + 1)); // up left
				// Contra-direction-pixel is (x + 1,t - 1) with corresponding gradient value Gdr.
				Gdr = cimg::abs(grad[0](x + 1, y - 1)) + cimg::abs(grad[1](x + 1, y - 1)); // down right
				if (Gul < Gxy && Gdr < Gxy)
				{
					result.draw_point(x, y, black);
				}
			}
		}
	}
}
