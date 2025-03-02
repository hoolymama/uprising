/**
 * @file addVectorPP.h
 * @brief Defines the addVectorPP node for adding a scalar value to per-point vector data.
 * 
 * This file contains the addVectorPP class which extends Maya's MPxNode to provide
 * functionality for adding a scalar value to per-point vector data in Maya's dependency graph.
 * This node is useful for modifying per-point vector attributes in particle systems or other
 * point-based data structures.
 */

#ifndef addVectorPP_H
#define addVectorPP_H



#include <maya/MPxNode.h>

/**
 * @class addVectorPP
 * @brief A node for adding a scalar value to per-point vector data.
 * 
 * The addVectorPP class extends MPxNode to create a specialized node that
 * adds a scalar value to per-point vector data. This is useful for modifying
 * per-point vector attributes in particle systems or other point-based data structures.
 */
class addVectorPP: public MPxNode {
	public:
        /**
         * @brief Post-constructor initialization.
         */
virtual	void		postConstructor();
        /**
         * @brief Default constructor.
         */
		addVectorPP (); 
        
        /**
         * @brief Virtual destructor.
         */
        virtual ~addVectorPP ();
        
        /**
         * @brief Compute method for calculating the node's outputs.
         * @param plug The plug to compute.
         * @param data The data block containing the node's data.
         * @return MStatus indicating success or failure.
         */
		virtual MStatus compute (const MPlug& plug, MDataBlock& data);
        
        /**
         * @brief Creator function for Maya's plugin system.
         * @return Pointer to a new addVectorPP object.
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
        /** @brief Input per-point vector data attribute. */
		static MObject aInput;
        /** @brief Scalar value to add to each vector component. */
        static MObject aScalarInput;
        /** @brief Output per-point vector data attribute. */
		static MObject aOutput;
};

 
#endif