
#include "brushRack.h"

BrushRack::BrushRack(const std::map<int, Brush> &brushes, Brush::Shape shape) : m_shape(shape),
																				m_brushModels(),
																				m_lastPaintId(-1)

{

	// For every brush of input brushes.
	std::map<int, Brush>::const_iterator brushIter = brushes.begin();

	for (; brushIter != brushes.end(); brushIter++)
	{
		const Brush &brush = brushIter->second;
		if (brush.shape() != m_shape)
		{
			continue;
		}
		int brushId = brushIter->first;
		const std::string modelName = std::string(brush.model().asChar());
		float width = brush.width();

		BrushModelKey modelKey(modelName, width);
		std::pair<std::map<BrushModelKey, BrushModel>::iterator, bool> brushModelInsertResult;
		brushModelInsertResult = m_brushModels.insert(std::make_pair(modelKey, BrushModel()));

		// .first is  the iterator. Then .first->second is the BrushModel.
		brushModelInsertResult.first->second.addBrush(brushId, brush);
	}
}
BrushRack::~BrushRack()
{
}

int BrushRack::getBrushId(float strokeWidth, int strokePaintId)
{

	std::map<BrushModelKey, BrushModel>::iterator resultIter = m_brushModels.end();
	std::map<BrushModelKey, BrushModel>::iterator modeliter = m_brushModels.begin();
	// Select the first brushMoedel that is big enough to paint the stroke
	// If the stroke width is too large, use the last brushModel (the biggest).
	for (; modeliter != m_brushModels.end(); modeliter++)
	{
		float brushWidth = modeliter->first.width;
		resultIter = modeliter;
		if (brushWidth >= strokeWidth)
		{
			// This brush can cover it.
			break;
		}
	}

	if (resultIter == m_brushModels.end())
	{
		// If there are no brushes (unlikely).
		return -1;
	}

	// If the paintId changed, then we need to select a new brush.
	BrushModel &model = resultIter->second;
	const std::pair<int, Brush> brushPair = model.selectBrush(strokePaintId);
	return brushPair.first;
}

std::map<BrushModelKey, BrushModel>::const_iterator BrushRack::find(const BrushModelKey &rhs) const
{
	return m_brushModels.find(rhs);
}

std::map<BrushModelKey, BrushModel>::const_iterator BrushRack::begin() const
{
	return m_brushModels.begin();
}

std::map<BrushModelKey, BrushModel>::const_iterator BrushRack::end() const
{
	return m_brushModels.end();
}

const std::string BrushRack::shapeName() const
{
	return m_shape == Brush::kFlat ? "Flat" : "Round";
}

ostream &operator<<(ostream &os, const BrushRack &rack)
{
	std::map<BrushModelKey, BrushModel>::const_iterator it;
	os << rack.shapeName() << endl;
	for (it = rack.begin(); it != rack.end(); it++)
	{
		os << "[" << it->first << " " << it->second << "]" << endl;
	}
	return os;
}
