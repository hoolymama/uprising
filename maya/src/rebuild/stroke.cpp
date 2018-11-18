#include <maya/MFnNurbsCurve.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <stroke.h>
#include <algorithm>
#include <vector>

#include <typeinfo>
#include "errorMacros.h"
#include "mayaMath.h"

const double epsilon = 0.0001;
// const double pi = 3.1415926535;
// const double rad_to_deg = (180.0 / pi);
// const double tau = 2.0 * pi;
// const double half_pi = 0.5 * pi;


unsigned Stroke::create(
  const MObject &thisObj,
  const MObject &dCurve,
  double curveLength,
  double startDist,
  double endDist,
  double entryLength,
  double exitLength,
  double pointDensity,
  const StrokeRotationSpec &rotSpec,
  const StrokeRepeatSpec &repeatSpec,
  DirectionMethod strokeDirection,
  double pivotParam,
  std::vector<Stroke> &strokes

) {
	/* This is the motherstroke. Determine whether forward or back */
	bool backstroke = shouldMakeBackstroke(dCurve, startDist, endDist, strokeDirection);

	if (backstroke) {
		std::swap(startDist, endDist);
	}

	int repeatId = 0;
	Stroke stk(
	  dCurve,
	  curveLength,
	  startDist,
	  endDist,
	  entryLength,
	  exitLength,
	  pointDensity,
	  repeatId++);

	stk.setRotations(thisObj, rotSpec, backstroke);
	stk.setPivot(dCurve, pivotParam, startDist, endDist);

	strokes.push_back(stk);

	const Stroke &mother = stk;

	for (int j = 0; j < repeatSpec.repeats; ++j) {
		bool reverse = false;
		bool repeatIsBackstroke = backstroke;
		if (repeatSpec.oscillate && (j % 2 == 0)) {
			reverse = true;
			repeatIsBackstroke = !backstroke;
		}

		double offset = repeatSpec.offset * (j + 1);
		stk = Stroke(mother, offset, reverse, repeatId++);
		stk.setRotations(thisObj, rotSpec, backstroke);
		strokes.push_back(stk);

		if (repeatSpec.mirror) {
			stk = Stroke(mother, -offset, reverse, repeatId++);
			stk.setRotations(thisObj, rotSpec, backstroke);
			strokes.push_back(stk);
		}
	}
	return 1 + repeatSpec.count() ;
}


Stroke::Stroke()	:
	m_targets(),
	m_pivot(),
	m_arcLength(0),
	m_entryLength(),
	m_exitLength(),
	m_repeatId() {}


Stroke::Stroke(
  const MObject &curveObject ,
  double curveLength,
  double startDist,
  double endDist,
  double entryLength,
  double exitLength,
  double density,
  int repeatId)	:
	m_targets(),
	m_pivot(),
	m_arcLength(),
	m_entryLength(entryLength),
	m_exitLength(exitLength),
	m_repeatId(repeatId)

{

	double strokeLength = endDist - startDist;
	unsigned numPoints = unsigned(density * strokeLength);
	if (numPoints < 2) { numPoints = 2; }
	double gap = strokeLength / (numPoints - 1) ;

	for (unsigned i = 0; i < numPoints; i++) {
		double dist = startDist + (i * gap);
		Target target = Target(curveObject, startDist, endDist, dist, curveLength);
		m_targets.push_back(target);
	}
	setArcLength();
}

/* Copy constructor */
Stroke::Stroke(
  const Stroke &other,
  double offset,
  bool reverse,
  int repeatId):
	m_repeatId(repeatId)
{

	unsigned len = other.targets().size();
	// m_targets.setLength(len)
	m_repeatId = repeatId;
	m_pivot = other.pivot();

	if (reverse)
	{
		std::vector<Target>::const_reverse_iterator citer;

		for (citer = other.targets().rbegin() ; citer != other.targets().rend(); citer++) {
			m_targets.push_back(*citer);
		}
	}
	else {
		std::vector<Target>::const_iterator citer;
		for (citer = other.targets().begin() ; citer != other.targets().end(); citer++) {
			m_targets.push_back(*citer);
		}
	}

	if (fabs(offset) > epsilon) {
		std::vector<Target>::iterator iter;
		for (iter = m_targets.begin(); iter != m_targets.end(); iter++) {
			MVector offsetVec = (iter->tangent() ^ MVector::zAxis) * offset;
			iter->offsetBy(offsetVec);
		}
		setArcLength();
	}
	else {
		m_arcLength = other.arcLength();
	}
}


const MPoint &Stroke::pivot() const {
	return m_pivot;
}

Stroke::~Stroke() {}


void Stroke::setRotations(
  const MObject &thisObj,
  const StrokeRotationSpec &rotSpec,
  bool backstroke)
{
	bool follow = rotSpec.followStroke;

	MDoubleArray sampleVals;
	if (rotSpec.rampScope == StrokeRotationSpec::kCurve) {
		this->getCurveParams(sampleVals);
	}
	else
	{
		this->getParams(sampleVals);
		if (rotSpec.rampScope ==  StrokeRotationSpec::kTravelStroke && backstroke)
		{	// reverse them
			reverseArray(sampleVals);
		}
	}

	unsigned nVals = sampleVals.length();
	MDoubleArray outTilt(nVals);
	MDoubleArray outBank(nVals);
	MDoubleArray outTwist(nVals);

	const double &tiltMin = rotSpec.tiltRampMin;
	const double &tiltMax = rotSpec.tiltRampMax;
	const double &bankMin = rotSpec.bankRampMin;
	const double &bankMax = rotSpec.bankRampMax;
	const double &twistMin = rotSpec.twistRampMin;
	const double &twistMax = rotSpec.twistRampMax;

	doRampLookup(thisObj, rotSpec.tiltRampAtt, sampleVals, outTilt, 0.0, 1.0, tiltMin,
	             tiltMax );
	doRampLookup(thisObj, rotSpec.bankRampAtt, sampleVals, outBank, 0.0, 1.0, bankMin,
	             bankMax );
	doRampLookup(thisObj, rotSpec.twistRampAtt, sampleVals, outTwist, 0.0, 1.0, twistMin,
	             twistMax );

	std::vector<Target>::iterator iter = m_targets.begin();
	unsigned i = 0;
	for (; iter != m_targets.end(); iter++, i++) {
		iter->setRotation(outTilt[i], outBank[i], outTwist[i], follow, backstroke );
	}
}

void Stroke::setArcLength() {
	m_arcLength = 0;
	std::vector<Target>::const_iterator previter = m_targets.begin();
	std::vector<Target>::const_iterator iter = std::next(previter);
	std::vector<Target>::const_iterator enditer = m_targets.end();
	unsigned i = 0;
	for ( ; iter != enditer; iter++, previter++, i++) {
		m_arcLength += iter->distanceTo(*previter);
	}
}

void Stroke::setPivot(
  const MObject &curveObject,
  double pivotParam,
  double startDist,
  double endDist)
{
	MFnNurbsCurve curveFn(curveObject);
	double dist = startDist + (pivotParam * (endDist - startDist) );
	double crvParam = curveFn.findParamFromLength(dist);
	curveFn.getPointAtParam(crvParam, m_pivot, MSpace::kWorld);
}


void Stroke::getParams(MDoubleArray &result) const {
	std::vector<Target>::const_iterator citer = m_targets.begin();
	for (; citer != m_targets.end(); citer++) {
		result.append(citer->param());
	}
}

void Stroke::getCurveParams(MDoubleArray &result) const {
	std::vector<Target>::const_iterator citer = m_targets.begin();
	for (; citer != m_targets.end(); citer++) {
		result.append(citer->curveParam());
	}
}

void Stroke::appendTargets(MMatrixArray &result) const {
	std::vector<Target>::const_iterator citer;
	unsigned i = 0;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++, i++) {
		result.append(citer->matrix() );
	}
}

void Stroke::appendTangents(MVectorArray &result) const {
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(citer->tangent());
	}
}



const std::vector<Target> &Stroke::targets() const {
	return m_targets;
}

const double &Stroke::arcLength() const {
	return m_arcLength;
}

unsigned Stroke::size() const {
	return m_targets.size();
}

int Stroke::repeatId() const {
	return m_repeatId;
}

///////// TO ADD ////////


int Stroke::id() const
{
	return m_id;
}
int Stroke::parentId() const
{
	return m_parentId;
}


void Stroke::setParentId(int parentId)
{
	m_parentId = parentId;
}



int Stroke::layerId() const
{
	return m_layerId;
}


int Stroke::brushId() const {
	return m_brushId;
}
int Stroke::paintId() const {
	return m_paintId;
}

const MIntArray &Stroke::sortStack() const {
	return m_sortStack;
}

void Stroke::clearSortStack()
{
	m_sortStack.clear();
}


void Stroke::setBrushId(int val) {
	m_brushId = val;
}
void Stroke::setPaintId(int val) {
	m_paintId = val;
}

void Stroke::setSortColor(const MFloatVector &color)
{
	m_sortColor = color;
}
void Stroke::setFilterColor(const MFloatVector &color)
{
	m_filterColor = color;
}


void Stroke::appendIdToSortStack(bool ascending)
{
	int val = ascending ? m_id : -m_id;
	m_sortStack.append(val);
}

void Stroke::appendParentIdToSortStack(bool ascending)
{
	int val = ascending ? m_parentId : -m_parentId;
	m_sortStack.append(val);
}

void Stroke::appendBrushIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_brushId) : -int(m_brushId);
	m_sortStack.append(val);
}

void Stroke::appendPaintIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_paintId) : -int(m_paintId);
	m_sortStack.append(val);
}

void Stroke::appendLayerIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_layerId) : -int(m_layerId);
	m_sortStack.append(val);
}

void Stroke::appendRepeatIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_repeatId) : -int(m_repeatId);
	m_sortStack.append(val);
}

void Stroke::appendCustomBrushIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_customBrushId) : -int(m_customBrushId);
	m_sortStack.append(val);
}

void Stroke::appendCustomPaintIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_customPaintId) : -int(m_customPaintId);
	m_sortStack.append(val);
}

void Stroke::appendMapRedIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_sortColor.x * 256) : -int(m_sortColor.x * 256);
	m_sortStack.append(val);
}
void Stroke::appendMapGreenIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_sortColor.y * 256) : -int(m_sortColor.y * 256);
	m_sortStack.append(val);
}
void Stroke::appendMapBlueIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_sortColor.z * 256) : -int(m_sortColor.z * 256);
	m_sortStack.append(val);
}


bool Stroke::testAgainstValue(int lhs, StrokeFilterOperator op, int rhs ) const
{
	switch (op)
	{
		case Stroke::kLessThan:
			return lhs < rhs;
		case Stroke::kGreaterThan:
			return lhs > rhs;
		case Stroke::kEqualTo:
			return lhs == rhs;
		case Stroke::kNotEqualTo:
			return lhs != rhs;
		default:
			return false;
	}
}

bool Stroke::testId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(m_id, op, value);
}
bool Stroke::testParentId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(m_parentId, op, value);
}
bool Stroke::testBrushId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(m_brushId, op, value);
}
bool Stroke::testPaintId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(m_paintId, op, value);
}
bool Stroke::testLayerId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(m_layerId, op, value);
}
bool Stroke::testRepeatId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(m_repeatId, op, value);
}
bool Stroke::testCustomBrushId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(int(m_customBrushId), op, value);
}
bool Stroke::testCustomPaintId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(int(m_customPaintId), op, value);
}
bool Stroke::testMapRedId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(int(m_filterColor.x * 256), op, value);
}
bool Stroke::testMapGreenId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(int(m_filterColor.y * 256), op, value);
}
bool Stroke::testMapBlueId(StrokeFilterOperator op, int value) const
{
	return  testAgainstValue(int(m_filterColor.z * 256), op, value);
}


///////// ///////// /////////

// private


void Stroke::reverseArray(MDoubleArray &arr) const

{
	int i = arr.length() - 1;
	int j = 0;
	while (i > j)
	{
		double temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
		i--;
		j++;
	}
}


bool Stroke::shouldMakeBackstroke(MObject dCurve, double startDist, double endDist,
                                  Stroke::DirectionMethod strokeDirection) const
{

	/* 	enum DirectionMethod { kForwards, kBackwards, kStartUppermost, kEndUppermost  };  */
	if (strokeDirection == Stroke::kForwards) {return false;}
	if (strokeDirection == Stroke::kBackwards) {return true;}

	MFnNurbsCurve curveFn(dCurve);

	MVector comparison(MVector::yAxis);

	double startParam = curveFn.findParamFromLength(startDist);
	double endParam = curveFn.findParamFromLength(endDist);
	MVector startTangent = curveFn.tangent(startParam).normal();
	MVector endTangent = curveFn.tangent(endParam).normal();

	double startUpFacing = startTangent * MVector::yAxis;
	double endUpFacing = endTangent * MVector::yAxis;

	if (startUpFacing > endUpFacing && strokeDirection == Stroke::kEndUppermost)
	{
		/* its a backstroke */
		return true;
	}

	if (startUpFacing <= endUpFacing && strokeDirection == Stroke::kStartUppermost)
	{
		/* its a backstroke */
		return true;
	}

	/* its a forwardstroke */
	return false;
}

