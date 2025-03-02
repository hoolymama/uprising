/**
 * @file gridableField.h
 * @brief Defines the gridableField class for creating grid-based force fields in Maya.
 * 
 * This file contains the gridableField class which extends Maya's MPxFieldNode to provide
 * a base class for creating grid-based force fields. It handles the common functionality
 * for calculating forces on a grid and outputting the grid points and vectors.
 * 
 * Created by Julian Mann on Sun Jul 18 2021.
 * Copyright 2006 hooly|mama. All rights reserved.
 */

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

/**
 * @class gridableField
 * @brief Base class for creating grid-based force fields in Maya.
 * 
 * The gridableField class extends MPxFieldNode to provide a base class for
 * creating grid-based force fields. It handles the common functionality for
 * calculating forces on a grid and outputting the grid points and vectors.
 * Derived classes can implement specific force calculation algorithms by
 * overriding the calcForce method.
 */
class gridableField : public MPxFieldNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	gridableField();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~gridableField();

	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new gridableField object.
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
	 * @brief Compute method for calculating the node's outputs.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus compute(const MPlug &plug, MDataBlock &data) override;

	/**
	 * @brief Indicates whether this class is abstract.
	 * @return False, as this is not an abstract class.
	 */
	virtual bool isAbstractClass() const override {return false;}

	/**
	 * @brief Gets the force at a specific point.
	 * @param point Array of points to calculate forces for.
	 * @param velocity Array of velocities for the points.
	 * @param mass Array of masses for the points.
	 * @param force Output array to store the calculated forces.
	 * @param deltaTime Time step for the calculation.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus getForceAtPoint(const MVectorArray &point,
									const MVectorArray &velocity,
									const MDoubleArray &mass,
									MVectorArray &force,
									double deltaTime) override;


	/** @brief X resolution of the grid. */
	static MObject aResolutionX;
	/** @brief Y resolution of the grid. */
	static MObject aResolutionY;
	/** @brief Z resolution of the grid. */
	static MObject aResolutionZ;
	/** @brief Output grid points attribute. */
	static MObject aOutputGridPoints;
	/** @brief Output grid vectors attribute. */
	static MObject aOutputGridVectors;

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
		MVectorArray &outputForce); 

	/**
	 * @brief Computes the output force attribute.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	MStatus computeOutputForce(const MPlug &plug, MDataBlock &data);
	
	/**
	 * @brief Computes the output grid attributes.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	MStatus computeOutputGrid(const MPlug &plug, MDataBlock &data);
};

#endif
