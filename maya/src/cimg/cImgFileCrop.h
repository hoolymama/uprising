/**
 * @file cImgFileCrop.h
 * @brief Maya node for image file loading and cropping.
 *
 * This file implements a Maya node for image file operations:
 * - Loads images from disk with sequence support
 * - Provides cropping and resizing functionality
 * - Handles different boundary conditions
 * - Supports letterboxing and aspect ratio control
 */

#ifndef _cImgFileCrop
#define _cImgFileCrop

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>


#include "cImgData.h"

/**
 * @class cImgFileCrop
 * @brief Maya node for image file loading and cropping.
 *
 * cImgFileCrop provides image file and crop functionality:
 * - Image file loading with sequence support
 * - Dynamic cropping and resizing
 * - Multiple boundary handling methods
 * - Letterboxing and aspect ratio control
 */
class cImgFileCrop : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgFileCrop();

	/**
	 * @brief Virtual destructor.
	 */
	virtual				~cImgFileCrop();

	/**
	 * @brief Computes the output cropped image.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	/**
	 * @brief Calculates color palette from input image.
	 * @param image Source image
	 * @param maxPaletteSize Maximum number of colors in palette
	 * @param palette Output color palette
	 */
	void calculate_pallete(
	const CImg<unsigned char> &image, 
	int maxPaletteSize,
	MColorArray &palette) const;

	/**
	 * @brief Updates dirty state of dependent attributes.
	 * @param plugBeingDirtied The plug that is being dirtied
	 * @param affectedPlugs Array of plugs affected by the dirty state
	 * @return MStatus Success or failure status
	 */
	virtual MStatus setDependentsDirty(
        const MPlug &plugBeingDirtied,
        MPlugArray &affectedPlugs );


	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgFileCrop instance
	 */
	static  void		*creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	/** @brief Input image filename */
	static MObject aImageFilename;
	/** @brief Frame number for image sequence */
	static MObject aImageFrameNumber;
	/** @brief Enable/disable image sequence mode */
	static MObject aUseImageSequence;
	/** @brief Frame number padding for sequences */
	static MObject aPadding;
	/** @brief Computed full image filename */
	static MObject aComputedImageFilename;
	
	/** @brief Enable/disable resizing */
	static MObject aResize;
	/** @brief Target resolution for resizing */
	static MObject aResizeResolution;
	/** @brief Boundary handling method */
	static MObject aBoundary;
	/** @brief Enable/disable cropping */
	static MObject aApplyCrop;
	/** @brief Crop region corner points */
	static MObject aCropCorner;
	/** @brief Crop region resolution */
	static MObject aCropResolution;
	/** @brief Letterboxing method */
	static MObject aLetterbox;


	/** @brief Output processed image */
	static MObject aOutput;
	/** @brief Output crop factor */
	static MObject aOutputCropFactor;
	/** @brief Output X offset factor */
	static MObject aOutputOffsetFactorX;
	/** @brief Output Y offset factor */
	static MObject aOutputOffsetFactorY;



	/** @brief Unique Maya type ID for the node */
	static MTypeId	id;


private:

	/** @brief Boundary handling methods */
	enum Boundary { kBoundaryDirichlet, kBoundaryNeumann};
	/** @brief Letterboxing methods */
	enum Letterbox { kBlack, kWhite, kRepeat };


};

#endif
