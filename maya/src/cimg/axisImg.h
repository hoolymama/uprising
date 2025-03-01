/**
 * @file axisImg.h
 * @brief Maya node for visualizing skeleton radius information
 * 
 * This class implements a Maya node that takes a skeleton image with radius information
 * and creates a visualization by drawing circles at each skeleton point with the 
 * corresponding radius. This is useful for debugging or visualizing medial axis transforms
 * and other skeleton-based algorithms.
 */

#ifndef _axisImg
#define _axisImg

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

/**
 * @class axisImg
 * @brief Maya node for visualizing skeleton radius information
 * 
 * This node takes a skeleton image where pixel values represent radii and creates
 * a visualization by drawing circles. Features include:
 * - Processing float-based skeleton images
 * - Configurable maximum radius limit
 * - Circle-based visualization of radius data
 */
class axisImg : public MPxNode
{
public:
	/**
	 * @brief Default constructor
	 */
	axisImg();

	/**
	 * @brief Virtual destructor
	 */
	virtual				~axisImg();

	/**
	 * @brief Compute method called by Maya to process the node
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	/**
	 * @brief Creator method for Maya plugin system
	 * @return Pointer to a new instance of axisImg
	 */
	static  void		*creator();

	/**
	 * @brief Initialize method for Maya plugin system
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	// Input Attributes
	static MObject aSkeletonImage;   ///< Input skeleton image with radius information (float data)
	static MObject aMaxRadius;       ///< Maximum radius to draw (clamps larger values)

	// Output Attributes
	static MObject aOutput;          ///< Processed visualization image

	static MTypeId	id;


private:
	/**
	 * @brief Helper method to retrieve image data from a plug
	 * @param data The data block to read from
	 * @param attribute The attribute to read
	 * @return Pointer to the CImg<float> image, or nullptr if error
	 */
	CImg<float> *getImage(MDataBlock &data, MObject &attribute );

};

#endif
