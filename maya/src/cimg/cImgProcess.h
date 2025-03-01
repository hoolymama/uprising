/**
 * @file cImgProcess.h
 * @brief Abstract base class for image processing nodes in Maya.
 *
 * This file defines the base class for image processing operations in Maya.
 * It provides common functionality and interface for derived image processing nodes:
 * - Common input/output attributes
 * - Virtual process method for implementing specific image operations
 * - Inversion control for all derived nodes
 */

#ifndef _cImgProcess
#define _cImgProcess

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>


#include "cImgData.h"

/**
 * @class cImgProcess
 * @brief Abstract base class for image processing nodes.
 *
 * cImgProcess serves as the foundation for image processing nodes in Maya:
 * - Defines common interface for image processing operations
 * - Handles basic input/output attribute management
 * - Provides inversion control for processed images
 * - Implements compute method for Maya's dependency graph
 */
class cImgProcess : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgProcess();

	/**
	 * @brief Virtual destructor.
	 */
	virtual				~cImgProcess();

	/**
	 * @brief Computes the output image based on input parameters.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	/**
	 * @brief Indicates this is an abstract class.
	 * @return true as this is an abstract base class
	 */
	virtual bool    isAbstractClass() const {return true;}


	/**
	 * @brief Pure virtual method for image processing implementation.
	 * @param data The data block containing the node's data
	 * @param image The input image to process
	 * @param result The output processed image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                        CImg<unsigned char> &result);

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to the new instance
	 */
	static  void		*creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();


	/** @brief Unique Maya type ID for the node */
	static MTypeId	id;


protected:

	/** @brief Controls whether to invert the processed image */
	static MObject aInvert;
	/** @brief Input image data */
	static MObject aInput;
	/** @brief Output processed image data */
	static MObject aOutput;

};

#endif
