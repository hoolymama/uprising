#ifndef cImgShade_H
#define cImgShade_H


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

	static MObject aInput;
	static MObject aSampleInterpolation;
	static MObject aUVCoord;
	static MObject aFilterSize;
	static MObject aDefaultColor 	;
	static MObject aOutColor;
	static MObject aOutAlpha;


};


#endif