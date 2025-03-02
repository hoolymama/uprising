/**
 * @file brushData.h
 * @brief Defines the Maya data type for brush data.
 *
 * This file implements a custom Maya data type for brushes:
 * - Wraps Brush in Maya's data system
 * - Enables brush data to flow through Maya's dependency graph
 * - Supports copying and assignment of brush data
 * - Provides type identification for Maya's type system
 */

#ifndef _brushData
#define _brushData

#include <map>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

#include "brush.h"

/**
 * @class brushData
 * @brief Custom Maya data type for brush data.
 *
 * brushData wraps Brush in Maya's data system:
 * - Implements Maya's MPxData interface
 * - Enables brush data to flow through Maya's dependency graph
 * - Supports copying and assignment of brush data
 * - Provides type identification for Maya's type system
 */
class brushData : public MPxData
{
public:
	/**
	 * @brief Default constructor.
	 */
	brushData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~brushData();

	/**
	 * @brief Clears the brush data.
	 */
	void clear();

	/**
	 * @brief Gets the type ID of this data type.
	 * @return Maya type ID for this data
	 */
	virtual MTypeId typeId() const;
	
	/**
	 * @brief Gets the type name of this data type.
	 * @return String name of this data type
	 */
	virtual MString name() const;
	
	/**
	 * @brief Copies data from another instance.
	 * @param other Source data to copy from
	 */
	void copy(const MPxData &other);

	/**
	 * @brief Creates a new instance of the data type.
	 * @return Pointer to new brushData instance
	 */
	static void *creator();

public:
	/** @brief Type name string for this data type */
	static const MString typeName;
	/** @brief Maya type ID for this data type */
	static const MTypeId id;

	/** @brief The actual brush data */
	Brush *fGeometry;
};
#endif


