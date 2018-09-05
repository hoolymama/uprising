#include <maya/MFnNurbsCurve.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <stroke.h>
#include <algorithm>
#include <vector>

#include <typeinfo>
#include "errorMacros.h"
#include "backstroke.h"
#include "mayaMath.h"

const double epsilon = 0.0001;
const double pi = 3.1415926535;
const double rad_to_deg = (180.0 / pi);
const double tau = 2.0 * pi;
const double half_pi = 0.5 * pi;

bool shouldMakeBackstroke(bool motherBackstroke, bool oscillate, int index )  {
	if ((index % 2) == 0) {
		return motherBackstroke ? !oscillate : oscillate;
	}
	else {
		return motherBackstroke;
	}
}

unsigned Stroke::factory(
  const MObject &thisObj,
  const MObject &dCurve,
  const MMatrix &inversePlaneMatrix,
  const MVector &planeNormal,
  double curveLength,
  double startDist,
  double endDist,
  double pointDensity,
  double liftLength,
  double liftHeight,
  double liftBias,
  const MObject &profileRampAttribute,
  double strokeProfileScaleMin,
  double strokeProfileScaleMax,
  const MObject &tiltRamp,
  const MObject &bankRamp,
  const MObject &twistRamp,
  Stroke::Scope brushRampScope,
  bool follow,
  bool backstroke,
  int repeats,
  double repeatOffset,
  bool repeatMirror,
  bool repeatOscillate,
  double pivotFraction,
  /*  short brushId,
    short paintId,*/
  std::vector<std::unique_ptr<Stroke> > &strokes

) {
	/*
	This factory provides a stroke or a backstroke
	*/

	std::unique_ptr<Stroke> stk;
	if (backstroke) {
		stk = std::make_unique<BackStroke>();
	}
	else {
		stk = std::make_unique<Stroke>();
	}

	stk->initialize(
	  dCurve,
	  planeNormal,
	  curveLength,
	  startDist,
	  endDist,
	  pointDensity,
	  liftLength,
	  liftBias
	  // , brushId, paintId
	);

	stk->setHeights(
	  thisObj,
	  profileRampAttribute,
	  strokeProfileScaleMin,
	  strokeProfileScaleMax,
	  liftHeight
	);

	stk->setRotations(
	  thisObj,
	  tiltRamp,
	  bankRamp,
	  twistRamp,
	  brushRampScope,
	  follow
	);

	stk->setPivot( thisObj,
	               pivotFraction,
	               startDist,
	               endDist);
	/*
	k will happen once (1) or twice (1 and -1) and its
	value will help determine the offset.
	*/
	unsigned count = 0;
	const Stroke &mother = *stk;
	if (stk->overlapsPlane(inversePlaneMatrix)) {
		strokes.push_back( std::move(stk) );
		count++;
	}

	std::vector<int> mirrorLoop = repeatMirror ?  std::vector<int> { -1, 1} :
	                              std::vector<int> { 1} ;

	for (int j = 0; j < repeats; ++j) {
		for (int k : mirrorLoop) {

			double offset = repeatOffset * (j + 1) * k;
			bool makeBackStroke = shouldMakeBackstroke(backstroke, repeatOscillate, j );

			std::unique_ptr<Stroke> rstk;
			if (makeBackStroke) {
				rstk = std::make_unique<BackStroke>();
			}
			else {
				rstk = std::make_unique<Stroke>();
			}
			rstk->offsetFrom(mother, offset);
			if (rstk->overlapsPlane(inversePlaneMatrix)) {
				strokes.push_back(std::move(rstk));
				count++;
			}
		}
	}
	return count;
}


void Stroke::offsetFrom(
  const Stroke &other,
  double offset) {

	m_planeNormal = other.planeNormal();
	m_targets = other.targets();
	m_profile = other.profile();
	m_pivot = other.pivot();
	m_follow = other.follow();
	// m_brushId = other.brushId();
	// m_paintId = other.paintId();

	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++) {
		MVector offsetVec = (iter->tangent() ^ m_planeNormal) * offset;
		iter->offsetBy(offsetVec);
	}
	setArcLength();
}

Stroke::Stroke()	:
	m_targets(),
	m_profile(),
	m_pivot(),
	m_arcLength(),
	m_approachDistStart(1),
	m_approachDistEnd(1),
	m_planeNormal(),
	m_follow(true)
{}

const MPoint &Stroke::pivot() const {
	return m_pivot;
}

bool Stroke::follow() const {
	return m_follow;
}

Stroke::~Stroke() {}

void Stroke::initialize(
  const MObject &curveObject ,
  const MVector &planeNormal,
  double curveLength,
  double startDist,
  double endDist,
  double density,
  double liftLength,
  double liftBias
  // , short brushId, short paintId
)
{

	m_planeNormal = planeNormal;
	double liftOffset  = liftLength - liftBias;
	double startLiftDist = startDist - liftOffset;
	double startBiasDist = startDist + liftBias;

	double endLiftDist = endDist + liftOffset;
	double endBiasDist = endDist - liftBias;

	if (endBiasDist < startBiasDist) {
		double centerDist = (endDist + startDist) * 0.5;
		endBiasDist = centerDist + 0.1;
		startBiasDist = centerDist - 0.1;
	}


	Target startLiftTarget = Target(curveObject, curveLength, startDist, endDist,
	                                startLiftDist );
	m_targets.push_back(startLiftTarget);

	double contactDist = endBiasDist - startBiasDist;

	unsigned numPoints = unsigned(density * contactDist);
	if (numPoints < 2) { numPoints = 2; }
	double recip = 1.0 / (numPoints - 1);

	for (unsigned i = 0; i < numPoints; i++) {
		double fraction =  (double(i) * recip);
		double dist = startBiasDist + (fraction * contactDist);
		Target target = Target(curveObject, curveLength, startDist, endDist, dist );
		m_targets.push_back(target);
	}

	Target endLiftTarget = Target(curveObject, curveLength, startDist, endDist,
	                              endLiftDist );
	m_targets.push_back(endLiftTarget);

	setArcLength();

	// m_brushId = brushId;
	// m_paintId = paintId;

}


void Stroke::setHeights( const MObject &thisObj,
                         const MObject &profileRampAttribute,
                         double  strokeProfileScaleMin,
                         double strokeProfileScaleMax,
                         double liftHeight) {

	// heights are backwards in a backstroke
	MDoubleArray vals;
	getStrokeFractions(vals);

	unsigned nVals = vals.length();
	m_profile.setLength(nVals);

	doRampLookup(thisObj, profileRampAttribute, vals, m_profile, 0.0, 1.0,
	             strokeProfileScaleMin,
	             strokeProfileScaleMax );

	m_profile[0] = liftHeight;
	m_profile[(nVals - 1)] = liftHeight;
}

void Stroke::setPivot(
  const MObject &curveObject,
  double fraction,
  double startDist,
  double endDist)
{
	MFnNurbsCurve curveFn(curveObject);
	double dist = startDist + (fraction * (endDist - startDist) );
	double param = curveFn.findParamFromLength(dist);
	curveFn.getPointAtParam(param, m_pivot, MSpace::kObject);
}

void Stroke::setApproach(double start, double end)  {
	m_approachDistStart = start;
	m_approachDistEnd = end;
}

void Stroke::getTravelStrokeFractions(MDoubleArray &result) const {
	std::vector<Target>::const_iterator citer = m_targets.begin();
	for (; citer != m_targets.end(); citer++) {
		result.append(citer->strokeFraction());
	}
}

void Stroke::getStrokeFractions(MDoubleArray &result) const {
	std::vector<Target>::const_iterator citer = m_targets.begin();
	for (; citer != m_targets.end(); citer++) {
		result.append(citer->strokeFraction());
	}
}

void Stroke::getCurveFractions(MDoubleArray &result) const {
	std::vector<Target>::const_iterator citer = m_targets.begin();
	for (; citer != m_targets.end(); citer++) {
		result.append(citer->curveFraction());
	}
}

void Stroke::appendTargets(MMatrixArray &result) const {
	std::vector<Target>::const_iterator citer;
	// cerr << "m_follow: " << m_follow << endl;
	unsigned i = 0;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++, i++) {
		result.append(citer->matrix(m_planeNormal, m_profile[i], false, m_follow));
	}
}

void Stroke::appendTangents(MVectorArray &result) const {
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(citer->tangent());
	}
}

void Stroke::appendPoints(MVectorArray &result) const {
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(MVector(citer->curvePoint()));
	}
}

void Stroke::getApproachTargets( MMatrix &startApproach,
                                 MMatrix &endApproach) const {
	unsigned num = m_profile.length();


	double startHeight =  m_profile[0] + m_approachDistStart;
	double endHeight =  m_profile[(num - 1)] + m_approachDistEnd;
	// std::vector<Target>::const_iterator citer = m_targets.begin();
	startApproach = m_targets.front().matrix(m_planeNormal, startHeight, false, m_follow);
	endApproach = m_targets.back().matrix(m_planeNormal, endHeight, false, m_follow);
}

short Stroke::direction() const {
	return 1;
}


void Stroke::setRotations(
  const MObject &thisObj,
  const MObject &tiltRamp,
  const MObject &bankRamp,
  const MObject &twistRamp,
  Stroke::Scope rampScope,
  bool follow
) {
	m_follow = follow;

	MDoubleArray sampleVals;
	if (rampScope ==  Stroke::kStroke) {
		getStrokeFractions(sampleVals);
	}
	else if (rampScope ==  Stroke::kTravelStroke) {
		getTravelStrokeFractions(sampleVals);
	}
	else {   // curve
		getCurveFractions(sampleVals);
	}
	unsigned nVals = sampleVals.length();
	MDoubleArray outTilt(nVals);
	MDoubleArray outBank(nVals);
	MDoubleArray outTwist(nVals);



	doRampLookup(thisObj, tiltRamp, sampleVals, outTilt, 0.0, 1.0, -half_pi, half_pi );
	doRampLookup(thisObj, bankRamp, sampleVals, outBank, 0.0, 1.0, -half_pi, half_pi );
	doRampLookup(thisObj, twistRamp, sampleVals, outTwist, 0.0, 1.0, -pi, pi );

	std::vector<Target>::iterator iter = m_targets.begin();
	unsigned i = 0;
	for (; iter  != m_targets.end(); iter++, i++) {
		iter->setRotation(outTilt[i], outBank[i], outTwist[i]);
	}
}

bool Stroke::overlapsPlane(const MMatrix &inversePlaneMatrix) const {
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		MPoint p = citer->curvePoint() * inversePlaneMatrix;
		if (p.x > -1 && p.x < 1 && p.y > -1 && p.y < 1) {
			return true;
		}
	}
	return false;
}

unsigned Stroke::length() const {
	return m_targets.size();
}

void Stroke::setArcLength() {
	m_arcLength = 0;
	std::vector<Target>::const_iterator previter = std::next(m_targets.begin());
	std::vector<Target>::const_iterator iter = std::next(previter);
	std::vector<Target>::const_iterator enditer = std::prev(m_targets.end());
	unsigned i = 0;
	for ( ; iter != enditer; iter++, previter++, i++) {
		m_arcLength += iter->curvePoint().distanceTo(previter->curvePoint());
	}
}

const std::vector<Target> &Stroke::targets() const {
	return m_targets;
}

const MVector &Stroke::planeNormal() const {
	return m_planeNormal;
}

const MDoubleArray &Stroke::profile() const {
	return m_profile;
}

double Stroke::arcLength() const {
	return m_arcLength;
}


// short Stroke::brushId() const {
// 	return m_brushId;
// }
// short Stroke::paintId() const {
// 	return m_paintId;
// }

// void setApproach()


// MStatus calcBrushMatrixStroke(const MVector &position, const MVector &tangent,
//                               const MVector &planeNormal,
//                               const MVector &brushRotate, MMatrix &result,
//                               bool isBackstroke) {

// 	MMatrix mat =  mayaMath::matFromAim(position, tangent, planeNormal, mayaMath::yAxis ,
// 	                                    mayaMath::zAxisNeg);

// 	MTransformationMatrix tmat = MTransformationMatrix::identity;
// 	double3 rot;
// 	rot[0] = isBackstroke ? -brushRotate.x : brushRotate.x;
// 	rot[1] = brushRotate.y;
// 	rot[2] = brushRotate.z;
// 	tmat.setRotation(rot, MTransformationMatrix::kZYX);
// 	result =  tmat.asMatrix() * mat;
// 	return MS::kSuccess;
// }


// MStatus calcBrushMatrixWorld(const MVector &position,
//                              const MVector &tangent,
//                              const MVector &planeNormal,
//                              const MVector &brushRotate,
//                              MMatrix &result,
//                              bool isBackstroke) {

// 	MMatrix mat =  mayaMath::matFromAim(MVector::zero, MVector::xNegAxis, MVector::zAxis,
// 	                                    mayaMath::yAxis ,
// 	                                    mayaMath::zAxisNeg);


// 	// tilt it along the direction of travel
// 	MVector side(planeNormal ^ tangent);

// 	double tilt = isBackstroke ? -brushRotate.x : brushRotate.x;
// 	double bank = brushRotate.y;
// 	double twist = brushRotate.z;

// 	MMatrix twistMat = MQuaternion(twist, planeNormal).asMatrix();
// 	MMatrix bankMat = MQuaternion(bank, tangent).asMatrix();
// 	MMatrix tiltMat = MQuaternion(tilt, side).asMatrix();

// 	result = mat * twistMat * bankMat * tiltMat;
// 	result[3][0] = position.x;
// 	result[3][1] = position.y;
// 	result[3][2] = position.z;


// 	return MS::kSuccess;
// }



// void Stroke::appendTargets(MMatrixArray &result) const {
// 	std::vector<Target>::const_iterator citer;
// 	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
// 		result.append(citer->matrix());
// 	}
// }

// void Stroke::appendTangents(MVectorArray &result) const {
// 	std::vector<Target>::const_iterator citer;
// 	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
// 		result.append(citer->tangent());
// 	}
// }

// void Stroke::appendPoints(MVectorArray &result) const {
// 	std::vector<Target>::const_iterator citer;
// 	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
// 		result.append(MVector(citer->curvePoint()));
// 	}
// }



/*
The two points that describe entry and exit at the boundaries of the stroke
*/

// MStatus calcBoundaryPositions(const MPoint &curvePoint, const MVector &tangent,
//                               const MVector &planeNormal,
//                               const Brush &brush, MVectorArray &result) {
// 	result = MVectorArray(2, MVector::zero);
// 	result[0] = curvePoint + ((tangent * brush.liftLength) + (planeNormal *
// 	                          brush.liftHeight));
// 	result[1] = curvePoint - (tangent * brush.liftBias);
// 	return MS::kSuccess;
// }





// MStatus calcBrushMatrix(const MVector &position, const MVector &tangent,
//                         const MVector &planeNormal,
//                         const MVector &brushRotate, MMatrix &result,
//                         bool follow, bool isBackstroke = false) {

// 	if (follow) {
// 		return calcBrushMatrixStroke(position, tangent, planeNormal, brushRotate,
// 		                             result, isBackstroke);
// 	}


// 	return calcBrushMatrixWorld(position , tangent, planeNormal, brushRotate,
// 	                            result, isBackstroke);
// }


// MVector Stroke::getLiftStartTarget(const MObject &curveObject) {
// 	MFnNurbsCurve curveFn(curveObject);
// 	liftOffset  = m_liftBias - m_liftLength;
// 	startLiftDist = m_startDist - liftOffset;
// 	if (startLiftDist < 0) {

// 	}

// }


// MStatus Stroke::generateTargets(const MObject &curveObject) {
// 	MFnNurbsCurve curveFn(curveObject);
// 	double curveLength = curveFn.length(epsilon);


// 	liftOffset  = m_liftBias - m_liftLength;
// 	startLiftDist = m_startDist - liftOffset;
// 	startBiasDist = m_startDist + m_liftBias;
// 	if (startLiftDist < 0) {
// 		// find point back along tangent

// 	}
// 	else {
// 		// find point on curve
// 	}



// 	endLiftDist = m_endDist + liftOffset;
// 	endBiasDist = m_endDist - m_liftBias;


// 	m_curvePoints.append(getStartLiftPoint(MFnNurbsCurve & curveObject, )

// 	if (startLiftDist < 0) {

// }

// {
// double absCurveDist = fabs(endDist - startDist);

// unsigned numPoints = unsigned(density * absCurveDist);

// if (backstroke) {
// 	double tmp = startDist;
// 	startDist = endDist;
// 	endDist = tmp;
// }

// double curveSpan = endDist - startDist;

// MFnNurbsCurve curveFn(curveObject, &st);


// if (numPoints < 2) { numPoints = 2; }
// double recip = 1.0 / (numPoints - 1);

// // get an array of params
// MDoubleArray params(numPoints);
// for (unsigned i = 0; i < numPoints; i++) {
// 	double fraction =  (double(i) * recip);
// 	double dist = startDist + (fraction * curveSpan);
// 	double param = curveFn.findParamFromLength(dist, &st); er;
// 	params.set(param, i);
// }

// MPoint lastPoint;

// MPoint curvePoint;
// MMatrix brushMatrix;


// for (unsigned i = 0; i < numPoints; i++) {

// 	st = curveFn.getPointAtParam(params[i], curvePoint, MSpace::kObject);

// 	// get the flat tangent
// 	MVector curveTangent = curveFn.tangent(params[i]).normal();

// 	MVector tangent = (planeNormal ^ (curveTangent ^ planeNormal)).normal();
// 	if (backstroke) {
// 		tangent = -tangent;
// 	}

// 	if (i == 0) {

// 		MVectorArray boundaryPositions;
// 		// MVector boundaryTangent = backstroke ? tangent : -tangent;
// 		st = calcBoundaryPositions(curvePoint, -tangent, planeNormal, brush ,
// 		                           boundaryPositions);
// 		er;



// 		st = calcBrushMatrix(boundaryPositions[0] , curveTangent, planeNormal, brushRotate,
// 		                     brushMatrix, follow, backstroke);



// 		m_targets.append(brushMatrix);
// 		st = calcBrushMatrix(boundaryPositions[1], curveTangent, planeNormal, brushRotate,
// 		                     brushMatrix, follow, backstroke);

// 		m_targets.append(brushMatrix);
// 		m_tangents.append(tangent);
// 		m_tangents.append(tangent);

// 	}
// 	else if (i == (numPoints - 1))  {
// 		MVectorArray boundaryPositions;
// 		// MVector boundaryTangent = backstroke ? -tangent : tangent;
// 		st = calcBoundaryPositions(curvePoint, tangent, planeNormal, brush,
// 		                           boundaryPositions);
// 		er;

// 		st = calcBrushMatrix(boundaryPositions[1], curveTangent, planeNormal, brushRotate,
// 		                     brushMatrix, follow, backstroke);
// 		m_targets.append(brushMatrix);

// 		st = calcBrushMatrix(boundaryPositions[0], curveTangent, planeNormal, brushRotate,
// 		                     brushMatrix, follow, backstroke);
// 		m_targets.append(brushMatrix);
// 		m_tangents.append(tangent);
// 		m_tangents.append(tangent);
// 	}
// 	else {
// 		st = calcBrushMatrix(curvePoint, curveTangent, planeNormal, brushRotate,  brushMatrix,
// 		                     follow,
// 		                     backstroke);
// 		m_targets.append(brushMatrix);
// 		m_tangents.append(tangent);
// 	}

// 	if (i != 0) {
// 		m_arcLength += lastPoint.distanceTo(curvePoint);
// 	}
// 	lastPoint = curvePoint;
// }

// For now, set pivot halfway along stroke.

// double dist = startDist + (pivotFraction * curveSpan );
// double param = curveFn.findParamFromLength(dist, &st); er;
// st = curveFn.getPointAtParam(param, m_pivot, MSpace::kObject);
// }



// Stroke::Stroke(
//   const Stroke & mother,
//   double offset,
//   double advance,
//   bool reverse,
//   const MVector & planeNormal
// )	:
// m_curveId(mother.curveId()),
// m_targets(),
// m_tangents(),
// m_brush(mother.brush()),
// m_paint(mother.paint()),
// m_rotation(mother.rotation()),
// m_translation(mother.translation()),
// m_pivot(mother.pivot()),
// m_brushRotate(mother.brushRotate()),
// m_follow(mother.follow()),
// m_forceDip(false),
// m_arcLength(),
// m_approachDistStart(1),
// m_approachDistEnd(1),
// m_isBackstroke(false)
// {
// 	MStatus st;
// 	// m_arcLength = endDist - startDist;

// 	if (mother.isBackstroke()) {
// 		m_isBackstroke = !reverse;
// 	}
// 	else {
// 		m_isBackstroke = reverse;
// 	}


// 	unsigned numPoints = mother.targets().length();
// 	MPoint lastPoint;
// 	m_arcLength = 0.0;
// 	MPoint thisPoint;
// 	MMatrix brushMatrix;

// 	for (int i = 0; i < numPoints; ++i)
// 	{
// 		int j = reverse ? numPoints - (i + 1) : i;


// 		const MMatrix &mm = mother.targets()[j];

// 		MVector curveTangent = mother.isBackstroke() ?  -mother.tangents()[j] :
// 		                       mother.tangents()[j];
// 		// const MVector &tangent = mother.tangents()[j];
// 		MVector tangent = reverse ? -mother.tangents()[j]  : mother.tangents()[j];


// 		MVector advance_vec = MVector::zero;
// 		if (i < 2) {
// 			advance_vec = tangent * advance;
// 			// if (m_isBackstroke) {
// 			// 	advance_vec = advance_vec * -1;
// 			// }
// 		}


// 		MVector offsetVec = ((curveTangent ^ planeNormal) * offset) - advance_vec;
// 		thisPoint = MPoint(mm[3][0], mm[3][1], mm[3][2]) + offsetVec;

// 		st = calcBrushMatrix(thisPoint, curveTangent, planeNormal, m_brushRotate,
// 		                     brushMatrix, m_follow, m_isBackstroke);

// 		m_targets.append(brushMatrix);



// 		// if (m_isBackstroke) {
// 		m_tangents.append(tangent);
// 		// }
// 		// else {
// 		// 	m_tangents.append(tangent);
// 		// }


// 		if (i > 1 && i < numPoints - 1) {
// 			m_arcLength += lastPoint.distanceTo(thisPoint);
// 		}
// 		lastPoint = thisPoint;
// 	}
// }




// const MMatrixArray & Stroke::targets() const {
// 	return m_targets;
// }
// const MVectorArray & Stroke::tangents() const {
// 	return m_tangents;
// }

// const Brush & Stroke::brush() const {
// 	return m_brush;
// }
// const Paint & Stroke::paint() const {
// 	return m_paint;
// }

// const MPoint & Stroke::pivot() const {
// 	return m_pivot;
// }

// const MVector & Stroke::brushRotate() const {
// 	return m_brushRotate;
// }

// double Stroke::arcLength() const {
// 	return m_arcLength;
// }
// double Stroke::rotation() const {
// 	return m_rotation;
// }
// double Stroke::translation() const {
// 	return m_translation;
// }

// bool Stroke::follow() const {
// 	return m_follow;
// }

// bool  Stroke::forceDip() const {
// 	return m_forceDip;
// }

// bool Stroke::isBackstroke() const {
// 	return m_isBackstroke;
// }

// unsigned  Stroke::curveId() const {
// 	return m_curveId;
// }

// void Stroke::setApproach(double start, double end)  {
// 	m_approachDistStart = start;
// 	m_approachDistEnd = end;
// }

// double Stroke::approachStart() const {
// 	return m_approachDistStart;
// }

// double Stroke::approachEnd() const {
// 	return m_approachDistEnd;
// }

// void Stroke::getPivotUVs(const MMatrix & inversePlaneMatrix, float & u, float & v) const {
// 	MPoint p = ((m_pivot * inversePlaneMatrix) * 0.5) + MVector(0.5, 0.5, 0.0);
// 	u = p.x;
// 	v = p.y;
// }


// bool Stroke::overlapsPlane(const MMatrix & inversePlaneMatrix) const {
// 	unsigned len = m_targets.length();
// 	for (int i = 0; i < len; ++i)
// 	{
// 		MPoint p = MPoint(m_targets[i][3][0], m_targets[i][3][1],
// 		                  m_targets[i][3][2]) * inversePlaneMatrix;
// 		if (p.x > -1 && p.x < 1 && p.y > -1 && p.y < 1) {
// 			return true;
// 		}
// 	}
// 	return false;
// }

// void Stroke::rotate(float rotation, const MVector & axis) {
// 	double  rotateAmount = rotation * m_rotation;
// 	MMatrix mat = MQuaternion(rotateAmount, axis).asMatrix();
// 	unsigned len = m_targets.length();
// 	MMatrix pivotMat;
// 	pivotMat[3][0] = m_pivot.x;
// 	pivotMat[3][1] = m_pivot.y;
// 	pivotMat[3][2] = m_pivot.z;

// 	for (int i = 0; i < len; ++i)
// 	{
// 		m_targets[i] = ((m_targets[i] * pivotMat.inverse()) * mat) * pivotMat ;
// 	}
// }


// void Stroke::translate(const MFloatVector & translation, const MVector & planeNormal) {
// 	// rotation to planNormal.
// 	MMatrix mat = MQuaternion(MVector::zAxis, planeNormal).asMatrix();
// 	MVector trans = (MVector(translation) * m_translation) * mat;
// 	MMatrix transMat;
// 	transMat[3][0] = trans.x;
// 	transMat[3][1] = trans.y;
// 	transMat[3][2] = trans.z;

// 	unsigned len = m_targets.length();
// 	for (int i = 0; i < len; ++i)
// 	{
// 		m_targets[i]  = m_targets[i] + transMat ;
// 	}
// }
