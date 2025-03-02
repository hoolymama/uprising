/**
 * @file pearlChainData.h
 * @brief Defines the pearlChainData class for storing collections of pearl chains.
 * 
 * This file contains the pearlChainData class which provides a custom Maya data type
 * for storing collections of pearl chains. It encapsulates a vector of pearlChain
 * objects that can be passed through Maya's dependency graph.
 */
#ifndef _pearlChainData
#define _pearlChainData

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "pearlChain.h"

/**
 * @class pearlChainData
 * @brief A custom Maya data type for storing collections of pearl chains.
 * 
 * The pearlChainData class extends MPxData to create a custom Maya data type for
 * storing collections of pearl chains. It encapsulates a vector of pearlChain
 * objects, allowing pearl chain data to flow through Maya's dependency graph.
 */
class pearlChainData : public MPxData
{
public:
	/**
	 * @brief Default constructor.
	 */
	pearlChainData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~pearlChainData();
	
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
	 * @return Pointer to a new pearlChainData object.
	 */
	static void *creator();
public:
	/** @brief Type name for this data type. */
	static const MString typeName;
	
	/** @brief Type ID for this data type. */
	static const MTypeId id;

	/** @brief Pointer to a vector of pearl chains. */
	std::vector< pearlChain > *fGeometry;
};
#endif



