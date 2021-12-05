
#include <algorithm> // std::max

#include <maya/MDataHandle.h>
#include <maya/MObject.h>
#include <maya/MFnPluginData.h>
#include <maya/MStatus.h>

#include "cImgUtils.h"
#include "cImgFloatData.h"

CImg<float> *cImgUtils::getFloatImage(MDataBlock &data, MObject &attribute)
{
	MStatus st;
	MDataHandle hImageData = data.inputValue(attribute, &st);
	if (st.error())
	{
		return 0;
	}
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData(dImageData, &st);
	if (st.error())
	{
		return 0;
	}
	cImgFloatData *imageData = (cImgFloatData *)fnImageData.data();
	return imageData->fImg;
}

CImg<unsigned char> *cImgUtils::getImage(MDataBlock &data, MObject &attribute)
{
	MStatus st;
	MDataHandle hImageData = data.inputValue(attribute, &st);
	if (st.error())
	{
		return 0;
	}
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData(dImageData, &st);
	if (st.error())
	{
		return 0;
	}
	cImgData *imageData = (cImgData *)fnImageData.data();
	return imageData->fImg;
}

cImgData *cImgUtils::getOutImage(MDataBlock &data, MObject &attribute)
{
	MStatus st;
	MDataHandle hData = data.outputValue(attribute, &st);
	if (st.error())
	{
		return 0;
	}
	MFnPluginData fnData;
	MTypeId kdid(cImgData::id);
	MObject dData = fnData.create(kdid, &st);
	if (st.error())
	{
		return 0;
	}
	cImgData *imageData = (cImgData *)fnData.data(&st);
	if (st.error())
	{
		return 0;
	}
	return imageData;
}

void cImgUtils::toImageCoords(
	const MFloatPoint &point,
	const MFloatMatrix &inverseProjection,
	int w, int h, float &x, float &y, bool clamp)
{
	// puts p  in -1 -> 1
	MFloatPoint p = point * inverseProjection;

	x = ((p.x * 0.5f) + 0.5f) * w;
	y = (1.0f - ((p.y * 0.5f) + 0.5f)) * h;
	if (clamp)
	{
		x = std::max(0.0f, std::min(x, float(w)));
		y = std::max(0.0f, std::min(y, float(h)));
	}
}

void cImgUtils::getImageChannel(const CImg<unsigned char> &image,
								cImgData::Channel channel, CImg<unsigned char> &result)
{

	int nChannels = image.spectrum();

	if (channel == cImgData::kRed || nChannels < 3)
	{
		result = image.get_channel(0);
	}
	else if (channel == cImgData::kGreen)
	{
		result = image.get_channel(1);
	}
	else if (channel == cImgData::kBlue)
	{
		result = image.get_channel(2);
	}
	else // average
	{
		result = image.get_norm();
	}
}

// void cImgUtils::getImageChannel(const CImg<unsigned char> &image,
//                                 cImgData::Channel channel, CImg<float> &result)
// {

// 	int nChannels = image.spectrum();

// 	if (channel == cImgData::kRed || nChannels  < 3 ) {
// 		result = image.get_channel(0);
// 	}
// 	else if (channel == cImgData::kGreen) {
// 		result =  image.get_channel(1);
// 	}
// 	else if (channel == cImgData::kBlue) {
// 		result = image.get_channel(2);
// 	}
// 	else   // average
// 	{
// 		result =  image.get_norm();
// 	}
// }

void cImgUtils::sampleNearest(
	const CImg<unsigned char> &image,
	const MDoubleArray &uVals,
	const MDoubleArray &vVals,
	MFloatArray &result)
{
	unsigned len = uVals.length();
	if (vVals.length() != len)
	{
		return;
	}
	int w = image.width();
	int h = image.height();
	if (!(w && h))
	{
		return;
	}
	for (int i = 0; i < len; ++i)
	{
		float x, y;
		unsigned char val;
		cImgUtils::toImageCoords(float(uVals[i]), float(vVals[i]), w, h, x, y);
		result.append(image(int(x + 0.5), int(y + 0.5)) / 255.0);
	}
}

void cImgUtils::sampleNearest(
	const CImg<unsigned char> &image,
	const MDoubleArray &uVals,
	const MDoubleArray &vVals,
	MIntArray &result)
{
	unsigned len = uVals.length();
	if (vVals.length() != len)
	{
		return;
	}
	int w = image.width();
	int h = image.height();
	if (!(w && h))
	{
		return;
	}
	for (int i = 0; i < len; ++i)
	{
		float x, y;
		cImgUtils::toImageCoords(float(uVals[i]), float(vVals[i]), w, h, x, y);
		result.append(int(image(int(x + 0.5), int(y + 0.5))));
	}
}

void cImgUtils::sampleLinear(
	const CImg<unsigned char> &image,
	const MDoubleArray &uVals,
	const MDoubleArray &vVals,
	MFloatArray &result)
{
	unsigned len = uVals.length();
	if (vVals.length() != len)
	{
		return;
	}
	int w = image.width();
	int h = image.height();
	if (!(w && h))
	{
		return;
	}
	for (int i = 0; i < len; ++i)
	{
		float x, y;
		unsigned char val;
		cImgUtils::toImageCoords(float(uVals[i]), float(vVals[i]), w, h, x, y);
		val = result.append(float(image.linear_atXY(x, y, 0, 0)) / 255.0);
	}
}

void cImgUtils::sampleCubic(
	const CImg<unsigned char> &image,
	const MDoubleArray &uVals,
	const MDoubleArray &vVals,
	MFloatArray &result)
{
	unsigned len = uVals.length();
	if (vVals.length() != len)
	{
		return;
	}
	int w = image.width();
	int h = image.height();
	if (!(w && h))
	{
		return;
	}
	for (int i = 0; i < len; ++i)
	{
		float x, y;
		unsigned char val;
		cImgUtils::toImageCoords(float(uVals[i]), float(vVals[i]), w, h, x, y);
		val = result.append(float(image.cubic_atXY(x, y, 0, 0)) / 255.0);
	}
}

void cImgUtils::toImageCoords(float u, float v, int w, int h, float &x, float &y)
{
	u = std::max(0.0f, std::min(u, 1.0f));
	v = std::max(0.0f, std::min(v, 1.0f));
	x = u * w;
	y = (1.0 - v) * h;
}

void cImgUtils::invert(CImg<unsigned char> &image)
{
	int w = image.width();
	int h = image.height();
	int s = image.spectrum();
	for (int c = 0; c < s; ++c)
	{
		for (int y = 0; y < h; ++y)
		{
			for (int x = 0; x < w; ++x)
			{
				image(x, y, 0, c) = 255 - image(x, y, 0, c);
			}
		}
	}
}

void cImgUtils::get_invert(const CImg<unsigned char> &image, CImg<unsigned char> &result)
{
	result.assign(image);
	invert(result);
}


void cImgUtils::img_stats(const MString&name, const CImg<unsigned char>*img)
{
	cerr << "------ " << name << " ------" << endl;
	cerr << "w: " <<  img->width() << endl;
	cerr << "h: " <<  img->height() << endl;
	cerr << "s: " <<  img->spectrum() << endl;
	cerr << "d: " <<  img->depth() << endl;
}

void cImgUtils::img_stats(const MString&name, const CImg<float>*img)
{
	cerr << "------ " << name << " ------" << endl;
	cerr << "w: " <<  img->width() << endl;
	cerr << "h: " <<  img->height() << endl;
	cerr << "s: " <<  img->spectrum() << endl;
	cerr << "d: " <<  img->depth() << endl;
}