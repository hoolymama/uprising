
#ifndef _cImgSkel
#define _cImgSkel

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


class cImgSkel : public MPxNode
{
public:

	cImgSkel();

	virtual				~cImgSkel();

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );




private:


	static MObject aInput;

	static MObject aThreshold;
	static MObject aSoftThreshold;
	static MObject aStrictThreshold;

	static MObject aMedian;
	static MObject aInvert;
	static MObject aSkeletonThreshold;
	static MObject aCreateCurve;
	static MObject aCorrection;
	static MObject aDiscreteStep;



	static MObject aOutSkeleton;
	static MObject aOutBinaryImage;
	// static MObject aOutDistanceImage;
	// static MObject aOutFluxImage;


	MStatus process(
	  MDataBlock &data,
	  const CImg<unsigned char> &inimage,
	  CImg<float> &outSkeleton,
	  CImg<unsigned char> &outBinary
	  // ,
	  // CImg<unsigned char> &outDistance,
	  // CImg<unsigned char> &outFlux
	);


};

#endif
