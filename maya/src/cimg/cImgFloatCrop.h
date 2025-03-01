/**
 * @file cImgFloatCrop.h
 * @brief Maya node for cropping and resizing floating-point images.
 *
 * This file implements a Maya node that provides advanced image cropping and resizing
 * functionality for floating-point images. It supports:
 * - Dynamic image resizing with resolution control
 * - Cropping with adjustable corner position and size
 * - Configurable boundary conditions
 * - Output of crop and offset factors for downstream nodes
 */

#ifndef _cImgFloatCrop
#define _cImgFloatCrop

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

/**
 * @class cImgFloatCrop
 * @brief Maya node for cropping and resizing floating-point images.
 *
 * This node provides advanced image manipulation capabilities:
 * - Resizing images while maintaining aspect ratio
 * - Cropping images with adjustable position and size
 * - Configurable boundary conditions for edge handling
 * - Output of crop factors and offset information
 * - Real-time preview in Maya viewport
 */
class cImgFloatCrop : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgFloatCrop();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgFloatCrop();

	/**
	 * @brief Computes the output image based on input parameters.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	/**
	 * @brief Updates the dirty state of dependent attributes.
	 * @param plugBeingDirtied The plug that is being dirtied
	 * @param affectedPlugs Array of plugs affected by the change
	 * @return MStatus Success or failure status
	 */
	virtual MStatus setDependentsDirty(
		const MPlug &plugBeingDirtied,
		MPlugArray &affectedPlugs);

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to the new cImgFloatCrop instance
	 */
	static void* creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	/** @brief Input image data */
	static MObject aInput;

	/** @brief Enable/disable resizing */
	static MObject aResize;

	/** @brief Target resolution for resizing */
	static MObject aResizeResolution;

	/** @brief Boundary condition for cropping */
	static MObject aBoundary;

	/** @brief Enable/disable cropping */
	static MObject aApplyCrop;

	/** @brief Top-left corner position for cropping */
	static MObject aCropCorner;

	/** @brief Resolution of the crop region */
	static MObject aCropResolution;

	/** @brief Output image data */
	static MObject aOutput;

	/** @brief Output crop factor (for downstream nodes) */
	static MObject aOutputCropFactor;

	/** @brief Output X offset factor (for downstream nodes) */
	static MObject aOutputOffsetFactorX;

	/** @brief Output Y offset factor (for downstream nodes) */
	static MObject aOutputOffsetFactorY;

	/** @brief Unique Maya type ID for the node */
	static MTypeId id;

private:
	/**
	 * @enum Boundary
	 * @brief Defines boundary conditions for cropping operations.
	 */
	enum Boundary {
		kBoundaryDirichlet, ///< Zero value at boundaries
		kBoundaryNeumann    ///< Repeat edge values at boundaries
	};
};

#endif
