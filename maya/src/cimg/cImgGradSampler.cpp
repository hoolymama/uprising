#include <math.h>
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
#include "cImgUtils.h"
#include "cImgData.h"
#include "cImgGradSampler.h"
#include "jMayaIds.h"
#include "textureSampling.h"

MTypeId     cImgGradSampler::id( k_cImgGradSampler );




MObject 	cImgGradSampler::aSamplePoints;

MObject 	cImgGradSampler::aInputImage;
MObject 	cImgGradSampler::aChannel;
// MObject 	cImgGradSampler::aDirection;
MObject 	cImgGradSampler::aBlur;
MObject   cImgGradSampler::aInterpolation;

MObject   cImgGradSampler::aRotation;



MObject   cImgGradSampler::aStrokeLength;
MObject   cImgGradSampler::aPointDensity;

MObject 	cImgGradSampler::aOutPoints;
MObject 	cImgGradSampler::aOutDirections;
MObject   cImgGradSampler::aProjectionMatrix;

// void toImageCoords(const MFloatPoint &point, const MFloatMatrix &inverseProjection, int w,
//                    int h,  float &x, float &y, bool clamp = true)
// {
// 	// puts p  in -1 -> 1
// 	MFloatPoint p = point * inverseProjection;
// 	// if (verbose) { cerr << "point in proj: " << p << endl; }

// 	x = ((p.x * 0.5f) + 0.5f) * w;
// 	y = (1.0f - ((p.y * 0.5f) + 0.5f)) * h;
// 	if (clamp) {
// 		x = std::max(0.0f, std::min(x, float(w)));
// 		y = std::max(0.0f, std::min(y, float(h)));
// 	}
// }

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
	MFnNumericAttribute nAttr;
	MFnUnitAttribute uAttr;

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


	aChannel = eAttr.create("channel", "ch", cImgData::kAverage);
	eAttr.addField( "red",   cImgData::kRed);
	eAttr.addField( "green",   cImgData::kGreen);
	eAttr.addField( "blue",   cImgData::kBlue);
	eAttr.addField( "average",   cImgData::kAverage);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute( aChannel );


	aBlur = nAttr.create( "blur", "blr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(1.0);
	st = addAttribute(aBlur);

	aRotation = uAttr.create( "rotation", "rot",
	                          MFnUnitAttribute::kAngle );
	uAttr.setHidden( false );
	uAttr.setKeyable( true );
	uAttr.setStorable(true);
	st = addAttribute(aRotation); mser;

	aInterpolation = eAttr.create("interpolation", "itp");
	eAttr.addField("Nearest", cImgData::kNearest);
	eAttr.addField("Bilinear", cImgData::kBilinear);
	eAttr.addField("Bicubic", cImgData::kBicubic);

	eAttr.setDefault( cImgData::kBilinear );
	eAttr.setKeyable(true);
	eAttr.setWritable(true);
	addAttribute(aInterpolation);




	aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kDouble);
	nAttr.setHidden( false );
	nAttr.setKeyable( true );
	nAttr.setStorable(true);
	nAttr.setDefault( 1.0 );
	st = addAttribute(aPointDensity); mser;

	aStrokeLength = nAttr.create( "strokeLength", "stl", MFnNumericData::kDouble);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setMin(0.01);
	nAttr.setDefault(10.0);
	st = addAttribute(aStrokeLength); mser;



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

	st = attributeAffects(aBlur, aOutPoints ); mser;
	st = attributeAffects(aBlur, aOutDirections ); mser;

	st = attributeAffects(aInterpolation, aOutPoints ); mser;
	st = attributeAffects(aInterpolation, aOutDirections ); mser;

	st = attributeAffects(aPointDensity, aOutPoints ); mser;
	st = attributeAffects(aPointDensity, aOutDirections ); mser;

	st = attributeAffects(aStrokeLength, aOutPoints ); mser;
	st = attributeAffects(aStrokeLength, aOutDirections ); mser;

	st = attributeAffects(aRotation, aOutPoints ); mser;
	st = attributeAffects(aRotation, aOutDirections ); mser;


	return MS::kSuccess;
}

MStatus cImgGradSampler::getImageChannel(MDataBlock &data,
    cImgData::Channel channel, CImg<unsigned char> &result)
{
	MStatus st;
	MDataHandle hImageData = data.inputValue(aInputImage, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st); msert;
	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *image = imageData->fImg;

	int nChannels = image->spectrum();
	if (! nChannels) {return MS::kFailure;}

	if (channel == cImgData::kRed || nChannels  < 3 ) {
		result = image->get_channel(0);
	}
	else if (channel == cImgData::kGreen) {
		result =  image->get_channel(1);
	}
	else if (channel == cImgData::kBlue) {
		result = image->get_channel(2);
	}
	else   // average
	{
		result =  image->get_norm();
	}
	return MS::kSuccess;
}

MFloatMatrix  cImgGradSampler::getFullProjection(float angle,
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



int cImgGradSampler::calcFlowPoints(const MFloatPoint &point, const CImgList<float> &grad,
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


MStatus cImgGradSampler::compute( const MPlug &plug, MDataBlock &data ) {

	if (!
	    (plug == aOutPoints) ||
	    (plug == aOutDirections)
	   ) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;


	MVectorArray points;
	MVectorArray directions;


	float blur = data.inputValue(aBlur).asFloat();

	cImgData::Channel channel = cImgData::Channel(data.inputValue(
	                              aChannel).asShort());

	float angle = float(data.inputValue(aRotation).asAngle().asRadians());


	cImgData::Interpolation interp = cImgData::Interpolation(data.inputValue(
	                                   aInterpolation).asShort());

	MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();
	MFloatMatrix imat = projection.inverse();


	float density = float(data.inputValue(aPointDensity).asDouble());
	float length = float(data.inputValue(aStrokeLength).asDouble());
	if (length < 0.001) { length = 0.001; }
	if (density > length) { density = length; }
	int count = int(length / density) + 1;
	float  gap = length / int(length / density);

	CImg<unsigned char> single;
	st = getImageChannel(data, channel, single);
	if (! st.error()) {


		int w = single.width();
		int h = single.height();


		MDataHandle hPts = data.inputValue( aSamplePoints , &st);
		MObject dPts = hPts.data();
		const MVectorArray samplePoints = MFnVectorArrayData( dPts ).array( &st );
		CImgList<float> grad = single.get_gradient("xy", 0);


		projection = getFullProjection(angle, projection);

		grad(0).blur(blur);
		grad(1).blur(blur);


		unsigned len = samplePoints.length();
		for (int i = 0; i < len; ++i)
		{
			// local (-1 to +1)
			MFloatPoint point = MFloatPoint(samplePoints[i]);

			MFloatPointArray flowPoints;
			// bool verbose = (i == 0);

			int flowCount = calcFlowPoints(point, grad, projection, imat, count, gap, flowPoints,
			                               interp);


			if (flowCount > 1)
			{
				for (int j = 0; j < (flowCount - 1); ++j)
				{
					points.append(MVector(flowPoints[j]));
					directions.append( MVector(flowPoints[j + 1] - flowPoints[j]) );
				}
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

	return MS::kSuccess;

}
