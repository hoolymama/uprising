/**
 * @file cluster.h
 * @brief Defines the Cluster class for managing collections of strokes.
 * 
 * This file contains the Cluster class which provides functionality for managing
 * collections of strokes that can be painted with a loaded brush. It supports
 * tracking of paint usage, brush associations, and stroke organization.
 */
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
 * The Cluster class manages a collection of strokes that share the same brush and paint.
 * It tracks paint usage through travel distance, maintains brush and paint associations,
 * and provides organization for stroke collections. Clusters can be created for different
 * reasons such as attaching a brush or dipping in paint.
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


	/**
	 * @enum Reason
	 * @brief Enumeration for the reason a Cluster was created.
	 * 
	 * Defines different reasons why a Cluster might be created, such as
	 * attaching a brush or dipping in paint.
	 */
	enum Reason
	{
		kNone,   /**< No specific reason. */
		kBrush,  /**< Cluster created to attach a brush. */
		kPaint,  /**< Cluster created to dip in paint. */
		kTcp     /**< Cluster created for tool combination purposes. */
	};

/**
 * @brief Construct a new Cluster object
 * 
 * @param brushId ID of the brush to attach.
 * @param paintId ID of the paint to attach.
 * @param potId ID of the paint pot to use.
 * @param travelCutoff When to stop adding strokes due to running out of paint.
 * @param reason Why this Cluster started? To dip in paint, or to attach a brush.
 */
	Cluster(
		short brushId,
		short paintId,
		short potId,
		float travelCutoff,
		Cluster::Reason reason);

	/**
	 * @brief Destructor.
	 */
	~Cluster();

	/**
	 * @brief Gets the paint ID associated with this cluster.
	 * @return The paint ID.
	 */
	short paintId() const;

	/**
	 * @brief Gets the pot ID associated with this cluster.
	 * @return The pot ID.
	 */
	short potId() const;

	/**
	 * @brief Gets the brush ID associated with this cluster.
	 * @return The brush ID.
	 */
	short brushId() const;

	/**
	 * @brief Checks if the cluster has run out of paint.
	 * @return True if the cluster has run out of paint, false otherwise.
	 */
	bool ranOutOfPaint() const;

	/**
	 * @brief Gets the travel cutoff value for this cluster.
	 * @return The travel cutoff value.
	 */
	float travelCutoff() const;

	/**
	 * @brief Gets the reason this cluster was created.
	 * @return The reason for cluster creation.
	 */
	Cluster::Reason reason() const;

	/**
	 * @brief Gets the total travel distance for this cluster.
	 * @return The total travel distance.
	 */
	float travel() const;

	/**
	 * @brief Gets the strokes in this cluster.
	 * @return Constant reference to the vector of strokes.
	 */
	const std::vector<Stroke> &strokes() const;

	/**
	 * @brief Add a stroke to this Cluster
	 * 
	 * @param stroke The stroke to add.
	 * @param parentIndex Index of the parent stroke, or -1 if none.
	 */
	void pushStroke(const Stroke &stroke, int parentIndex);

private:

	std::vector<Stroke> m_strokes;    /**< Collection of strokes in this cluster. */
	Reason m_reason;                  /**< Reason this cluster was created. */
	MString m_name;                   /**< Name of this cluster. */
	short m_paintId;                  /**< ID of the paint used by this cluster. */
	short m_potId;                    /**< ID of the paint pot used by this cluster. */
	short m_brushId;                  /**< ID of the brush used by this cluster. */
	float m_travelCutoff;             /**< Distance at which paint runs out. */
	float m_travel;                   /**< Current travel distance. */
};

#endif