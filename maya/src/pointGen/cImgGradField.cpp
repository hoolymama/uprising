

/*
 *  cImgGradField.cpp
 *  jtools
 *
 *  Created by Julian Mann on 31/12/2006.
 *  Copyright 2006 hooly|mama. All rights reserved.
 *
 */

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MTime.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MVector.h>

#include <maya/MMatrix.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MRampAttribute.h>

#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnNurbsCurveData.h>
#include <maya/MFnPluginData.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MFnUnitAttribute.h>
#include "cImgUtils.h"
#include "attrUtils.h"
#include "nodeUtils.h"

#include "cImgFloatData.h"
#include "cImgGradField.h"
#include "jMayaIds.h"

MObject cImgGradField::aInputGradientImage;
// MObject cImgGradField::aRotation;

MObject cImgGradField::aMagnitudeRemapRange;

MObject cImgGradField::aRadialMag;
MObject cImgGradField::aTangentMag;

MObject cImgGradField::aRadialMagnitudeRemapRamp;
MObject cImgGradField::aTangentMagnitudeRemapRamp;
// MObject cImgGradField::aMagnitudeRemapRange;
// MObject cImgGradField::aRadialMag;
// MObject cImgGradField::aTangentMag;

// MObject cImgGradField::aResolutionX;
// MObject cImgGradField::aResolutionY;
// MObject cImgGradField::aResolutionZ;

// MObject cImgGradField::aOutputGridPoints;
// MObject cImgGradField::aOutputGridVectors;

const double epsilon = 0.000001;
const double deg_to_rad = (3.1415927 / 180);

MTypeId cImgGradField::id(k_cImgGradField);

cImgGradField::cImgGradField(){};

cImgGradField::~cImgGradField(){};

void *cImgGradField::creator()
{
    return new cImgGradField;
}
void cImgGradField::postConstructor()
{
    MPxNode::postConstructor();
}

MStatus cImgGradField::initialize()
{
    MStatus st;


	inheritAttributesFrom("gridableField");



    MFnTypedAttribute tAttr;
    MFnNumericAttribute nAttr;
 
    aInputGradientImage = tAttr.create("inputGradientImage", "igi", cImgFloatData::id);
    mser;
    tAttr.setStorable(false);
    addAttribute(aInputGradientImage);


    aRadialMag = nAttr.create("radialMagnitude", "urd", MFnNumericData::kDouble);
    nAttr.setDefault(1.0);
    st = addAttribute(aRadialMag);
    mser;

    aTangentMag = nAttr.create("tangentMagnitude", "utn", MFnNumericData::kDouble);
    nAttr.setDefault(1.0);
    st = addAttribute(aTangentMag);
    mser;

    aRadialMagnitudeRemapRamp = MRampAttribute::createCurveRamp("radialMagnitudeRemap", "rmgr");
    st = addAttribute(aRadialMagnitudeRemapRamp);
    mser;

    aTangentMagnitudeRemapRamp = MRampAttribute::createCurveRamp("tangentMagnitudeRemap", "tmgr");
    st = addAttribute(aTangentMagnitudeRemapRamp);
    mser;

    aMagnitudeRemapRange = nAttr.create("magnitudeRemapRange", "mrx", MFnNumericData::kDouble);
    nAttr.setDefault(1.0);
    st = addAttribute(aMagnitudeRemapRange);
    mser;


    attributeAffects(aInputGradientImage, aOutputGridVectors);
    attributeAffects(aRadialMagnitudeRemapRamp, aOutputGridVectors);
    attributeAffects(aTangentMagnitudeRemapRamp, aOutputGridVectors);
    attributeAffects(aTangentMag, aOutputGridVectors);
    attributeAffects(aRadialMag, aOutputGridVectors);
    attributeAffects(aMagnitudeRemapRange, aOutputGridVectors);
    attributeAffects(mMagnitude, aOutputGridVectors);

    return (MS::kSuccess);
}


MStatus cImgGradField::calcForce(
    MDataBlock &data,
    const MVectorArray &points,
    const MVectorArray &velocities,
    const MDoubleArray &masses,
    MVectorArray &outputForce)
{

    MStatus st;

    MObject thisObj = thisMObject();
    unsigned int pl = points.length();
    outputForce.clear();
    outputForce.setLength(pl);
    CImg<float> *pImage = cImgUtils::getFloatImage(data, aInputGradientImage);
    if (!pImage)
    {
        return MS::kUnknownParameter;
    }

    int w = pImage->width();
    int h = pImage->height();
    if (!(w && h))
    {
        return MS::kUnknownParameter;
    }

    MMatrix wm = NodeUtils::fieldWorldMatrix(thisMObject());
    MMatrix iwm = wm.inverse();

    MMatrix norm;
    norm[0][0] = 0.5;
    norm[1][1] = -0.5;
    norm[3][0] = 0.5;
    norm[3][1] = 0.5;

    double magnitudeRemapRange = data.inputValue(aMagnitudeRemapRange).asDouble();
    double tangentMag = data.inputValue(aTangentMag).asDouble();
    double radialMag = data.inputValue(aRadialMag).asDouble();

    MRampAttribute radialMagRemapAttr(thisObj, aRadialMagnitudeRemapRamp);
    MRampAttribute tangentMagRemapAttr(thisObj, aTangentMagnitudeRemapRamp);

    mser;

    for (unsigned i = 0; i < pl; i++)
    {
        // JPMDBG;
        MPoint imageCoord(points[i]);
        imageCoord *= iwm;
        imageCoord *= norm;

        int imageCoordX = int(imageCoord.x * w);
        int imageCoordY = int(imageCoord.y * h);
        if (
            (imageCoordX < 0) ||
            (imageCoordY < 0) ||
            (imageCoordX >= w) ||
            (imageCoordY >= h))
        {
            outputForce.set(MVector::zero, i);
            continue;
        }
        // JPMDBG;
        float x = (*pImage)(imageCoordX, imageCoordY, 0, 0);
        float y = -(*pImage)(imageCoordX, imageCoordY, 0, 1);
        MVector gradient = MVector(x, y, 0.0);

        if (gradient.isEquivalent(MVector::zero, epsilon))
        {
            outputForce.set(MVector::zero, i);
            continue;
        }
        // JPMDBG;
        bool useTangent = fabs(tangentMag) > 0.0;
        bool useRadial = fabs(radialMag) > 0.0;

        MVector tForce, rForce;
        if (useTangent)
        {
            tForce = gradient.rotateBy(MVector::kZaxis, deg_to_rad * 90.0);
        }
        if (useRadial)
        {
            rForce = gradient;
        }

        if (magnitudeRemapRange > epsilon)
        {
            float sampleValue = float(gradient.length() / magnitudeRemapRange);
            sampleValue = fmin(sampleValue, 1.0f);
            float newRadialMagValue, newTangentailMagValue;
            if (useRadial)
            {
                radialMagRemapAttr.getValueAtPosition(sampleValue, newRadialMagValue, &st);
                mser;
                rForce = rForce.normal() * double(newRadialMagValue);
            }
            if (useTangent)
            {
                tangentMagRemapAttr.getValueAtPosition(sampleValue, newTangentailMagValue, &st);
                mser;
                tForce = tForce.normal() * double(newTangentailMagValue);
            }
        }
        MVector result = ((rForce * radialMag) + (tForce * tangentMag)) * wm;
        outputForce.set(result, i);
    }
    return MS::kSuccess;
}

