/**
 * @file cImgGradField.h
 * @brief Defines the cImgGradField class for creating force fields based on image gradients.
 * 
 * This file contains the cImgGradField class which extends gridableField to create
 * force fields in Maya based on image gradient data. It allows for the creation of
 * complex force fields that follow the gradient patterns of input images, with
 * customizable magnitude remapping.
 * 
 * Created by Julian Mann on Sun Jul 18 2021.
 * Copyright 2006 hooly|mama. All rights reserved.
 */

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

/**
 * @class cImgGradField
 * @brief A field node that generates forces based on image gradient data.
 * 
 * The cImgGradField class extends gridableField to create a specialized field node
 * that generates forces based on image gradient data. It provides attributes for
 * controlling how the gradient data is interpreted and mapped to force magnitudes,
 * allowing for both radial and tangential force components.
 */
class cImgGradField : public gridableField
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgGradField();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgGradField();

	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new cImgGradField object.
	 */
	static void *creator();
	
	/**
	 * @brief Initialize attributes and connections for the node.
	 * @return MStatus indicating success or failure.
	 */
	static MStatus initialize();
	
	/**
	 * @brief Post-constructor initialization.
	 */
	void postConstructor() override;

	/**
	 * @brief Indicates whether this class is abstract.
	 * @return True, as this is an abstract class.
	 */
	virtual bool    isAbstractClass()  const  override {return true;}


	/** @brief Input gradient image attribute. */
	static MObject aInputGradientImage;
	/** @brief Magnitude remapping range attribute. */
	static MObject aMagnitudeRemapRange;

	/** @brief Radial magnitude remapping ramp attribute. */
	static MObject aRadialMagnitudeRemapRamp;
	/** @brief Tangent magnitude remapping ramp attribute. */
	static MObject aTangentMagnitudeRemapRamp;

	/** @brief Radial magnitude attribute. */
	static MObject aRadialMag;
	/** @brief Tangent magnitude attribute. */
	static MObject aTangentMag;

	/** @brief Unique type ID for this node type. */
	static MTypeId id;

private:
	/**
	 * @brief Calculates the force at each point.
	 * @param data The data block containing the node's data.
	 * @param points Array of points to calculate forces for.
	 * @param velocities Array of velocities for the points.
	 * @param masses Array of masses for the points.
	 * @param outputForce Output array to store the calculated forces.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus calcForce(
		MDataBlock &data,
		const MVectorArray &points,
		const MVectorArray &velocities,
		const MDoubleArray &masses,
		MVectorArray &outputForce) override;

};

#endif
