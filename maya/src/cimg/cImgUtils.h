
#ifndef cImgUtils_H
#define cImgUtils_H
#define cimg_display 0
#include <math.h>

// #include "CImg.h"
#include <maya/MFloatMatrix.h>
#include <maya/MFloatPoint.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatArray.h>
#include <maya/MIntArray.h>
#include <maya/MDataBlock.h>

#include "cImgData.h"

// using namespace cimg_library;

class cImgUtils
{

public:
	static CImg<float> *getFloatImage(MDataBlock &data, MObject &attribute);

	static CImg<unsigned char> *getImage(MDataBlock &data, MObject &attribute);

	static cImgData *getOutImage(MDataBlock &data, MObject &attribute);

	static void toImageCoords(
		const MFloatPoint &point,
		const MFloatMatrix &inverseProjection,
		int w, int h, float &x, float &y, bool clamp = true);

	static void getImageChannel(const CImg<unsigned char> &image,
								cImgData::Channel channel, CImg<unsigned char> &result);

	static void sampleNearest(
		const CImg<unsigned char> &image,
		const MDoubleArray &uVals,
		const MDoubleArray &vVals,
		MFloatArray &result);

	static void sampleLinear(
		const CImg<unsigned char> &image,
		const MDoubleArray &uVals,
		const MDoubleArray &vVals,
		MFloatArray &result);

	static void sampleCubic(
		const CImg<unsigned char> &image,
		const MDoubleArray &uVals,
		const MDoubleArray &vVals,
		MFloatArray &result);

	static void sampleNearest(
		const CImg<unsigned char> &image,
		const MDoubleArray &uVals,
		const MDoubleArray &vVals,
		MIntArray &result);

	static void toImageCoords(float u, float v, int w, int h, float &x, float &y);

	static void transformUvToImage(float w, float h, MFloatMatrix &result);

	static void invert(CImg<unsigned char> &image);

	static void get_invert(const CImg<unsigned char> &image, CImg<unsigned char> &result);


	static void img_stats(const MString&name, const CImg<unsigned char>*img);
	static void img_stats(const MString&name, const CImg<float>*img);

};

#endif

// struct {
// 	bool operator()(std::vector<unsigned char> a, std::vector<unsigned char> b) const
// 	{
// 		int spectrum = a.size();
// 		int last = spectrum - 1;
// 		for (int i = 0; i < last; ++i)
// 		{
// 			if (a[i] < b[i]) {
// 				return true;
// 			}
// 			else if (a[i] > b[i]) {
// 				return false;
// 			}
// 		}
// 		return (a[last] < b[last]);

// 	}
// } ColorCompare;

// struct {
// 	bool operator()(std::vector<unsigned char> a, std::vector<unsigned char> b) const
// 	{
// 		int spectrum = a.size();
// 		for (int i = 0; i < spectrum; ++i)
// 		{
// 			if (a[i] != b[i]) {
// 				return false;
// 			}
// 		}
// 		return true;
// 	}
// } ColorEquals;

// if (colorList.size() > 0) {
// 	cerr << "colorList[0][0]: " << int(colorList[0][0]) << endl;
// }
// std::sort(colorList.begin(), colorList.end(), ColorCompare); // 1 1 2 2 3 3 3 4 4 5 5 6 7

// for (auto &elem : idxMap) {
// 	for (int i = 0; i < spectrum; ++i)
// 	{
// 		cerr << int(elem.first[i]) << " ";
// 	}
// 	cerr << " : " << elem.second << endl;
// }

// auto last = std::unique(colorList.begin(), colorList.end(), ColorEquals);
// colorList.erase(last, colorList.end());

// cerr << "size: " << colorList.size() << endl;

// int i;
// for (auto color : colorList)
// {
// 	for (int c = 0; c < spectrum; c++)
// 	{
// 		cerr << int(color[c]) << " ";
// 	}
// 	cerr << endl;
// }

// result.assign(image);
