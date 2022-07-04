
#ifndef _brushShopNode_H
#define _brushShopNode_H

#include <maya/MPxNode.h>
#include "brush.h"

class brushShopNode : public MPxNode
{
public:
    brushShopNode();
    virtual ~brushShopNode();

    static void *creator();
    static MStatus initialize();
    virtual void postConstructor();
    virtual MStatus compute(const MPlug &plug, MDataBlock &data);

    static MTypeId id;

private:
    MStatus collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const;

    static MObject aBrushes; ///> The list of available brushes.
    static MObject aOutput;
};

#endif