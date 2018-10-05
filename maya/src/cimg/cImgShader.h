
#ifndef _cImgShader
#define _cImgShader

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MFloatVector.h>

class cImgShader : public MPxNode
{
public:
	cImgShader();
	virtual           ~cImgShader();

	virtual MStatus   compute( const MPlug &, MDataBlock & );
	virtual void      postConstructor();

	static  void     *creator();
	static  MStatus   initialize();
	static  MTypeId   id;

private:


	enum Interpolation { kNearest, kBilinear, kBicubic };



	static MObject aImageData;
	static MObject aInterpolation;
	static MObject aUVCoord;
	static MObject aDefaultColor 	;
	static MObject aOutColor;
	static MObject aOutAlpha;


};


#endif