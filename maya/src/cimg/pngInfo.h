/**
 * @file pngInfo.h
 * @brief Maya command for PNG file analysis.
 *
 * This file implements a Maya command for PNG file analysis:
 * - Extracts PNG file information
 * - Analyzes color palettes
 * - Reports image properties
 * - Supports palette size optimization
 */

#ifndef	__pngInfo_H__
#define	__pngInfo_H__

#include <maya/MIOStream.h>
#include <math.h>
#include "lodepng.h"
#include <maya/MPxCommand.h>


// #define kFilenameFlag	"-fn"
// #define kFilenameFlagL "-filename"

/** @brief Flag for analyzing palette */
#define kPaletteFlag "-p"
#define kPaletteFlagL "-palette"

/** @brief Flag for specifying palette size */
#define kPaletteSizeFlag "-ps"
#define kPaletteSizeFlagL "-paletteSize"

/**
 * @class pngInfo
 * @brief Maya command for PNG file analysis.
 *
 * pngInfo provides PNG file analysis functionality:
 * - Extracts file information and properties
 * - Analyzes and optimizes color palettes
 * - Reports detailed image statistics
 * - Supports palette size optimization
 */
class pngInfo : public MPxCommand
{
public:
	/**
	 * @brief Default constructor.
	 */
	pngInfo() {}

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~pngInfo() {}

	/**
	 * @brief Executes the command.
	 * @param args Command arguments
	 * @return MStatus Success or failure status
	 */
	MStatus doIt( const MArgList &args );

	/**
	 * @brief Creates a new instance of the command.
	 * @return Pointer to new pngInfo instance
	 */
	static void *creator();

	/**
	 * @brief Creates command syntax.
	 * @return MSyntax object defining command syntax
	 */
	static MSyntax      newSyntax();



private:



	/**
	 * @brief Reads PNG image data.
	 * @param filename Input PNG filename
	 * @param state LodePNG state object
	 * @param w Output image width
	 * @param h Output image height
	 * @param image Output image data
	 * @return MStatus Success or failure status
	 */
	MStatus readImage(const MString &filename, lodepng::State &state, unsigned &w,
	                  unsigned &h, std::vector<unsigned char> &image );

	/**
	 * @brief Handles palette analysis flag.
	 * @param filename Input PNG filename
	 * @return MStatus Success or failure status
	 */
	MStatus handlePaletteFlag(const MString &filename);

	/**
	 * @brief Handles palette size optimization flag.
	 * @param filename Input PNG filename
	 * @return MStatus Success or failure status
	 */
	MStatus handlePaletteSizeFlag(const MString &filename);

};

#endif

