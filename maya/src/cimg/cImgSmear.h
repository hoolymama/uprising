
#ifndef _cImgSmear
#define _cImgSmear

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

#include "cImgProcess.h"

class cImgSmear : public cImgProcess
{
public:
    cImgSmear();

    virtual ~cImgSmear();

    virtual bool isAbstractClass() const { return false; }

    virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
                            CImg<unsigned char> &result);

    static void *creator();

    static MStatus initialize();

    static MTypeId id;

    static MObject aSmearImage;
    static MObject aSmearAmount;
    static MObject aDilate;
};

#endif
