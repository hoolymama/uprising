/**
 * @file multVectorVectorPP.h
 * @brief Defines the multVectorVectorPP node for multiplying two per-point vector data arrays.
 * 
 * This file contains the multVectorVectorPP class which extends Maya's MPxNode to provide
 * functionality for multiplying two per-point vector data arrays in Maya's dependency graph.
 * This node is useful for combining vector attributes in particle systems or other
 * point-based data structures.
 */

#include <maya/MPxNode.h>

/**
 * @class multVectorVectorPP
 * @brief A node for multiplying two per-point vector data arrays.
 * 
 * The multVectorVectorPP class extends MPxNode to create a specialized node that
 * multiplies two per-point vector data arrays. This is useful for combining
 * vector attributes in particle systems or other point-based data structures.
 * It also provides an option to normalize the first input vector.
 */
class multVectorVectorPP: public MPxNode {
	public:
    /**
     * @brief Post-constructor initialization.
     */
virtual	void		postConstructor();
    
    /**
     * @brief Default constructor.
     */
		multVectorVectorPP (); 
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~multVectorVectorPP ();
    
    /**
     * @brief Compute method for calculating the node's outputs.
     * @param plug The plug to compute.
     * @param data The data block containing the node's data.
     * @return MStatus indicating success or failure.
     */
		virtual MStatus compute (const MPlug& plug, MDataBlock& data);
    
    /**
     * @brief Creator function for Maya's plugin system.
     * @return Pointer to a new multVectorVectorPP object.
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
    /** @brief First input vector data attribute. */
		static MObject aInput1;
    
    /** @brief Second input vector data attribute. */
		static MObject aInput2;	   
    
    /** @brief Flag to normalize the first input vector. */
		static MObject aNormalizeInput1;	   
    
    /** @brief Default value to use when input data is missing. */
		static MObject aDefault;	   
		
    /** @brief Output vector data attribute. */
		static MObject aOutput;

};