#ifndef _paintingData
#define _paintingData
#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "paintingGeom.h"
class paintingData : public MPxData
{
public:

    paintingData();
    virtual ~paintingData();
    virtual void            copy ( const MPxData &other );
    virtual MTypeId         typeId() const;
    virtual MString         name() const;

    static void *creator();
public:

    static const MString typeName;
    static const MTypeId id;

    paintingGeom *fGeometry;
};
#endif

