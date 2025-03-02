/**
 * @file stepRamp.h
 * @brief Defines the stepRamp class for creating stepped ramp functions.
 * 
 * This file contains the stepRamp class which provides functionality for creating
 * stepped ramp functions in Maya. These functions can be used for various effects
 * that require discrete steps rather than continuous transitions.
 */

#ifndef stepRamp_H
#define stepRamp_H

#include <maya/MPxNode.h>

/**
 * @class stepRamp
 * @brief A node for creating stepped ramp functions.
 * 
 * The stepRamp class extends MPxNode to create a specialized node that generates
 * stepped ramp functions. It provides functionality for:
 * - Creating discrete steps in a ramp function
 * - Controlling the number of steps
 * - Setting the range of the ramp
 * - Adding random offsets to the steps
 */
class stepRamp: public MPxNode {

public:
	/**
	 * @brief Post-constructor setup for the node.
	 */
	virtual	void		postConstructor();
	
	/**
	 * @brief Default constructor.
	 */
	stepRamp ();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~stepRamp ();

	/**
	 * @brief Compute method for calculating the node's outputs.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus compute (const MPlug &plug, MDataBlock &data);
	
	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new stepRamp object.
	 */
	static void *creator ();
	
	/**
	 * @brief Initialize attributes and connections for the node.
	 * @return MStatus indicating success or failure.
	 */
	static MStatus initialize ();
	
	/** @brief Unique type ID for this node type. */
	static MTypeId id;

private:
	/** @brief Number of steps attribute. */
	static MObject aNumSteps;
	
	/** @brief Range attribute. */
	static MObject aRange;
	
	/** @brief Ramp attribute. */
	static MObject aRamp;
	
	/** @brief Output attribute. */
	static MObject aOutput;

	/** @brief Random offset attribute. */
	static MObject aRandomOffset;
	
	/** @brief Random seed attribute. */
	static MObject aRandomSeed;
};

#endif

