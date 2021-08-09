

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

class cImgGradField : public MPxFieldNode
{
public:
	cImgGradField();
	virtual ~cImgGradField();

	static void *creator();
	static MStatus initialize();

	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	virtual MStatus getForceAtPoint(const MVectorArray &point,
									const MVectorArray &velocity,
									const MDoubleArray &mass,
									MVectorArray &force,
									double deltaTime);

	static MObject aInputGradientImage;
	static MObject aProjectionMatrix;
	// static MObject aInMatrixInverse;ˆø
	
	

	static MTypeId id;

private:
	MStatus calcForce(
		MDataBlock &data,
		const MVectorArray &points,
		const MVectorArray &velocities,
		const MDoubleArray &masses,
		MVectorArray &outputForce);
};

// namespace
// {
//   static MCallbackId id;

//   static void makeDefaultConnections(MObject &node, void *clientData)
//   {

//     MPlug wmPlugmulti(node, cImgGradField::worldMatrix);
//     MPlug wm(wmPlugmulti.elementByLogicalIndex(0));
//     MPlug mt(node, cImgGradField::aInMatrix);

//     MPlug wmiPlugmulti(node, cImgGradField::worldInverseMatrix);
//     MPlug wmi(wmiPlugmulti.elementByLogicalIndex(0));
//     MPlug mti(node, cImgGradField::aInMatrixInverse);


//     MDGModifier mod;
//     mod.connect(wm, mt);
// 	mod.connect(wmi, mti);
	
//     MStatus stat = mod.doIt();
//     if (stat != MS::kSuccess)
//     {
//       stat.perror("painting ERROR :: callback unable to make matrix connections");
//     }
//   }
// }
#endif
