/**
 * @file brushShop.h
 * @brief Defines the BrushShop class for managing collections of brushes.
 *
 * This file implements the top-level brush management system:
 * - Organizes brushes into racks by shape
 * - Provides brush selection based on width, shape, and paint color
 * - Manages the complete collection of available brushes
 * - Supports brush lookup by various criteria
 */

#ifndef _BrushShop_
#define _BrushShop_

#include <map>

#include "brush.h"
#include "brushRack.h"

/**
 * @class BrushShop
 * @brief Top-level container for organizing brushes by shape.
 *
 * BrushShop manages the complete collection of available brushes:
 * - Organizes brushes into racks by shape (flat, round)
 * - Provides brush selection based on width, shape, and paint color
 * - Supports brush lookup by various criteria
 * - Acts as the main interface for brush management
 */
class BrushShop
{
public:
    /**
     * @brief Default constructor.
     */
    BrushShop();

    /**
     * @brief Destructor.
     */
    ~BrushShop();

    /**
     * @brief Collection of brush racks organized by shape.
     */
    std::map<Brush::Shape, BrushRack> racks;

    /**
     * @brief Inserts a brush rack into the shop.
     * @param rack Pair containing shape and rack to insert
     */
    void insertRack(const std::pair<Brush::Shape, BrushRack> &rack);

    /**
     * @brief Stream output operator for BrushShop.
     * @param os Output stream
     * @param shop BrushShop to output
     * @return Reference to output stream
     */
    friend ostream &operator<<(ostream &os, const BrushShop &shop);
 
    /**
     * @brief Gets all brushes in the shop.
     * @param brushes Output map to store brushes
     */
    void getBrushes(std::map<int, Brush> & brushes) const;

    /**
     * @brief Finds a model ID based on stroke width and shape.
     * @param strokeWidth Width of the stroke
     * @param shape Shape of the brush
     * @param st Optional status output
     * @return ID of the found model or -1 if not found
     */
    int findModelId(float strokeWidth, Brush::Shape shape, MStatus *st=0);

    /**
     * @brief Finds a brush ID based on stroke width, paint ID, and shape.
     * @param strokeWidth Width of the stroke
     * @param strokePaintId ID of the paint color
     * @param shape Shape of the brush
     * @param st Optional status output
     * @return ID of the found brush or -1 if not found
     */
    int findBrushId(float strokeWidth, int strokePaintId, Brush::Shape shape, MStatus *st=0);
};
#endif
