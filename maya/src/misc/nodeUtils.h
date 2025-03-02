/**
 * @file nodeUtils.h
 * @brief Defines utility functions for working with Maya nodes.
 * 
 * This file contains the NodeUtils class which provides utility functions for
 * working with Maya nodes, particularly for retrieving transformation matrices
 * from shape and field nodes.
 */

#ifndef nodeUtils_H
#define nodeUtils_H

#include <maya/MObject.h>
#include <maya/MMatrix.h>

/**
 * @class NodeUtils
 * @brief Utility class for working with Maya nodes.
 * 
 * The NodeUtils class provides static utility functions for working with Maya nodes,
 * particularly for retrieving transformation matrices from shape and field nodes.
 */
class NodeUtils {

public:
	/**
	 * @brief Gets the first world matrix from a shape node.
	 * @param shapeNode The shape node to get the matrix from.
	 * @return MMatrix representing the world transformation.
	 */
	static MMatrix firstWorldMatrix(const MObject & shapeNode);
	
	/**
	 * @brief Gets the world matrix from a field node.
	 * @param fieldNode The field node to get the matrix from.
	 * @return MMatrix representing the world transformation.
	 */
	static MMatrix fieldWorldMatrix(const MObject & fieldNode);
};

#endif

