#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>

#include <maya/MVectorArray.h>


#include <maya/MFnTypedAttribute.h>
#include <maya/MFnVectorArrayData.h>


#include <maya/MFnNumericAttribute.h>

#include "skGraphNode.h"

#include <jMayaIds.h>
#include <attrUtils.h>


#include "cImgData.h"


#include "errorMacros.h"


MObject skGraphNode::aSkeletonImage;
MObject skGraphNode::aOutPoints;
MObject skGraphNode::aOutEdges;
MObject skGraphNode::aPointSize;
MObject skGraphNode::aColor;


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

	// MFnMatrixAttribute mAttr;


	// MFloatMatrix identity;
	// identity.setToIdentity();


	aSkeletonImage = tAttr.create("skeletonImage", "simg", cImgData::id ) ;
	tAttr.setStorable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute( aSkeletonImage ); mser;



	aPointSize = nAttr.create("pointSize", "psz", MFnNumericData::kFloat);
	nAttr.setDefault( 2.0f );
	nAttr.setKeyable( true );
	st = addAttribute( aPointSize ); mser

	aColor = nAttr.createColor( "drawColor", "dc");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute( aColor ); mser

	aOutPoints = tAttr.create("outPoints", "opts", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutPoints );


	aOutEdges = tAttr.create("outEdges", "oed", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutEdges );


	st = attributeAffects( aSkeletonImage, aOutPoints);
	st = attributeAffects( aSkeletonImage, aOutEdges);


	return (MS::kSuccess );
}


CImg<unsigned char> *skGraphNode::getImage(MDataBlock &data, MObject &attribute )
{
	MStatus st;
	MDataHandle hImageData = data.inputValue(attribute, &st);
	if (st.error()) {	return 0;}
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);
	if (st.error()) {	return 0;}
	cImgData *imageData = (cImgData *)fnImageData.data();
	return imageData->fImg;
}


MStatus skGraphNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus st;
	if (! ( plug == aOutPoints || plug == aOutEdges	 ))
	{
		return (MS::kUnknownParameter );
	}


	MVectorArray resultPoints;
	MVectorArray resultEdges;

	CImg<unsigned char>  *pImage = getImage(data, skGraphNode::aSkeletonImage );

	if (! pImage) {
		outputData(skGraphNode::aOutPoints, data, resultPoints );
		outputData(skGraphNode::aOutEdges, data, resultEdges );
		return MS::kSuccess;
	}


	skGraph g(pImage);
	// g.build(pImage);

	// // Create a deque containing integers
	// std::deque<coord> q;


	// std::vector< coord >	kernel;
	// kernel.push_back(coord(0, 1));
	// kernel.push_back(coord(-1, 1));
	// kernel.push_back(coord(1, 1));
	// kernel.push_back(coord(-1, 0));
	// kernel.push_back(coord(1, 0));
	// kernel.push_back(coord(-1, -1));
	// kernel.push_back(coord(1, -1));
	// kernel.push_back(coord(0, -1));



	// CImg<bool> image = pImage->get_norm();

	// int w = image.width();
	// int h = image.height();

	// int nWhitePixels = image.sum();

	// coord start(0, 0, 0);


	// while (nWhitePixels > 0)
	// {
	// 	coord whitePixel = findWhitePixel(start,  image );
	// 	// cerr << "W:" << whitePixel.x << "," << whitePixel.y << endl;
	// 	if (!whitePixel.valid(w, h)) {
	// 		// cerr << "whitePixel invalid -> BREAK" << endl;
	// 		break;
	// 	}
	// 	int x =  whitePixel.x;
	// 	int y =  whitePixel.y;

	// 	// skNode startNode(x, y, 0);

	// 	q.push_back(whitePixel);
	// 	image(x, y) = false;
	// 	nWhitePixels--;

	// 	g.addNode(x, y);
	// 	while (! q.empty())
	// 	{
	// 		coord curr = q.front();

	// 		std::vector< coord >::const_iterator iter;
	// 		for (iter = kernel.begin(); iter != kernel.end(); iter++ )
	// 		{
	// 			coord neighbor = curr.offset(*iter);
	// 			if (! neighbor.valid(w, h))
	// 			{
	// 				continue;
	// 			}
	// 			if (image(neighbor.x, neighbor.y))
	// 			{
	// 				g.addNode(neighbor.x, neighbor.y);
	// 				g.connect(neighbor, curr);
	// 				q.push_back(neighbor);
	// 				image(neighbor.x, neighbor.y) = false;
	// 				nWhitePixels--;
	// 			}
	// 		}
	// 		q.pop_front();
	// 	}
	// 	start = whitePixel;
	// 	// cerr << "setting start to whitePixel: " << start.x << "," << start.y << endl;
	// }

	// g.resolveBranches();


	g.getEdges(resultEdges);
	g.getPoints(resultPoints);


	outputData(skGraphNode::aOutPoints, data, resultPoints );
	outputData(skGraphNode::aOutEdges, data, resultEdges );
	return MS::kSuccess;
}



void skGraphNode::draw( M3dView &view,
                        const MDagPath &path,
                        M3dView::DisplayStyle style,
                        M3dView:: DisplayStatus status  )
{


	MStatus st;

	MObject thisObj = thisMObject();

	double pointSize;
	MPlug(thisObj, aPointSize).getValue(pointSize);

	MPlug pointsPlug( thisObj, aOutPoints );
	MObject dPoints;
	st = pointsPlug.getValue(dPoints); mser;
	MFnVectorArrayData fnPoints(dPoints);
	MVectorArray points = fnPoints.array();




	MPlug edgesPlug( thisObj, aOutEdges );
	MObject dEdges;
	st = edgesPlug.getValue(dEdges); mser;
	MFnVectorArrayData fnEdges(dEdges);
	MVectorArray edges = fnEdges.array();


	MPlug colorPlug(thisObj, aColor);


	float colorr , colorg , colorb;
	colorPlug.child(0).getValue(colorr);
	colorPlug.child(1).getValue(colorg);
	colorPlug.child(2).getValue(colorb);



	view.beginGL();

	unsigned len = points.length();



	glPushAttrib(GL_CURRENT_BIT);
	glColor4f(colorr , colorg , colorb, 1.0f);
	glPointSize(float(pointSize));
	glBegin( GL_POINTS );
	for (int i = 0; i < len; ++i)
	{
		MFloatPoint p = MFloatPoint(points[i]);
		glVertex3f( p.x, p.y, p.z);
	}
	glEnd();


	len = edges.length();

	glBegin(GL_LINES);
	for (int i = 0; i < len;  i += 2)
	{
		int j = i + 1;
		MFloatPoint start = MFloatPoint(edges[i]);
		MFloatPoint end = MFloatPoint(edges[j]);
		glVertex3f( start.x, start.y, start.z);
		glVertex3f( end.x, end.y, end.z);

	}
	glEnd();
	glPopAttrib();
	view.endGL();

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
