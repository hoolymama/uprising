

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MFloatVector.h>

class indexShader : public MPxNode
{
public:
	indexShader();
	virtual           ~indexShader();

	virtual MStatus   compute( const MPlug &, MDataBlock & );
	virtual void      postConstructor();

	static  void     *creator();
	static  MStatus   initialize();
	static  MTypeId   id;

private:

	enum ColorSpace { kNone, kRGB, kHCL };


	static MObject aColor;
	static MObject aInputs;

	static MObject aUVCoord;
	static MObject aOutColor;
	static MObject aOutAlpha;
	static MObject aFilterSize;
	static MObject aColorSpace;







};
