
#ifndef _cImgNode
#define _cImgNode

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


// #include <maya/MImage.h>

#include "cImgData.h"

class cImgNode : public MPxNode
{
public:

	cImgNode();

	virtual				~cImgNode();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static  MStatus		initialize();
	static MObject aImageFilename;
	// static MObject aSampleResolution;
	static  MObject		aOutput;



	static  MObject		aOutPoints;
	static  MObject		aOutDirections;
	static  MObject		aNormalizeOutput;



	static MTypeId	id;


private:


	// enum MapUpdate { kSeedFrame, kAllFrames };
	// // enum SlopeRotation { kRotNone, kRot90, kRot180, kRot270,};

	// int cellIndex(int x, int y, int xoff, int yoff, int resolution) const;

	// MStatus sampleTexture(const MObject &attribute,
	//                       int resolution, MFloatArray &result) const;

	// MStatus  sampleTexture(const MObject &attribute,
	//                        int resolution, MFloatVectorArray &result) const;

	// MStatus getTextureName(const MObject &attribute,
	//                        MString &name) const;

	// MStatus setKernelImage(MDataBlock &data, int sampleResolution ) ;
	// MStatus setFeedKillImage(MDataBlock &data,  int sampleResolution ) ;
	// MStatus setDiffImage(MDataBlock &data, int sampleResolution ) ;
	// MStatus updateMaps(MDataBlock &data) ;


	// MTime m_lastTimeIEvaluated;
	cImgData *m_data;

};



// inline int cImgNode::cellIndex(int x, int y, int xoff, int yoff,
//                                     int resolution) const {
// 	unsigned col = ((x + xoff + resolution) % resolution);
// 	unsigned row = ((y + yoff + resolution) % resolution);
// 	return int((row * resolution) + col);
// }



#endif
