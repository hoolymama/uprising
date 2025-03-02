/**
 * @file filterDefinition.h
 * @brief Defines the FilterDefinition class for managing stroke filtering criteria.
 * 
 * This file contains the FilterDefinition class which provides functionality for
 * managing and applying filtering criteria to strokes. It encapsulates a collection
 * of filter definitions, each consisting of a key, operator, and operand value,
 * which are used to determine which strokes should be included or excluded from
 * processing.
 */

#ifndef _FilterDefinition_
#define _FilterDefinition_

#include <maya/MArrayDataHandle.h>
#include <maya/MStatus.h>

#include "stroke.h"

/**
 * @class FilterDefinition
 * @brief A class for managing stroke filtering criteria.
 * 
 * The FilterDefinition class encapsulates a collection of filter definitions,
 * each consisting of a key (what property to filter on), an operator (how to compare),
 * and an operand value (what to compare against). These definitions are used to
 * determine which strokes should be included or excluded from processing based on
 * their properties.
 */
class FilterDefinition {
public:
	/**
	 * @brief Constructor that initializes filter definitions from Maya attributes.
	 * @param ha Array data handle containing filter data.
	 * @param keyAtt The attribute containing filter keys.
	 * @param operatorAtt The attribute containing filter operators.
	 * @param operandAtt The attribute containing filter operands.
	 */
	FilterDefinition(MArrayDataHandle &ha, MObject &keyAtt, MObject &operatorAtt,
	                 MObject &operandAtt);

	/**
	 * @brief Destructor.
	 */
	~FilterDefinition();

	/**
	 * @brief Checks if there are any filter definitions.
	 * @return True if there are filter definitions, false otherwise.
	 */
	bool hasFilters() const;
	/**
	 * @brief Checks if any filter uses map-based filtering.
	 * @return True if map-based filtering is used, false otherwise.
	 */
	bool usesMap() const;

	/**
	 * @brief Gets an iterator to the beginning of the filter definitions.
	 * @return Const iterator to the beginning of the filter definitions.
	 */
	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
	begin() const;

	/**
	 * @brief Gets an iterator to the end of the filter definitions.
	 * @return Const iterator to the end of the filter definitions.
	 */
	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
	end() const;

private:
	/** @brief Vector of filter definitions, each containing a key, operator, and operand. */
	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >
	m_definition;

	/** @brief Flag indicating whether any filter uses map-based filtering. */
	bool m_usesMap;

};
#endif
