/**
 * A brushRack holds brush models.
 * A brushModel holds several brushes of the same width and shape.
 * A stroke that uses a particular brush model can use any of the brushes in the brush model.
 *
 */

#ifndef _BrushRack_
#define _BrushRack_

#include <string>
#include <set>
#include <map>

#include "brush.h"
#include "brushModel.h"

const float epsilon = 0.0001; // cm.

class BrushModelKey
{
public:
    BrushModelKey() : name("none"), width(0) {}

    BrushModelKey(const std::string &n, float w) : name(n), width(w) {}

    ~BrushModelKey() {}

    bool operator<(const BrushModelKey &other) const
    {

        bool different = fabs(width - other.width) > epsilon;

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

    friend ostream &operator<<(ostream &os, const BrushModelKey &key)
    {
        os << "<model:" << key.name << ", width:" << key.width << ">";
        return os;
    }

    std::string name;
    float width;
};

class BrushRack
{
public:
    /// Build a BrushRack from brushes of the given shape. Ignore other shapes.
    BrushRack(const std::map<int, Brush> &brushes, Brush::Shape shape);

    ~BrushRack();

    // Iterators are const so that no other object messes with the contents.
    std::map<BrushModelKey, BrushModel>::const_iterator begin() const;

    std::map<BrushModelKey, BrushModel>::const_iterator end() const;

    std::map<BrushModelKey, BrushModel>::const_iterator find(const BrushModelKey &rhs) const;

    friend ostream &operator<<(ostream &os, const BrushRack &rack);

    int getBrushId(float width, int paintId);

    const std::string shapeName() const;

private:
    std::map<BrushModelKey, BrushModel> m_brushModels;
    Brush::Shape m_shape;

    // BrushModelKey m_lastBrushModelKey;
    int m_lastPaintId;
};
#endif
