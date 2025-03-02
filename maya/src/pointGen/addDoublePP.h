/**
 * @file addDoublePP.h
 * @brief Defines the addDoublePP node for adding a scalar value to per-point double data.
 * 
 * This file contains the addDoublePP class which extends Maya's MPxNode to provide
 * functionality for adding a scalar value to per-point double data in Maya's dependency graph.
 * This node is useful for modifying per-point double attributes in particle systems or other
 * point-based data structures.
 */

#ifndef addDoublePP_H
#define addDoublePP_H



#include <maya/MPxNode.h>

/**
 * @class addDoublePP
 * @brief A node for adding a scalar value to per-point double data.
 * 
 * The addDoublePP class extends MPxNode to create a specialized node that
 * adds a scalar value to per-point double data. This is useful for modifying
 * per-point attributes in particle systems or other point-based data structures.
 */
class addDoublePP: public MPxNode {
	public:
        /**
         * @brief Post-constructor initialization.
         */
virtual	void		postConstructor();
        /**
         * @brief Default constructor.
         */
		addDoublePP (); 
        
        /**
         * @brief Virtual destructor.
         */
        virtual ~addDoublePP ();
        
        /**
         * @brief Compute method for calculating the node's outputs.
         * @param plug The plug to compute.
         * @param data The data block containing the node's data.
         * @return MStatus indicating success or failure.
         */
		virtual MStatus compute (const MPlug& plug, MDataBlock& data);
        
        /**
         * @brief Creator function for Maya's plugin system.
         * @return Pointer to a new addDoublePP object.
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
        /** @brief Input per-point double data attribute. */
		static MObject aInput;
        /** @brief Scalar value to add to each point. */
        static MObject aScalarInput;
        /** @brief Output per-point double data attribute. */
		static MObject aOutput;
};

 
#endif