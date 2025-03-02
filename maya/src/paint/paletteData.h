/**
 * @file paletteData.h
 * @brief Defines the paletteData class for storing palette data in Maya.
 * 
 * This file contains the paletteData class which provides a custom Maya data type
 * for storing palette data. It encapsulates a collection of Paint objects that can
 * be passed through Maya's dependency graph.
 */

#ifndef _paletteData
#define _paletteData
#include <map>
#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "paint.h"

/**
 * @class paletteData
 * @brief A custom Maya data type for storing palette data.
 * 
 * The paletteData class extends MPxData to create a custom Maya data type for
 * storing palette data. It encapsulates a map of Paint objects indexed by ID,
 * allowing palette data to flow through Maya's dependency graph.
 */
class paletteData : public MPxData
{
public:
	/**
	 * @brief Default constructor.
	 */
	paletteData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~paletteData();
	
	/**
	 * @brief Copies data from another MPxData object.
	 * @param other The source data object to copy from.
	 */
	virtual void            copy ( const MPxData &other );
	
	/**
	 * @brief Returns the type ID of this data type.
	 * @return MTypeId representing the data type.
	 */
	virtual MTypeId         typeId() const;
	
	/**
	 * @brief Returns the name of this data type.
	 * @return MString containing the data type name.
	 */
	virtual MString         name() const;

	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new paletteData object.
	 */
	static void *creator();
	
public:
	/** @brief Type name for this data type. */
	static const MString typeName;
	
	/** @brief Type ID for this data type. */
	static const MTypeId id;

	/** @brief Pointer to a map of Paint objects indexed by ID. */
	std::map<int, Paint > *fGeometry;
};
#endif



