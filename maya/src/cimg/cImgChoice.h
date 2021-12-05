
#ifndef _cImgChoice
#define _cImgChoice

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

#include "cImgData.h"

class cImgChoice : public MPxNode
{
public:
    cImgChoice();

    virtual ~cImgChoice();

    virtual MStatus compute(const MPlug &plug, MDataBlock &data);

    static void *creator();

    static MStatus initialize();

    static MTypeId id;

protected:
    static MObject aSelector;
    static MObject aInput;
    static MObject aOutput;
};

#endif
