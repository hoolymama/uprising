#ifndef _Cluster
#define _Cluster

#include <vector>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>
#include "paint.h"
#include "brush.h"

#include "stroke.h"


/**
 * @brief A Cluster is a collection of strokes that can be painted with a loaded brush.
 * 
 */
class Cluster
{
public:

	using stroke_iterator = typename std::vector<Stroke>::iterator;
	using const_stroke_iterator = typename std::vector<Stroke>::const_iterator;

	stroke_iterator strokes_begin() { return m_strokes.begin(); }
	stroke_iterator strokes_end() { return m_strokes.end(); }
	const_stroke_iterator strokes_begin() const { return m_strokes.begin(); }
	const_stroke_iterator strokes_end() const { return m_strokes.end(); }

 

	enum Reason
	{
		kNone,
		kBrush,
		kPaint,
		kTcp
	};

/**
 * @brief Construct a new Cluster object
 * 
 * @param brushId ID of the brush to attach.
 * @param paintId ID of the paint to attach.
 * @param travelCutoff When to stop adding strokes due to running out of paint.
 * @param reason Why this Cluster started? To dip in paint, or to attach a brush.
 */
	Cluster(
		short brushId,
		short paintId,
		float travelCutoff,
		Cluster::Reason reason);

	~Cluster();

	short paintId() const;

	short brushId() const;

	bool ranOutOfPaint() const;

	float travelCutoff() const;

	Cluster::Reason reason() const;

	float travel() const;

	const std::vector<Stroke> &strokes() const;

	/**
	 * @brief add a stroke to this Cluster
	 * 
	 * @param stroke
	 * @param parentIndex 
	 */
	void pushStroke(const Stroke &stroke, int parentIndex);

	// void setApproaches(float approachStart, float approachMid,
	// 				   float approachEnd, float ptpThresh);


private:
	// void setDeparture(float approachMid, float approachEnd);
	// void setArrival(float approachStart, float approachMid, float ptpThresh);

	std::vector<Stroke> m_strokes;
	Reason m_reason;
	MString m_name;
	short m_paintId;
	short m_brushId;
	float m_travelCutoff;
	float m_travel;
};

#endif