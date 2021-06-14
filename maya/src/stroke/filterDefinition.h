#ifndef _FilterDefinition_
#define _FilterDefinition_

#include <maya/MArrayDataHandle.h>
#include <maya/MStatus.h>

#include "stroke.h"

class FilterDefinition {
public:


	FilterDefinition(MArrayDataHandle &ha, MObject &keyAtt, MObject &operatorAtt,
	                 MObject &operandAtt);

	~FilterDefinition();


	bool hasFilters() const;
	bool usesMap() const;

	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
	begin() const;

	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
	end() const;

private:
	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >
	m_definition;

	bool m_usesMap;

};
#endif
