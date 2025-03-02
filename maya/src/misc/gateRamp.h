/**
 * @file gateRamp.h
 * @brief Defines the gateRamp class for creating gated ramp functions.
 * 
 * This file contains the gateRamp class which provides functionality for creating
 * gated ramp functions in Maya. These functions can be used for various effects
 * that require controlled transitions between values with padding.
 */

#ifndef gateRamp_H
#define gateRamp_H

#include <maya/MPxNode.h>

/**
 * @class gateRamp
 * @brief A node for creating gated ramp functions.
 * 
 * The gateRamp class extends MPxNode to create a specialized node that generates
 * gated ramp functions. It provides functionality for:
 * - Creating gates in a ramp function
 * - Controlling the number of gates
 * - Setting the range of the ramp
 * - Adding padding between gates
 * - Mapping output to a specified range
 */
class gateRamp: public MPxNode {

public:
	/**
	 * @brief Post-constructor setup for the node.
	 */
	virtual	void		postConstructor();
	
	/**
	 * @brief Default constructor.
	 */
	gateRamp ();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~gateRamp ();

	/**
	 * @brief Compute method for calculating the node's outputs.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus compute (const MPlug &plug, MDataBlock &data);
	
	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new gateRamp object.
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
	/** @brief Number of gates attribute. */
	static MObject aNumGates;
	
	/** @brief Range attribute. */
	static MObject aRange;
	
	/** @brief Padding attribute. */
	static MObject aPadding;
	
	/** @brief Ramp attribute. */
	static MObject aRamp;

	/** @brief Minimum output value attribute. */
	static MObject aOutputMin;
	
	/** @brief Maximum output value attribute. */
	static MObject aOutputMax;
	
	/** @brief Output attribute. */
	static MObject aOutput;
};

#endif