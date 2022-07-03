
#include "brushRack.h"

BrushRack::BrushRack(const std::map<int, Brush> &brushes):
	m_brushModels(),
	m_lastBrushModelKey(), // Keep tyrack of previous brushModelKey.
	m_lastPaintId(-1) // Keep track of previous paintId.

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


int BrushRack::getBrushId(float strokeMaxWidth, Brush::Shape strokeShapeMask, int strokePaintId) 
{


	std::map<BrushModelKey, BrushModel>::iterator modeliter = m_brushModels.begin();

	// std::map<BrushModelKey, BrushModel>::iterator modeliter = m_brushModels.rbegin();

	// std::map<BrushModelKey, BrushModel>::iterator result= m_brushModels.rend();

	// bool started = false;
	// for (; modeliter != m_brushModels.rend(); modeliter++)
	// {

	// 	Brush::Shape brushShape = modeliter->second.shape;
	// 	if (!(strokeShapeMask == Brusk::kAll || strokeShapeMask==brushShape))
	// 	{
	// 		// This brush model doesn't match the shape mask.
	// 		continue;
	// 	}
	 
	// 	float brushWidth = modeliter->second.width;
    //     if (brushWidth <= strokeMaxWidth){
	// 		if (! started)
	// 		{
	// 			result = modeliter;
	// 		}
	// 		break;
	// 	}
	// 	result = modeliter;
	// 	started = true;
	// }

	// if (result == m_brushModels.rend())
	// {
	// 	return -1;
	// }

	// // Now we have the model.

	return -1;


}


// const std::pair<int, Brush> skeletonStrokeNode::selectBrush(
//     float radius,
//     const std::vector<std::pair<int, Brush> > &brushes) const
// {
//     /*
//     The brushes are already sorted widest to finest. We test each brush in turn to
//     see if it is big enough for the stroke. When we come across the first brush that
//     is too small, we select the previous brush. It will in theory be the best suited.
//     If the first brush (the biggest brush) is too small for the stroke, we're just
//     going to have to use it.

//     If there are no brushes (should never happen!!),
//     then we return a pair with key -1.
//     */
//     if (!brushes.size())
//     {
//         return std::pair<int, Brush>();
//     }
//     std::pair<int, Brush> result;

//     std::vector<std::pair<int, Brush> >::const_iterator brushIter;
//     for (brushIter = brushes.begin(); brushIter != brushes.end(); brushIter++)
//     {
//         float brushRad = brushIter->second.width() * 0.5;
//         if (brushRad <= radius)
//         {
//             if (brushIter == brushes.begin())
//             {
//                 result = *brushIter;
//             }
//             break;
//         }
//         result = *brushIter;
//     }
//     return result;
// }









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
