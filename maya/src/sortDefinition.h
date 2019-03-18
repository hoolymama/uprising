#ifndef _SortDefinition_
#define _SortDefinition_

#include <maya/MArrayDataHandle.h>
#include <maya/MStatus.h>

#include "stroke.h"

class SortDefinition {
public:


	SortDefinition(MArrayDataHandle &ha, MObject &keyAtt,
	               MObject &directionAtt);

	~SortDefinition();


	bool hasSort() const;
	bool usesMap() const;


	std::vector< std::pair <Stroke::SortFilterKey, Stroke::SortDirection> >::const_iterator
	begin() const;

	std::vector< std::pair <Stroke::SortFilterKey, Stroke::SortDirection> >::const_iterator
	end() const;

private:
	std::vector< std::pair <Stroke::SortFilterKey, Stroke::SortDirection> >
	m_definition;

	bool m_usesMap;


	bool hasKey( Stroke::SortFilterKey key) const;

};
#endif
