
#include "brushModel.h"

const float epsilon = 0.0001;

 

BrushModel::BrushModel( ):
	m_brushes(),
	m_peg(-1)
{
}
BrushModel::~BrushModel() {}

// TODO: Check validity.
void BrushModel::addBrush(int brushId, const Brush &brush)
{
    m_brushes.push_back(std::make_pair(brushId, brush));
}

ostream &operator<<(ostream &os, const BrushModel &bm)
{
    os << "{Count: " << bm.m_brushes.size() << "}";
    return os;
}
