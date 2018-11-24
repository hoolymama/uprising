#ifndef _FilterDefinition_
#define _FilterDefinition_

#include <maya/MArrayDataHandle.h>
#include <maya/MStatus.h>

#include "stroke.h"

class FilterDefinition {
public:


	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >
	m_definition;

	bool m_usesMap;


	FilterDefinition(MArrayDataHandle &ha, MObject &keyAtt, MObject &operatorAtt,
	                 MObject &operandAtt) :
		m_definition(),
		m_usesMap(false)
	{
		MStatus st;
		unsigned nPlugs = ha.elementCount();

		for (unsigned i = 0; i < nPlugs; i++, ha.next()) {
			unsigned index = ha.elementIndex(&st);

			if (st.error()) {
				continue;
			}

			MDataHandle hComp = ha.inputValue(&st);
			if (st.error()) {
				continue;
			}

			Stroke::SortFilterKey key =  Stroke::SortFilterKey(
			                               hComp.child(keyAtt).asShort());

			Stroke::FilterOperator op =  Stroke::FilterOperator(
			                               hComp.child(
			                                 operatorAtt).asShort());

			int value = hComp.child(operandAtt).asInt();

			m_definition.push_back( std::make_tuple(key, op,  value) );

		}
	};

	~FilterDefinition() {};


	bool hasFilters() const
	{
		return (m_definition.size() > 0);
	}
	bool usesMap() const
	{
		return m_usesMap;
	}

	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
	begin() const
	{
		return m_definition.begin();
	}

	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
	end() const
	{
		return m_definition.end();
	}





};
#endif



// std::vector< std::tuple <StrokeSortFilterKey, Stroke::FilterOperator, int> >::const_iterator
// citer;

// for (citer = m_definition.begin(); citer != m_definition.end(); citer++)
// {
// 	StrokeSortFilterKey key = std::get<0>(*citer);


// 	if ( key == painting::kMapRed || key == painting::kMapGreen || key == painting::kMapBlue )
// 	{
// 		return true;
// 	}
// }
// return false;