#ifndef _BrushModel_
#define _BrushModel_

// #include <pair>
#include <vector>

#include <maya/MArrayDataHandle.h>

#include <string>

#include <maya/MStatus.h>
#include "brush.h"

// #include "stroke.h"


class BrushModel {
public:
	BrushModel();
	~BrushModel();
	
	bool isFlat() const;
	bool isRound() const;
	float width() const;
	std::string name () const;
	Brush::Shape shape() const;

	void addBrush(int brushId, const Brush &brush);

	bool operator<(const BrushModel& other ) const;

	friend ostream &operator<<(ostream &os, const BrushModel &bm);

private:

	std::string m_name;
	float m_width;
	Brush::Shape m_shape;
	std::vector< std::pair<int, Brush > > m_brushes;



};


// class BrushRack {
// public:


// BrushRack(const std::map<int, Brush> &brushes);

// ~BrushRack();


// // 	bool hasFilters() const;
// // 	bool usesMap() const;

// std::vector< BrushModel >::const_iterator begin() const;

// std::vector< BrushModel > ::const_iterator	end() const;

// private:
// // 	std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >
// // 	m_definition;

// std::vector< BrushModel > m_brushModels;


// // 	bool m_usesMap;

// };
#endif
