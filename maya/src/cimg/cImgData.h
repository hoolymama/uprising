/**
 * @file cImgData.h
 * @brief Custom Maya data type for image data handling.
 *
 * This file defines a custom Maya data type for handling image data:
 * - Wraps CImg library functionality in Maya's data system
 * - Provides image data storage and manipulation
 * - Supports Maya's dependency graph data flow
 */

#ifndef _cImgData
#define _cImgData

#define cimg_display 0

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

#include <queue>
#define cimg_plugin "plugins/skeleton.h"

#define cimg_display 0
#include "CImg.h"

using namespace cimg_library;

/**
 * @class cImgData
 * @brief Custom Maya data type for image data.
 *
 * cImgData provides image data handling in Maya:
 * - Wraps CImg<unsigned char> for image storage
 * - Implements Maya's data type interface
 * - Supports copying and assignment
 * - Defines image channels and common operations
 */
class cImgData : public MPxData
{
public:
	/**
	 * @brief Enumeration of image channels.
	 */
	enum Channel {
		kRed = 0,    ///< Red channel
		kGreen = 1,  ///< Green channel
		kBlue = 2,   ///< Blue channel
		kAlpha = 3   ///< Alpha channel
	};

	/**
	 * @brief Default constructor. Creates an empty CImg instance.
	 */
	cImgData();

	/**
	 * @brief Virtual destructor. Cleans up the CImg instance.
	 */
	virtual	~cImgData();

	/**
	 * @brief Creates a new instance of cImgData.
	 * @return Pointer to the new cImgData instance.
	 */
	static	void		*creator();

	/**
	 * @brief Deep copies data from another MPxData object.
	 * @param other The source MPxData object to copy from.
	 */
	virtual	void		copy(const MPxData &);

	/**
	 * @brief Assignment operator for deep copying between cImgData instances.
	 * @param other The source cImgData object to copy from.
	 * @return Reference to this object.
	 */
	cImgData &operator=(const cImgData &);

	/**
	 * @brief Returns the type ID of this data type.
	 * @return MTypeId unique identifier for this data type.
	 */
	virtual	MTypeId		typeId() const {return id;}

	/** @brief Static type ID for Maya type system */
	static	MTypeId		id;

	/** @brief Static type name for Maya type system */
	virtual	MString		name() const {return typeName;}
	static	MString		typeName;

	/** @brief Pointer to the CImg instance storing the actual image data */
	CImg<unsigned char> *fImg;

	/**
	 * @enum SampleInterpolation
	 * @brief Defines different interpolation methods for sampling.
	 */
	enum SampleInterpolation { kNearest, kBilinear, kBicubic };

	/**
	 * @enum NoiseType
	 * @brief Defines different types of noise for image processing.
	 */
	enum NoiseType { kGaussian, kUniform, kSaltPepper, kPoisson, kRician};

	/**
	 * @enum ResizeInterpolation
	 * @brief Specifies interpolation methods for image resizing.
	 */
	enum ResizeInterpolation {
		kNoInterpolation,
		kNearestNeighbor ,
		kMovingAverage ,
		kLinear ,
		kGrid ,
		kCubic ,
		kLanczos
	};




};

#endif


