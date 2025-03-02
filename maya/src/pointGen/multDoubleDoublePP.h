/**
 * @file multDoubleDoublePP.h
 * @brief Defines the multDoubleDoublePP node for multiplying per-point double data.
 * 
 * This file contains the multDoubleDoublePP class which extends Maya's MPxNode to provide
 * functionality for multiplying two per-point double data arrays in Maya's dependency graph.
 * This node is useful for modifying per-point double attributes in particle systems or other
 * point-based data structures.
 */

#include <maya/MPxNode.h>

/**
 * @class multDoubleDoublePP
 * @brief A node for multiplying two per-point double data arrays.
 * 
 * The multDoubleDoublePP class extends MPxNode to create a specialized node that
 * multiplies two per-point double data arrays. This is useful for modifying
 * per-point attributes in particle systems or other point-based data structures.
 */
class multDoubleDoublePP: public MPxNode {
	public:
    /**
     * @brief Post-constructor initialization.
     */
virtual	void		postConstructor();
    /**
     * @brief Default constructor.
     */
		multDoubleDoublePP (); 
    /**
     * @brief Virtual destructor.
     */
    virtual ~multDoubleDoublePP ();
    /**
     * @brief Compute method for calculating the node's outputs.
     * @param plug The plug to compute.
     * @param data The data block containing the node's data.
     * @return MStatus indicating success or failure.
     */
		virtual MStatus compute (const MPlug& plug, MDataBlock& data);
    /**
     * @brief Creator function for Maya's plugin system.
     * @return Pointer to a new multDoubleDoublePP object.
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
    /** @brief First input per-point double data attribute. */
		static MObject aInput1;
    /** @brief Second input per-point double data attribute. */
		static MObject aInput2;	   
    /** @brief Default value to use when input data is missing. */
		static MObject aDefault;	   
		
    /** @brief Output per-point double data attribute. */
		static MObject aOutput;

};
