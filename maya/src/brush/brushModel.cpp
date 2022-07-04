
#include "brushModel.h"

const float epsilon = 0.0001;

BrushModel::BrushModel() : m_brushes(),
                           m_peg(-1),
                           m_lastPaintId(-1),
                           m_count(0)
{
}
BrushModel::~BrushModel() {}

// TODO: Check validity.
void BrushModel::addBrush(int brushId, const Brush &brush)
{
    m_brushes.push_back(std::make_pair(brushId, brush));
    m_count = m_brushes.size();
}

const std::pair<int, Brush> BrushModel::selectBrush(int paintId)
{
    // If the paintId changed since last access, cycle to the next brush.
    if (m_lastPaintId != paintId)
    {
        m_peg++;
        m_peg = m_peg % m_count;
        m_lastPaintId = paintId;
    }
    return m_brushes[m_peg];
}

ostream &operator<<(ostream &os, const BrushModel &bm)
{
    os << "Count: " << bm.m_brushes.size();
    return os;
}
