
#include "sortDefinition.h"

SortDefinition::SortDefinition(MArrayDataHandle &ha, MObject &keyAtt,
                               MObject &directionAtt) :
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

		if (hasKey(key))
		{
			continue;
		}

		if (
		  key == Stroke::kMapRed ||
		  key == Stroke::kMapGreen ||
		  key == Stroke::kMapBlue )
		{
			m_usesMap = true;
		}

		Stroke::SortDirection direction =  Stroke::SortDirection(
		                                     hComp.child(directionAtt).asShort());

		m_definition.push_back(  std::make_pair(key, direction ));

	}
};

SortDefinition::~SortDefinition() {};


bool SortDefinition::hasSort() const
{
	return (m_definition.size() > 0);
}
bool SortDefinition::usesMap() const
{
	return m_usesMap;
}

std::vector< std::pair <Stroke::SortFilterKey, Stroke::SortDirection> >::const_iterator
SortDefinition::begin() const
{
	return m_definition.begin();
}

std::vector< std::pair <Stroke::SortFilterKey, Stroke::SortDirection> >::const_iterator
SortDefinition::end() const
{
	return m_definition.end();
}



bool SortDefinition::hasKey( Stroke::SortFilterKey key) const
{
	std::vector< std::pair <Stroke::SortFilterKey, Stroke::SortDirection> >::const_iterator it
	  = m_definition.begin();
	for (; it != m_definition.end(); ++it)
	{
		if (it->first == key) {
			return true;
		}
	}
	return false;
}