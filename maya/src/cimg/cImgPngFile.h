/**
 * @file cImgPngFile.h
 * @brief Maya node for PNG file loading and processing.
 *
 * This file implements a Maya node for PNG file operations:
 * - Loads PNG files with palette support
 * - Provides resizing functionality
 * - Separates color and index data
 * - Extracts color palettes
 */

#ifndef _cImgPngFile
#define _cImgPngFile

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgData.h"

/**
 * @class cImgPngFile
 * @brief Maya node for PNG file loading and processing.
 *
 * cImgPngFile provides PNG file handling functionality:
 * - PNG file loading with palette support
 * - Image resizing capabilities
 * - Color and index data separation
 * - Palette extraction and management
 */
class cImgPngFile : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgPngFile();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgPngFile();

	/**
	 * @brief Computes the output image data.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgPngFile instance
	 */
	static void* creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	/** @brief Input PNG filename */
	static MObject aImageFilename;
	/** @brief Enable/disable resizing */
	static MObject aResize;
	/** @brief Target resolution for resizing */
	static MObject aResolution;

	/** @brief Output indexed image data */
	static MObject aOutIndexImage;
	/** @brief Output color image data */
	static MObject aOutColorImage;
	/** @brief Output color palette data */
	static MObject aOutPalette;

	/** @brief Unique Maya type ID for the node */
	static MTypeId id;

private:
	/**
	 * @brief Creates color image from PNG buffer.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @param buffer PNG file buffer
	 * @return MStatus Success or failure status
	 */
	MStatus createColorImage(const MPlug& plug, MDataBlock& data,
		const std::vector<unsigned char>& buffer);

	/**
	 * @brief Creates indexed image from PNG buffer.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @param buffer PNG file buffer
	 * @return MStatus Success or failure status
	 */
	MStatus createIndexImage(const MPlug& plug, MDataBlock& data,
		const std::vector<unsigned char>& buffer);

	/**
	 * @brief Creates palette image from PNG buffer.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @param buffer PNG file buffer
	 * @return MStatus Success or failure status
	 */
	MStatus createPaletteImage(const MPlug& plug, MDataBlock& data,
		const std::vector<unsigned char>& buffer);
};

#endif
