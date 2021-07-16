

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>

#include <maya/MFnMatrixData.h>

#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnVectorArrayData.h>



#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>


#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>


#include <maya/MFnMatrixAttribute.h>

#include "imagePoints.h"

#include <jMayaIds.h>
#include <attrUtils.h>

#include "mayaMath.h"

#include "dotData.h"
#include "dotTree.h"
#include "cImgData.h"
#include "cImgUtils.h"


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


MObject imagePoints::aDensityImage;
MObject imagePoints::aRadiusImage;
MObject imagePoints::aMask;

MObject imagePoints::aDensityRamp;
MObject imagePoints::aDensityRangeMin;
MObject imagePoints::aDensityRangeMax;
MObject imagePoints::aDensityRange;
MObject imagePoints::aRadiusRamp;
MObject imagePoints::aRadiusRangeMin;
MObject imagePoints::aRadiusRangeMax;
MObject imagePoints::aRadiusRange;
MObject imagePoints::aIterations;
MObject imagePoints::aNeighbors;
MObject imagePoints::aMagnitude;
// MObject imagePoints::aProjectionMatrix;
MObject imagePoints::aSeed;
MObject imagePoints::aOutPoints;
MObject imagePoints::aOutRadius;
MObject imagePoints::aOutU;
MObject imagePoints::aOutV;



MObject imagePoints::aLineThickness;
MObject imagePoints::aPointSize;
MObject imagePoints::aColor;
MObject imagePoints::aDisplayPoints;
MObject imagePoints::aDisplayCircles;
MObject imagePoints::aCircleDisplaySize;

  
MTypeId imagePoints::id(k_imagePoints);
MString imagePoints::drawDbClassification("drawdb/geometry/imagePoints");
MString imagePoints::drawRegistrantId("imagePointsPlugin");

imagePoints:: imagePoints() {}

imagePoints:: ~imagePoints() {}

void *imagePoints::creator() {
	return new imagePoints();
}

// const double epsilon = 0.0001;

MStatus imagePoints::initialize()
{
	MStatus st;
	MString method("imagePoints::initialize");

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	MFnMatrixAttribute mAttr;


	MFloatMatrix identity;
	identity.setToIdentity();


	// aProjectionMatrix = mAttr.create( "projectionMatrix", "pmat",
	//                                   MFnMatrixAttribute::kFloat );
	// mAttr.setStorable( false );
	// mAttr.setHidden( false );
	// nAttr.setKeyable(true);

	// mAttr.setDefault(identity);
	// addAttribute(aProjectionMatrix);

	aDensityImage = tAttr.create("densityImage", "dim", cImgData::id ) ;
	tAttr.setStorable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute( aDensityImage ); mser;

	aRadiusImage = tAttr.create("radiusImage", "rim", cImgData::id ) ;
	tAttr.setStorable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute( aRadiusImage ); mser;

	aMask = tAttr.create("mask", "msk", cImgData::id ) ;
	tAttr.setStorable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute( aMask ); mser;


	aDensityRamp  = MRampAttribute::createCurveRamp("densityRamp", "drmp");
	st = addAttribute( aDensityRamp ); mser;

	aDensityRangeMin = nAttr.create( "densityRangeMin", "dnrn", MFnNumericData::kDouble);
	nAttr.setDefault(0.0);
	aDensityRangeMax = nAttr.create( "densityRangeMax", "dnrx", MFnNumericData::kDouble );
	nAttr.setDefault(0.0);
	aDensityRange = nAttr.create("densityRange", "dnr", aDensityRangeMin,  aDensityRangeMax);
	nAttr.setHidden( false );
	nAttr.setKeyable( true );
	st = addAttribute(aDensityRange); mser;

	aRadiusRamp  = MRampAttribute::createCurveRamp("radiusRamp", "rrmp");
	st = addAttribute( aRadiusRamp ); mser;

	aRadiusRangeMin = nAttr.create( "radiusRangeMin", "rdrn", MFnNumericData::kDouble);
	nAttr.setDefault(0.0);
	aRadiusRangeMax = nAttr.create( "radiusRangeMax", "rdrx", MFnNumericData::kDouble );
	nAttr.setDefault(0.0);
	aRadiusRange = nAttr.create("radiusRange", "rdr", aRadiusRangeMin,  aRadiusRangeMax);
	nAttr.setHidden( false );
	nAttr.setKeyable( true );
	st = addAttribute(aRadiusRange); mser;

	aSeed = nAttr.create( "seed", "sd", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0);
	st = addAttribute(aSeed);

	aIterations = nAttr.create( "iterations", "it", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1);
	st = addAttribute(aIterations);

	aNeighbors = nAttr.create( "neighbors", "nbs", MFnNumericData::kInt);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0);
	st = addAttribute(aNeighbors);

	aMagnitude = nAttr.create("magnitude", "mag", MFnNumericData::kFloat);
	nAttr.setHidden( false );
	nAttr.setKeyable( true );
	nAttr.setDefault( 100 );
	st = addAttribute(  aMagnitude );

	aOutPoints = tAttr.create("outPoints", "opts", MFnData::kVectorArray);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutPoints );

	aOutRadius = tAttr.create("outRadius", "orad", MFnData::kDoubleArray);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutRadius );

	aOutU = tAttr.create("outU", "ou", MFnData::kDoubleArray);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutU );


	aOutV = tAttr.create("outV", "ov", MFnData::kDoubleArray);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutV );

	aLineThickness = nAttr.create("lineThickness", "ltk", MFnNumericData::kInt);
	nAttr.setDefault( 1  );
	nAttr.setKeyable( true );
	st = addAttribute( aLineThickness ); mser;

	aPointSize = nAttr.create("pointSize", "psz", MFnNumericData::kInt);
	nAttr.setDefault( 2);
	nAttr.setKeyable( true );
	st = addAttribute( aPointSize ); mser

	aColor = nAttr.createColor( "drawColor", "dc");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute( aColor ); mser

	aDisplayPoints = nAttr.create( "displayPoints", "dpts",
	                               MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(true);
	addAttribute(aDisplayPoints);


	aDisplayCircles = nAttr.create( "displayCircles", "dcrc",
	                                MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(true);
	addAttribute(aDisplayCircles);

	aCircleDisplaySize = nAttr.create("circleDisplaySize", "cdsz", MFnNumericData::kFloat);
	nAttr.setDefault( 2.0f );
	nAttr.setKeyable( true );
	st = addAttribute( aCircleDisplaySize ); mser

	st = attributeAffects( aDensityImage, aOutPoints);
	st = attributeAffects( aRadiusImage, aOutPoints);
	st = attributeAffects( aMask, aOutPoints);
	st = attributeAffects( aSeed, aOutPoints);
	// st = attributeAffects( aProjectionMatrix, aOutPoints);
	st = attributeAffects( aIterations, aOutPoints);
	st = attributeAffects( aNeighbors, aOutPoints);
	st = attributeAffects( aMagnitude, aOutPoints);
	st = attributeAffects( aDensityRamp, aOutPoints);
	st = attributeAffects( aDensityRange, aOutPoints);
	st = attributeAffects( aRadiusRamp, aOutPoints);
	st = attributeAffects( aRadiusRange, aOutPoints);

	st = attributeAffects( aDensityImage, aOutRadius);
	st = attributeAffects( aRadiusImage, aOutRadius);
	st = attributeAffects( aMask, aOutRadius);
	st = attributeAffects( aSeed, aOutRadius);
	// st = attributeAffects( aProjectionMatrix, aOutRadius);
	st = attributeAffects( aDensityRamp, aOutRadius);
	st = attributeAffects( aDensityRange, aOutRadius);
	st = attributeAffects( aRadiusRamp, aOutRadius);
	st = attributeAffects( aRadiusRange, aOutRadius);

	st = attributeAffects( aDensityImage, aOutU);
	st = attributeAffects( aRadiusImage, aOutU);
	st = attributeAffects( aMask, aOutU);
	st = attributeAffects( aSeed, aOutU);
	// st = attributeAffects( aProjectionMatrix, aOutU);
	st = attributeAffects( aIterations, aOutU);
	st = attributeAffects( aNeighbors, aOutU);
	st = attributeAffects( aMagnitude, aOutU);
	st = attributeAffects( aDensityRamp, aOutU);
	st = attributeAffects( aDensityRange, aOutU);
	st = attributeAffects( aRadiusRamp, aOutU);
	st = attributeAffects( aRadiusRange, aOutU);


	st = attributeAffects( aDensityImage, aOutV);
	st = attributeAffects( aRadiusImage, aOutV);
	st = attributeAffects( aMask, aOutV);
	st = attributeAffects( aSeed, aOutV);
	// st = attributeAffects( aProjectionMatrix, aOutV);
	st = attributeAffects( aIterations, aOutV);
	st = attributeAffects( aNeighbors, aOutV);
	st = attributeAffects( aMagnitude, aOutV);
	st = attributeAffects( aDensityRamp, aOutV);
	st = attributeAffects( aDensityRange, aOutV);
	st = attributeAffects( aRadiusRamp, aOutV);
	st = attributeAffects( aRadiusRange, aOutV);

	return (MS::kSuccess );
}


CImg<unsigned char> *imagePoints::getImage(MDataBlock &data, MObject &attribute )
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

void imagePoints::makeDots(MDataBlock &data, std::vector<dotData> &dots)
{
	MStatus st;
	int seedVal = data.inputValue( aSeed).asInt()  ;
	srand48(seedVal);

	MDataHandle hDensityRange = data.inputValue(aDensityRange);
	double densityRangeMin = hDensityRange.child( aDensityRangeMin).asDouble();
	double densityRangeMax = hDensityRange.child( aDensityRangeMax).asDouble();

	int count = int(densityRangeMax);
	densityRangeMin = densityRangeMin / densityRangeMax;
	densityRangeMax = 1.0;
	double densityRange = densityRangeMax - densityRangeMin;

	// MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();

	CImg<unsigned char>  *pDensityImage = getImage(data, imagePoints::aDensityImage );
	CImg<unsigned char>  *pRadiusImage = getImage(data, imagePoints::aRadiusImage);

	CImg<unsigned char> densityImage ;
	CImg<unsigned char>  radiusImage ;

	bool densityMapped = false;
	bool radiusMapped = false;
	int dw = 0;
	int dh = 0;
	int rw = 0;
	int rh = 0;

	if (pDensityImage) {
		dw = pDensityImage->width();
		dh = pDensityImage->height();
		if (dw && dh) {
			densityMapped =  true;
			densityImage = pDensityImage->get_channel(0).mirror("y");
		}
	}

	if (pRadiusImage) {
		rw = pRadiusImage->width();
		rh = pRadiusImage->height();
		if (rw && rh) {
			radiusMapped = true;
			radiusImage = pRadiusImage->get_channel(0).mirror("y");
		}
	}


	MObject thisObj = thisMObject();
	MRampAttribute densityAttr( thisObj, aDensityRamp ); mser;
	MRampAttribute radiusAttr( thisObj, aRadiusRamp ); mser;


	MDataHandle hRadiusRange = data.inputValue(aRadiusRange);
	double radiusRangeMin = hRadiusRange.child( aRadiusRangeMin).asDouble();
	double radiusRangeMax = hRadiusRange.child( aRadiusRangeMax).asDouble();

	double radiusRange = radiusRangeMax - radiusRangeMin;

	float normalizer  = 1.0 / 255.0;

	int j = 0;

	// cerr << "count: " << count << endl;
	for (int i = 0; i < count; ++i)
	{
		float u = float(drand48()) ;
		float v = float(drand48()) ;
		float rx = u * rw;
		float ry = v * rh;
		float dx = u * dw;
		float dy = v * dh;

		float dval = densityMapped ? float(densityImage.atXY(dx, dy)) * normalizer : 1.0;
		// dval is between 0 and 1
		float newDval;
		densityAttr.getValueAtPosition( dval, newDval, &st ); mser;
		newDval = (newDval * densityRange) + densityRangeMin;

		if ((!densityMapped) || drand48() < newDval) {

			float rval = radiusMapped ? float(radiusImage.atXY(rx, ry)) * normalizer : 1.0;
			float newRval;
			radiusAttr.getValueAtPosition( rval, newRval, &st ); mser;
			newRval = (newRval * radiusRange) + radiusRangeMin;
			// dots.push_back(dotData(projection, u, v , newDval , newRval, j)) ;
			dots.push_back(dotData(u, v , newDval , newRval, j)) ;
			
			j++;
		}
	}

}

void imagePoints::relaxDots(MDataBlock &data, std::vector<dotData> &dots)
{

	int iterations = int(data.inputValue(aIterations).asInt());
	int neighbors = int(data.inputValue(aNeighbors).asInt());
	float magnitude = data.inputValue(aMagnitude).asFloat();
	// MFloatMatrix invmat = data.inputValue(aProjectionMatrix).asFloatMatrix().inverse();


	if (! iterations) { return; }

	magnitude = magnitude / float(iterations);


	const int numPoints = dots.size();

	for (int i = 0; i < iterations; ++i)
	{
		dotTree *pTree = new dotTree();
		pTree->populate(dots);
		pTree->build();

		for (unsigned i = 0; i < numPoints; i++ ) {


			KNN_PD_QUEUE *q = new KNN_PD_QUEUE;
			for (int n = 0; n < neighbors; n++) {
				knnSearchPdData k;
				k.weakness = 1.0f;
				k.dot = 0;
				q->push(k);
			}

			const dotData *searchDot = pTree->dots()->at(i);

			pTree->strongestNPoints( pTree->root(), searchDot, q);

			JVector2D repulsion;
			while (!q->empty()) {
				if (q->top().dot) {
					if (q->top().hasDifference()) {

						repulsion += q->top().force() ;
					}
				}
				q->pop();
			}
			repulsion *= magnitude;
			dots[i].applyForce(repulsion);
			delete q;
		}
		if (pTree) {delete pTree; pTree = 0;}
	}

	for (auto dot : dots)
	{
		dot.setUV();
	}

}


bool shouldCull(const dotData &dot,
                const CImg<unsigned char>   &maskImage )
{

	int w = maskImage.width();
	int h = maskImage.height();
	float u = dot.u();
	float v = dot.v();
	float x, y;

	cImgUtils::toImageCoords(u, v, w, h, x, y);
	float val  = float(maskImage.atXY(x, y))  ; /* 0 -> 255 */
	return ( float(drand48() * 255.0) >  val );
}

void imagePoints::cullDots(MDataBlock &data, std::vector<dotData> &dots)
{
	CImg<unsigned char>  *pMaskImage = getImage(data, imagePoints::aMask );

	if (!pMaskImage) {
		return ;
	}



	int w = pMaskImage->width();
	int h = pMaskImage->height();

	if (! (w && h)) {
		return ;
	}

	std::vector<dotData>::iterator new_end  = std::remove_if(dots.begin(), dots.end(),
	    [pMaskImage](const dotData & dot)
	{
		return  shouldCull(dot, *pMaskImage);
	}   );

	dots.erase(new_end, dots.end());

}



MStatus imagePoints::compute(const MPlug &plug, MDataBlock &data )
{
	MStatus st;
	if (! (
	      plug == aOutPoints ||
	      plug == aOutRadius ||
	      plug == aOutU ||
	      plug == aOutV ))
	{
		return (MS::kUnknownParameter );
	}


	std::vector<dotData> dots;
	makeDots(data, dots);
	relaxDots(data, dots);
	cullDots(data, dots);



	MVectorArray resultPoints;
	MDoubleArray resultRadii;
	MDoubleArray resultU;
	MDoubleArray resultV;

	std::vector<dotData>::const_iterator citer;
	for (citer = dots.begin(); citer != dots.end(); citer++)
	{
		resultPoints.append(citer->asVector());
		resultRadii.append(citer->radius());
		resultU.append(citer->u());
		resultV.append(citer->v());
	}

	st = outputData(imagePoints::aOutPoints, data, resultPoints ); mser;
	st = outputData(imagePoints::aOutRadius, data, resultRadii ); mser;
	st = outputData(imagePoints::aOutU, data, resultU ); mser;
	st = outputData(imagePoints::aOutV, data, resultV ); mser;

	return MS:: kSuccess;
}



void imagePoints::draw( M3dView &view,
                             const MDagPath &path,
                             M3dView::DisplayStyle style,
                             M3dView:: DisplayStatus status  )
{


	// MStatus st;

	// MObject thisObj = thisMObject();


	// bool displayPoints, displayCircles;
	// MPlug(thisObj, aDisplayPoints).getValue(displayPoints);
	// MPlug(thisObj, aDisplayCircles).getValue(displayCircles);


	// double lineThickness;
	// MPlug(thisObj, aLineThickness).getValue(lineThickness);

	// double circleDisplaySize;
	// MPlug(thisObj, aCircleDisplaySize).getValue(circleDisplaySize);



	// double pointSize;
	// MPlug(thisObj, aPointSize).getValue(pointSize);


	// MPlug pointsPlug( thisObj, aOutPoints );
	// MObject dPoints;
	// st = pointsPlug.getValue(dPoints); mser;
	// MFnVectorArrayData fnPoints(dPoints);
	// MVectorArray points = fnPoints.array();

	// MPlug radiusPlug( thisObj, aOutRadius );
	// MObject dRadius;
	// st = radiusPlug.getValue(dRadius); mser;
	// MFnDoubleArrayData fnRadius(dRadius);
	// MDoubleArray radii = fnRadius.array();


	// MPlug matrixPlug( thisObj, aProjectionMatrix );
	// MObject dmat;
	// st = matrixPlug.getValue(dmat); mser;
	// MFnMatrixData fnMatrix(dmat);
	// // const MMatrix mat =  fnMatrix.matrix();
	// float dest[4][4];
	// fnMatrix.matrix().get(dest);
	// const MFloatMatrix imat = MFloatMatrix(dest).inverse();



	// MPlug colorPlug(thisObj, aColor);


	// float colorr , colorg , colorb;
	// colorPlug.child(0).getValue(colorr);
	// colorPlug.child(1).getValue(colorg);
	// colorPlug.child(2).getValue(colorb);



	// view.beginGL();

	// unsigned len = points.length();

	// if (displayPoints) {
	// 	glPushAttrib(GL_CURRENT_BIT);
	// 	glColor4f(colorr , colorg , colorb, 1.0f);

	// 	glPointSize(float(pointSize));
	// 	glBegin( GL_POINTS );

	// 	for (int i = 0; i < len; ++i)
	// 	{
	// 		MFloatPoint inverted = MFloatPoint(points[i]) * imat;
	// 		glVertex3f( inverted.x, inverted.y, inverted.z);
	// 	}
	// 	glEnd();
	// 	glPopAttrib();
	// }




	// glPushAttrib(GL_CURRENT_BIT);
	// glPushAttrib(GL_LINE_BIT);
	// glLineWidth(GLfloat(lineThickness));
	// glBegin(GL_LINES);
	// glVertex3f( -1.0f , 1.0f , 0.0f );
	// glVertex3f( 1.0f , 1.0f , 0.0f );

	// glVertex3f( 1.0f , 1.0f , 0.0f );
	// glVertex3f( 1.0f , -1.0f , 0.0f );

	// glVertex3f( 1.0f , -1.0f , 0.0f );
	// glVertex3f( -1.0f , -1.0f , 0.0f );

	// glVertex3f( -1.0f , -1.0f , 0.0f );
	// glVertex3f( -1.0f , 1.0f , 0.0f );
	// glEnd();
	// glPopAttrib();
	// glPopAttrib();


	// if (displayCircles) {


	// 	MFloatPointArray cScaled(circleVertexCount);
	// 	for (int j = 0; j < circleVertexCount; ++j) {
	// 		cScaled[j] = MPoint(circle[j]) * circleDisplaySize;
	// 	}


	// 	glPushAttrib(GL_CURRENT_BIT);

	// 	glColor4f(colorr , colorg , colorb, 1.0f);

	// 	glPushAttrib(GL_LINE_BIT);
	// 	glLineWidth(GLfloat(lineThickness));
	// 	glBegin(GL_LINES);

	// 	for (int i = 0; i < len; ++i)
	// 	{
	// 		MFloatPointArray c(circleVertexCount);
	// 		for (int j = 0; j < circleVertexCount; ++j) {
	// 			c[j] = ((cScaled[j] * radii[i]) + MFloatPoint(points[i])) * imat;
	// 		}
	// 		for (int j = 0; j < circleVertexCount; ++j)
	// 		{
	// 			int next = (j + 1) % circleVertexCount;
	// 			glVertex3f( float(c[j].x) , float(c[j].y) , float(c[j].z) );
	// 			glVertex3f( float(c[next].x) , float(c[next].y) , float(c[next].z) );
	// 		}
	// 	}
	// 	glEnd();
	// 	glPopAttrib();
	// 	glPopAttrib();

	// }



	// view.endGL();

 

}





bool imagePoints::isBounded() const
{
	return true;
}

MBoundingBox imagePoints::boundingBox() const
{
	return MBoundingBox(MPoint(-1.0, -1.0,0.0), MPoint(1.0, 1.0,0.0));

}

void imagePoints::postConstructor()
{
	// setExistWithoutInConnections(true);
	// setExistWithoutOutConnections(true);
}
