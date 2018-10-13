#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMatrixAttribute.h>
// #include <maya/MRenderUtil.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgGradSampler.h"
#include "jMayaIds.h"
#include "textureSampling.h"

MTypeId     cImgGradSampler::id( k_cImgGradSampler );




MObject 	cImgGradSampler::aSamplePoints;

MObject 	cImgGradSampler::aInputImage;
MObject 	cImgGradSampler::aChannel;
MObject 	cImgGradSampler::aDirection;


// MObject 	cImgGradSampler::aOutput;
MObject 	cImgGradSampler::aOutPoints;
MObject 	cImgGradSampler::aOutDirections;
MObject   cImgGradSampler::aProjectionMatrix;

// MObject 	cImgGradSampler::aNormalizeOutput;



void toImageCoords(const MFloatPoint &point, const MFloatMatrix &inverseProjection, int w,
                   int h,
                   float &x, float &y, bool clamp = true)
{
	// puts p  in -1 -> 1
	MFloatPoint p = point * inverseProjection;
	x = ((p.x * 0.5f) + 0.5f) * w;
	y = (1.0f - ((p.y * 0.5f) + 0.5f)) * h;
	if (clamp) {
		x = std::max(0.0f, std::min(x, float(w)));
		y = std::max(0.0f, std::min(y, float(h)));
	}
}


cImgGradSampler::cImgGradSampler() {}

cImgGradSampler::~cImgGradSampler() {}

void *cImgGradSampler::creator()
{
	return new cImgGradSampler();
}

MStatus cImgGradSampler::initialize()
{
	MStatus st;

	MFnTypedAttribute tAttr;
	MFnMatrixAttribute mAttr;
	MFnEnumAttribute eAttr;


	aInputImage = tAttr.create("inputImage", "ini", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aInputImage ); mser;

	MFloatMatrix identity;
	identity.setToIdentity();
	aProjectionMatrix = mAttr.create( "projectionMatrix", "pmat",
	                                  MFnMatrixAttribute::kFloat );
	mAttr.setStorable( false );
	mAttr.setHidden( false );
	mAttr.setKeyable(true);
	mAttr.setDefault(identity);
	addAttribute(aProjectionMatrix);


	aSamplePoints = tAttr.create("samplePoints", "spts", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( false);
	tAttr.setHidden( false );
	tAttr.setWritable(true);
	st = addAttribute( aSamplePoints );


	aChannel = eAttr.create("channel", "ch", cImgGradSampler::kAverage);
	eAttr.addField( "red",   cImgGradSampler::kRed);
	eAttr.addField( "green",   cImgGradSampler::kGreen);
	eAttr.addField( "blue",   cImgGradSampler::kBlue);
	eAttr.addField( "average",   cImgGradSampler::kAverage);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute( aChannel );

	aDirection = eAttr.create("direction", "dir", cImgGradSampler::kUp);
	eAttr.addField( "up",   cImgGradSampler::kUp);
	eAttr.addField( "down",   cImgGradSampler::kDown);
	eAttr.addField( "left",   cImgGradSampler::kLeft);
	eAttr.addField( "right",   cImgGradSampler::kRight);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute( aDirection );

	aOutPoints = tAttr.create("outPoints", "opts", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutPoints );

	aOutDirections = tAttr.create("outDirections", "odrs", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutDirections );

	st = attributeAffects(aInputImage, aOutPoints ); mser;
	st = attributeAffects(aInputImage, aOutDirections ); mser;

	st = attributeAffects(aSamplePoints, aOutPoints ); mser;
	st = attributeAffects(aSamplePoints, aOutDirections ); mser;

	st = attributeAffects(aProjectionMatrix, aOutPoints ); mser;
	st = attributeAffects(aProjectionMatrix, aOutDirections ); mser;

	st = attributeAffects(aChannel, aOutPoints ); mser;
	st = attributeAffects(aChannel, aOutDirections ); mser;

	st = attributeAffects(aDirection, aOutPoints ); mser;
	st = attributeAffects(aDirection, aOutDirections ); mser;



	return MS::kSuccess;
}





MStatus cImgGradSampler::compute( const MPlug &plug, MDataBlock &data ) {

	if (!
	    (plug == aOutPoints) ||
	    (plug == aOutDirections)
	   ) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;


	MVectorArray points;
	MVectorArray directions;


	cImgGradSampler::Channel channel = cImgGradSampler::Channel(data.inputValue(
	                                     aChannel).asShort());


	cImgGradSampler::Direction direction = cImgGradSampler::Direction(data.inputValue(
	    aDirection).asShort());



	MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();
	MFloatMatrix imat = projection.inverse();



	MDataHandle hImageData = data.inputValue(aInputImage, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);



	if (! st.error()) {
		// cerr << "MFnPluginData NO ERROR" << endl;
		cImgData *imageData = (cImgData *)fnImageData.data();
		CImg<unsigned char> *image = imageData->fImg;
		CImg<unsigned char> single(*image) ;

		// int nChannels = image->spectrum();
		// if (nChannels > 2) {
		// 	if (channel == cImgGradSampler::kRed) {
		// 		single = image->get_channel(0);
		// 	}
		// 	else if (channel == cImgGradSampler::kGreen) {
		// 		single = image->get_channel(1);
		// 	}
		// 	else if (channel == cImgGradSampler::kBlue) {
		// 		single = image->get_channel(2);
		// 	}
		// 	else   // average
		// 	{
		// 		single = image->get_norm();
		// 	}

		// }
		// else {
		// 	single = image->get_channel(0);
		// }


		int w = single.width();
		int h = single.height();

		// cerr << "w: " << w << " h:" << h << endl;



		MDataHandle hPts = data.inputValue( aSamplePoints , &st);
		MObject dPts = hPts.data();
		points = MFnVectorArrayData( dPts ).array( &st );
		// cerr << "points: " << points << endl;;
		CImgList<float> grad = single.get_gradient("xy", 0);


		// float cz = 1.0f;
		// float sz = 0.0f;
		// if (direction == cImgGradSampler::kDown) {
		// 	cz = -1.0f;
		// 	sz = 0.0f;
		// }
		// else if (direction == cImgGradSampler::kLeft) {
		// 	cz = 0.0f;
		// 	sz = 1.0f;
		// }
		// else if (direction == cImgGradSampler::kRight) {  // right
		// 	cz = 0.0f;
		// 	sz = -1.0f;
		// }

		// MFloatMatrix rot;
		// rot.setToIdentity();
		// rot[0][0] = cz;	rot[0][1] = sz;
		// rot[1][0] = -sz;	rot[1][1] = cz;

		// MFloatMatrix rprojection =  rot * projection  ;
		// RZ = |  cz   sz   0    0 |     sx = sin(rx), cx = cos(rx)
		//    | -sz   cz   0    0 |     sy = sin(ry), cx = cos(ry)
		//    |  0    0    1    0 |     sz = sin(rz), cz = cos(rz)
		//    |  0    0    0    1 |


		unsigned len = points.length();
		for (int i = 0; i < len; ++i)
		{
			// local (-1 to +1)
			float x, y;
			MFloatPoint point = MFloatPoint(points[i]);
			toImageCoords(point, imat, w, h, x, y);

			// cerr << point << " -- " << x << " " << y << endl;
			MFloatVector gradient = MFloatVector(
			                          grad(0).atXY(x, y),
			                          -grad(1).atXY(x, y),
			                          0.0) * projection;
			// directions.append(MVector(gradient.x, gradient.y, 0.0));

			directions.append(MVector(gradient.y, -gradient.x, 0.0));

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

	return MS::kSuccess;


	// MDataHandle hPts = data.inputValue( aSamplePoints , &st);
	// MObject dPts = hPts.data();
	// MVectorArray samplePts = MFnVectorArrayData( dPts ).array( &st );


	// CImgList<float> grad = image.get_gradient("xy", 0);



}



// // CImg<unsigned char> col( w, h, 1, 3);
// // col.fill(255, 1, 1);
// // if (gradList.size() == 2) {
// // 	gradList(1).channels(0, 1);
// // 	gradList(1).get_shared_channel(1).assign(gradList(0));

// // 	image->draw_quiver(gradList(1), col, 0.5f, 10, -100, 0);

// // 	int channels = image->spectrum();
// // 	cerr << "channels: "  << channels << endl;
// // }
// gradList(0).resize(w, h, -100, -100, 5);
// gradList(1).resize(w, h, -100, -100, 5);

// float wg = gradList(0).width();
// float hg = gradList(0).height();
// // int channelsg = gradList(0).spectrum();


// MVectorArray points;
// MVectorArray directions;


// if (gradList.size() == 2 && wg > 0 && hg > 0) {


// 	for (int y = 0; y < h; y += 2)
// 	{
// 		;
// 		for (int x = 0; x < w ; x += 2)
// 		{
// 			// float u = (x+0.5) / w;
// 			float gx = gradList(0).atXY(x, y);
// 			float gy = gradList(1).atXY(x, y);
// 			// float length = 2;
// 			// float lengthrecip = sqrt((gx * gx) + (gy * gy) );

// 			//				cerr << "gx,gy: " << gx << " " << gy << endl;					// gx /= length;
// 			// normalize
// 			// gx /= length;
// 			// gy /= length;
// 			int newx = x + int(gx);
// 			int newy = y + int(gy);

// 			// if (newx >= 0 && newx < w && newy >= 0 && newy < h)
// 			// {
// 			// 	image->draw_line	(	x, y, newx, newy, redCol, 1.0f, 0x11111111, false);
// 			// }

// 			// points.append(
// 			//   MVector(
// 			//     ((x + 0.5) / w),
// 			//     (1.0 - (y + 0.5) / h),
// 			//     0));
// 			// directions.append(
// 			//   MVector(
// 			//     gx ,
// 			//     -gy,
// 			//     0).normal());

// 			points.append(
// 			  MVector(
// 			    ((x + 0.5) / w),
// 			    (1.0 - (y + 0.5) / h),
// 			    0));


// 			/* gy is normally */
// 			directions.append(
// 			  MVector(
// 			    gy ,
// 			    gx,
// 			    0).normal());

// 		}
// 	}
// }



// MDataHandle hOutPoints = data.outputValue(aOutPoints);
// MFnVectorArrayData fnOutPoints;
// MObject dOutPoints = fnOutPoints.create(points);
// hOutPoints.set(dOutPoints);
// st = data.setClean( aOutPoints );

// MDataHandle hOutDirections = data.outputValue(aOutDirections);
// MFnVectorArrayData fnOutDirections;
// MObject dOutDirections = fnOutDirections.create(directions);
// hOutDirections.set(dOutDirections);
// st = data.setClean( aOutDirections );


// // image->assign(  imageFilename.asChar()  );



// // CImgList<Tfloat> get_gradient	(	const char *const 	axes = 0,
// //                                 const int 	scheme = 3
// //                               )		const



// hOutput.set(newData);
// data.setClean( plug );
// return MS::kSuccess;
// }

// unsigned int numgrad = gradList.size();
// cerr << "Num grad images:" << numgrad << endl;
// cerr << "wg:" << wg << endl;
// cerr << "hg:" << hg << endl;
// cerr << "channelsg:" << channelsg << endl;



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
// st = sampleUVTexture(thisObj, cImgGradSampler::aTexture, uVals, vVals,
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

// cerr << "about to make image: "  << endl;
// CImg<unsigned char> image(vals, resolution, resolution, 1, 3);

