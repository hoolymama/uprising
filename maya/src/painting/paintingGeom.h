/**
 * @file paintingGeom.h
 * @brief Defines the paintingGeom class for managing painting geometry.
 * 
 * This file contains the paintingGeom class which provides functionality for managing
 * painting geometry, including clusters of strokes, brushes, and paints. It serves as
 * the main data structure for the painting system.
 */
#ifndef _paintingGeom
#define _paintingGeom

#include <vector>
#include <map>

#include "stroke.h"

#include "cluster.h"

/**
 * @class paintingGeom
 * @brief Manages painting geometry including clusters, strokes, brushes, and paints.
 * 
 * The paintingGeom class serves as the main data structure for the painting system,
 * managing collections of clusters, each containing strokes. It also maintains
 * mappings of brushes and paints by ID, and provides functionality for adding strokes,
 * setting approaches, and calculating travel cutoffs.
 */
class paintingGeom
{
public:
	using cluster_iterator = typename std::vector<Cluster>::iterator;
	using const_cluster_iterator = typename std::vector<Cluster>::const_iterator;

	cluster_iterator clusters_begin() { return m_clusters.begin(); }
	cluster_iterator clusters_end() { return m_clusters.end(); }
	const_cluster_iterator clusters_begin() const { return m_clusters.begin(); }
	const_cluster_iterator clusters_end() const { return m_clusters.end(); }

	/**
	 * @brief Default constructor.
	 */
	paintingGeom();

	/**
	 * @brief Destructor.
	 */
	~paintingGeom();

	/**
	 * @brief Gets the clusters in this painting geometry.
	 * @return Constant reference to the vector of clusters.
	 */
	const std::vector<Cluster> &clusters() const;
	
	/**
	 * @brief Gets the paints in this painting geometry.
	 * @return Constant reference to the map of paints.
	 */
	const std::map<int, Paint> &paints() const;
	
	/**
	 * @brief Gets the brushes in this painting geometry.
	 * @return Constant reference to the map of brushes.
	 */
	const std::map<int, Brush> &brushes() const;

	/**
	 * @brief Sets the paints for this painting geometry.
	 * @param palette Map of paints indexed by ID.
	 */
	void setPaints(const std::map<int, Paint> &palette);

	/**
	 * @brief Sets the brushes for this painting geometry.
	 * @param brushes Map of brushes indexed by ID.
	 */
	void setBrushes(const std::map<int, Brush> &brushes);

	/**
	 * @brief Gets a brush by its ID.
	 * @param id The ID of the brush to retrieve.
	 * @return Constant reference to the brush.
	 */
	const Brush &brushFromId(int id) const;
	
	/**
	 * @brief Gets a paint by its ID.
	 * @param id The ID of the paint to retrieve.
	 * @return Constant reference to the paint.
	 */
	const Paint &paintFromId(int id) const;

	/**
	 * @brief Calculates the travel cutoff for a brush and paint combination.
	 * @param b The brush to use.
	 * @param p The paint to use.
	 * @return The travel cutoff value.
	 */
	float travelCutoff(const Brush &b, const Paint &p) const;

	/**
	 * @brief Adds a stroke to the painting geometry.
	 * @param stroke The stroke to add.
	 * @param parentIndex Index of the parent stroke, or -1 if none.
	 */
	void addStroke(const Stroke &stroke, int parentIndex);

	/**
	 * @brief Sets the approach vectors for the painting geometry.
	 * @param approachStart The start approach vector.
	 * @param approachMid The middle approach vector.
	 * @param approachEnd The end approach vector.
	 * @param ptpThresh The point-to-point threshold.
	 */
	void setApproaches(
		const MFloatVector &approachStart, 
		const MFloatVector &approachMid,
		const MFloatVector &approachEnd, 
		float ptpThresh);


	/**
	 * @brief Gets the tool combinations used in this painting geometry.
	 * @param result Output array to store the tool combinations.
	 */
	void toolCombinations(MIntArray &result) const;


private:
	/**
	 * @brief Prepares a cluster for adding strokes.
	 * @param brushId The ID of the brush to use.
	 * @param paintId The ID of the paint to use.
	 * @return Reference to the prepared cluster.
	 */
	Cluster &prepCluster(
		int brushId,
		int paintId);

	std::vector<Cluster> m_clusters;  /**< Collection of clusters in this painting geometry. */
	std::map<int, Paint> m_paints;    /**< Map of paints indexed by ID. */
	std::map<int, Brush> m_brushes;   /**< Map of brushes indexed by ID. */
};

#endif