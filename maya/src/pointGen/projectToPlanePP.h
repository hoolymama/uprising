/**
 * @file projectToPlanePP.h
 * @brief Defines the projectToPlanePP node for projecting per-point data onto a plane.
 * 
 * This file contains the projectToPlanePP class which extends Maya's MPxNode to provide
 * functionality for projecting per-point vector data onto a plane defined by a normal vector.
 * This node is useful for constraining points to a plane in particle systems or other
 * point-based data structures.
 */

#include <maya/MPxNode.h>

/**
 * @class projectToPlanePP
 * @brief A node for projecting per-point data onto a plane.
 * 
 * The projectToPlanePP class extends MPxNode to create a specialized node that
 * projects per-point vector data onto a plane defined by a normal vector.
 * This is useful for constraining points to a plane in particle systems or other
 * point-based data structures.
 */
class projectToPlanePP : public MPxNode
{
public:
	/**
	 * @brief Post-constructor initialization.
	 */
	virtual void postConstructor();
	
	/**
	 * @brief Default constructor.
	 */
	projectToPlanePP();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~projectToPlanePP();
	
	/**
	 * @brief Compute method for calculating the node's outputs.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	
	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new projectToPlanePP object.
	 */
	static void *creator();
	
	/**
	 * @brief Initialize attributes and connections for the node.
	 * @return MStatus indicating success or failure.
	 */
	static MStatus initialize();
	
	/** @brief Unique type ID for this node type. */
	static MTypeId id;

private:
	/** @brief Input vector data attribute. */
	static MObject aInput;
	
	/** @brief Plane normal vector attribute. */
	static MObject aPlaneNormal;
	
	/** @brief Output vector data attribute. */
	static MObject aOutput;
};
