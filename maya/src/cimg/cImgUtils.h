/**
 * @file cImgUtils.h
 * @brief Utility functions for image processing operations.
 *
 * This file provides utility functions for image processing:
 * - Image data retrieval and manipulation
 * - Coordinate transformations
 * - Image sampling and interpolation
 * - Common image operations
 */

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

/**
 * @class cImgUtils
 * @brief Static utility class for image processing operations.
 *
 * cImgUtils provides common utility functions:
 * - Image data retrieval from Maya data block
 * - Coordinate space transformations
 * - Image sampling with different interpolation methods
 * - Common image operations like inversion
 */
class cImgUtils
{

public:
	/**
	 * @brief Gets a float image from a Maya data block.
	 * @param data Maya data block
	 * @param attribute Attribute to retrieve image from
	 * @return Pointer to float image data
	 */
	static CImg<float> *getFloatImage(MDataBlock &data, MObject &attribute);

	/**
	 * @brief Gets an unsigned char image from a Maya data block.
	 * @param data Maya data block
	 * @param attribute Attribute to retrieve image from
	 * @return Pointer to image data
	 */
	static CImg<unsigned char> *getImage(MDataBlock &data, MObject &attribute);

	/**
	 * @brief Gets output image data from a Maya data block.
	 * @param data Maya data block
	 * @param attribute Attribute to retrieve image from
	 * @return Pointer to image data
	 */
	static cImgData *getOutImage(MDataBlock &data, MObject &attribute);

	/**
	 * @brief Transforms world space coordinates to image space.
	 * @param point World space point
	 * @param inverseProjection Inverse projection matrix
	 * @param w Image width
	 * @param h Image height
	 * @param x Output image x coordinate
	 * @param y Output image y coordinate
	 * @param clamp Whether to clamp coordinates to image bounds
	 */
	static void toImageCoords(
		const MFloatPoint &point,
		const MFloatMatrix &inverseProjection,
		int w, int h, float &x, float &y, bool clamp = true);

	/**
	 * @brief Extracts a specific channel from an image.
	 * @param image Source image
	 * @param channel Channel to extract
	 * @param result Output channel image
	 */
	static void getImageChannel(const CImg<unsigned char> &image,
								cImgData::Channel channel, CImg<unsigned char> &result);

	/**
	 * @brief Samples image using nearest neighbor interpolation.
	 * @param image Source image
	 * @param uVals U coordinates
	 * @param vVals V coordinates
	 * @param result Output sampled values
	 */
	static void sampleNearest(
		const CImg<unsigned char> &image,
		const MDoubleArray &uVals,
		const MDoubleArray &vVals,
		MFloatArray &result);

	/**
	 * @brief Samples image using linear interpolation.
	 * @param image Source image
	 * @param uVals U coordinates
	 * @param vVals V coordinates
	 * @param result Output sampled values
	 */
	static void sampleLinear(
		const CImg<unsigned char> &image,
		const MDoubleArray &uVals,
		const MDoubleArray &vVals,
		MFloatArray &result);

	/**
	 * @brief Samples image using cubic interpolation.
	 * @param image Source image
	 * @param uVals U coordinates
	 * @param vVals V coordinates
	 * @param result Output sampled values
	 */
	static void sampleCubic(
		const CImg<unsigned char> &image,
		const MDoubleArray &uVals,
		const MDoubleArray &vVals,
		MFloatArray &result);

	/**
	 * @brief Samples image using nearest neighbor interpolation (integer output).
	 * @param image Source image
	 * @param uVals U coordinates
	 * @param vVals V coordinates
	 * @param result Output sampled values
	 */
	static void sampleNearest(
		const CImg<unsigned char> &image,
		const MDoubleArray &uVals,
		const MDoubleArray &vVals,
		MIntArray &result);

	/**
	 * @brief Transforms UV coordinates to image space.
	 * @param u U coordinate
	 * @param v V coordinate
	 * @param w Image width
	 * @param h Image height
	 * @param x Output image x coordinate
	 * @param y Output image y coordinate
	 */
	static void toImageCoords(float u, float v, int w, int h, float &x, float &y);

	/**
	 * @brief Creates UV to image space transformation matrix.
	 * @param w Image width
	 * @param h Image height
	 * @param result Output transformation matrix
	 */
	static void transformUvToImage(float w, float h, MFloatMatrix &result);

	/**
	 * @brief Inverts an image in place.
	 * @param image Image to invert
	 */
	static void invert(CImg<unsigned char> &image);

	/**
	 * @brief Creates an inverted copy of an image.
	 * @param image Source image
	 * @param result Output inverted image
	 */
	static void get_invert(const CImg<unsigned char> &image, CImg<unsigned char> &result);

	/**
	 * @brief Prints statistics about an unsigned char image.
	 * @param name Name to identify the image in output
	 * @param img Image to analyze
	 */
	static void img_stats(const MString&name, const CImg<unsigned char>*img);

	/**
	 * @brief Prints statistics about a float image.
	 * @param name Name to identify the image in output
	 * @param img Image to analyze
	 */
	static void img_stats(const MString&name, const CImg<float>*img);

	/**
	 * @brief Applies Floyd-Steinberg dithering to an image.
	 * @param image Image to dither
	 */
	static void floydSteinberg(CImg<unsigned char> & image);

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
