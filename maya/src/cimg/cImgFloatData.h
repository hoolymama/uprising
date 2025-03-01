/**
 * @file cImgFloatData.h
 * @brief Custom Maya data type for floating-point image data.
 *
 * This file defines a custom Maya data type for handling floating-point image data:
 * - Extends Maya's data system with floating-point image support
 * - Used for high-precision image operations like gradients
 * - Supports Maya's dependency graph data flow
 */

#ifndef _cImgFloatData
#define _cImgFloatData

// #define cimg_display 0

#include "cImgData.h"

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

/**
 * @class cImgFloatData
 * @brief Custom Maya data type for floating-point image data.
 *
 * cImgFloatData provides floating-point image handling in Maya:
 * - Wraps CImg<float> for high-precision image storage
 * - Implements Maya's data type interface
 * - Supports copying and assignment
 * - Used for operations requiring floating-point precision
 */
class cImgFloatData : public MPxData
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgFloatData();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgFloatData();

	/**
	 * @brief Creates a new instance of the data type.
	 * @return Pointer to new cImgFloatData instance
	 */
	static void* creator();

	/**
	 * @brief Copies data from another instance.
	 * @param src Source data to copy from
	 */
	virtual void copy(const MPxData& src);

	/**
	 * @brief Assignment operator.
	 * @param src Source data to assign from
	 * @return Reference to this object
	 */
	cImgFloatData& operator=(const cImgFloatData& src);

	/**
	 * @brief Gets the type ID of this data type.
	 * @return Maya type ID for this data
	 */
	virtual MTypeId typeId() const { return id; }

	/**
	 * @brief Gets the type name of this data type.
	 * @return String name of this data type
	 */
	virtual MString name() const { return typeName; }

	/** @brief Maya type ID for this data type */
	static MTypeId id;
	/** @brief Type name string for this data type */
	static MString typeName;

	/** @brief The actual floating-point image data */
	CImg<float>* fImg;
};

#endif


