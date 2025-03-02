/**
 * @file brushRack.h
 * @brief Defines classes for organizing brushes by model and width.
 *
 * This file implements the brush organization system:
 * - BrushModelKey for identifying brush models by name and width
 * - BrushRack for managing collections of brush models of the same shape
 * - Provides brush selection based on width and paint color
 * - Supports brush lookup by various criteria
 */

#ifndef _BrushRack_
#define _BrushRack_

#include <string>
#include <set>
#include <map>

#include "brush.h"
#include "brushModel.h"

/** @brief Tolerance for comparing brush widths (in cm) */
const float tolerance = 0.0001;

/**
 * @class BrushModelKey
 * @brief Key for identifying brush models by name and width.
 *
 * BrushModelKey provides a unique identifier for brush models:
 * - Combines model name and width for identification
 * - Implements comparison operators for sorting and lookup
 * - Handles width comparison with tolerance
 */
class BrushModelKey
{
public:
    /**
     * @brief Default constructor.
     */
    BrushModelKey() : name("none"), width(0) {}

    /**
     * @brief Parameterized constructor.
     * @param n Model name
     * @param w Model width
     */
    BrushModelKey(const std::string &n, float w) : name(n), width(w) {}

    /**
     * @brief Destructor.
     */
    ~BrushModelKey() {}

    /**
     * @brief Less-than comparison operator.
     * 
     * Compares first by width (with tolerance), then by name.
     * 
     * @param other Other key to compare with
     * @return True if this key is less than the other
     */
    bool operator<(const BrushModelKey &other) const
    {
        bool different = fabs(width - other.width) > tolerance;

        if (different)
        {
            if (width < other.width)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        /// It doesn't matter which way around the name is sorted, just so long as we have a way to
        /// resolve different models with identical widths, which is very unlikely to happen.
        if (name < other.name)
            return true;
        return false;
    }

    /**
     * @brief Stream output operator for BrushModelKey.
     * @param os Output stream
     * @param key BrushModelKey to output
     * @return Reference to output stream
     */
    friend ostream &operator<<(ostream &os, const BrushModelKey &key)
    {
        os << "<model:" << key.name << ", width:" << key.width << ">";
        return os;
    }

    std::string name; ///< Model name
    float width;      ///< Model width
};

/**
 * @class BrushRack
 * @brief Collection of brush models of the same shape.
 *
 * BrushRack organizes brush models of the same shape:
 * - Groups models by BrushModelKey (name and width)
 * - Provides brush selection based on width and paint color
 * - Supports brush lookup by various criteria
 * - Manages brush rotation to prevent color contamination
 */
class BrushRack
{
public:
    /**
     * @brief Constructor that builds a rack from brushes of a specific shape.
     * @param brushes Map of brushes to include
     * @param shape Shape filter for brushes to include
     */
    BrushRack(const std::map<int, Brush> &brushes, Brush::Shape shape);

    /**
     * @brief Destructor.
     */
    ~BrushRack();

    /**
     * @brief Gets iterator to the beginning of the brush models.
     * @return Const iterator to the beginning
     */
    std::map<BrushModelKey, BrushModel>::const_iterator begin() const;

    /**
     * @brief Gets iterator to the end of the brush models.
     * @return Const iterator to the end
     */
    std::map<BrushModelKey, BrushModel>::const_iterator end() const;

    /**
     * @brief Finds a brush model by key.
     * @param rhs Key to search for
     * @return Const iterator to the found model or end()
     */
    std::map<BrushModelKey, BrushModel>::const_iterator find(const BrushModelKey &rhs) const;

    /**
     * @brief Stream output operator for BrushRack.
     * @param os Output stream
     * @param rack BrushRack to output
     * @return Reference to output stream
     */
    friend ostream &operator<<(ostream &os, const BrushRack &rack);

    /**
     * @brief Finds a brush ID based on stroke width and paint ID.
     * @param strokeWidth Width of the stroke
     * @param paintId ID of the paint color
     * @param st Optional status output
     * @return ID of the found brush or -1 if not found
     */
    int findBrushId(float strokeWidth, int paintId, MStatus *st=0);

    /**
     * @brief Finds a model ID based on stroke width.
     * @param strokeWidth Width of the stroke
     * @param st Optional status output
     * @return ID of the found model or -1 if not found
     */
    int findModelId(float strokeWidth, MStatus *st=0);

    /**
     * @brief Finds a brush model based on stroke width.
     * @param strokeWidth Width of the stroke
     * @return Iterator to the found model or end()
     */
    std::map<BrushModelKey, BrushModel>::iterator findModel(float strokeWidth);

    /**
     * @brief Gets the name of the brush shape.
     * @return String representation of the brush shape
     */
    const std::string shapeName() const;

    /**
     * @brief Gets all brush models in the rack.
     * @return Map of brush models keyed by BrushModelKey
     */
    const std::map<BrushModelKey, BrushModel> &models();

private:
    std::map<BrushModelKey, BrushModel> m_brushModels; ///< Collection of brush models
    Brush::Shape m_shape;                              ///< Shape of brushes in this rack

    int m_lastPaintId; ///< ID of the last paint color used
};
#endif
