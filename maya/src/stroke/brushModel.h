#ifndef _BrushModel_
#define _BrushModel_

#include <vector>
#include "brush.h"

/**	@brief	BrushModel is a collection of Brushes of the same model.
 * 
 * For example davinci30 or WindsorNewtonFlat100. Brushes of the same model have the same width.
 *
 * When a stroke wants to find a brush to use, it queries the brushRack to find the model with the
 * appropriate width. The brushRack calls selectBrush to find the brush. If the paint color for the
 * stroke in question is different than the last access, then we select the nexty brush, so that the
 * brush doesn't get contaminated.
 */


class BrushModel
{
public:
	BrushModel();

	~BrushModel();

	void addBrush(int brushId, const Brush &brush);

	// Note - this function is NOT const, since it can move the peg and update the lastPaintId
	const std::pair<int, Brush> selectBrush(int paintId);

	bool operator<(const BrushModel &other) const;

	friend ostream &operator<<(ostream &os, const BrushModel &bm);

private:
	std::vector<std::pair<int, Brush>> m_brushes;

	int m_peg;
	int m_lastPaintId;
	int m_count;
};

#endif
