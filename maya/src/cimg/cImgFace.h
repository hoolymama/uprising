
#ifndef _cImgFace
#define _cImgFace

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgData.h"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

class cImgFace : public MPxNode
{
public:
	cImgFace();

	virtual ~cImgFace();

	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	static void *creator();

	static MStatus initialize();
	static MObject aAssetDirectory;

	static MObject aDilate;
	static MObject aBlur;
	static MObject aThreshold;
	static MObject aIncludeEyebrows;

	static MObject aResolutionLow;
	static MObject aResolutionHigh;

	static MObject aOutputLow;
	static MObject aOutputHigh;
	static MObject aOutputFeatures;
	static MObject aOutputAlpha;

	static MTypeId id;

private:
	MStatus createFeaturesImage(const json &jsonData, MDataBlock &data, CImg<unsigned char> *image);
	MStatus createFeaturesImage(MDataBlock &data, CImg<unsigned char> *image);

	MStatus createMainImage(
		const json &jsonData, const MString &rootPath, MDataBlock &data, CImg<unsigned char> *image);
};

#endif
