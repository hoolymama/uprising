#ifndef _BrushModel_
#define _BrushModel_


#include <vector>
#include "brush.h"
 
class BrushModel {
public:
	BrushModel();

	~BrushModel();

	void addBrush(int brushId, const Brush &brush);

	bool operator<(const BrushModel& other ) const;

	friend ostream &operator<<(ostream &os, const BrushModel &bm);

private:

	std::vector< std::pair<int, Brush > > m_brushes;

	int m_peg;

};


// std::string m_name;
// float m_width;
// Brush::Shape m_shape;

// bool isFlat() const;
// bool isRound() const;
// float width() const;
// std::string name () const;
// Brush::Shape shape() const;

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
