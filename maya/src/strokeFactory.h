
#ifndef _strokeFactoryNODE_H
#define _strokeFactoryNODE_H


#include <maya/MPxLocatorNode.h>
#include <maya/MFloatArray.h>
#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MAngle.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "stroke.h"
#include "brush.cpp"
#include "paint.cpp"



class strokeFactory : public MPxLocatorNode
{
public:
	strokeFactory();
	virtual ~strokeFactory();

	static void *creator();
	static MStatus initialize();
	virtual void postConstructor();
	virtual bool            isBounded() const;
	virtual MBoundingBox    boundingBox() const;


	virtual void draw(	M3dView &view,
	                    const MDagPath &path,
	                    M3dView::DisplayStyle style,
	                    M3dView:: DisplayStatus	status	);

	virtual	MStatus	compute(const MPlug &plug, MDataBlock &data);


	static	MTypeId		id;


	static MObject aInMatrix;

	enum Spac { kParametric, kArcLen};

private:
	// int  gridValues(int density, MFloatArray & result) const;

	// MStatus getFractions(
	//   unsigned nPts,
	//   double min,
	//   double max,
	//   MDoubleArray &result,
	//   MFnNurbsCurve::Form form = MFnNurbsCurve::kOpen
	// ) const;


	// MStatus  sampleCurve(
	//   const MObject &curve,
	//   const MDoubleArray &fractions,
	//   const MVector &rotateTangent,
	//   MVectorArray &outPoints,
	//   MVectorArray &outTangents,
	//   Spac spacing
	// ) const  ;


	unsigned int getStrokeBoundaries(
	  const MObject &curve, double &strokeLength,
	  MVectorArray &boundaries
	) const;

	MStatus getStrokes(MDataBlock &data, const   std::map<short, brush> brushes,  std::vector<stroke> &strokes ) const;

	MStatus getBrushes(MDataBlock &data, std::map<short, brush> &brushes ) const;
	MStatus getPaints(MDataBlock &data, std::map<short, paint> &paints ) const ;
	// curves
	static MObject	aCurve;
	static MObject  aPointDensity;
	static MObject	aStrokeLength;
	static MObject	aAttack;
	static MObject	aLift;
	static MObject	aElevation;
	static MObject	aBrushId;
	static MObject	aPaintId;
	static MObject	aCurves;

	//  brushes
	static MObject	aBrushWidth;
	static MObject	aBrushName;
	static MObject	aBrushTcp;
	static MObject	aBrushTip;
	static MObject	aBrushes;


	// paint
	static MObject	aPaintColorR;
	static MObject	aPaintColorG;
	static MObject	aPaintColorB;
	static MObject	aPaintColor;
	static MObject	aPaintName;	
	static MObject	aPaintViscosity;
	static MObject	aPaints;







	// normal of the plane
	static MObject	 aNormal;	
	static MObject	 aSpacing;

	// output
	static MObject  aOutCounts;
	static MObject 	aOutBrushIds;
	static MObject 	aOutPaintIds;
	static MObject  aOutPointsWorld;
	static MObject  aOutPointsLocal;
	static MObject  aOutNormalsWorld;
	static MObject  aOutNormalsLocal;
	static MObject  aOutMesh;

	// display
	static MObject aDisplayPoints;
	static MObject aDisplayNormals;
	static MObject aDisplaySegments;
	// static MObject aDisplayStrokeGap;
	static MObject aNormalLength;
	static MObject aPointSize;
	static MObject aColor;
	static MObject aNormalColor;







};


namespace strokeFactoryCallback
{
static 	MCallbackId	id;

static void makeDefaultConnections(  MObject &node, void *clientData )
{

	MPlug wmPlugmulti( node, strokeFactory::worldMatrix );
	MPlug wm( wmPlugmulti.elementByLogicalIndex( 0 ) );
	MPlug mt( node, strokeFactory::aInMatrix );

	MDGModifier mod;
	mod.connect( wm, mt );
	MStatus stat = mod.doIt();
	if (stat != MS::kSuccess)
	{ stat.perror("strokeFactory ERROR :: callback unable to make matrix connections"); }
}
}


#endif




















