

/*
 *  gridableField.cpp
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
#include "gridableField.h"
#include "jMayaIds.h"



MObject gridableField::aResolutionX;
MObject gridableField::aResolutionY;
MObject gridableField::aResolutionZ;

MObject gridableField::aOutputGridPoints;
MObject gridableField::aOutputGridVectors;

const double epsilon = 0.000001;
const double deg_to_rad = (3.1415927 / 180);

MTypeId gridableField::id(k_gridableField);

gridableField::gridableField(){};

gridableField::~gridableField(){};

void *gridableField::creator()
{
    return new gridableField;
}
void gridableField::postConstructor()
{
    MPxNode::postConstructor();
}

MStatus gridableField::initialize()
{
    MStatus st;

    MFnTypedAttribute tAttr;
    MFnNumericAttribute nAttr;

    aResolutionX = nAttr.create("resolutionX", "rsx", MFnNumericData::kInt);
    nAttr.setDefault(2);
    nAttr.setKeyable(true);
    st = addAttribute(aResolutionX);
    mser;

    aResolutionY = nAttr.create("resolutionY", "rsy", MFnNumericData::kInt);
    nAttr.setDefault(2);
    nAttr.setKeyable(true);
    st = addAttribute(aResolutionY);
    mser;

    aResolutionZ = nAttr.create("resolutionZ", "rsz", MFnNumericData::kInt);
    nAttr.setDefault(2);
    nAttr.setKeyable(true);
    st = addAttribute(aResolutionZ);
    mser;

    aOutputGridPoints = tAttr.create("outputGridPoints", "ogp", MFnData::kVectorArray);
    tAttr.setStorable(false);
    tAttr.setReadable(true);
    st = addAttribute(aOutputGridPoints);

    aOutputGridVectors = tAttr.create("outputGridVectors", "ogv", MFnData::kVectorArray);
    tAttr.setStorable(false);
    tAttr.setReadable(true);
    st = addAttribute(aOutputGridVectors);

    attributeAffects(aResolutionX, aOutputGridPoints);
    attributeAffects(aResolutionY, aOutputGridPoints);
    attributeAffects(aResolutionZ, aOutputGridPoints);

    return (MS::kSuccess);
}

MStatus gridableField::compute(const MPlug &plug, MDataBlock &data)
{
    if (plug == mOutputForce)
    {
        return computeOutputForce(plug, data);
    }
    else if (plug == aOutputGridPoints || plug == aOutputGridVectors)
    {
        return computeOutputGrid(plug, data);
        ;
    }
    return MS::kUnknownParameter;
}

MStatus gridableField::computeOutputForce(const MPlug &plug, MDataBlock &data)
{
    MStatus st;
    int multiIndex = plug.logicalIndex(&st);
    mser;
    if (st.error())
        return (MS::kFailure);

    MArrayDataHandle hInputArray = data.outputArrayValue(mInputData, &st);
    mser;
    st = hInputArray.jumpToElement(multiIndex);
    msert;
    MDataHandle hCompound = hInputArray.inputValue(&st);
    mser;

    MDataHandle hPosition = hCompound.child(mInputPositions);
    MObject dPosition = hPosition.data();
    MFnVectorArrayData fnPosition(dPosition);
    MVectorArray points = fnPosition.array(&st);
    mser;

    MDataHandle hVelocity = hCompound.child(mInputVelocities);
    MObject dVelocity = hVelocity.data();
    MFnVectorArrayData fnVelocity(dVelocity);
    MVectorArray velocities = fnVelocity.array(&st);
    mser;

    MDataHandle hMass = hCompound.child(mInputMass);
    MObject dMass = hMass.data();
    MFnDoubleArrayData fnMass(dMass);
    MDoubleArray masses = fnMass.array(&st);
    mser;

    double dt = 0.0;
    MDataHandle dtH = hCompound.child(mDeltaTime);
    MTime dT = dtH.asTime();
    dt = dT.as(MTime::kSeconds);

    MVectorArray forceArray;

    calcForce(data, points, velocities, masses, forceArray);

    MArrayDataHandle hOutArray = data.outputArrayValue(mOutputForce, &st);
    mser;
    MArrayDataBuilder bOutArray = hOutArray.builder(&st);
    mser;
    MDataHandle hOut = bOutArray.addElement(multiIndex, &st);
    mser;

    MFnVectorArrayData fnOutputForce;
    MObject dOutputForce = fnOutputForce.create(forceArray, &st);
    mser;
    hOut.set(dOutputForce);
    data.setClean(plug);

    return (MS::kSuccess);
}

MStatus gridableField::computeOutputGrid(const MPlug &plug, MDataBlock &data)
{
    MStatus st;

    int rx = data.inputValue(aResolutionX).asInt();
    int ry = data.inputValue(aResolutionY).asInt();
    int rz = data.inputValue(aResolutionZ).asInt();

    MVectorArray points;

    int pl = rx * ry * rz;

    float gapz = 2.0 / (rz + 1);
    float gapy = 2.0 / (ry + 1);
    float gapx = 2.0 / (rx + 1);
    int x, y, z;
    float posx, posy, posz;
    MMatrix wm = NodeUtils::fieldWorldMatrix(thisMObject());

    for (posz = -1.0, z = 0; z < rz; z++)
    {
        posz += gapz;
        for (posy = -1.0, y = 0; y < ry; y++)
        {
            posy += gapy;
            for (posx = -1.0, x = 0; x < rx; x++)
            {
                posx += gapx;
                points.append(MVector(MPoint(posx, posy, posz) * wm));
            }
        }
    }
    MVectorArray velocities;
    MDoubleArray masses;
    MVectorArray forceArray;
    calcForce(data, points, velocities, masses, forceArray);
    MVectorArray resultPoints(pl);
    MVectorArray resultVectors(pl);

    for (int i = 0; i < pl; i++)
    {
        resultPoints[i] = points[i];
        resultVectors[i] = forceArray[i];
    }

    MDataHandle hOutGridPoints = data.outputValue(aOutputGridPoints);
    MFnVectorArrayData fnOutGridPoints;
    MObject dOutGridPoints = fnOutGridPoints.create(resultPoints);
    hOutGridPoints.set(dOutGridPoints);
    st = data.setClean(aOutputGridPoints);
    mser;

    MDataHandle hOutGridVectors = data.outputValue(aOutputGridVectors);
    MFnVectorArrayData fnOutGridVectors;
    MObject dOutGridVectors = fnOutGridVectors.create(resultVectors);
    hOutGridVectors.set(dOutGridVectors);
    st = data.setClean(aOutputGridVectors);
    mser;

    return (MS::kSuccess);
}

    MStatus gridableField::calcForce(
    MDataBlock &data,
    const MVectorArray &points,
    const MVectorArray &velocities,
    const MDoubleArray &masses,
    MVectorArray &outputForce)
    {

    return MS::kUnknownParameter;
    }



MStatus gridableField::getForceAtPoint(
    const MVectorArray &points,
    const MVectorArray &velocities,
    const MDoubleArray &masses,
    MVectorArray &forceArray,
    double deltaTime)
{
    MStatus ReturnStatus;

    MDataBlock data = forceCache();

    calcForce(
        data,
        points,
        velocities,
        masses,
        forceArray);

    return MS::kSuccess;
}
