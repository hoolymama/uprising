/**
 * @file brushShopData.h
 * @brief Defines the Maya data type for brush shop data.
 *
 * This file implements a custom Maya data type for brush shops:
 * - Wraps BrushShop in Maya's data system
 * - Enables brush shop data to flow through Maya's dependency graph
 * - Supports copying and assignment of brush shop data
 * - Provides type identification for Maya's type system
 */

#ifndef _brushShopData
#define _brushShopData

#include <map>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

#include "brush.h"
#include "brushShop.h"

/**
 * @class brushShopData
 * @brief Custom Maya data type for brush shop data.
 *
 * brushShopData wraps BrushShop in Maya's data system:
 * - Implements Maya's MPxData interface
 * - Enables brush shop data to flow through Maya's dependency graph
 * - Supports copying and assignment of brush shop data
 * - Provides type identification for Maya's type system
 */
class brushShopData : public MPxData
{
public:
	/**
	 * @brief Default constructor.
	 */
	brushShopData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~brushShopData();

	/**
	 * @brief Clears the brush shop data.
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
	 * @brief Assignment operator.
	 * @param otherData Source data to assign from
	 * @return Reference to this object
	 */
	brushShopData &operator=(const brushShopData &otherData);

	/**
	 * @brief Creates a new instance of the data type.
	 * @return Pointer to new brushShopData instance
	 */
	static void *creator();

public:
	/** @brief Type name string for this data type */
	static const MString typeName;
	/** @brief Maya type ID for this data type */
	static const MTypeId id;

	/** @brief The actual brush shop data */
	BrushShop *fGeometry;
};
#endif
