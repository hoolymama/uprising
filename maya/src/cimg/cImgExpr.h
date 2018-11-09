
#ifndef _cImgExpr
#define _cImgExpr

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgExpr : public MPxNode
{
public:

	cImgExpr();

	virtual				~cImgExpr();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();
	static MObject aExpression;
	static MObject aResolution;

	static MObject aNormalize;
	static MObject aSetRange;



	static MObject aOutput;

	static MTypeId	id;


private:

};

#endif
