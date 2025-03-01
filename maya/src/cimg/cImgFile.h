/**
 * @file cImgFile.h
 * @brief Maya node for loading and processing image files
 * 
 * This class implements a Maya node that can load image files, optionally resize them,
 * and make them available for other image processing nodes in the network.
 */

#ifndef _cImgFile
#define _cImgFile

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

/**
 * @class cImgFile
 * @brief Maya node for loading and processing image files
 * 
 * This node allows loading image files into Maya's node network. It supports:
 * - Loading various image file formats
 * - Optional image resizing with different interpolation methods
 * - Resolution control
 */
class cImgFile : public MPxNode
{
public:
	/**
	 * @brief Default constructor
	 */
	cImgFile();

	/**
	 * @brief Virtual destructor
	 */
	virtual				~cImgFile();

	/**
	 * @brief Compute method called by Maya to process the node
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	/**
	 * @brief Creator method for Maya plugin system
	 * @return Pointer to a new instance of cImgFile
	 */
	static  void		*creator();

	/**
	 * @brief Initialize method for Maya plugin system
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();
	static MObject aImageFilename;
	static MObject aResize;
	static MObject aResolution;
	static MObject aInterpolation;



	static MObject aOutput;

	static MTypeId	id;


private:

};

#endif
