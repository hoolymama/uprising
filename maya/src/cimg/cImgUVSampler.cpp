#include <math.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgUtils.h"
#include "cImgData.h"
#include "cImgUVSampler.h"
#include "jMayaIds.h"
#include "textureSampling.h"

MTypeId     cImgUVSampler::id( k_cImgUVSampler );




MObject 	cImgUVSampler::aInputImage;
MObject 	cImgUVSampler::aInterpolation;
MObject 	cImgUVSampler::aSampleU;
MObject 	cImgUVSampler::aSampleV;
MObject 	cImgUVSampler::aOutColors;
MObject 	cImgUVSampler::aOutAlphas;



cImgUVSampler::cImgUVSampler() {}

cImgUVSampler::~cImgUVSampler() {}

void *cImgUVSampler::creator()
{
	return new cImgUVSampler();
}

MStatus cImgUVSampler::initialize()
{
	MStatus st;

	MFnTypedAttribute tAttr;
	MFnMatrixAttribute mAttr;
	MFnEnumAttribute eAttr;
	MFnNumericAttribute nAttr;
	MFnUnitAttribute uAttr;

	aInputImage = tAttr.create("inputImage", "ini", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aInputImage ); mser;

	aSampleU = tAttr.create("sampleU", "su", MFnData::kDoubleArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( false);
	tAttr.setHidden( false );
	tAttr.setWritable(true);
	st = addAttribute( aSampleU );

	aSampleV = tAttr.create("sampleV", "sv", MFnData::kDoubleArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( false);
	tAttr.setHidden( false );
	tAttr.setWritable(true);
	st = addAttribute( aSampleV );

	aInterpolation = eAttr.create("interpolation", "itp");
	eAttr.addField("Nearest", cImgData::kNearest);
	eAttr.addField("Bilinear", cImgData::kBilinear);
	eAttr.addField("Bicubic", cImgData::kBicubic);

	eAttr.setDefault( cImgData::kBilinear );
	eAttr.setKeyable(true);
	eAttr.setWritable(true);
	addAttribute(aInterpolation);

	aOutColors = tAttr.create("outColors", "oc", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutColors );

	aOutAlphas = tAttr.create("outAlphas", "oa", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutAlphas );

	st = attributeAffects(aInputImage, aOutColors);
	st = attributeAffects(aSampleU, aOutColors);
	st = attributeAffects(aSampleV, aOutColors);
	st = attributeAffects(aInterpolation, aOutColors);

	st = attributeAffects(aInputImage, aOutAlphas);
	st = attributeAffects(aSampleU, aOutAlphas);
	st = attributeAffects(aSampleV, aOutAlphas);
	st = attributeAffects(aInterpolation, aOutAlphas);



	return MS::kSuccess;
}

// MStatus cImgUVSampler::getImageChannel(MDataBlock &data,
//                                        cImgData::Channel channel, CImg<unsigned char> &result)
// {
// 	MStatus st;
// 	MDataHandle hImageData = data.inputValue(aInputImage, &st); msert;
// 	MObject dImageData = hImageData.data();
// 	MFnPluginData fnImageData( dImageData , &st); msert;
// 	cImgData *imageData = (cImgData *)fnImageData.data();
// 	CImg<unsigned char> *image = imageData->fImg;

// 	int nChannels = image->spectrum();
// 	if (! nChannels) {return MS::kFailure;}

// 	if (channel == cImgData::kRed || nChannels  < 3 ) {
// 		result = image->get_channel(0);
// 	}
// 	else if (channel == cImgData::kGreen) {
// 		result =  image->get_channel(1);
// 	}
// 	else if (channel == cImgData::kBlue) {
// 		result = image->get_channel(2);
// 	}
// 	else   // average
// 	{
// 		result =  image->get_norm();
// 	}
// 	return MS::kSuccess;
// }
/*
MFloatMatrix  cImgUVSampler::getFullProjection(float angle,
    const MFloatMatrix &projection)
{

	float cz = cos(angle);
	float sz = sin(angle);

	MFloatMatrix rot;
	rot.setToIdentity();
	rot[0][0] = cz;	rot[0][1] = sz;
	rot[1][0] = -sz;	rot[1][1] = cz;

	MFloatMatrix flip;
	flip.setToIdentity();
	flip[1][1] = -1;

	return flip * (rot * projection)  ;

}
*/

/*
int cImgUVSampler::calcFlowPoints(const MFloatPoint &point, const CImgList<float> &grad,
                                  const MFloatMatrix &mat, const MFloatMatrix &imat, int count, float gap,
                                  MFloatPointArray &resultPoints, cImgData::Interpolation interp)
{


	MFloatPoint p = point;
	int w = grad(0).width();
	int h = grad(0).height();


	// if (verbose)  { cerr << "count: " << count << " gap:" << gap << endl; }
	for (int i = 0; i < count; ++i)
	{
		resultPoints.append(p);

		float x, y;
		cImgUtils::toImageCoords(p, imat, w, h, x, y, false);

		// if (verbose) {cerr << "w:" << w << " h:" << h << " x:" << x << " y:" << y << endl;}
		if (x < 0 || x > w || y < 0 || y > h) {
			break;
		}
		float gx ;
		float gy ;

		if (interp ==  cImgData::kNearest) {
			gx = grad(0).atXY(x, y);
			gy = grad(1).atXY(x, y);
		}
		else if (interp ==  cImgData::kBilinear) {
			gx = grad(0).linear_atXY(x, y);
			gy = grad(1).linear_atXY(x, y);
		}
		else {    // bicubic
			gx = grad(0).cubic_atXY(x, y);
			gy = grad(1).cubic_atXY(x, y);
		}
		MFloatVector gradient = MFloatVector(gx, gy, 0)  * mat;
		p += (gradient.normal() * gap);

	}

	// if (verbose)	{ cerr << "flow points length:" << resultPoints.length() << endl; }

	return resultPoints.length();
}
*/






MStatus cImgUVSampler::compute( const MPlug &plug, MDataBlock &data ) {

	if (!
	    (plug == aOutColors) ||
	    (plug == aOutAlphas)
	   ) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;


	MVectorArray colors;
	MDoubleArray alphas;

	MDataHandle hImageData = data.inputValue(aInputImage, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st); msert;
	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *image = imageData->fImg;
	int w = image->width();
	int h = image->height();


	cImgData::Interpolation interp = cImgData::Interpolation(data.inputValue(
	                                   aInterpolation).asShort());


	MDataHandle hSampleU = data.inputValue( aSampleU , &st);
	MObject dSampleU = hSampleU.data();
	const MDoubleArray sampleU = MFnDoubleArrayData( dSampleU ).array( &st );

	MDataHandle hSampleV = data.inputValue( aSampleV , &st);
	MObject dSampleV = hSampleV.data();
	const MDoubleArray sampleV = MFnDoubleArrayData( dSampleV ).array( &st );

	unsigned len = sampleU.length();
	if ((sampleV.length() == len) && w && h) {
		int spectrum = image->spectrum();

		for (int i = 0; i < len; ++i)
		{
			float x, y;
			unsigned char red, green, blue;
			cImgUtils::toImageCoords(float(sampleU[i]), float(sampleV[i]), w, h, x, y);
			if (interp == cImgData::kNearest)
			{
				red = (*image)(int(x + 0.5), int(y + 0.5));
				if (spectrum == 3) {
					green = (*image)(int(x + 0.5), int(y + 0.5), 0, 1);
					blue = (*image)(int(x + 0.5), int(y + 0.5), 0, 2);
				}
			}
			else if (interp == cImgData::kBilinear)
			{
				red =  image->linear_atXY(x, y, 0, 0);
				if (spectrum == 3) {
					green = image->linear_atXY(x, y, 0, 1);
					blue =  image->linear_atXY(x, y, 0, 2);
				}

			}
			else if (interp == cImgData::kBicubic)
			{
				red =  image->cubic_atXY(x, y, 0, 0);
				if (spectrum == 3) {
					green = image->cubic_atXY(x, y, 0, 1);
					blue =  image->cubic_atXY(x, y, 0, 2);
				}
			}
			if (spectrum != 3) {
				green = blue = red;
			}
			double dred = double(red) / 255.0;
			double dgreen = double(green) / 255.0;
			double dblue = double(blue) / 255.0;

			colors.append(MVector(dred, dgreen, dblue));
			alphas.append((dred + dgreen + dblue) * 0.33333);
		}

	}

	MDataHandle hOutColor = data.outputValue(aOutColors);
	MFnVectorArrayData fnOutColor;
	MObject dOutColor = fnOutColor.create(colors);
	hOutColor.set(dOutColor);
	st = data.setClean( aOutColors );

	MDataHandle hOutAlpha = data.outputValue(aOutAlphas);
	MFnDoubleArrayData fnOutAlpha;
	MObject dOutAlpha = fnOutAlpha.create(alphas);
	hOutAlpha.set(dOutAlpha);
	st = data.setClean( aOutAlphas );

	return MS::kSuccess;

}
