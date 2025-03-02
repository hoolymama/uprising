/**
 * @file particleTrailsData.h
 * @brief Defines the particleTrailsData class for storing collections of particle trails.
 * 
 * This file contains the particleTrailsData class which provides a custom Maya data type
 * for storing collections of particle trails. It encapsulates a vector of particleTrail
 * objects that can be passed through Maya's dependency graph.
 */
#ifndef _particleTrailsData
#define _particleTrailsData
#include <vector>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "particleTrail.h"

/**
 * @class particleTrailsData
 * @brief A custom Maya data type for storing collections of particle trails.
 * 
 * The particleTrailsData class extends MPxData to create a custom Maya data type for
 * storing collections of particle trails. It encapsulates a vector of particleTrail
 * objects, allowing particle trail data to flow through Maya's dependency graph.
 */
class particleTrailsData : public MPxData
{
public:
	/**
	 * @brief Default constructor.
	 */
	particleTrailsData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~particleTrailsData();
	
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
	 * @return Pointer to a new particleTrailsData object.
	 */
	static void *creator();
public:
	/** @brief Type name for this data type. */
	static const MString typeName;
	
	/** @brief Type ID for this data type. */
	static const MTypeId id;

	/** @brief Pointer to a vector of particle trails. */
	std::vector< particleTrail > *fGeometry;
};
#endif



