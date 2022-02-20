

#ifndef gridableField_H
#define gridableField_H
/*
 *  gridableField.h
 *  Created by Julian Mann on Sun Jul 18 2021.
 *  Copyright 2006 hooly|mama. All rights reserved.
 */

#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MPxFieldNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

class gridableField : public MPxFieldNode
{
public:
	gridableField();
	virtual ~gridableField();

	static void *creator();
	static MStatus initialize();
	void postConstructor() override;

	virtual MStatus compute(const MPlug &plug, MDataBlock &data) override;

	virtual bool    isAbstractClass()  const override {return false;}

	virtual MStatus getForceAtPoint(const MVectorArray &point,
									const MVectorArray &velocity,
									const MDoubleArray &mass,
									MVectorArray &force,
									double deltaTime) override;


	static MObject aResolutionX;
	static MObject aResolutionY;
	static MObject aResolutionZ;
	static MObject aOutputGridPoints;
	static MObject aOutputGridVectors;

	static MTypeId id;

private:
	virtual MStatus calcForce(
		MDataBlock &data,
		const MVectorArray &points,
		const MVectorArray &velocities,
		const MDoubleArray &masses,
		MVectorArray &outputForce); 

	MStatus computeOutputForce(const MPlug &plug, MDataBlock &data);
	MStatus computeOutputGrid(const MPlug &plug, MDataBlock &data);
};

#endif
