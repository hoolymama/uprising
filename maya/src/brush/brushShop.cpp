#include <map>
#include "brushShop.h"

BrushShop::BrushShop() : racks()
{
}
BrushShop::~BrushShop()
{
}

void BrushShop::insertRack(const std::pair<Brush::Shape, BrushRack> &rack)
{
    racks.insert(rack);
}

void BrushShop::getBrushes(std::map<int, Brush> &brushes) const
{
    // std::map<int, Brush> brushes;
    std::map<Brush::Shape, BrushRack>::const_iterator rackIter;
    for (rackIter = racks.begin(); rackIter != racks.end(); rackIter++)
    {
        const BrushRack &currentRack = rackIter->second;
        std::map<BrushModelKey, BrushModel>::const_iterator modelIter = currentRack.begin();
        for (; modelIter != currentRack.end(); modelIter++)
        {
            const BrushModel &currentModel = modelIter->second;
            const std::vector<std::pair<int, Brush>> &modelBrushes = currentModel.brushes();
            std::vector<std::pair<int, Brush>>::const_iterator brushIter = modelBrushes.begin();
            for (; brushIter != modelBrushes.end(); brushIter++)
            {
                brushes.insert(*brushIter);
            }
        }
    }
}

int BrushShop::findModelId(float strokeWidth, Brush::Shape shape, MStatus *st)
{
    std::map<Brush::Shape, BrushRack>::iterator rackIter = racks.find(shape);
    if (rackIter == racks.end())
    {
        if (st)
        {
            *st = MS::kFailure;
        }
        return -1;
    }
    BrushRack &rack = rackIter->second;
    return rack.findModelId(strokeWidth);
}

int BrushShop::findBrushId(float strokeWidth, int strokePaintId, Brush::Shape shape, MStatus *st)
{
  std::map<Brush::Shape, BrushRack>::iterator rackIter = racks.find(shape);
    if (rackIter == racks.end())
    {
        if (st)
        {
            *st = MS::kFailure;
        }
        return -1;
    }
    BrushRack &rack = rackIter->second;
    return rack.findBrushId(strokeWidth, strokePaintId, st);
}

ostream &operator<<(ostream &os, const BrushShop &shop)
{
    std::map<Brush::Shape, BrushRack>::const_iterator it;

    for (it = shop.racks.begin(); it != shop.racks.end(); it++)
    {
        os << it->second << endl;
    }
    return os;
}
