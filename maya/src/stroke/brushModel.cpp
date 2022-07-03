
#include "brushModel.h"

const float epsilon = 0.0001;

BrushModel::BrushModel()
{
}
BrushModel::~BrushModel() {}

// TODO: Check validity.
void BrushModel::addBrush(int brushId, const Brush &brush)
{
    if (m_brushes.size() == 0)
    {
        m_name = std::string(brush.model().asChar());
        m_width = brush.width();
        m_shape = brush.shape();
    }
    m_brushes.push_back(std::make_pair(brushId, brush));
}

bool BrushModel::isFlat() const
{
    return m_shape == Brush::kFlat;
}
bool BrushModel::isRound() const
{
    return m_shape == Brush::kRound;
}

float BrushModel::width() const
{
    return m_width;
}

std::string BrushModel::name () const
{
    return m_name;
}

Brush::Shape BrushModel::shape() const
{
    return m_shape;
}

/** \brief Compare two brush models.
 *
 *  \param other The brush model to compare with.
 *  \return True if the OTHER brush models is less than this. Less is defined by:
 *  1. Being round in shape if this is flat.
 *  2. If shape is the same, having less width.
 *  3. If width is equivalent, having a lexigraphically less name.
 */
 
bool BrushModel::operator<(const BrushModel &other) const
{
    if (int(other.shape() < int(m_shape)))
        return false;
    if (int(other.shape() > int(m_shape)))
        return true;

    bool different = fabs(m_width - other.width()) > epsilon;

    if (different)
    {
        if (m_width < other.width())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if (m_name < other.name())
        return true;
    return false;
}

ostream &operator<<(ostream &os, const BrushModel &bm)
{
    MString brushShape = bm.isFlat() ? "flat" : "round";
    os << "{name: " << bm.m_name << ", width: " << bm.m_width << ", shape: " << brushShape << ", count: " << bm.m_brushes.size() << "}" << endl;
    return os;
}

// BrushRack::BrushRack(MArrayDataHandle &ha, MObject &keyAtt,
//                                    MObject &operatorAtt,
//                                    MObject &operandAtt) :
// 	m_definition(),
// 	m_usesMap(false)
// {
// 	MStatus st;
// 	unsigned nPlugs = ha.elementCount();

// 	for (unsigned i = 0; i < nPlugs; i++, ha.next()) {
// 		unsigned index = ha.elementIndex(&st);

// 		if (st.error()) {
// 			continue;
// 		}

// 		MDataHandle hComp = ha.inputValue(&st);
// 		if (st.error()) {
// 			continue;
// 		}

// 		Stroke::SortFilterKey key =  Stroke::SortFilterKey(
// 		                               hComp.child(keyAtt).asShort());

// 		if (
// 		  key == Stroke::kMapRed ||
// 		  key == Stroke::kMapGreen ||
// 		  key == Stroke::kMapBlue )
// 		{
// 			m_usesMap = true;
// 		}

// 		Stroke::FilterOperator op =  Stroke::FilterOperator(
// 		                               hComp.child(
// 		                                 operatorAtt).asShort());

// 		int value = hComp.child(operandAtt).asInt();

// 		m_definition.push_back( std::make_tuple(key, op,  value) );

// 	}
// };

// BrushRack::~BrushRack() {};

// bool BrushRack::hasFilters() const
// {
// 	return (m_definition.size() > 0);
// }
// bool BrushRack::usesMap() const
// {
// 	return m_usesMap;
// }

// std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
// BrushRack::begin() const
// {
// 	return m_definition.begin();
// }

// std::vector< std::tuple <Stroke::SortFilterKey, Stroke::FilterOperator, int> >::const_iterator
// BrushRack::end() const
// {
// 	return m_definition.end();
// }
