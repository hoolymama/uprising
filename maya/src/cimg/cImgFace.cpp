#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgData.h"

#include "jsonUtils.h"

#include "cImgFace.h"
#include "jMayaIds.h"
#include "ConvexHull.h"

MTypeId cImgFace::id(k_cImgFace);

MObject cImgFace::aAssetDirectory;
MObject cImgFace::aDilate;
MObject cImgFace::aBlur;
MObject cImgFace::aThreshold;
MObject cImgFace::aIncludeEyebrows;

MObject cImgFace::aResolutionLow;
MObject cImgFace::aResolutionHigh;
MObject cImgFace::aOutputLow;
MObject cImgFace::aOutputHigh;
MObject cImgFace::aOutputFeatures;
MObject cImgFace::aOutputAlpha;

cImgFace::cImgFace() {}

cImgFace::~cImgFace() {}

void *cImgFace::creator()
{
	return new cImgFace();
}

MStatus cImgFace::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;

	aAssetDirectory = tAttr.create("assetDirectory", "adr", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setUsedAsFilename(true);
	addAttribute(aAssetDirectory);

	// feature detection attribs
	aDilate = nAttr.create("dilate", "dil", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0);
	st = addAttribute(aDilate);

	aIncludeEyebrows = nAttr.create("includeEyebrows", "ieb", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aIncludeEyebrows);

	aBlur = nAttr.create("blur", "blr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f);
	st = addAttribute(aBlur);

	aThreshold = nAttr.create("threshold", "thr", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0);
	st = addAttribute(aThreshold);

	aResolutionLow = nAttr.create("resolutionLow", "resl", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aResolutionLow);

	aResolutionHigh = nAttr.create("resolutionHigh", "resh", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aResolutionHigh);

	aOutputLow = tAttr.create("outputLow", "outl", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutputLow);

	aOutputHigh = tAttr.create("outputHigh", "outh", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutputHigh);

	aOutputFeatures = tAttr.create("outputFeatures", "oft", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutputFeatures);

	aOutputAlpha = tAttr.create("outputAlpha", "oal", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutputAlpha);

	st = attributeAffects(aAssetDirectory, aOutputLow);
	st = attributeAffects(aResolutionLow, aOutputLow);

	st = attributeAffects(aAssetDirectory, aOutputHigh);
	st = attributeAffects(aResolutionHigh, aOutputHigh);

	st = attributeAffects(aDilate, aOutputFeatures);
	st = attributeAffects(aBlur, aOutputFeatures);
	st = attributeAffects(aThreshold, aOutputFeatures);
	st = attributeAffects(aIncludeEyebrows, aOutputFeatures);

	st = attributeAffects(aAssetDirectory, aOutputFeatures);
	st = attributeAffects(aResolutionHigh, aOutputFeatures);

	st = attributeAffects(aAssetDirectory, aOutputAlpha);
	st = attributeAffects(aResolutionLow, aOutputAlpha);

	return MS::kSuccess;
}

MStatus cImgFace::compute(const MPlug &plug, MDataBlock &data)
{

	if (!(
			(plug == aOutputLow) ||
			(plug == aOutputHigh) ||
			(plug == aOutputFeatures) ||
			(plug == aOutputAlpha)

				))
	{
		return (MS::kUnknownParameter);
	}

	MStatus st = MS::kSuccess;

	MTypeId kdid(cImgData::id);
	MFnPluginData fn0, fn1, fn2, fn3;

	// create some instances of image data
	fn0.create(kdid, &st);
	cImgData *imageDataLow = (cImgData *)fn0.data();
	CImg<unsigned char> *imageLow = imageDataLow->fImg;

	fn1.create(kdid, &st);
	cImgData *imageDataHigh = (cImgData *)fn1.data();
	CImg<unsigned char> *imageHigh = imageDataHigh->fImg;

	fn2.create(kdid, &st);
	cImgData *imageDataFeatures = (cImgData *)fn2.data();
	CImg<unsigned char> *imageFeatures = imageDataFeatures->fImg;

	fn3.create(kdid, &st);
	cImgData *imageDataAlpha = (cImgData *)fn3.data();
	CImg<unsigned char> *imageAlpha = imageDataAlpha->fImg;
	//

	MString rootPath = data.inputValue(aAssetDirectory).asString() + "/";

	// float overlayBlend = data.inputValue(aOverlayFeaturesBlend).asFloat();

	MString jsonFilename = rootPath + "manifest.json";
	std::ifstream strm(jsonFilename.asChar());
	std::stringstream buffer;
	buffer << strm.rdbuf();

	if (buffer)
	{
		// JPMDBG;
		auto json_data = json::parse(buffer.str());

		// JPMDBG;
		MString alphaFilename = rootPath + "alpha.png";
		MString fullFilename = rootPath + "main_styled.png";
		MString featuresFilename = rootPath + "features.png";

		const int2 &resolutionLow = data.inputValue(aResolutionLow).asInt2();
		const int2 &resolutionHigh = data.inputValue(aResolutionHigh).asInt2();
		imageHigh->assign(fullFilename.asChar()).resize(resolutionHigh[0], resolutionHigh[1], -100, -100, cImgData::kLinear);
		imageLow->assign(*imageHigh).resize(resolutionLow[0], resolutionLow[1], -100, -100, cImgData::kLinear);

		// FEATURES FILE
		std::ifstream featuresFile(featuresFilename.asChar());
		if (!featuresFile.good())
		{
			st = createFeaturesImage(json_data, data, imageFeatures);
			msert;
		}
		else
		{
			imageFeatures->assign(featuresFilename.asChar()).resize(resolutionHigh[0], resolutionHigh[1], -100, -100, cImgData::kLinear);
			((*imageFeatures) -= 255) *= -1;
			st = createFeaturesImage(data, imageFeatures);
		}

		imageAlpha->assign(alphaFilename.asChar()).resize(resolutionLow[0], resolutionLow[1], -100, -100, cImgData::kLinear);
	}

	data.outputValue(aOutputLow).set(imageDataLow);
	data.outputValue(aOutputHigh).set(imageDataHigh);
	data.outputValue(aOutputFeatures).set(imageDataFeatures);
	data.outputValue(aOutputAlpha).set(imageDataAlpha);

	data.setClean(aOutputLow);
	data.setClean(aOutputHigh);
	data.setClean(aOutputFeatures);
	data.setClean(aOutputAlpha);

	return MS::kSuccess;
}

MStatus cImgFace::createFeaturesImage(MDataBlock &data, CImg<unsigned char> *image)
{

	MStatus st;

	int dilate = data.inputValue(aDilate).asInt();
	float blur = data.inputValue(aBlur).asFloat();
	int thresh = data.inputValue(aThreshold).asInt();

	if (dilate < 0)
	{
		image->erode(-dilate);
	}
	else if (dilate > 0)
	{
		image->dilate(dilate);
	}

	if (blur > 0.0f)
	{
		image->blur(blur, true, true);
	}

	if (thresh > -1)
	{
		image->threshold(thresh).normalize(0, 255);
	}

	return MS::kSuccess;
}

MStatus cImgFace::createFeaturesImage(const json &jsonData, MDataBlock &data, CImg<unsigned char> *image)
{
	// JPMDBG;
	MStatus st;
	json resolution;
	st = jsonUtils::getValue(jsonData, "resolution", resolution);
	msert;
	// JPMDBG;
	int w, h;
	st = jsonUtils::getValue(resolution, "x", w);
	msert;
	st = jsonUtils::getValue(resolution, "y", h);
	msert;
	// JPMDBG;

	int dilate = data.inputValue(aDilate).asInt();
	float blur = data.inputValue(aBlur).asFloat();
	int thresh = data.inputValue(aThreshold).asInt();

	bool includeEyebrows = data.inputValue(aIncludeEyebrows).asBool();

	image->assign(w, h, 1, 1, 0);

	json faces;
	st = jsonUtils::getValue(jsonData, "faces", faces);
	msert;

	std::string eyebrow = "eyebrow";

	const unsigned char white[] = {255};
	for (json::const_iterator face = faces.begin(); face != faces.end(); ++face)
	{
		json features;
		st = jsonUtils::getValue(*face, "features", features);
		msert;
		for (json::const_iterator feature = features.begin(); feature != features.end(); ++feature)
		{

			std::vector<Point> featurePoints;

			const json &landmarks = feature.value();

			for (json::const_iterator landmark = landmarks.begin(); landmark != landmarks.end(); ++landmark)
			{
				if (includeEyebrows || (std::string((*landmark)["name"]).find(eyebrow) == std::string::npos))
				{
					featurePoints.push_back(Point{int((*landmark)["x"]), int((*landmark)["y"])});
				}
			}

			const std::vector<Point> convexHull = makeConvexHull(featurePoints);

			CImgList<int> cpts;

			for (std::vector<Point>::const_iterator citer = convexHull.begin(); citer != convexHull.end(); ++citer)
			{
				cpts.insert(CImg<>::vector(citer->x, citer->y));
			}
			CImg<> npoints = cpts > 'x';

			image->draw_polygon(npoints, white);
		}
	}

	if (dilate < 0)
	{
		image->erode(-dilate);
	}
	else if (dilate > 0)
	{
		image->dilate(dilate);
	}

	if (blur > 0.0f)
	{
		image->blur(blur, true, true);
	}

	if (thresh > -1)
	{
		image->threshold(thresh).normalize(0, 255);
	}

	return MS::kSuccess;
}
