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

const float epsilon = 0.0001;

class BrushModelKey
{
public:
    BrushModelKey(const std::string &n, float w, Brush::Shape s) : name(n), width(w), shape(s) {}

    ~BrushModelKey() {}

    bool operator<(const BrushModelKey &other) const
    {
        if (int(other.shape > int(shape)))
            return false;
        if (int(other.shape < int(shape)))
            return true;

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

        if (name < other.name)
            return true;
        return false;
    }

    const std::string shapeName() const
    {
        return shape == Brush::kFlat ? "Flat" : "Round";
    }
    
    friend ostream &operator<<(ostream &os, const BrushModelKey &key)
    {
        os << "<" <<key.name << ":" << key.shapeName() << ":" << key.width << ">";
        return os;
    }

    std::string name;
    float width;
    Brush::Shape shape;
};

class BrushRack
{
public:
    BrushRack(const std::map<int, Brush> &brushes);

    ~BrushRack();

    // Iterators are const so that no other object messes with the contents.
    std::map<BrushModelKey, BrushModel>::const_iterator begin() const;

    std::map<BrushModelKey, BrushModel>::const_iterator end() const;

    std::map<BrushModelKey, BrushModel>::const_iterator find(const BrushModelKey &rhs) const;

    friend ostream &operator<<(ostream &os, const BrushRack &rack);

    int getBrushId(float width, Brush::Shape shapeMask, int paintId) const;

private:

    std::map<BrushModelKey, BrushModel> m_brushModels;
};
#endif
