
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPluginData.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgSmear.h"
#include "cImgUtils.h"
#include "jMayaIds.h"

MTypeId cImgSmear::id(k_cImgSmear);

MObject cImgSmear::aSmearImage;
MObject cImgSmear::aSmearAmount;
MObject cImgSmear::aDilate;
cImgSmear::cImgSmear() {}

cImgSmear::~cImgSmear() {}

void *cImgSmear::creator()
{
    return new cImgSmear();
}

MStatus cImgSmear::initialize()
{
    MStatus st;
    inheritAttributesFrom("cImgProcess");

    MFnTypedAttribute tAttr;
    MFnNumericAttribute nAttr;

    aSmearImage = tAttr.create("smearImage", "sim", cImgData::id);
    tAttr.setReadable(false);
    tAttr.setKeyable(true);
    st = addAttribute(aSmearImage);
    mser;

    aSmearAmount = nAttr.create("amount", "amt", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setDefault(1.0f);
    st = addAttribute(aSmearAmount);

    aDilate = nAttr.create("dilateInput", "dil", MFnNumericData::kInt);
    nAttr.setHidden(false);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setDefault(0);
    st = addAttribute(aDilate);

    st = attributeAffects(aSmearAmount, aOutput);
    st = attributeAffects(aSmearImage, aOutput);
    st = attributeAffects(aDilate, aOutput);

    mser;

    return MS::kSuccess;
}

// MStatus cImgSmear::checkPlug(const MPlug &plug)
// {
// 	if ( plug != aOutput) {
// 		return ( MS::kUnknownParameter );
// 	}
// 	return MS::kSuccess;
// }

MStatus cImgSmear::process(MDataBlock &data, const CImg<unsigned char> &image,
                           CImg<unsigned char> &result)

{
    MStatus st;

    MDataHandle hImageData = data.inputValue(aSmearImage, &st);
    msert;
    MObject dImageData = hImageData.data();
    MFnPluginData fnImageData(dImageData, &st);
    msert;
    cImgData *imageData = (cImgData *)fnImageData.data();
    CImg<unsigned char> *smearImage = imageData->fImg;

    int w = image.width();
    int h = image.height();
    int s = image.spectrum();

    result.assign(w, h, 1, s);

    float dist = data.inputValue(aSmearAmount).asFloat();
    int dilate = data.inputValue(aDilate).asInt();
    CImgList<> grad(smearImage->get_norm().resize(w, h, -100, -100, cImgData::kLinear).get_gradient());

    bool shared = dilate == 0;
    CImg<unsigned char> buffer(image, shared);

    if (dilate < 0)
    {
        buffer.erode(-dilate);
    }
    else if (dilate > 0)
    {
        buffer.dilate(dilate);
    }

    cimg_forXY(image, x, y)
    {
        int sx = (w + x + int(grad[0](x, y) * dist)) % w;
        int sy = (h + y + int(grad[1](x, y) * dist)) % h;
        result(x, y) = buffer(sx, sy);
    }

    return MS::kSuccess;
}
