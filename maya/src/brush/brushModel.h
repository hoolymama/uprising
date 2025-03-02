/**
 * @file brushModel.h
 * @brief Defines the BrushModel class for managing collections of similar brushes.
 *
 * This file implements the BrushModel class that manages collections of brushes:
 * - Groups brushes of the same model and width
 * - Provides brush selection based on paint color
 * - Manages brush rotation to prevent color contamination
 * - Supports brush reuse strategies
 */

#ifndef _BrushModel_
#define _BrushModel_

#include <vector>
#include "brush.h"

/**	
 * @class BrushModel
 * @brief Collection of Brushes of the same model.
 *
 * BrushModel manages a collection of brushes of the same model (e.g., davinci30 or WindsorNewtonFlat100).
 * Brushes of the same model have the same width. When a stroke wants to find a brush to use, it queries 
 * the brushRack to find the model with the appropriate width. The brushRack calls selectBrush to find 
 * the brush. If the paint color for the stroke is different than the last access, then we select the 
 * next brush, so that the brush doesn't get contaminated.
 */
class BrushModel
{
public:
	/**
	 * @brief Default constructor.
	 */
	BrushModel();

	/**
	 * @brief Destructor.
	 */
	~BrushModel();

	/**
	 * @brief Adds a brush to the model.
	 * @param brushId Unique identifier for the brush
	 * @param brush Brush to add
	 */
	void addBrush(int brushId, const Brush &brush);

	/**
	 * @brief Selects a brush based on paint color.
	 * 
	 * This function is NOT const, since it can move the peg and update the lastPaintId.
	 * It selects the next brush if the paint color has changed to prevent contamination.
	 * 
	 * @param paintId Identifier for the paint color
	 * @return Pair containing the brush ID and the selected brush
	 */
	const std::pair<int, Brush> selectBrush(int paintId);

	/**
	 * @brief Comparison operator for brush models.
	 * @param other Other brush model to compare with
	 * @return True if this model is less than the other
	 */
	bool operator<(const BrushModel &other) const;

	/**
	 * @brief Stream output operator for BrushModel.
	 * @param os Output stream
	 * @param bm BrushModel to output
	 * @return Reference to output stream
	 */
	friend ostream &operator<<(ostream &os, const BrushModel &bm);

	/**
	 * @brief Gets all brushes in the model.
	 * @return Vector of pairs containing brush IDs and brushes
	 */
	const std::vector<std::pair<int, Brush>> &brushes() const;

private:
	std::vector<std::pair<int, Brush>> m_brushes; ///< Collection of brushes in this model

	int m_peg;         ///< Current position in the brush rotation
	int m_lastPaintId; ///< ID of the last paint color used
	int m_count;       ///< Number of brushes in the model
};

#endif
