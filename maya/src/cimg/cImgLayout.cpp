
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <maya/MFnPluginData.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgLayout.h"
#include "cImgUtils.h"
#include "jMayaIds.h"

MTypeId cImgLayout::id(k_cImgLayout);

MObject cImgLayout::aResolution;
MObject cImgLayout::aMaskPadding;
MObject cImgLayout::aInvertMask;
MObject cImgLayout::aDarkenByMask;
MObject cImgLayout::aIsolate;

MObject cImgLayout::aInputImage;
MObject cImgLayout::aOffsetX;
MObject cImgLayout::aOffsetY;
MObject cImgLayout::aOffset;
MObject cImgLayout::aActive;

MObject cImgLayout::aCropCornerX;
MObject cImgLayout::aCropCornerY;
MObject cImgLayout::aCropCorner;

MObject cImgLayout::aCropResolutionX;
MObject cImgLayout::aCropResolutionY;
MObject cImgLayout::aCropResolution;

MObject cImgLayout::aInput;

MObject cImgLayout::aOutput;
MObject cImgLayout::aOutputMask;

cImgLayout::cImgLayout() {}

cImgLayout::~cImgLayout() {}

void *cImgLayout::creator()
{
    return new cImgLayout();
}

MStatus cImgLayout::initialize()
{
    MStatus st;
    MFnTypedAttribute tAttr;
    MFnNumericAttribute nAttr;
    MFnCompoundAttribute cAttr;

    aResolution = nAttr.create("resolution", "rrg", MFnNumericData::k2Int);
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    st = addAttribute(aResolution);

    aMaskPadding = nAttr.create("maskPadding", "mpd", MFnNumericData::kInt);
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    st = addAttribute(aMaskPadding);

    aInvertMask = nAttr.create("invertMask", "imsk", MFnNumericData::kBoolean);
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    st = addAttribute(aInvertMask);

    aDarkenByMask = nAttr.create("darkenByMask", "dmsk", MFnNumericData::kBoolean);
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    st = addAttribute(aDarkenByMask);

    aIsolate = nAttr.create("isolate", "iso", MFnNumericData::kInt);
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    nAttr.setDefault(-1);
    st = addAttribute(aIsolate);

    aInputImage = tAttr.create("inputImage", "ini", cImgData::id);
    tAttr.setReadable(false);
    tAttr.setKeyable(true);

    aOffsetX = nAttr.create("offsetX", "ofx", MFnNumericData::kInt, 0);
    aOffsetY = nAttr.create("offsetY", "ofy", MFnNumericData::kInt, 0);
    aOffset = nAttr.create("offset", "of", aOffsetX, aOffsetY);
    nAttr.setStorable(true);
    nAttr.setWritable(true);
    nAttr.setKeyable(true);

    aCropCornerX = nAttr.create("cropCornerX", "ccx", MFnNumericData::kInt, 0);
    aCropCornerY = nAttr.create("cropCornerY", "ccy", MFnNumericData::kInt, 0);
    aCropCorner = nAttr.create("cropCorner", "cc", aCropCornerX, aCropCornerY);
    nAttr.setStorable(true);
    nAttr.setWritable(true);
    nAttr.setKeyable(true);

    aCropResolutionX = nAttr.create("cropResolutionX", "crx", MFnNumericData::kInt, 100);
    aCropResolutionY = nAttr.create("cropResolutionY", "cry", MFnNumericData::kInt, 100);
    aCropResolution = nAttr.create("cropResolution", "cr", aCropResolutionX, aCropResolutionY);
    nAttr.setStorable(true);
    nAttr.setWritable(true);
    nAttr.setKeyable(true);

    aActive = nAttr.create("active", "ac", MFnNumericData::kBoolean);
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    nAttr.setWritable(true);
    nAttr.setDefault(true);

    aInput = cAttr.create("input", "in");
    cAttr.setKeyable(true);
    cAttr.addChild(aInputImage);
    cAttr.addChild(aOffset);
    cAttr.addChild(aCropCorner);
    cAttr.addChild(aCropResolution);
    cAttr.addChild(aActive);
    cAttr.setArray(true);
    cAttr.setIndexMatters(false);
    cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    addAttribute(aInput);

    aOutput = tAttr.create("output", "out", cImgData::id);
    tAttr.setReadable(true);
    tAttr.setStorable(false);
    addAttribute(aOutput);

    aOutputMask = tAttr.create("outputMask", "msk", cImgData::id);
    tAttr.setReadable(true);
    tAttr.setStorable(false);
    addAttribute(aOutputMask);

    // st = attributeAffecte(aInputImage, aOutput);
    // mser;
    // st = attributeAffecte(aOffset, aOutput);
    // mser;
    // st = attributeAffecte(aTopLeft, aOutput);
    // mser;
    // st = attributeAffecte(aWidthHeight, aOutput);
    // mser;

    st = attributeAffects(aDarkenByMask, aOutput);
    st = attributeAffects(aIsolate, aOutput);
    st = attributeAffects(aMaskPadding, aOutput);
    st = attributeAffects(aInvertMask, aOutput);
    st = attributeAffects(aResolution, aOutput);
    st = attributeAffects(aInputImage, aOutput);
    st = attributeAffects(aActive, aOutput);
    st = attributeAffects(aOffsetX, aOutput);
    st = attributeAffects(aOffsetY, aOutput);
    st = attributeAffects(aOffset, aOutput);
    st = attributeAffects(aCropCornerX, aOutput);
    st = attributeAffects(aCropCornerY, aOutput);
    st = attributeAffects(aCropCorner, aOutput);
    st = attributeAffects(aCropResolutionX, aOutput);
    st = attributeAffects(aCropResolutionY, aOutput);
    st = attributeAffects(aCropResolution, aOutput);
    st = attributeAffects(aInput, aOutput);

    st = attributeAffects(aDarkenByMask, aOutputMask);
    st = attributeAffects(aIsolate, aOutputMask);
    st = attributeAffects(aMaskPadding, aOutputMask);
    st = attributeAffects(aInvertMask, aOutputMask);
    st = attributeAffects(aResolution, aOutputMask);
    st = attributeAffects(aInputImage, aOutputMask);
    st = attributeAffects(aOffsetX, aOutputMask);
    st = attributeAffects(aOffsetY, aOutputMask);
    st = attributeAffects(aOffset, aOutputMask);
    st = attributeAffects(aActive, aOutputMask);
    st = attributeAffects(aCropCornerX, aOutputMask);
    st = attributeAffects(aCropCornerY, aOutputMask);
    st = attributeAffects(aCropCorner, aOutputMask);
    st = attributeAffects(aCropResolutionX, aOutputMask);
    st = attributeAffects(aCropResolutionY, aOutputMask);
    st = attributeAffects(aCropResolution, aOutputMask);
    st = attributeAffects(aInput, aOutputMask);
    mser;
    return MS::kSuccess;
}

MStatus cImgLayout::compute(const MPlug &plug, MDataBlock &data)
{

    if (!((plug == aOutput) || (plug == aOutputMask)))
    {
        return (MS::kUnknownParameter);
    }
    MStatus st;

    const int2 &resolution = data.inputValue(aResolution).asInt2();
    bool invertMask = data.inputValue(aInvertMask).asBool();
    bool darkenByMask = data.inputValue(aDarkenByMask).asBool();
    int maskPadding = data.inputValue(aMaskPadding).asInt();

    MTypeId kdid(cImgData::id);

    MDataHandle hOutput = data.outputValue(aOutput);
    MFnPluginData fnOut;
    MObject dOut = fnOut.create(kdid, &st);
    mser;
    cImgData *newData = (cImgData *)fnOut.data(&st);
    mser;

    MDataHandle hOutputMask = data.outputValue(aOutputMask);
    MFnPluginData fnOutMask;
    MObject dOutMask = fnOutMask.create(kdid, &st);
    mser;
    cImgData *newDataMask = (cImgData *)fnOutMask.data(&st);
    mser;

    if (!data.inputValue(state).asShort())
    {
        CImg<unsigned char> *outImage = newData->fImg;
        CImg<unsigned char> *outImageMask = newDataMask->fImg;

        MArrayDataHandle hInputs = data.inputValue(aInput, &st);
        msert;
        unsigned nInputs = hInputs.elementCount();

        int isolate = data.inputValue(aIsolate).asInt();
        bool doIsolate = false;
        if (isolate > -1)
        {
            for (unsigned i = 0; i < nInputs; i++, hInputs.next())
            {
                int index = hInputs.elementIndex();
                if (index == isolate)
                {
                    doIsolate = true;

                    MDataHandle hCompound = hInputs.inputValue(&st);
                    if (!st.error())
                    {
                        MDataHandle hImageData = hCompound.child(cImgLayout::aInputImage);
                        MObject dImageData = hImageData.data();
                        MFnPluginData fnImageData(dImageData, &st);
                        if (!st.error())
                        {
                            cImgData *imageData = (cImgData *)fnImageData.data();
                            CImg<unsigned char> *inImage = imageData->fImg;
                            outImage->assign(*(imageData->fImg)).resize(resolution[0], resolution[1]);
                            outImageMask->assign(resolution[0], resolution[1], 1, 1, 255);
                        }
                    }
                    break;
                }
            }
        }

        if (!doIsolate)
        {
            hInputs.jumpToArrayElement(0);
            bool started = false;
            for (unsigned i = 0; i < nInputs; i++, hInputs.next())
            {
                // int index = hInputs.elementIndex(&st);
                MDataHandle hCompound = hInputs.inputValue(&st);
                if (st.error())
                {
                    continue;
                }

                bool active = hCompound.child(aActive).asBool();
                if (!active)
                {
                    continue;
                }

                MDataHandle hImageData = hCompound.child(cImgLayout::aInputImage);
                MObject dImageData = hImageData.data();
                MFnPluginData fnImageData(dImageData, &st);
                if (st.error())
                {
                    continue;
                }
                cImgData *imageData = (cImgData *)fnImageData.data();
                CImg<unsigned char> *inImage = imageData->fImg;
                MDataHandle hOffset = hCompound.child(aOffset);
                int offsetX = hOffset.child(aOffsetX).asInt();
                int offsetY = hOffset.child(aOffsetY).asInt();

                MDataHandle hCropCorner = hCompound.child(aCropCorner);
                int cropCornerX = hCropCorner.child(aCropCornerX).asInt();
                int cropCornerY = hCropCorner.child(aCropCornerY).asInt();

                MDataHandle hCropResolution = hCompound.child(aCropResolution);
                int cropResX = hCropResolution.child(aCropResolutionX).asInt();
                int cropResY = hCropResolution.child(aCropResolutionY).asInt();

                int x1 = cropCornerX + cropResX;
                int y1 = cropCornerY + cropResY;

                if (!started)
                {
                    started = true;
                    int spectrum = inImage->spectrum();
                    outImage->assign(resolution[0], resolution[1], 1, spectrum, 255);
                    outImageMask->assign(resolution[0], resolution[1], 1, 1, 0);
                }

                CImg<unsigned char> sprite = inImage->get_crop(cropCornerX, cropCornerY, x1, y1);

                int maskCornerX = +maskPadding;
                int maskCornerY = cropCornerY + maskPadding;
                int maskCorner1X = x1 - maskPadding;
                int maskCorner1Y = y1 - maskPadding;

                outImage->draw_image(offsetX, offsetY, 0, 0, sprite);

                const unsigned char uc_col[1] = {255};

                outImageMask->draw_rectangle(
                    (offsetX + maskPadding),
                    (offsetY + maskPadding), 0,
                    (offsetX + cropResX - (2 * maskPadding)),
                    (offsetY + cropResY - (2 * maskPadding)),
                    1, uc_col);
            }

            if (invertMask)
            {
                ((*outImageMask) -= 255) *= -1;
            }

            if (darkenByMask)
            {
                outImage->min(*outImageMask);
            }
        }
    }

    hOutputMask.set(newDataMask);
    hOutput.set(newData);
    hOutput.setClean();
    hOutputMask.setClean();
    return MS::kSuccess;
}
