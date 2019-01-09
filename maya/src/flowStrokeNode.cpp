

#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>

#include <maya/MFnPluginData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>
#include <maya/MQuaternion.h>
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



MObject flowStroke::aFlowImage;


MObject flowStroke::aStrokeLengthImage;
MObject flowStroke::aStrokeLengthRemapRamp;
MObject flowStroke::aStrokeLengthRemapRange;

// MObject flowStroke::aInterpolation;
MObject flowStroke::aSamplePoints;
MObject flowStroke::aProjectionMatrix;
// MObject flowStroke::aChannel;
MObject flowStroke::aBlur;
MObject flowStroke::aRotation;
MObject flowStroke::aSampleDistance;
MObject flowStroke::aMaxCurvature;
MObject flowStroke::aSplitThreshold;
MObject flowStroke::aTrimThreshold;
MObject flowStroke::aBrushIdImage;
MObject flowStroke::aPaintIdImage;
MObject flowStroke::aBrushIdRemapRamp;
MObject flowStroke::aBrushIdRemapRange;

MObject flowStroke::aPaintIdRemapRamp;
MObject flowStroke::aPaintIdRemapRange;

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

	aFlowImage = tAttr.create("flowImage", "fim", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aFlowImage ); mser;


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


	aMaxCurvature = uAttr.create( "maxCurvature", "mxcv",
	                              MFnUnitAttribute::kAngle );
	uAttr.setDefault(MAngle(90, MAngle::kDegrees));
	uAttr.setHidden(false);
	uAttr.setStorable(true);
	uAttr.setReadable(true);
	st = addAttribute(aMaxCurvature);

	aSplitThreshold = uAttr.create( "splitThreshold", "sth",
	                                MFnUnitAttribute::kAngle );
	uAttr.setDefault(MAngle(-1, MAngle::kDegrees));
	uAttr.setHidden(false);
	uAttr.setStorable(true);
	uAttr.setReadable(true);
	st = addAttribute(aSplitThreshold);


	aTrimThreshold = uAttr.create( "trimThreshold", "tth",
	                               MFnUnitAttribute::kAngle );
	uAttr.setDefault(MAngle(-1, MAngle::kDegrees));
	uAttr.setHidden(false);
	uAttr.setStorable(true);
	uAttr.setReadable(true);
	st = addAttribute(aTrimThreshold);


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

	////////////////
	aStrokeLengthImage = tAttr.create("strokeLengthImage", "slim", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aStrokeLengthImage ); mser;

	aStrokeLengthRemapRamp  = MRampAttribute::createCurveRamp("strokeLengthRemapRamp",
	                          "slrp");
	st = addAttribute( aStrokeLengthRemapRamp ); mser;
	aStrokeLengthRemapRange = nAttr.create("strokeLengthRemapRange", "slrg",
	                                       MFnNumericData::k2Float);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aStrokeLengthRemapRange);
	////////////////

	// ////////////////
	// aCurvatureRamp  = MRampAttribute::createCurveRamp("aCrvatureRamp", "ctp");
	// st = addAttribute( aCurvatureRamp ); mser;

	// aCurvatureRangeMin = uAttr.create( "crvatureRangeMin", "ctrn",
	//                                    MFnUnitAttribute::kAngle );
	// uAttr.setDefault(MAngle(-180, MAngle::kDegrees));
	// aCurvatureRangeMax = uAttr.create( "crvatureRangeMax", "ctrx",
	//                                    MFnUnitAttribute::kAngle );
	// uAttr.setDefault(MAngle(180, MAngle::kDegrees));
	// aCurvatureRange = nAttr.create("crvatureRange", "ctr", aCurvatureRangeMin,
	//                                aCurvatureRangeMax);
	// nAttr.setHidden( false );
	// nAttr.setKeyable( true );
	// st = addAttribute(aCurvatureRange); mser;
	// ////////////////


	aBrushIdImage = tAttr.create("brushIdImage", "bidi", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aBrushIdImage ); mser;

	aBrushIdRemapRamp  = MRampAttribute::createCurveRamp("brushIdRemapRamp", "brrp");
	st = addAttribute( aBrushIdRemapRamp ); mser;

	aBrushIdRemapRange = nAttr.create("brushIdRemapRange", "brrg", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aBrushIdRemapRange);



	aPaintIdImage = tAttr.create("paintIdImage", "pidi", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aPaintIdImage ); mser;

	aPaintIdRemapRamp  = MRampAttribute::createCurveRamp("paintIdRemapRamp", "prrp");
	st = addAttribute( aPaintIdRemapRamp ); mser;

	aPaintIdRemapRange = nAttr.create("paintIdRemapRange", "prrg", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aPaintIdRemapRange);



	st = attributeAffects(aFlowImage, aOutput ); mser;
	st = attributeAffects(aStrokeLengthImage, aOutput);
	st = attributeAffects(aStrokeLengthRemapRamp, aOutput);
	st = attributeAffects(aStrokeLengthRemapRange, aOutput);

	// st = attributeAffects(aCurvatureRamp, aOutput ); mser;
	// st = attributeAffects(aCurvatureRange, aOutput ); mser;

	st = attributeAffects(aSamplePoints, aOutput ); mser;
	st = attributeAffects(aProjectionMatrix, aOutput ); mser;
	st = attributeAffects(aBlur, aOutput ); mser;
	st = attributeAffects(aMaxCurvature, aOutput ); mser;
	st = attributeAffects(aSplitThreshold, aOutput ); mser;
	st = attributeAffects(aTrimThreshold, aOutput ); mser;


	st = attributeAffects(aRotation, aOutput ); mser;
	st = attributeAffects(aSampleDistance, aOutput); mser;
	st = attributeAffects(aBrushRampScope, aOutput ); mser;

	st = attributeAffects(aBrushIdImage, aOutput);
	st = attributeAffects(aPaintIdImage, aOutput);


	st = attributeAffects(aPaintIdRemapRamp, aOutput);
	st = attributeAffects(aPaintIdRemapRange, aOutput);


	st = attributeAffects(aBrushIdRemapRamp, aOutput);
	st = attributeAffects(aBrushIdRemapRange, aOutput);

	st = attributeAffects(aApproachDistance, aOutput ); mser;

	return (MS::kSuccess );
}



MStatus flowStroke::getImageChannel(
  MDataBlock &data,
  MObject &attribute,
  cImgData::Channel channel,
  CImg<unsigned char> &result) const
{
	MStatus st;
	MDataHandle hImageData = data.inputValue(attribute, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st); msert;
	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *image = imageData->fImg;

	cImgUtils::getImageChannel(*image , channel, result);

	return MS::kSuccess;
}


MStatus flowStroke::getFloatRedChannel(
  MDataBlock &data,
  MObject &attribute,
  CImg<float> &result) const
{
	MStatus st;
	MDataHandle hImageData = data.inputValue(attribute, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st); msert;
	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *image = imageData->fImg;

	result = image->channel(0).get_normalize(0.0f, 1.0f);

	return MS::kSuccess;
}


MFloatMatrix getFullProjection(float angle,
                               const MFloatMatrix &projection)
{

	float cz = cos(angle);
	float sz = sin(angle);

	MFloatMatrix rot;
	rot.setToIdentity();
	rot[0][0] = cz;
	rot[0][1] = sz;
	rot[1][0] = -sz;
	rot[1][1] = cz;

	MFloatMatrix flip;
	flip.setToIdentity();
	flip[1][1] = -1;

	return flip * (rot * projection)  ;

}

int calcFlowPoints(const MFloatPoint &point,
                   const CImgList<float> &grad,
                   const CImg<float> &lengthImg,
                   const MFloatMatrix &mat,
                   const MFloatMatrix &imat,
                   float lengthRangeMin,
                   float lengthRangeMax,
                   MRampAttribute &lengthRemapRamp,
                   float gap,
                   double maxCurvature,
                   MPointArray &resultPoints)
{

	MFloatVector lastGradient(MFloatVector::zero) ;
	MFloatPoint p = point;

	float x, y;

	/* Calc stroke length */
	int lw = lengthImg.width();
	int lh = lengthImg.height();
	cImgUtils::toImageCoords(p, imat, lw, lh, x, y, false);
	float length = lengthImg.atXY(x, y);

	// cerr << "lengthRangeMax: " << lengthRangeMax << endl;
	// cerr << "lengthRangeMin: " << lengthRangeMin << endl;


	float lengthRange = lengthRangeMax - lengthRangeMin;
	float lengthResult;
	MStatus st;
	lengthRemapRamp.getValueAtPosition( length, lengthResult, &st ); mser;
	if (st.error()) {
		length = lengthRangeMax;
	}
	else {
		length = (lengthResult *  lengthRange) + lengthRangeMin;
	}
	// cerr << "length:" << length << endl;

	int w = grad(0).width();
	int h = grad(0).height();


	float curr_length = 0.0;
	resultPoints.append(MPoint(p));

	maxCurvature = maxCurvature * gap;
	do {

		cImgUtils::toImageCoords(p, imat, w, h, x, y, false);
		if (x < 0 || x > w || y < 0 || y > h) {
			break;
		}
		float gx = grad(0).linear_atXY(x, y);
		float	gy = grad(1).linear_atXY(x, y);
		MFloatVector gradient = (MFloatVector(gx, gy, 0)  * mat).normal();

		if (! lastGradient.isEquivalent(MFloatVector::zero)) {
			float angleBetween = gradient.angle(lastGradient);

			if (double(angleBetween) > maxCurvature) {
				// cerr << "angleBetween is greater" << angleBetween << endl;
				MVector cross = MVector( lastGradient ^ gradient ).normal();
				MQuaternion q(maxCurvature, cross);
				gradient = MFloatVector(MVector(lastGradient).rotateBy(q));
			}
		}


		p += (gradient * gap);
		resultPoints.append(MPoint(p));
		curr_length += gap;
		lastGradient = gradient;

	}
	while (curr_length < length) ;

	return resultPoints.length();
}



void flowStroke::setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                             double approachDist) const {
	for (auto iter = strokes.begin(); iter != strokes.end(); iter++) {
		(*iter)->setApproach(approachDist, approachDist);
	}
}

void flowStroke::trimOnAngle(double trimThreshold, const MVector &planeNormal,
                             MPointArray &flowPoints) const
{
	// MPointArray curr;
	int len = flowPoints.length();
	MVector lastVec; // start as zero

	double leftExtent  = 0;
	double rightExtent = 0;
	double accumAngle = 0;
	// curr.append(flowPoints[0]);

	// int newLen = len;
	int i;
	for (i = 1; i < len; i++) {
		MPoint lastPt = flowPoints[(i - 1)];
		MPoint thisPt = flowPoints[i];
		MVector thisVec = (thisPt - lastPt).normal();

		if (lastVec != MVector::zero) {
			// there is a last vec and a new vec, so test to see if we exceeded
			MQuaternion	q(lastVec, thisVec);
			double angle;
			MVector axis(MVector::zAxis);
			bool rotated = q.getAxisAngle(axis, angle);
			if (rotated) {
				double dir = (axis * planeNormal  < 0) ? -1.0 : 1.0 ;
				accumAngle += (dir * angle);
				if (accumAngle < leftExtent) {leftExtent = accumAngle;}
				if (accumAngle > rightExtent) {rightExtent = accumAngle;}

				if ((rightExtent - leftExtent) > trimThreshold)
				{
					break;
					// need to stop
					// flowPointsList.push_back(curr);
					// curr = MPointArray();
					// curr.append(flowPoints[i - 1]);

					// leftExtent  = 0;
					// rightExtent = 0;
					// accumAngle = 0;
				}
			}
		}

		// curr.append(flowPoints[i]);
		lastVec = thisVec;
	}
	cerr << "len:" << len << " i:" << i << endl;
	if ( i < len) {
		flowPoints.setLength(i);
	}
	// flowPointsList.push_back(curr);
}



void flowStroke::splitOnAngle(const MPointArray &flowPoints, double splitThreshold,
                              const MVector &planeNormal, std::vector<MPointArray> &flowPointsList) const
{
	MPointArray curr;
	int len = flowPoints.length();
	MVector lastVec; // start as zero

	double leftExtent  = 0;
	double rightExtent = 0;
	double accumAngle = 0;
	curr.append(flowPoints[0]);

	for (int i = 1; i < len; i++) {
		MPoint lastPt = flowPoints[(i - 1)];
		MPoint thisPt = flowPoints[i];
		MVector thisVec = (thisPt - lastPt).normal();

		if (lastVec != MVector::zero) {
			// there is a last vec and a new vec, so test to see if we exceeded
			MQuaternion	q(lastVec, thisVec);
			double angle;
			MVector axis(MVector::zAxis);
			bool rotated = q.getAxisAngle(axis, angle);
			if (rotated) {
				double dir = (axis * planeNormal  < 0) ? -1.0 : 1.0 ;
				accumAngle += (dir * angle);
				if (accumAngle < leftExtent) {leftExtent = accumAngle;}
				if (accumAngle > rightExtent) {rightExtent = accumAngle;}

				if ((rightExtent - leftExtent) > splitThreshold)
				{	// need to start a new array
					flowPointsList.push_back(curr);
					curr = MPointArray();
					curr.append(flowPoints[i - 1]);

					leftExtent  = 0;
					rightExtent = 0;
					accumAngle = 0;
				}
			}
		}

		curr.append(flowPoints[i]);
		lastVec = thisVec;
	}
	flowPointsList.push_back(curr);
}


MStatus flowStroke::generateStrokeGeometry(MDataBlock &data,
    std::vector < strokeGeom > *geom) const
{

	MStatus st;
	MObject thisObj = thisMObject();

	std::vector<std::unique_ptr <Stroke> > strokes;

	if (! data.inputValue(aActive).asBool()) {
		return MS:: kSuccess;
	}
	// cImgData::Channel channel = cImgData::Channel(data.inputValue(
	//                               aChannel).asShort());
	CImg<unsigned char> singleFlow;
	st = getImageChannel(data, flowStroke::aFlowImage, cImgData::kRed, singleFlow);
	if (st.error()) {
		return MS::kUnknownParameter;
	}


	double sampleDist =  data.inputValue(aSampleDistance).asDouble();
	if (sampleDist < 0.1)  { sampleDist = 0.1; }




	////////////////////// LENGTH ////////////////////
	CImg<float> singleLength;
	st = getFloatRedChannel(data, flowStroke::aStrokeLengthImage, singleLength);
	if (st.error()) {
		singleLength.assign(1, 1, 1, 1, 1.0f);
	}
	const float2 &lengthRange = data.inputValue( aStrokeLengthRemapRange ).asFloat2();
	float lengthRangeMin =   std::max(lengthRange[0], 0.01f);
	float lengthRangeMax = std::max(lengthRange[1], lengthRange[0]);
	MRampAttribute lengthRemapRamp( thisObj, aStrokeLengthRemapRamp ); mser;
	//////////////////////



	float blur = data.inputValue(aBlur).asFloat();
	float angle = float(data.inputValue(aRotation).asAngle().asRadians());

	MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();
	MFloatMatrix imat = projection.inverse();

	int w = singleFlow.width();
	int h = singleFlow.height();

	MDataHandle hPts = data.inputValue( aSamplePoints , &st);
	MObject dPts = hPts.data();
	const MVectorArray samplePoints = MFnVectorArrayData( dPts ).array( &st );
	CImgList<float> grad = singleFlow.get_gradient("xy", 0);

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

	/*	double lengthMult = data.inputValue(aStrokeLength).asDouble();
		if (lengthMult < 0.001) { length = 0.001; }
		double sampleDist =  data.inputValue(aSampleDistance).asDouble();*/
	// int numPoints = int(length / sampleDist) + 1;
	// if (numPoints < 2) { numPoints = 2; }
	// sampleDist = length / (numPoints - 1);

	MDataHandle hLift = data.inputValue(aLift);
	double liftLength = hLift.child(aLiftLength).asDouble();
	double liftHeight = hLift.child(aLiftHeight).asDouble();
	double liftBias = hLift.child(aLiftBias).asDouble();
	MVector lift(liftLength, liftHeight, liftBias);


	double approachDistance = data.inputValue(aApproachDistance).asDouble();

	Stroke::DirectionMethod strokeDirection = Stroke::DirectionMethod(data.inputValue(
	      aStrokeDirection).asShort());

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


	double countFactor = data.inputValue(aStrokeCountFactor).asDouble();
	int seed = data.inputValue(aSeed).asInt();
	srand48(seed);

	double maxCurvature = data.inputValue(aMaxCurvature).asAngle().asRadians();
	double splitThreshold = data.inputValue(aSplitThreshold).asAngle().asRadians();
	double trimThreshold = data.inputValue(aTrimThreshold).asAngle().asRadians();

	// MRampAttribute curvaturRemapRamp( thisObj, aCurvatureRamp ); mser;
	// MDataHandle hCurvatureRange = data.inputValue(aCurvatureRange);
	// double curvatureRangeMin = hCurvatureRange.child(
	//                              hCurvatureRangeMin).asAngle().asRadians();
	// double curvatureRangeMax = hCurvatureRange.child(
	//                              hCurvatureRangeMax).asAngle().asRadians();

	// cerr << "aMaxCurvature: " << maxCurvature << endl;

	unsigned len = samplePoints.length();
	// cerr << "flowStrokeNode-----" <<  endl;

	for (int i = 0; i < len; ++i)
	{
		if (drand48() > countFactor) {
			continue;
		}


		// local (-1 to +1)
		MFloatPoint point = MFloatPoint(samplePoints[i]);
		MPointArray flowPoints;
		int flowCount = calcFlowPoints(
		                  point,
		                  grad,
		                  singleLength,
		                  fullProjection,
		                  imat,
		                  lengthRangeMin,
		                  lengthRangeMax,
		                  lengthRemapRamp,
		                  sampleDist,
		                  maxCurvature,
		                  flowPoints);




		// cerr << "flowPoints" << flowPoints << endl;

		if (flowCount > 1) // we can make a strokeGroup
		{
			bool doTrim = trimThreshold > 0;
			if (doTrim && flowCount > 2)
			{
				trimOnAngle(trimThreshold, planeNormal, flowPoints);
			}
			flowCount = flowPoints.length();

			std::vector<MPointArray> flowPointsList;
			if (splitThreshold > 0 && (splitThreshold < trimThreshold || (!doTrim))
			    && flowCount > 2) {
				splitOnAngle(flowPoints, splitThreshold, planeNormal, flowPointsList);
			}
			else {
				flowPointsList.push_back(flowPoints);
			}

			// iterate
			std::vector<MPointArray>::const_iterator citer;
			for (citer = flowPointsList.begin(); citer != flowPointsList.end(); citer++)
			{

				MFnNurbsCurve curveFn;
				MFnNurbsCurveData dataCreator;
				MObject curveData = dataCreator.create( &st ); mser;
				MObject dCurve = curveFn.createWithEditPoints(
				                   (*citer), 3, MFnNurbsCurve::kOpen, true, false, true, curveData, &st);
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
				                             strokeDirection,
				                             repeats,
				                             repeatOffset,
				                             repeatMirror,
				                             repeatOscillate,
				                             pivotFraction,
				                             strokes);

			}
		}
	}

	// cerr << "--------" << endl;


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





	MObject thisObj = thisMObject();
	MRampAttribute remapRamp( thisObj, aBrushIdRemapRamp ); mser;
	const int2 &range = data.inputValue( aBrushIdRemapRange ).asInt2();
	int low = range[0];
	int high = range[1];
	int span = high - low;

	bool imageValid = false;
	MDataHandle hImageData = data.inputValue(aBrushIdImage, &st);

	if (! st.error()) {
		MObject dImageData = hImageData.data();
		MFnPluginData fnImageData( dImageData , &st);
		if (! st.error()) {
			cImgData *imageData = (cImgData *)fnImageData.data();
			const CImg<unsigned char> *image = imageData->fImg;
			int w = image->width();
			int h = image->height();
			if (w && h)
			{
				imageValid = true;
				float normalizer = 1.0f / 255.0f;

				std::vector<strokeGeom>::iterator iter;
				for (iter = geom->begin(); iter !=  geom->end(); iter++)
				{
					float u, v;
					iter->getUV(u, v);
					float x, y;
					cImgUtils::toImageCoords(u, v, w, h, x, y);
					float value = float( image->atXY(int(x), int(y) , 0, 0)) * normalizer;

					float remappedVal;
					remapRamp.getValueAtPosition( value, remappedVal, &st );
					if (st.error()) {
						remappedVal =  value;
					}
					int ival = int((remappedVal * span) + low);
					ival = std::max(low, std::min(ival, high));
					iter->setBrushId(ival);
				}
			}
		}
	}
	if (! imageValid) {
		// cerr << "high: " << high << endl;
		std::vector<strokeGeom>::iterator iter;
		for (iter = geom->begin(); iter !=  geom->end(); iter++)
		{
			iter->setBrushId(high);
		}
	}
}


void flowStroke::setPaintIds(MDataBlock &data, std::vector<strokeGeom> *geom) const
{

	MStatus st;


	MObject thisObj = thisMObject();
	MRampAttribute remapRamp( thisObj, aPaintIdRemapRamp ); mser;
	const int2 &range = data.inputValue( aPaintIdRemapRange ).asInt2();
	int low = range[0];
	int high = range[1];
	int span = high - low;


	MDataHandle hImageData = data.inputValue(aPaintIdImage, &st);

	if (st.error()) {return;}

	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);

	if (st.error()) {return;}

	cImgData *imageData = (cImgData *)fnImageData.data();
	// const CImg<unsigned char> *image = imageData->fImg;
	const CImg<float> image = imageData->fImg->get_normalize(0.0f, 1.0f);
	int w = image.width();
	int h = image.height();

	if (!(w && h )) {return;}

	// unsigned char minpixel;
	// unsigned char maxpixel = image->max_min(minpixel)	;
	// 	unsigned char pixelrange = maxpixel-minpixel;


	// float normalizer = 1.0f / float(pixelrange);
	std::vector<strokeGeom>::iterator iter;

	for (iter = geom->begin(); iter !=  geom->end(); iter++)
	{
		float u, v;
		iter->getUV(u, v);
		float x, y;
		cImgUtils::toImageCoords(u, v, w, h, x, y);

		float value = image.atXY(int(x), int(y) , 0, 0);

		// float value = float() * normalizer;
		float remappedVal;
		remapRamp.getValueAtPosition( value, remappedVal, &st );
		if (st.error()) {
			remappedVal =  value;
		}

		int ival = int((remappedVal * span) + low);
		// cerr << "remappedVal:" << remappedVal << " * span:" << span << " + low:" << low << " = " << ival << endl;
		ival = std::max(low, std::min(ival, high));
		iter->setPaintId(short(ival));
	}

}


void flowStroke::postConstructor()
{
	// setExistWithoutInConnections(true);
	// setExistWithoutOutConnections(true);
}
