
#ifndef _cImgDetail
#define _cImgDetail

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgDetail : public cImgProcess
{
public:

	cImgDetail();

	virtual				~cImgDetail();

	virtual bool    isAbstractClass() const {return false;}

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                        CImg<unsigned char> &result);

	static MObject aExpandBoundary;
	static MObject aBlurGradient;
	static MObject aLowThreshold;
	static MObject aHighThreshold;

	static MObject aMode;

	static MObject aMedian;





private:


	void findEdgesHysteresis(
	  const CImg<unsigned char> &img,
	  float gradientBlur ,
	  float lowThreshold ,
	  float highThreshold,
	  CImg<unsigned char> &result);


	void findEdgesGradient(
	  const CImg<unsigned char> &img,
	  float gradientBlur ,
	  CImg<unsigned char> &result);

	void detailMagnitude(
	  const CImg<unsigned char> &img,
	  float gradientBlur ,
	  float lowThreshold ,
	  float highThreshold,
	  CImg<unsigned char> &result);

	enum Mode { kGradient, kHysteresis, kDetailMagnitude};

};

#endif
