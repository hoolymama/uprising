#ifndef _strokeCurveData
#define _strokeCurveData
#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
// #include "strokeCurveGeom.h"
class strokeCurveData : public MPxData
{
public:

    strokeCurveData();
    virtual ~strokeCurveData();
    virtual void            copy ( const MPxData &other );
    virtual MTypeId         typeId() const;
    virtual MString         name() const;

    static void *creator();
public:

    static const MString typeName;
    static const MTypeId id;

    // strokeCurveGeom *fGeometry;
    std::vector<strokeGeom> *fGeometry;
};
#endif



/*
    brush:
    TCP matrix:
    width:
    shape: round or flat
*/