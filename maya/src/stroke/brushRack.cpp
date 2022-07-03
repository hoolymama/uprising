
#include "brushRack.h"

BrushRack::BrushRack(const std::map<int, Brush> &brushes)
{

	// For every brush of input brushes.
	std::map<int, Brush>::const_iterator brushIter = brushes.begin();

	for (; brushIter != brushes.end(); brushIter++)
	{
		int brushId = brushIter->first;
		const Brush &brush = brushIter->second;

		const std::string modelName = std::string(brush.model().asChar());
		float width = brush.width();
		Brush::Shape shape = brush.shape();

		BrushModelKey modelKey(modelName, width, shape);
		std::pair<std::map<BrushModelKey, BrushModel>::iterator, bool> brushModelInsertResult;
		brushModelInsertResult = m_brushModels.insert(std::make_pair(modelKey, BrushModel()));

		// .first is  the iterator. Then .first->second is the BrushModel.
		brushModelInsertResult.first->second.addBrush(brushId, brush);
	}
}
BrushRack::~BrushRack()
{
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

ostream &operator<<(ostream &os, const BrushRack &rack)
{
	std::map<BrushModelKey, BrushModel>::const_iterator it;
	for (it = rack.begin(); it != rack.end(); it++)
	{
		os << it->first << " : " << it->second;
	}
	return os;
}
