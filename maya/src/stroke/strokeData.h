#ifndef _strokeData
#define _strokeData
#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "stroke.h"
class strokeData : public MPxData
{
public:

    strokeData();
    virtual ~strokeData();
    virtual void            copy ( const MPxData &other );
    virtual MTypeId         typeId() const;
    virtual MString         name() const;

    static void *creator();
public:

    static const MString typeName;
    static const MTypeId id;


    std::vector<Stroke> *fGeometry;
};
#endif



