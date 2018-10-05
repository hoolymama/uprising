#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgNode.h"
#include "jMayaIds.h"
#include "textureSampling.h"

MTypeId     cImgNode::id( k_cImgNode );

MObject 	cImgNode::aImageFilename;
// MObject 	cImgNode::aSampleResolution;
MObject 	cImgNode::aOutput;




MObject 	cImgNode::aOutPoints;
MObject 	cImgNode::aOutDirections;
MObject 	cImgNode::aNormalizeOutput;



cImgNode::cImgNode()
{
	// m_data = new cImgData;
}

cImgNode::~cImgNode() {

	// if (m_gsData) {
	// 	delete m_gsData;
	// }
	// m_gsData = 0;
}

void *cImgNode::creator()
{
	return new cImgNode();
}

MStatus cImgNode::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;


	aImageFilename = tAttr.create("imageFilename", "im", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setUsedAsFilename(true);
	addAttribute( aImageFilename );

	aNormalizeOutput = nAttr.create( "normalizeOutput", "nout", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(true);
	addAttribute(aNormalizeOutput );



	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);



	aOutPoints = tAttr.create("outPoints", "opts", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutPoints );

	aOutDirections = tAttr.create("outDirections", "odrs", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutDirections );



	st = attributeAffects(aImageFilename, aOutput ); mser;
	st = attributeAffects(aImageFilename, aOutPoints ); mser;
	st = attributeAffects(aImageFilename, aOutDirections ); mser;

	st = attributeAffects(aNormalizeOutput, aOutput ); mser;
	st = attributeAffects(aNormalizeOutput, aOutPoints ); mser;
	st = attributeAffects(aNormalizeOutput, aOutDirections ); mser;

	return MS::kSuccess;
}





MStatus cImgNode::compute( const MPlug &plug, MDataBlock &data ) {

	if ( plug != aOutput) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;

	// get the image up front, then if something goes wrong
	// we just set output and make it clean
	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;

	cImgData *newData = (cImgData *)fnOut.data(&st); mser;

	// CImg<unsigned char> *image;

	CImg<unsigned char> *image = newData->fImg;

	// int resolution =  data.inputValue(aSampleResolution).asInt();
	MString imageFilename = data.inputValue(aImageFilename).asString();

	// MObject thisObj = thisMObject();

	// int numPixels = resolution * resolution;

	// MFloatArray uVals(numPixels);
	// MFloatArray vVals(numPixels);

	// for ( int i = 0, y = (resolution - 1); y >= 0; y--) {
	// 	float vVal =  (y + 0.5) / resolution;
	// 	for (int x = 0; x < resolution; x++) {
	// 		float uVal = (x + 0.5) / resolution;
	// 		uVals.set( uVal, i );
	// 		vVals.set( vVal, i );
	// 		i++;
	// 	}
	// }

	// MFloatVectorArray colors;
	// st = sampleUVTexture(thisObj, cImgNode::aTexture, uVals, vVals,
	//                      colors) ;

	// if (! st.error()) {

	// 	unsigned char col;
	// 	unsigned char *vals = new unsigned char[numPixels * 3];
	// 	unsigned char *cPtr = vals;

	// 	for (int i = 0; i < 3; i++)
	// 	{
	// 		for (int j = 0; j < numPixels; j++)
	// 		{
	// 			col = (unsigned char)(colors[j][i] * 255);
	// 			*cPtr = col; cPtr++;
	// 		}
	// 	}

	cerr << "about to make image: "  << endl;
	// CImg<unsigned char> image(vals, resolution, resolution, 1, 3);

	image->assign(imageFilename.asChar());
	CImg<unsigned char> small_image = image->get_resize(32, 32, -100, -100, 5 );
	// image->blur(5);
	// (*(image) -= 255) *= -1;

	int w = image->width();
	int h = image->height();
	int channels = image->spectrum();
	// cerr << "Set image to " << imageFilename << " Dims are now: " << w << "X" << h << endl;
	// CImg<unsigned char> col( );
	CImgList<float> gradList = small_image.get_gradient("xy", 0);
	const unsigned char redCol[3] = {0, 0, 0};
	// CImg<unsigned char> col( w, h, 1, 3);
	// col.fill(255, 1, 1);
	// if (gradList.size() == 2) {
	// 	gradList(1).channels(0, 1);
	// 	gradList(1).get_shared_channel(1).assign(gradList(0));

	// 	image->draw_quiver(gradList(1), col, 0.5f, 10, -100, 0);

	// 	int channels = image->spectrum();
	// 	cerr << "channels: "  << channels << endl;
	// }
	gradList(0).resize(w, h, -100, -100, 5);
	gradList(1).resize(w, h, -100, -100, 5);

	float wg = gradList(0).width();
	float hg = gradList(0).height();
	// int channelsg = gradList(0).spectrum();


	MVectorArray points;
	MVectorArray directions;


	if (gradList.size() == 2 && wg > 0 && hg > 0) {


		// unsigned int numgrad = gradList.size();
		// cerr << "Num grad images:" << numgrad << endl;
		// cerr << "wg:" << wg << endl;
		// cerr << "hg:" << hg << endl;
		// cerr << "channelsg:" << channelsg << endl;


		for (int y = 0; y < h; y += 2)
		{
			;
			for (int x = 0; x < w ; x += 2)
			{
				// float u = (x+0.5) / w;
				float gx = gradList(0).atXY(x, y);
				float gy = gradList(1).atXY(x, y);
				float length = 2;
				// float lengthrecip = sqrt((gx * gx) + (gy * gy) );

				cerr << "gx,gy: " << gx << " " << gy << endl;					// gx /= length;
				// normalize
				gx /= length;
				gy /= length;
				int newx = x + int(gx);
				int newy = y + int(gy);

				// if (newx >= 0 && newx < w && newy >= 0 && newy < h)
				// {
				// 	image->draw_line	(	x, y, newx, newy, redCol, 1.0f, 0x11111111, false);
				// }

				// points.append(
				//   MVector(
				//     ((x + 0.5) / w),
				//     (1.0 - (y + 0.5) / h),
				//     0));
				// directions.append(
				//   MVector(
				//     gx ,
				//     -gy,
				//     0).normal());

				points.append(
				  MVector(
				    ((x + 0.5) / w),
				    (1.0 - (y + 0.5) / h),
				    0));
				directions.append(
				  MVector(
				    gy ,
				    gx,
				    0).normal());



			}
		}
	}



	MDataHandle hOutPoints = data.outputValue(aOutPoints);
	MFnVectorArrayData fnOutPoints;
	MObject dOutPoints = fnOutPoints.create(points);
	hOutPoints.set(dOutPoints);
	st = data.setClean( aOutPoints );



	MDataHandle hOutDirections = data.outputValue(aOutDirections);
	MFnVectorArrayData fnOutDirections;
	MObject dOutDirections = fnOutDirections.create(directions);
	hOutDirections.set(dOutDirections);
	st = data.setClean( aOutDirections );


	// image->assign(  imageFilename.asChar()  );



	// CImgList<Tfloat> get_gradient	(	const char *const 	axes = 0,
	//                                 const int 	scheme = 3
	//                               )		const



	hOutput.set(newData);
	data.setClean( plug );
	return MS::kSuccess;
}



