
#include "filterDefinition.h"




FilterDefinition::FilterDefinition(MArrayDataHandle &ha, MObject &keyAtt,
                                   MObject &operatorAtt,
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

		if (
		  key == Stroke::kMapRed ||
		  key == Stroke::kMapGreen ||
		  key == Stroke::kMapBlue )
		{
			m_usesMap = true;
		}

		Stroke::FilterOperator op =  Stroke::FilterOperator(
		                               hComp.child(
		                                 operatorAtt).asShort());

		int value = hComp.child(operandAtt).asInt();

		m_definition.push_back( std::make_tuple(key, op,  value) );

	}
};

FilterDefinition::~FilterDefinition() {};


bool FilterDefinition::hasFilters() const
{
	return (m_definition.size() > 0);
}
bool FilterDefinition::usesMap() const
{
	return m_usesMap;
}

std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
FilterDefinition::begin() const
{
	return m_definition.begin();
}

std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
FilterDefinition::end() const
{
	return m_definition.end();
}
