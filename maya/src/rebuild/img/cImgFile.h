
#ifndef _cImgFile
#define _cImgFile

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgFile : public MPxNode
{
public:

	cImgFile();

	virtual				~cImgFile();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();
	static MObject aImageFilename;
	static MObject aResize;
	static MObject aResolution;
	static MObject aInterpolation;



	static MObject aOutput;

	static MTypeId	id;


private:

};

#endif
