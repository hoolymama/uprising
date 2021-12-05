
#ifndef _cImgLayout
#define _cImgLayout

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

#include "cImgData.h"

class cImgLayout : public MPxNode
{
public:
    cImgLayout();

    virtual ~cImgLayout();

    virtual MStatus compute(const MPlug &plug, MDataBlock &data);
    // virtual bool    isAbstractClass() const {return true;}

    // virtual MStatus process(MDataBlock &data, const CImgList<unsigned char> &images,
    //                         CImg<unsigned char> &result);

    static void *creator();

    static MStatus initialize();

    static MTypeId id;

protected:
    static MObject aResolution;
    static MObject aMaskPadding;
    static MObject aInvertMask;
    static MObject aDarkenByMask;
    static MObject aIsolate;

    static MObject aInputImage;
    static MObject aOffsetX;
    static MObject aOffsetY;
    static MObject aOffset;
    static MObject aActive;

    static MObject aCropCornerX;
    static MObject aCropCornerY;
    static MObject aCropCorner;

    static MObject aCropResolutionX;
    static MObject aCropResolutionY;
    static MObject aCropResolution;

    static MObject aInput;

    static MObject aOutput;
    static MObject aOutputMask;
};

#endif
