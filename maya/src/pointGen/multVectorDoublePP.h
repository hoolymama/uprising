/**
 * @file multVectorDoublePP.h
 * @brief Defines the multVectorDoublePP node for multiplying per-point vector data by double values.
 * 
 * This file contains the multVectorDoublePP class which extends Maya's MPxNode to provide
 * functionality for multiplying per-point vector data by double values in Maya's dependency graph.
 * This node is useful for scaling vector attributes in particle systems or other
 * point-based data structures.
 */

#include <maya/MPxNode.h>

/**
 * @class multVectorDoublePP
 * @brief A node for multiplying per-point vector data by double values.
 * 
 * The multVectorDoublePP class extends MPxNode to create a specialized node that
 * multiplies per-point vector data by double values. This is useful for scaling
 * vector attributes in particle systems or other point-based data structures.
 * It also provides an option to normalize the resulting vectors.
 */
class multVectorDoublePP: public MPxNode {
public:
	/**
	 * @brief Post-constructor initialization.
	 */
	virtual	void		postConstructor();
	
	/**
	 * @brief Default constructor.
	 */
	multVectorDoublePP (); 
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~multVectorDoublePP ();
	
	/**
	 * @brief Compute method for calculating the node's outputs.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus compute (const MPlug& plug, MDataBlock& data);
	
	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new multVectorDoublePP object.
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
	/** @brief Input vector data attribute. */
	static MObject aInput1;
	
	/** @brief Input double data attribute. */
	static MObject aInput2;	  
	
	/** @brief Scalar input value attribute. */
	static MObject aScalarInput;
	
	/** @brief Flag to normalize the output vectors. */
	static MObject aNormalizeVector;	   
	
	/** @brief Output vector data attribute. */
	static MObject aOutput;

};
