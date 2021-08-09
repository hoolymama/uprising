

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

#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnNurbsCurveData.h>
#include <maya/MFnPluginData.h>
#include <maya/MArrayDataBuilder.h>

#include "cImgUtils.h"
#include "attrUtils.h"
#include  "cImgFloatData.h"
#include "cImgGradField.h"
#include "jMayaIds.h"


 
MObject cImgGradField::aInputGradientImage;
MObject cImgGradField::aProjectionMatrix;
// MObject cImgGradField::aInMatrixInverse;
	

const double epsilon = 0.00000001;
MTypeId cImgGradField::id( k_cImgGradField );

cImgGradField::cImgGradField(){ 
};

cImgGradField::~cImgGradField() { 
};



void *cImgGradField::creator()
{
    return new cImgGradField;
}

MStatus cImgGradField::initialize()
{
    MStatus st;

    MFnTypedAttribute tAttr;
    MFnNumericAttribute nAttr;
    MFnEnumAttribute eAttr;
	MFnMatrixAttribute mAttr;

    aInputGradientImage = tAttr.create("inputGradientImage", "igi",  cImgFloatData::id) ; mser;
    tAttr.setStorable(false);
    addAttribute(aInputGradientImage);


	aProjectionMatrix = mAttr.create( "projectionMatrix", "pmat",
	                                  MFnMatrixAttribute::kDouble );
	mAttr.setStorable( false );
	mAttr.setHidden( false );
	nAttr.setKeyable(true);
	// mAttr.setDefault(identity);
	addAttribute(aProjectionMatrix);


    return( MS::kSuccess );
}

MStatus cImgGradField::compute( const MPlug& plug, MDataBlock& data)
{
    MStatus st;

    if(!(plug == mOutputForce))return( MS::kUnknownParameter ); 

    int multiIndex = plug.logicalIndex( &st ); mser;
    if (st.error()) return( MS::kFailure);

    MArrayDataHandle hInputArray = data.outputArrayValue( mInputData, &st );mser;
    st = hInputArray.jumpToElement( multiIndex );msert;
    MDataHandle hCompound = hInputArray.inputValue( &st );mser;

    MDataHandle hPosition = hCompound.child( mInputPositions );
    MObject dPosition = hPosition.data();
    MFnVectorArrayData fnPosition( dPosition );
    MVectorArray points = fnPosition.array( &st );mser;

    MDataHandle hVelocity = hCompound.child( mInputVelocities );
    MObject dVelocity = hVelocity.data();
    MFnVectorArrayData fnVelocity( dVelocity );
    MVectorArray velocities = fnVelocity.array( &st );mser;

    MDataHandle hMass = hCompound.child( mInputMass );
    MObject dMass = hMass.data();
    MFnDoubleArrayData fnMass( dMass );
    MDoubleArray masses = fnMass.array( &st);mser;

    double dt= 0.0;
    MDataHandle dtH = hCompound.child( mDeltaTime );  
    MTime dT = dtH.asTime();
    dt = dT.as( MTime::kSeconds );

    MVectorArray forceArray;
 
    calcForce( data,  points,   velocities, masses,  forceArray );

    MArrayDataHandle hOutArray = data.outputArrayValue( mOutputForce, &st);mser;
    MArrayDataBuilder bOutArray = hOutArray.builder( &st );mser;
    MDataHandle hOut = bOutArray.addElement(multiIndex, &st);mser;

    MFnVectorArrayData fnOutputForce;
    MObject dOutputForce = fnOutputForce.create( forceArray, &st );mser;
    hOut.set( dOutputForce );
    data.setClean( plug );
 

    return( MS::kSuccess );
}


MStatus cImgGradField::calcForce(
    MDataBlock &data,       
    const MVectorArray &points,   
    const MVectorArray &velocities, 
    const MDoubleArray &masses,   
    MVectorArray &outputForce   
    )
{

    MStatus st;

    unsigned int pl = points.length() ;
    outputForce= MVectorArray(pl);


    // cerr << "In CalcForce" << endl;

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

    

	MDataHandle mh = data.inputValue(aProjectionMatrix, &st);
	mser;
	MMatrix wm = mh.asMatrix();

    MMatrix norm;
    norm[0][0] = 0.5;
    norm[1][1] = -0.5;
    norm[3][0] = 0.5;
    norm[3][1] = 0.5;
    

	MMatrix wmi = wm.inverse();

    // cerr << "wm" << wm << endl;
    // cerr << "wmi" << wmi << endl;



    double magnitude = data.inputValue(mMagnitude).asDouble();

    // cerr << "magnitude" << magnitude << endl;


    // MVector  resultVec;
 
    for (unsigned i = 0; i < pl; i ++ ) { 

        // resultVec = MVector::zero;


        MPoint imageCoord(points[i]);
        // imageCoord *= wmi;
        imageCoord *= norm;
        // cerr << "imageCoord["<<i<<"]" << imageCoord << endl;

        // cerr << "imageCoord["<<i<<"] transformed" << imageCoord << endl;
        int imageCoordX = int(imageCoord.x*w);
        int imageCoordY = int(imageCoord.y*h);
        // cerr <<  "imageCoordX" << imageCoordX<< endl;
        // cerr <<  "imageCoordY" << imageCoordY<< endl;

        if (
            (imageCoordX <0) ||
            (imageCoordY <0) ||
            (imageCoordX >= w) ||
            (imageCoordY >= h) 
            ) {
                continue;
            }

 
        float x = (*pImage)(imageCoordX, imageCoordY, 0, 0);
        float y = -(*pImage)(imageCoordX, imageCoordY, 0, 1);
        MVector force = (MVector(x,y,0.0)  * magnitude) * wm;
        outputForce.set(force, i); 

    }


    return MS::kSuccess;
}



MStatus cImgGradField::getForceAtPoint(
    const MVectorArray& points,
    const MVectorArray& velocities,
    const MDoubleArray& masses,
    MVectorArray& forceArray,
    double  deltaTime)
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
