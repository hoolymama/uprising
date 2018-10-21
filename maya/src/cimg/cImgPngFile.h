
#ifndef _cImgPngFile
#define _cImgPngFile

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgPngFile : public MPxNode
{
public:

	cImgPngFile();

	virtual				~cImgPngFile();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();


	static MObject aImageFilename;
	static MObject aResize;
	static MObject aResolution;

	static MObject aOutIndexImage;
	static MObject aOutColorImage;
	static MObject aOutPalette;

	static MTypeId	id;


private:

	MStatus createColorImage(const MPlug &plug, MDataBlock &data,
	                         const std::vector<unsigned char> &buffer);

	MStatus createIndexImage(const MPlug &plug, MDataBlock &data,
	                         const std::vector<unsigned char> &buffer);

	MStatus createPaletteImage(const MPlug &plug, MDataBlock &data,
	                           const std::vector<unsigned char> &buffer);


};

#endif
