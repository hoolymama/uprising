#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MIntArray.h>

#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>

#include <maya/MVectorArray.h>


#include <maya/MFnTypedAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnIntArrayData.h>



#include <maya/MFnNumericAttribute.h>

#include <maya/MFnMatrixAttribute.h>


#include "skGraphNode.h"

#include "skChain.h"


#include <jMayaIds.h>
#include <attrUtils.h>


#include "cImgFloatData.h"
#include "cImgUtils.h"


#include "errorMacros.h"


const double  PI  = 3.141592653;


const double rad_to_deg = (180 / PI);

const double  TAU = 2.0 * PI;

static int circleVertexCount = 16;
const float gap = TAU / circleVertexCount;

static float circle[][4] = {
	{sin(gap * 0),   cos(gap * 0),  0.0f, 1.0f},
	{sin(gap * 1),   cos(gap * 1),  0.0f, 1.0f},
	{sin(gap * 2),   cos(gap * 2),  0.0f, 1.0f},
	{sin(gap * 3),   cos(gap * 3),  0.0f, 1.0f},
	{sin(gap * 4),   cos(gap * 4),  0.0f, 1.0f},
	{sin(gap * 5),   cos(gap * 5),  0.0f, 1.0f},
	{sin(gap * 6),   cos(gap * 6),  0.0f, 1.0f},
	{sin(gap * 7),   cos(gap * 7),  0.0f, 1.0f},
	{sin(gap * 8),   cos(gap * 8),  0.0f, 1.0f},
	{sin(gap * 9),   cos(gap * 9),  0.0f, 1.0f},
	{sin(gap * 10),  cos(gap * 10), 0.0f, 1.0f},
	{sin(gap * 11),  cos(gap * 11), 0.0f, 1.0f},
	{sin(gap * 12),  cos(gap * 12), 0.0f, 1.0f},
	{sin(gap * 13),  cos(gap * 13), 0.0f, 1.0f},
	{sin(gap * 14),  cos(gap * 14), 0.0f, 1.0f},
	{sin(gap * 15),  cos(gap * 15), 0.0f, 1.0f}
};

MObject skGraphNode::aMaxRadius;
MObject skGraphNode::aPruneLength;


MObject skGraphNode::aProjectionMatrix;
MObject skGraphNode::aSkeletonImage;
MObject skGraphNode::aMinPixels;
MObject skGraphNode::aPixelStep;

MObject skGraphNode::aOutPoints;
MObject skGraphNode::aOutParams;
MObject skGraphNode::aOutRadius;
MObject skGraphNode::aOutCounts;

MObject skGraphNode::aPointSize;
MObject skGraphNode::aColor1;
MObject skGraphNode::aColor2;


MObject skGraphNode::aRadiusMult;
MObject skGraphNode::aDrawEdges;
MObject skGraphNode::aDrawPoints;
MObject skGraphNode::aDrawCircles;
MObject skGraphNode::aRandomChainColor;



MTypeId skGraphNode:: id(k_skGraphNode );

skGraphNode:: skGraphNode() {}

skGraphNode:: ~skGraphNode() {}

void *skGraphNode::creator() {
	return new skGraphNode();
}

// const double epsilon = 0.0001;

MStatus skGraphNode::initialize()
{
	MStatus st;
	MString method("skGraphNode::initialize");

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	MFnMatrixAttribute mAttr;


	// MFloatMatrix identity;
	// identity.setToIdentity();

	MFloatMatrix identity;
	identity.setToIdentity();
	aProjectionMatrix = mAttr.create( "projectionMatrix", "pmat",
	                                  MFnMatrixAttribute::kFloat );
	mAttr.setStorable( false );
	mAttr.setHidden( false );
	mAttr.setKeyable(true);
	mAttr.setDefault(identity);
	addAttribute(aProjectionMatrix);

	aPruneLength = nAttr.create("pruneLength", "prl", MFnNumericData::kInt);
	nAttr.setDefault( 2 );
	nAttr.setKeyable( true );
	st = addAttribute( aPruneLength ); mser


	aMaxRadius = nAttr.create("maxRadius", "mxr", MFnNumericData::kFloat);
	nAttr.setDefault( 20 );
	nAttr.setKeyable( true );
	st = addAttribute( aMaxRadius ); mser

	aSkeletonImage = tAttr.create("skeletonImage", "simg", cImgFloatData::id ) ;
	tAttr.setStorable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute( aSkeletonImage ); mser;

	aMinPixels = nAttr.create("minPixels", "mnpx", MFnNumericData::kInt);
	nAttr.setDefault( 2 );
	nAttr.setKeyable( true );
	st = addAttribute( aMinPixels ); mser


	aPixelStep = nAttr.create("pixelStep", "pxs", MFnNumericData::kInt);
	nAttr.setDefault( 2 );
	nAttr.setKeyable( true );
	st = addAttribute( aPixelStep ); mser


	aPointSize = nAttr.create("pointSize", "psz", MFnNumericData::kFloat);
	nAttr.setDefault( 2.0f );
	nAttr.setKeyable( true );
	st = addAttribute( aPointSize ); mser

	aRadiusMult = nAttr.create("radiusMult", "radiusMult", MFnNumericData::kFloat);
	nAttr.setDefault( 2.0f );
	nAttr.setKeyable( true );
	st = addAttribute( aRadiusMult ); mser

	aColor1 = nAttr.createColor( "drawColor1", "dc1");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute( aColor1 ); mser


	aColor2 = nAttr.createColor( "drawColor2", "dc2");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute( aColor2 ); mser


	aOutPoints = tAttr.create("outPoints", "opts", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutPoints );


	// aOutEdges = tAttr.create("outEdges", "oed", MFnData::kVectorArray, &st);
	// tAttr.setStorable( false);
	// tAttr.setReadable( true);
	// st = addAttribute( aOutEdges );

	aOutRadius = tAttr.create("outRadius", "orad", MFnData::kDoubleArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutRadius );

	aOutParams = tAttr.create("outParams", "opm", MFnData::kDoubleArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutParams );


	aOutCounts = tAttr.create("outCounts", "ocnt", MFnData::kIntArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutCounts);


	aDrawEdges = nAttr.create( "drawEdges", "ded",
	                           MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(true);
	addAttribute(aDrawEdges);

	aDrawPoints = nAttr.create( "drawPoints", "dpt",
	                            MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(true);
	addAttribute(aDrawPoints);

	aDrawCircles = nAttr.create( "drawCircles", "dci",
	                             MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(true);
	addAttribute(aDrawCircles);

	aRandomChainColor = nAttr.create( "randomChainColor", "rcc",
	                                  MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(true);
	addAttribute(aRandomChainColor);

	attributeAffects( aPruneLength, aOutPoints);
	attributeAffects( aPruneLength, aOutRadius);
	attributeAffects( aPruneLength, aOutCounts);
	attributeAffects( aPruneLength, aOutParams);



	attributeAffects( aMaxRadius, aOutPoints);
	attributeAffects( aMaxRadius, aOutRadius);
	attributeAffects( aMaxRadius, aOutCounts);
	attributeAffects( aMaxRadius, aOutParams);


	attributeAffects( aMinPixels, aOutPoints);
	attributeAffects( aMinPixels, aOutRadius);
	attributeAffects( aMinPixels, aOutCounts);
	attributeAffects( aMinPixels, aOutParams);

	attributeAffects( aPixelStep, aOutPoints);
	attributeAffects( aPixelStep, aOutRadius);
	attributeAffects( aPixelStep, aOutCounts);
	attributeAffects( aPixelStep, aOutParams);

	attributeAffects( aSkeletonImage, aOutPoints);
	attributeAffects( aSkeletonImage, aOutRadius);
	attributeAffects( aSkeletonImage, aOutCounts);
	attributeAffects( aSkeletonImage, aOutParams);


	attributeAffects( aProjectionMatrix, aOutPoints);
	attributeAffects( aProjectionMatrix, aOutRadius);
	attributeAffects( aProjectionMatrix, aOutCounts);
	attributeAffects( aProjectionMatrix, aOutParams);


	return (MS::kSuccess );
}


// CImg<float> *skGraphNode::getImage(MDataBlock &data, MObject &attribute )
// {
// 	MStatus st;
// 	MDataHandle hImageData = data.inputValue(attribute, &st);
// 	if (st.error()) {	return 0;}
// 	MObject dImageData = hImageData.data();
// 	MFnPluginData fnImageData( dImageData , &st);
// 	if (st.error()) {	return 0;}
// 	cImgFloatData *imageData = (cImgFloatData *)fnImageData.data();
// 	return imageData->fImg;
// }


MStatus skGraphNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus st;
	// cerr << plug.name() << endl;
	if (! (
	      plug == aOutCounts
	      ||  plug == aOutPoints
	      ||   plug == aOutRadius
	      ||   plug == aOutParams		 ))
	{
		return (MS::kUnknownParameter );
	}


	MVectorArray resultPoints;
	MDoubleArray resultRadius;
	MDoubleArray resultParams;
	MIntArray resultCounts;


	int minBranchLength =  data.inputValue(aPruneLength).asInt();
	int step =  data.inputValue(aPixelStep).asInt();
	int minPixels =  data.inputValue(aMinPixels).asInt();
	int maxRadius =  data.inputValue(aMaxRadius).asFloat();

	CImg<float>  *pImage = cImgUtils::getFloatImage(data, skGraphNode::aSkeletonImage );


	MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();

	if (! pImage) {
		outputData(skGraphNode::aOutPoints, data, resultPoints );
		outputData(skGraphNode::aOutParams, data, resultParams );
		outputData(skGraphNode::aOutRadius, data, resultRadius );
		outputData(skGraphNode::aOutCounts, data, resultCounts );
		return MS::kSuccess;
	}

	skGraph g(pImage);
	g.prune(minBranchLength);
	g.detachBranches();

	std::vector< skChain > chains;
	g.getChains(projection, chains, step, minPixels);

	std::vector< skChain >::const_iterator iter;
	for (iter = chains.begin(); iter != chains.end();  iter++)
	{
		iter->appendPoints(resultPoints);
		iter->appendRadii(resultRadius);
		iter->appendParams(resultParams);
		resultCounts.append(iter->size());
	}


	// cerr <<  ", " << resultPoints.length() << ", " << resultParams.length() << ", " <<
	//      resultRadius.length() << ", " << resultCounts.length() << endl;
	// JPMDBG;
	outputData(skGraphNode::aOutPoints, data, resultPoints );
	outputData(skGraphNode::aOutParams, data, resultParams );
	outputData(skGraphNode::aOutRadius, data, resultRadius );
	outputData(skGraphNode::aOutCounts, data, resultCounts );

	return MS::kSuccess;
}



void skGraphNode::draw( M3dView &view,
                        const MDagPath &path,
                        M3dView::DisplayStyle style,
                        M3dView:: DisplayStatus status  )
{


	MStatus st;

	MObject thisObj = thisMObject();

	float pointSize;
	MPlug(thisObj, aPointSize).getValue(pointSize);

	float radiusMult;
	MPlug(thisObj, aRadiusMult).getValue(radiusMult);

	bool drawPoints, drawEdges, drawCircles, randomChainColor;

	MPlug(thisObj, aDrawPoints).getValue(drawPoints);
	MPlug(thisObj, aDrawEdges).getValue(drawEdges);
	MPlug(thisObj, aDrawCircles).getValue(drawCircles);
	MPlug(thisObj, aRandomChainColor).getValue(randomChainColor);


	MPlug pointsPlug( thisObj, aOutPoints );
	MObject dPoints;
	st = pointsPlug.getValue(dPoints); mser;
	MFnVectorArrayData fnPoints(dPoints);
	MVectorArray points = fnPoints.array();

	// MPlug edgesPlug( thisObj, aOutEdges );
	// MObject dEdges;
	// st = edgesPlug.getValue(dEdges); mser;
	// MFnVectorArrayData fnEdges(dEdges);
	// MVectorArray edges = fnEdges.array();

	MPlug radiusPlug( thisObj, aOutRadius );
	MObject dRadius;
	st = radiusPlug.getValue(dRadius); mser;
	MFnDoubleArrayData fnRadius(dRadius);
	MDoubleArray radius = fnRadius.array();


	MPlug paramsPlug( thisObj, aOutParams );
	MObject dParams;
	st = paramsPlug.getValue(dParams); mser;
	MFnDoubleArrayData fnParams(dParams);
	MDoubleArray params = fnParams.array();



	MPlug countsPlug( thisObj, aOutCounts );
	MObject dCounts;
	st = countsPlug.getValue(dCounts); mser;
	MFnIntArrayData fnCounts(dCounts);
	MIntArray counts = fnCounts.array();


	MPlug colorPlug1(thisObj, aColor1);

	float color1r , color1g , color1b;
	colorPlug1.child(0).getValue(color1r);
	colorPlug1.child(1).getValue(color1g);
	colorPlug1.child(2).getValue(color1b);

	MPlug colorPlug2(thisObj, aColor2);

	float color2r , color2g , color2b;
	colorPlug2.child(0).getValue(color2r);
	colorPlug2.child(1).getValue(color2g);
	colorPlug2.child(2).getValue(color2b);




	view.beginGL();



	glPushAttrib(GL_CURRENT_BIT);
	glPointSize(float(pointSize));

	srand48(0);

	unsigned nChains = counts.length();
	int pIndex = 0;
	int eIndex = 0;
	int cIndex = 0;




	// cerr << "points.length():" << points.length() << endl;
	// cerr << "radius.length():" << radius.length() << endl;


	// cerr << "counts.length():" << counts.length() << endl;
	float r, g, b;

	for (int c = 0; c < counts.length(); ++c)
	{
		int len = counts[c];
		if (randomChainColor) {
			r = drand48();
			g = drand48();
			b = drand48();
			glColor3f(r, g, b);
		}
		// int elen = len - 1;

		// glColor4f(colorr , colorg , colorb, 1.0f);

		if (drawPoints) {
			glBegin( GL_POINTS );
			for (int i = 0; i < len; ++i)
			{
				if (!randomChainColor) {
					float p = params[pIndex];
					r =  ((color1r * (1.0 - p)) + (color2r * p));
					g =  ((color1g * (1.0 - p)) + (color2g * p));
					b =  ((color1b * (1.0 - p)) + (color2b * p));
					glColor3f(r, g, b);
				}
				MFloatPoint p = MFloatPoint(points[pIndex]);
				glVertex3f( p.x, p.y, p.z);
				pIndex++;
			}
			glEnd();
		}




		if (drawEdges) {
			glPushAttrib(GL_LINE_BIT);
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < len;  i++)
			{
				if (!randomChainColor) {
					float p = params[eIndex];
					r =  ((color1r * (1.0 - p)) + (color2r * p));
					g =  ((color1g * (1.0 - p)) + (color2g * p));
					b =  ((color1b * (1.0 - p)) + (color2b * p));
					glColor3f(r, g, b);
				}
				// int j = i + 1;
				MFloatPoint p = MFloatPoint(points[eIndex]);
				// MFloatPoint end = MFloatPoint(points[pIndex + 1]);
				glVertex3f( p.x, p.y, p.z);
				eIndex++;
				// glVertex3f( end.x, end.y, end.z);

			}
			glEnd();
			glPopAttrib();
		}






		if (drawCircles) {
			// len = points.length();
			MFloatPointArray cScaled(circleVertexCount);
			for (int j = 0; j < circleVertexCount; ++j) {
				cScaled[j] = MPoint(circle[j]) * radiusMult;
			}



			glPushAttrib(GL_LINE_BIT);


			for (int i = 0; i < len; ++i)
			{
				if (!randomChainColor) {
					float p = params[cIndex];
					r =  ((color1r * (1.0 - p)) + (color2r * p));
					g =  ((color1g * (1.0 - p)) + (color2g * p));
					b =  ((color1b * (1.0 - p)) + (color2b * p));
					glColor3f(r, g, b);
				}

				MFloatPointArray c(circleVertexCount);
				for (int j = 0; j < circleVertexCount; ++j) {
					c[j] = ((cScaled[j] * radius[cIndex]) + MFloatPoint(points[cIndex])) /** imat*/;
				}
				glBegin(GL_LINE_STRIP);
				for (int j = 0; j < circleVertexCount; ++j)
				{
					// int next = (j + 1) % circleVertexCount;
					glVertex3f( float(c[j].x) , float(c[j].y) , float(c[j].z) );
					// glVertex3f( float(c[next].x) , float(c[next].y) , float(c[next].z) );
				}
				glVertex3f( float(c[0].x) , float(c[0].y) , float(c[0].z) );
				cIndex++;
				glEnd();
			}

			glPopAttrib();

		}

	}
	glPopAttrib();
}








bool skGraphNode::isBounded() const
{
	return false;
}

MBoundingBox skGraphNode::boundingBox() const
{
	return MBoundingBox();

}

void skGraphNode::postConstructor()
{
	// setExistWithoutInConnections(true);
	// setExistWithoutOutConnections(true);
}
