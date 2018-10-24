

# include <maya/MIOStream.h>
# include <math.h>
# include <algorithm>
# include <map>

# include <maya/MFnPluginData.h>
# include <maya/MDoubleArray.h>
# include <maya/MFloatVectorArray.h>
# include <maya/MFloatArray.h>
# include <maya/MFloatPointArray.h>
# include <maya/MPoint.h>

#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>


#include <maya/MRampAttribute.h>

#include <maya/MArrayDataHandle.h>
#include <maya/MAngle.h>

#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>

#include <maya/MFnVectorArrayData.h>

#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MTransformationMatrix.h>


#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnEnumAttribute.h>

#include <maya/MFnNurbsCurveData.h>

#include "strokeGeometryData.h"
#include "flowStrokeNode.h"
#include "stroke.h"
#include "backstroke.h"
#include "strokeRotationSpec.h"
#include "cImgUtils.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);
const double epsilon = 0.0001;

// void toImageCoords(const MFloatPoint &point, const MFloatMatrix &inverseProjection, int w,
//                    int h,  float &x, float &y, bool clamp = true)
// {
// 	// puts p  in -1 -> 1
// 	MFloatPoint p = point * inverseProjection;
// 	x = ((p.x * 0.5f) + 0.5f) * w;
// 	y = (1.0f - ((p.y * 0.5f) + 0.5f)) * h;
// 	if (clamp) {
// 		x = std::max(0.0f, std::min(x, float(w)));
// 		y = std::max(0.0f, std::min(y, float(h)));
// 	}
// }



MObject flowStroke::aInputImage;
MObject flowStroke::aInterpolation;
MObject flowStroke::aSamplePoints;
MObject flowStroke::aProjectionMatrix;
MObject flowStroke::aChannel;
MObject flowStroke::aBlur;
MObject flowStroke::aRotation;
MObject flowStroke::aSampleDistance;
MObject flowStroke::aBrushIdImage;
MObject flowStroke::aPaintIdImage;
MObject flowStroke::aBrushIdRemapRamp;
MObject flowStroke::aBrushIdRemapRange;

MObject flowStroke::aBrushRampScope;
MObject flowStroke::aApproachDistance;



MTypeId flowStroke:: id(k_flowStroke );

flowStroke:: flowStroke() {}

flowStroke:: ~flowStroke() {}

void *flowStroke::creator() {
	return new flowStroke();
}


MStatus flowStroke:: initialize()
{
	MStatus st;
	MString method("flowStroke::initialize");

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnCompoundAttribute cAttr;
	MFnMatrixAttribute mAttr;
	MFnEnumAttribute eAttr;



	inheritAttributesFrom("strokeNode");




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

	aSampleDistance = nAttr.create( "sampleDistance", "sds", MFnNumericData::kDouble);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(5.0);
	st = addAttribute(aSampleDistance);

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


	// aBrushIds = tAttr.create("brushIds", "bids", MFnData::kIntArray, &st);
	// tAttr.setStorable( false);
	// tAttr.setReadable( false);
	// tAttr.setHidden( false );
	// tAttr.setWritable(true);
	// st = addAttribute( aBrushIds );


	// aPaintIds = tAttr.create("paintIds", "pids", MFnData::kIntArray, &st);
	// tAttr.setStorable( false);
	// tAttr.setReadable( false);
	// tAttr.setHidden( false );
	// tAttr.setWritable(true);
	// st = addAttribute( aPaintIds );

	aBrushRampScope = eAttr.create("brushRampScope", "brsc",
	                               StrokeRotationSpec::kStroke);
	eAttr.addField("stroke", StrokeRotationSpec::kStroke);
	eAttr.addField("travelStroke", StrokeRotationSpec::kTravelStroke);
	eAttr.setKeyable(true);
	eAttr.setHidden(false);
	st = addAttribute(aBrushRampScope);
	mser;

	aApproachDistance = nAttr.create("approachDistance", "apds", MFnNumericData::kDouble);
	nAttr.setDefault(5.0);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	st = addAttribute(aApproachDistance);




	aBrushIdImage = tAttr.create("brushIdImage", "bidi", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aBrushIdImage ); mser;

	aPaintIdImage = tAttr.create("paintIdImage", "pidi", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aPaintIdImage ); mser;

	aBrushIdRemapRamp  = MRampAttribute::createCurveRamp("brushIdRemapRamp", "brrp");
	st = addAttribute( aBrushIdRemapRamp ); mser;

	aBrushIdRemapRange = nAttr.create("brushIdRemapRange", "brrg", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aBrushIdRemapRange);



	st = attributeAffects(aInputImage, aOutput ); mser;
	st = attributeAffects(aSamplePoints, aOutput ); mser;
	st = attributeAffects(aProjectionMatrix, aOutput ); mser;
	st = attributeAffects(aChannel, aOutput ); mser;
	st = attributeAffects(aBlur, aOutput ); mser;
	st = attributeAffects(aInterpolation, aOutput ); mser;
	st = attributeAffects(aRotation, aOutput ); mser;
	st = attributeAffects(aSampleDistance, aOutput); mser;
	st = attributeAffects(aBrushRampScope, aOutput ); mser;

	st = attributeAffects(aBrushIdImage, aOutput);
	st = attributeAffects(aPaintIdImage, aOutput);


	st = attributeAffects(aBrushIdRemapRamp, aOutput);
	st = attributeAffects(aBrushIdRemapRange, aOutput);

	st = attributeAffects(aApproachDistance, aOutput ); mser;

	return (MS::kSuccess );
}



MStatus flowStroke::getImageChannel(MDataBlock &data,
                                    cImgData::Channel channel, CImg<unsigned char> &result) const
{
	MStatus st;
	MDataHandle hImageData = data.inputValue(aInputImage, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st); msert;
	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *image = imageData->fImg;

	cImgUtils::getImageChannel(*image , channel, result);

	return MS::kSuccess;
}

MFloatMatrix getFullProjection(float angle,
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



int calcFlowPoints(const MFloatPoint &point, const CImgList<float> &grad,
                   const MFloatMatrix &mat, const MFloatMatrix &imat, int count, float gap,
                   MPointArray &resultPoints, cImgData::Interpolation interp)
{

	MFloatPoint p = point;
	int w = grad(0).width();
	int h = grad(0).height();

	for (int i = 0; i < count; ++i)
	{
		resultPoints.append(MPoint(p));

		float x, y;
		cImgUtils::toImageCoords(p, imat, w, h, x, y, false);


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

	return resultPoints.length();
}



void flowStroke::setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                             double approachDist) const {
	for (auto iter = strokes.begin(); iter != strokes.end(); iter++) {
		(*iter)->setApproach(approachDist, approachDist);
	}
}

MStatus flowStroke::generateStrokeGeometry(MDataBlock &data,
    std::vector < strokeGeom > *geom) const
{

	MStatus st;

	std::vector<std::unique_ptr <Stroke> > strokes;

	if (! data.inputValue(aActive).asBool()) {
		return MS:: kSuccess;
	}
	cImgData::Channel channel = cImgData::Channel(data.inputValue(
	                              aChannel).asShort());
	CImg<unsigned char> single;
	st = getImageChannel(data, channel, single);
	if (st.error()) {
		return MS::kUnknownParameter;
	}

	float blur = data.inputValue(aBlur).asFloat();
	float angle = float(data.inputValue(aRotation).asAngle().asRadians());
	cImgData::Interpolation interp = cImgData::Interpolation(data.inputValue(
	                                   aInterpolation).asShort());
	MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();
	MFloatMatrix imat = projection.inverse();

	int w = single.width();
	int h = single.height();

	MDataHandle hPts = data.inputValue( aSamplePoints , &st);
	MObject dPts = hPts.data();
	const MVectorArray samplePoints = MFnVectorArrayData( dPts ).array( &st );
	CImgList<float> grad = single.get_gradient("xy", 0);

	MFloatMatrix fullProjection = getFullProjection(angle, projection);
	grad(0).blur(blur);
	grad(1).blur(blur);


	//////// GET STUFF FROM INHERITED
	MVector planeNormal = data.inputValue(aPlaneNormal).asVector();

	// pointDensity is points per cm
	double pointDensity = data.inputValue(aPointDensity).asDouble();
	if (pointDensity < 0.001) {
		pointDensity = 0.001;
	}
	double length = data.inputValue(aStrokeLength).asDouble();
	if (length < 0.001) { length = 0.001; }

	double sampleDist =  data.inputValue(aSampleDistance).asDouble();
	int numPoints = int(length / sampleDist) + 1;
	if (numPoints < 2) { numPoints = 2; }
	sampleDist = length / (numPoints - 1);

	MDataHandle hLift = data.inputValue(aLift);
	double liftLength = hLift.child(aLiftLength).asDouble();
	double liftHeight = hLift.child(aLiftHeight).asDouble();
	double liftBias = hLift.child(aLiftBias).asDouble();
	MVector lift(liftLength, liftHeight, liftBias);


	double approachDistance = data.inputValue(aApproachDistance).asDouble();

	bool backstroke = data.inputValue(aBackstroke).asBool();

	short repeats = data.inputValue(aRepeats).asShort();
	double repeatOffset = data.inputValue(aRepeatOffset).asDouble();
	bool repeatMirror = data.inputValue(aRepeatMirror).asBool();
	bool repeatOscillate = data.inputValue(aRepeatOscillate).asBool();

	MDataHandle hStrokeProfileScale = data.inputValue(aStrokeProfileScale);
	double strokeProfileScaleMin = hStrokeProfileScale.child(
	                                 aStrokeProfileScaleMin).asDouble();
	double strokeProfileScaleMax = hStrokeProfileScale.child(
	                                 aStrokeProfileScaleMax).asDouble();

	double pivotFraction = data.inputValue(aPivotFraction).asDouble();

	StrokeRotationSpec rotSpec;
	rotSpec.tiltRampAtt =  strokeNode::aBrushTiltRamp;
	rotSpec.bankRampAtt =  strokeNode::aBrushBankRamp;
	rotSpec.twistRampAtt =  strokeNode::aBrushTwistRamp;

	MDataHandle hRangeHandle = data.inputValue(aBrushTiltRange);
	rotSpec.tiltRampMin = hRangeHandle.child(aBrushTiltRangeMin).asAngle().asRadians();
	rotSpec.tiltRampMax = hRangeHandle.child(aBrushTiltRangeMax).asAngle().asRadians();

	hRangeHandle = data.inputValue(aBrushBankRange);
	rotSpec.bankRampMin = hRangeHandle.child(aBrushBankRangeMin).asAngle().asRadians();
	rotSpec.bankRampMax = hRangeHandle.child(aBrushBankRangeMax).asAngle().asRadians();

	hRangeHandle = data.inputValue(aBrushTwistRange);
	rotSpec.twistRampMin = hRangeHandle.child(aBrushTwistRangeMin).asAngle().asRadians();
	rotSpec.twistRampMax = hRangeHandle.child(aBrushTwistRangeMax).asAngle().asRadians();
	rotSpec.rampScope = StrokeRotationSpec::Scope(data.inputValue(aBrushRampScope).asShort());
	rotSpec.followStroke =  data.inputValue(aBrushFollowStroke).asBool();

	MObject thisObj = thisMObject();


	unsigned len = samplePoints.length();
	for (int i = 0; i < len; ++i)
	{
		// local (-1 to +1)
		MFloatPoint point = MFloatPoint(samplePoints[i]);
		MPointArray flowPoints;
		int flowCount = calcFlowPoints(point, grad, fullProjection, imat, numPoints, sampleDist,
		                               flowPoints,
		                               interp);




		if (flowCount > 1) // we can make a strokeGroup
		{
			MFnNurbsCurve curveFn;
			MFnNurbsCurveData dataCreator;
			MObject curveData = dataCreator.create( &st ); mser;
			MObject dCurve = curveFn.createWithEditPoints(
			                   flowPoints, 3, MFnNurbsCurve::kOpen, true, false, true, curveData, &st);
			double curveLength = curveFn.length(epsilon);
			const double startDist = 0.0;
			const double endDist = curveLength;


			unsigned strokeGroupSize = Stroke::createFromCurve(
			                             thisObj,
			                             dCurve,
			                             planeNormal,
			                             curveLength,
			                             startDist,
			                             endDist,
			                             pointDensity,
			                             liftLength,
			                             liftHeight,
			                             liftBias,
			                             strokeNode::aStrokeProfileRamp,
			                             strokeProfileScaleMin,
			                             strokeProfileScaleMax,
			                             rotSpec,
			                             backstroke,
			                             repeats,
			                             repeatOffset,
			                             repeatMirror,
			                             repeatOscillate,
			                             pivotFraction,
			                             strokes);
		}
	}

	setApproach(  strokes, approachDistance);

	int layerId = data.inputValue(aLayerId).asInt();

	// these will ultimately be mapped
	short brushId = data.inputValue(aBrushId).asShort();
	short paintId = data.inputValue(aPaintId).asShort();

	float matfloats[4][4];
	imat.get(matfloats);
	MMatrix mImat(matfloats);

	int i = 0;
	for (auto &citer : strokes)
	{
		strokeGeom g(i, *citer, brushId, paintId, layerId, false);
		g.setUV(mImat);
		geom->push_back(g);
		i++;
	}

	setBrushIds(data, geom);
	setPaintIds(data, geom);




	return MS::kSuccess;
}

void flowStroke::setBrushIds(MDataBlock &data, std::vector<strokeGeom> *geom) const
{

	MStatus st;
	MDataHandle hImageData = data.inputValue(aBrushIdImage, &st);

	if (st.error()) {return;}

	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);

	if (st.error()) {return;}

	cImgData *imageData = (cImgData *)fnImageData.data();
	const CImg<unsigned char> *image = imageData->fImg;
	int w = image->width();
	int h = image->height();

	if (!(w && h )) {return;}

	MObject thisObj = thisMObject();
	MRampAttribute remapRamp( thisObj, aBrushIdRemapRamp ); mser;
	const int2 &range = data.inputValue( aBrushIdRemapRange ).asInt2();
	int low = range[0];
	int high = range[1];
	int span = high - low;

	float normalizer = 1.0f / 255.0f;

	std::vector<strokeGeom>::iterator iter;
	for (iter = geom->begin(); iter !=  geom->end(); iter++)
	{
		float u, v;
		iter->getUV(u, v);
		float x, y;

		cImgUtils::toImageCoords(u, v, w, h, x, y);

		// int pixelValue = (*image)(int(x + 0.5), int(y + 0.5) , 0, 0);
		float value = float( image->atXY(int(x), int(y) , 0, 0)) * normalizer;
		// cerr << "Pixel Val: " << pixelValue << " - value " << value << endl;

		float remappedVal;
		remapRamp.getValueAtPosition( value, remappedVal, &st );
		if (st.error()) {
			remappedVal =  value;
		}
		int ival = int((remappedVal * span) + low);
		// cerr << "Normalized: " << value <<   " - remapped to: " << remappedVal << " - int val: " << ival <<  endl;
		ival = std::max(low, std::min(ival, high));
		iter->setBrushId(ival);
	}
}


void flowStroke::setPaintIds(MDataBlock &data, std::vector<strokeGeom> *geom) const
{
	// cerr << "flowStroke::setPaintIds" << endl;

	MStatus st;
	MDataHandle hImageData = data.inputValue(aPaintIdImage, &st);

	if (st.error()) {return;}

	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);

	if (st.error()) {return;}

	cImgData *imageData = (cImgData *)fnImageData.data();
	const CImg<unsigned char> *image = imageData->fImg;
	int w = image->width();
	int h = image->height();

	if (!(w && h )) {return;}

	// cerr << "geom->size():" << geom->size() << endl;
	std::vector<strokeGeom>::iterator iter;
	// cerr << "got iter" << endl;
	// cerr << "w:"  << w << "h:" << h << endl;

	for (iter = geom->begin(); iter !=  geom->end(); iter++)
	{
		float u, v;

		iter->getUV(u, v);
		// cerr << "u:"  << u << "v:" << v << endl;

		float x, y;
		// unsigned char val;
		cImgUtils::toImageCoords(u, v, w, h, x, y);
		// cerr << "x:"  << x << "y:" << y << endl;

		iter->setPaintId(short(image->atXY(int(x),  int(y), 0, 0)));

	}
	// cerr << "Done" << endl;


}


void flowStroke::postConstructor()
{
	// setExistWithoutInConnections(true);
	// setExistWithoutOutConnections(true);
}
