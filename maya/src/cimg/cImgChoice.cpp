
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <maya/MFnPluginData.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgChoice.h"
#include "cImgUtils.h"
#include "jMayaIds.h"

MTypeId cImgChoice::id(k_cImgChoice);

MObject cImgChoice::aSelector;
MObject cImgChoice::aInput;
MObject cImgChoice::aOutput;

cImgChoice::cImgChoice() {}

cImgChoice::~cImgChoice() {}

void *cImgChoice::creator()
{
    return new cImgChoice();
}

MStatus cImgChoice::initialize()
{
    MStatus st;
    MFnTypedAttribute tAttr;
    MFnNumericAttribute nAttr;
    MFnCompoundAttribute cAttr;

    aSelector = nAttr.create("selector", "sel", MFnNumericData::kInt);
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    nAttr.setDefault(-1);
    st = addAttribute(aSelector);

    aInput = tAttr.create("input", "in", cImgData::id);
    tAttr.setReadable(false);
    tAttr.setKeyable(true);
    tAttr.setArray(true);
    tAttr.setIndexMatters(false);
    tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    addAttribute(aInput);

    aOutput = tAttr.create("output", "out", cImgData::id);
    tAttr.setReadable(true);
    tAttr.setStorable(false);
    addAttribute(aOutput);

    attributeAffects(aSelector, aOutput);
    attributeAffects(aInput, aOutput);
    return MS::kSuccess;
}

MStatus cImgChoice::compute(const MPlug &plug, MDataBlock &data)
{

    if (plug != aOutput)
    {
        return (MS::kUnknownParameter);
    }
    MStatus st;

    MTypeId kdid(cImgData::id);

    MDataHandle hOutput = data.outputValue(aOutput);
    MFnPluginData fnOut;
    MObject dOut = fnOut.create(kdid, &st);
    mser;
    cImgData *newData = (cImgData *)fnOut.data(&st);
    mser;

    if (!data.inputValue(state).asShort())
    {
        CImg<unsigned char> *outImage = newData->fImg;

        MArrayDataHandle hImages = data.inputValue(aInput, &st);
        msert;

        int isolate = data.inputValue(aSelector).asInt();

        st = hImages.jumpToElement(isolate);
        msert;

        MDataHandle hImage = hImages.inputValue(&st);
        msert;

        MObject dImageData = hImage.data();
        MFnPluginData fnImageData(dImageData, &st);
        msert;

        cImgData *imageData = (cImgData *)fnImageData.data();
        CImg<unsigned char> *inImage = imageData->fImg;
        outImage->assign(*(imageData->fImg));
    }

    hOutput.set(newData);
    hOutput.setClean();
    return MS::kSuccess;
}
