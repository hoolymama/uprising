

#ifndef _cImgGradField_H
#define _cImgGradField_H
/*
 *  cImgGradField.h
 *  Created by Julian Mann on Sun Jul 18 2021.
 *  Copyright 2006 hooly|mama. All rights reserved.
 */

#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MPxFieldNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
#include "gridableField.h"

class cImgGradField : public gridableField
{
public:
	cImgGradField();
	virtual ~cImgGradField();

	static void *creator();
	static MStatus initialize();
	void postConstructor() override;

	virtual bool    isAbstractClass()  const  override {return true;}


	static MObject aInputGradientImage;
	static MObject aMagnitudeRemapRange;

	static MObject aRadialMagnitudeRemapRamp;
	static MObject aTangentMagnitudeRemapRamp;

	static MObject aRadialMag;
	static MObject aTangentMag;

	static MTypeId id;

private:
	virtual MStatus calcForce(
		MDataBlock &data,
		const MVectorArray &points,
		const MVectorArray &velocities,
		const MDoubleArray &masses,
		MVectorArray &outputForce) override;

};

#endif
