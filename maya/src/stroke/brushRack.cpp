
#include "brushRack.h"

BrushRack::BrushRack(const std::map<int, Brush> &brushes)
{
	// JPMDBG;
	// For every brush of input brushes.
	std::map<int, Brush>::const_iterator brushIter = brushes.begin();

	for (; brushIter != brushes.end(); brushIter++)
	{
		int brushId = brushIter->first;
		const Brush& brush = brushIter->second;
	
		const std::string modelName = std::string(brush.model().asChar());

		std::map<std::string,BrushModel>::iterator brushModelIter = m_brushModels.find(modelName);
		if (brushModelIter == m_brushModels.end())
		{
			BrushModel brushModel;
			brushModel.addBrush(brushId, brush);
			m_brushModels.insert(std::make_pair(modelName, brushModel));
		}
		else
		{
			brushModelIter->second.addBrush(brushId, brush);
		}
		
		// brushModel.addBrush(brushId, brrush);
		
		// = m_brushModels[modelName];
		// model = std::

		// model.addBrush(brushId, brush);

	}
}
BrushRack::~BrushRack()
{
}

std::map<std::string,BrushModel>::const_iterator  BrushRack::find(const std::string &key) const
{
	return m_brushModels.find(key) ;
} 

std::map<std::string,BrushModel>::const_iterator BrushRack::begin() const
{
	return m_brushModels.begin();
}

std::map<std::string,BrushModel>::const_iterator BrushRack::end() const
{
	return m_brushModels.end();
}

ostream &operator<<(ostream &os, const BrushRack &rack)
{
	std::map<std::string,BrushModel>::const_iterator modelIter;
	for (modelIter = rack.begin(); modelIter != rack.end(); modelIter++)
	{
		os << modelIter->first << ": " << modelIter->second << endl;
	}
	return os;
}

// {
//     MString brushShape = bm.isFlat() ? "flat" : "round";
//     os << "{name: " << bm.m_name << ", width: " << bm.m_width << ", shape: " << brushShape << ", count: " << bm.m_brushes.size() << "}" << endl;
//     return os;
// }



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
