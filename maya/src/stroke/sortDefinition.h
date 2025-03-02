/**
 * @file sortDefinition.h
 * @brief Defines the SortDefinition class for managing stroke sorting criteria.
 * 
 * This file contains the SortDefinition class which provides functionality for
 * managing and applying sorting criteria to strokes. It encapsulates a collection
 * of sort definitions, each consisting of a key and a direction, which are used
 * to determine the order in which strokes should be processed.
 */

#ifndef _SortDefinition_
#define _SortDefinition_

#include <maya/MArrayDataHandle.h>
#include <maya/MStatus.h>

#include "stroke.h"

/**
 * @class SortDefinition
 * @brief A class for managing stroke sorting criteria.
 * 
 * The SortDefinition class encapsulates a collection of sort definitions,
 * each consisting of a key (what property to sort on) and a direction (ascending
 * or descending). These definitions are used to determine the order in which
 * strokes should be processed based on their properties.
 */
class SortDefinition {
public:
	/**
	 * @brief Constructor that initializes sort definitions from Maya attributes.
	 * @param ha Array data handle containing sort data.
	 * @param keyAtt The attribute containing sort keys.
	 * @param directionAtt The attribute containing sort directions.
	 */
	SortDefinition(MArrayDataHandle &ha, MObject &keyAtt,
	               MObject &directionAtt);

	/**
	 * @brief Destructor.
	 */
	~SortDefinition();

	/**
	 * @brief Checks if there are any sort definitions.
	 * @return True if there are sort definitions, false otherwise.
	 */
	bool hasSort() const;
	/**
	 * @brief Checks if any sort uses map-based sorting.
	 * @return True if map-based sorting is used, false otherwise.
	 */
	bool usesMap() const;

	/**
	 * @brief Gets an iterator to the beginning of the sort definitions.
	 * @return Const iterator to the beginning of the sort definitions.
	 */
	std::vector< std::pair <Stroke::SortFilterKey, Stroke::SortDirection> >::const_iterator
	begin() const;

	/**
	 * @brief Gets an iterator to the end of the sort definitions.
	 * @return Const iterator to the end of the sort definitions.
	 */
	std::vector< std::pair <Stroke::SortFilterKey, Stroke::SortDirection> >::const_iterator
	end() const;

private:
	/** @brief Vector of sort definitions, each containing a key and a direction. */
	std::vector< std::pair <Stroke::SortFilterKey, Stroke::SortDirection> >
	m_definition;

	/** @brief Flag indicating whether any sort uses map-based sorting. */
	bool m_usesMap;

	/**
	 * @brief Checks if a specific key is used in any sort definition.
	 * @param key The key to check for.
	 * @return True if the key is used, false otherwise.
	 */
	bool hasKey(Stroke::SortFilterKey key) const;

};
#endif
