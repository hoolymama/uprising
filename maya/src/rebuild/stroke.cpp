#include <maya/MFnNurbsCurve.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <algorithm>
#include <vector>

// #include <typeinfo>
#include "errorMacros.h"
#include "mayaMath.h"
#include <stroke.h>

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
  int strokeId,
  int brushId,
  int paintId,
  int layerId,
  std::vector<Stroke> *strokes
) {
	/* This is the motherstroke. Determine whether forward or back */
	bool backstroke = shouldMakeBackstroke(dCurve, startDist, endDist, strokeDirection);

	if (backstroke) {
		std::swap(startDist, endDist);
	}

	int repeatId = 0;

	Stroke motherStroke(
	  dCurve,
	  curveLength,
	  startDist,
	  endDist,
	  entryLength,
	  exitLength,
	  pointDensity,
	  pivotParam,
	  strokeId,
	  brushId,
	  paintId,
	  layerId,
	  repeatId,
	  backstroke);

	repeatId++;

	motherStroke.setRotations(thisObj, rotSpec);

	// motherStroke.setPivot(dCurve, pivotParam, startDist, endDist);

	strokes->push_back(motherStroke);

	// const Stroke &mother = stk;

	for (int j = 0; j < repeatSpec.repeats; ++j) {

		bool	reverse = (repeatSpec.oscillate && (j % 2 == 0));

		double offset = repeatSpec.offset * (j + 1);
		Stroke stk(motherStroke);
		stk.offset(offset, reverse, repeatId++);
		stk.setRotations(thisObj, rotSpec);
		strokes->push_back(stk);
		// cerr << "repeatSpec.mirror" << repeatSpec.mirror << endl;
		if ( repeatSpec.mirror) {
			Stroke stk(motherStroke);
			stk.offset(-offset, reverse, repeatId++);
			stk.setRotations(thisObj, rotSpec);
			strokes->push_back(stk);
		}
	}

	return repeatSpec.count() ;
}


Stroke::Stroke()	:
	m_targets(),
	m_pivot(),
	m_arcLength(0),
	m_entryLength(),
	m_exitLength(),
	m_repeatId(),
	m_arrivals(),
	m_departure()
{}





Stroke::Stroke(
  const MObject &curveObject ,
  double curveLength,
  double startDist,
  double endDist,
  double entryLength,
  double exitLength,
  double density,
  double pivotParam,
  int strokeId,
  int brushId,
  int paintId,
  int layerId,
  int repeatId,
  bool backstroke)	:
	m_targets(),
	m_pivot(),
	m_arcLength(),
	m_entryLength(entryLength),
	m_exitLength(exitLength),
	m_strokeId(strokeId),
	m_brushId(brushId),
	m_paintId(paintId),
	m_layerId(layerId),
	m_repeatId(repeatId),
	m_backstroke(backstroke),
	m_arrivals(),
	m_departure()

{

	MFnNurbsCurve curveFn(curveObject);
	double strokeRange = endDist - startDist; // can be negative
	unsigned numPoints = unsigned(density * fabs(strokeRange));
	if (numPoints < 2) { numPoints = 2; }
	double gap = strokeRange / (numPoints - 1) ; // can be negative
	for (unsigned i = 0; i < numPoints; i++) {
		double dist = startDist + (i * gap);
		m_targets.push_back(
		  Target(curveFn, dist, startDist, strokeRange, curveLength)
		);
	}

	double pivotDist = startDist  + (strokeRange * pivotParam);
	m_pivot = Target(curveFn, pivotDist, startDist, strokeRange, curveLength);

	setArcLength();
}

void Stroke::offset(
  double offset,
  bool reverse,
  int repeatId)
{

	m_repeatId = repeatId;
	if (reverse) {
		m_backstroke =  !m_backstroke;
		std::reverse(m_targets.begin(), m_targets.end());
	}

	MVector offsetVec;
	if (fabs(offset) > epsilon) {
		std::vector<Target>::iterator iter;
		for (iter = m_targets.begin(); iter != m_targets.end(); iter++) {
			offsetVec = (iter->tangent() ^ MVector::zAxis) * offset;
			iter->offsetBy(offsetVec);
		}
		offsetVec = (m_pivot.tangent() ^ MVector::zAxis) * offset;
		m_pivot.offsetBy(offsetVec);
	}
	setArcLength();
}


const Target &Stroke::pivot() const {
	return m_pivot;
}

Stroke::~Stroke() {}


void Stroke::setRotations(
  const MObject &thisObj,
  const StrokeRotationSpec &rotSpec)
{
	bool follow = rotSpec.followStroke;

	MDoubleArray sampleVals;
	if (rotSpec.rampScope == StrokeRotationSpec::kCurve) {
		this->getCurveParams(sampleVals);
	}
	else
	{
		this->getParams(sampleVals);
		if (rotSpec.rampScope ==  StrokeRotationSpec::kTravelStroke && m_backstroke)
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
		iter->setRotation(outTilt[i], outBank[i], outTwist[i], follow, m_backstroke );
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

bool Stroke::backstroke() const
{
	return m_backstroke;
}

// void Stroke::setPivot(
//   const MObject &curveObject,
//   double pivotParam,
//   double startDist,
//   double endDist)
// {
// 	MFnNurbsCurve curveFn(curveObject);
// 	double dist = startDist + (pivotParam * (endDist - startDist) );
// 	double crvParam = curveFn.findParamFromLength(dist);
// 	curveFn.getPointAtParam(crvParam, m_pivot, MSpace::kWorld);
// }


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

int Stroke::strokeId() const
{
	return m_strokeId;
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

void Stroke::setBrushId(int val) {
	m_brushId = val;
}
void Stroke::setPaintId(int val) {
	m_paintId = val;
}

/////////////////////// SORT AND FILTER ///////////////////////

const MIntArray &Stroke::sortStack() const {
	return m_sortStack;
}

void Stroke::clearSortStack()
{
	m_sortStack.clear();
}



void Stroke::setSortColor(const MFloatVector &color)
{
	m_sortColor = color;
}
void Stroke::setFilterColor(const MFloatVector &color)
{
	m_filterColor = color;
}


void Stroke::appendStrokeIdToSortStack(bool ascending)
{
	int val = ascending ? m_strokeId : -m_strokeId;
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


bool Stroke::testAgainstValue(int lhs, FilterOperator op, int rhs ) const
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

bool Stroke::testStrokeId(FilterOperator op, int value) const
{
	return  testAgainstValue(m_strokeId, op, value);
}
bool Stroke::testParentId(FilterOperator op, int value) const
{
	return  testAgainstValue(m_parentId, op, value);
}
bool Stroke::testBrushId(FilterOperator op, int value) const
{
	return  testAgainstValue(m_brushId, op, value);
}
bool Stroke::testPaintId(FilterOperator op, int value) const
{
	return  testAgainstValue(m_paintId, op, value);
}
bool Stroke::testLayerId(FilterOperator op, int value) const
{
	return  testAgainstValue(m_layerId, op, value);
}
bool Stroke::testRepeatId(FilterOperator op, int value) const
{
	return  testAgainstValue(m_repeatId, op, value);
}
bool Stroke::testCustomBrushId(FilterOperator op, int value) const
{
	return  testAgainstValue(int(m_customBrushId), op, value);
}
bool Stroke::testCustomPaintId(FilterOperator op, int value) const
{
	return  testAgainstValue(int(m_customPaintId), op, value);
}
bool Stroke::testMapRedId(FilterOperator op, int value) const
{
	return  testAgainstValue(int(m_filterColor.x * 256), op, value);
}
bool Stroke::testMapGreenId(FilterOperator op, int value) const
{
	return  testAgainstValue(int(m_filterColor.y * 256), op, value);
}
bool Stroke::testMapBlueId(FilterOperator op, int value) const
{
	return  testAgainstValue(int(m_filterColor.z * 256), op, value);
}

//////////////////////////////////////////////



/////////////////////// FOR DRAWING ///////////////////////


void Stroke::getDirectionMatrices(MMatrixArray &result, double stackHeight) const {
	MVector stackOffset = MVector::zAxis * stackHeight;
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		MMatrix mat =  citer->directionMatrix(m_backstroke);
		mat[3][2] += stackHeight;
		result.append(mat);
	}
}

void Stroke::getPoints(MFloatPointArray &result, double stackHeight,
                       bool withTraversal) const {


	cerr << "withTraversal: " << withTraversal << endl;
	MFloatVector stackOffset =  MFloatVector::zAxis * stackHeight;
	std::vector<Target>::const_iterator citer;
	if (withTraversal)
	{
		for (citer = m_arrivals.begin() ; citer != m_arrivals.end(); citer++) {
			cerr << "m_arrivals iter->position(): " << citer->position() << endl;
			result.append(MFloatPoint(citer->position()) + stackOffset);
		}
	}
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		cerr << "m_targets iter->position(): " << citer->position() << endl;
		result.append(MFloatPoint(citer->position()) + stackOffset);
	}
	if (withTraversal)
	{
		cerr << "m_departure.position(): " << m_departure.position() << endl;
		result.append(MFloatPoint(m_departure.position()) + stackOffset);
	}
}

void Stroke::transform(const MVector &vec, MFloatVectorArray &result,
                       bool withTraversal) const {
	std::vector<Target>::const_iterator citer;
	if (withTraversal)
	{
		for (citer = m_arrivals.begin() ; citer != m_arrivals.end(); citer++) {
			result.append(MFloatPoint(citer->transform(vec)));
		}
	}
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(MFloatVector(citer->transform(vec)));
	}
	if (withTraversal)
	{
		result.append(MFloatPoint(m_departure.transform(vec)));
	}
}

void Stroke::getXAxes(MFloatVectorArray &result, bool withTraversal) const {
	transform(MVector::xAxis, result, withTraversal);
}
void Stroke::getYAxes(MFloatVectorArray &result, bool withTraversal) const {
	transform(MVector::yAxis, result, withTraversal);
}
void Stroke::getZAxes(MFloatVectorArray &result, bool withTraversal) const {
	transform(MVector::zAxis, result, withTraversal);
}



void Stroke::getHead(MFloatPoint &result,  float stackHeight) const {
	MFloatVector stackOffset = MFloatVector::zAxis * stackHeight;
	result = MFloatPoint(m_targets[0].position()) + stackOffset;
}




void Stroke::getBorders(
  MFloatPointArray &lefts,
  MFloatPointArray &rights,
  const Brush &brush,
  double stackHeight) const
{
	unsigned len = m_targets.size();
	lefts.setLength(len);
	rights.setLength(len);

	MFloatVector stackOffset = MFloatVector(MVector::zAxis * stackHeight);
	double width = brush.width * 0.5;

	bool flat = (brush.shape == Brush::kFlat) ;
	std::vector<Target>::const_iterator citer;
	unsigned i = 0;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++, i++) {
		MFloatPoint &left = lefts[i];
		MFloatPoint &right = rights[i];
		citer->getBorderPoints(left, right, width, flat);
		left += stackOffset;
		right += stackOffset;
	}
}

///////// ///////// /////////

///////// ///////// /////////

void Stroke::positions(const MMatrix &space, MPointArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(citer->position(space));
	}
}

void Stroke::rotations(
  const MMatrix &space,
  MTransformationMatrix::RotationOrder order,
  MAngle::Unit unit,
  MVectorArray &result ) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(citer->rotation(order, unit, space));
	}
}



void Stroke::tangents(const MMatrix &space, MVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(citer->tangent() * space);
	}
}


void Stroke::setCustomSortData(const Brush &brush,  const Paint &paint)
{
	m_customBrushId = brush.customId;
	m_customPaintId =	paint.customId;
}


void Stroke::setUV(
  const MMatrix &inversePlaneMatrix)
{
	MPoint p = ((m_pivot.position() * inversePlaneMatrix) * 0.5) + MVector(0.5, 0.5, 0.0);
	m_u = p.x;
	m_v = p.y;
}
void Stroke::getUV( float &u, float &v) {
	u = m_u;
	v = m_v;
}

void Stroke::displace( MFnMesh &meshFn, MMeshIsectAccelParams &ap)
{
	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin() ; iter != m_targets.end(); iter++) {
		MFloatPoint rayOrigin = MFloatPoint(iter->position());
		rayOrigin.z = 0.0f;
		float dist = 0;
		MFloatPoint hitPoint;
		bool hit = meshFn.closestIntersection(
		             rayOrigin, MFloatVector::zAxis, 0, 0, false,
		             MSpace::kWorld,
		             100.0f, true, &ap, hitPoint, &dist, 0, 0, 0, 0);
		if (hit) {
			iter->offsetBy(MVector(0.0, 0.0, dist));
		}
	}
}


const Target &Stroke::departure() const
{
	return m_departure;
}




void Stroke::setDeparture(double offset)
{
	MVector offsetVector(0.0, 0.0, offset);
	m_departure = Target(m_targets.back());
	m_departure.offsetBy(offsetVector);
	cerr << "Stroke::setDeparture: " << m_departure.position() << endl;
}

void Stroke::setArrival(double offset)
{
	MVector offsetVector(0.0, 0.0, offset);
	Target arrival(m_targets.front());
	arrival.offsetBy(offsetVector);
	m_arrivals.push_back(arrival);
}

void Stroke::setArrival(double offset, double threshold, const Stroke &prev)
{
	const Target &departure = prev.departure();
	MVector offsetVector(0.0, 0.0, offset);
	Target arrival(m_targets.front());
	arrival.offsetBy(offsetVector);

	double dist = departure.distanceTo(arrival);

	MPoint departurePos = departure.position();
	MPoint arrivalPos = arrival.position();


	if (dist > threshold) {
		int num_inbetweens = int(dist / threshold);
		for (int i = 0; i < num_inbetweens; ++i)
		{
			double fraction = (i + 1) / double(num_inbetweens + 1);
			MPoint newPoint((departurePos * (1.0 - fraction)) + (arrivalPos * fraction));

			Target stop;
			if (fraction < 0.5) {
				stop = Target(departure);
			}
			else
			{
				stop = Target(arrival);
			}
			stop.setPosition(newPoint);
			m_arrivals.push_back(stop);
		}
	}
	m_arrivals.push_back(arrival);
}





// void clusterGeom::setPreStops(double threshold)
// {
// 	std::vector<Stroke>::iterator iter;
// 	iter = m_strokes.begin();
// 	MMatrix lastA = iter->endApproach();
// 	MPoint lastPoint(lastA[3][0], lastA[3][1], lastA[3][2]);
// 	iter++;
// 	for (; iter != m_strokes.end(); iter++)
// 	{
// 		const MMatrix &currentA = iter->startApproach();


// 		MPoint currentPoint(currentA[3][0], currentA[3][1], currentA[3][2]);

// 		double dist = lastPoint.distanceTo(currentPoint);

// 		if (dist > threshold) {
// 			int num_inbetweens = int(dist / threshold);
// 			for (int i = 0; i < num_inbetweens; ++i)
// 			{
// 				double fraction = (i + 1) / double(num_inbetweens + 1);
// 				MPoint newPoint((lastPoint * (1.0 - fraction)) + (currentPoint * fraction));
// 				MMatrix newMat(lastA);
// 				if (fraction > 0.5) {
// 					newMat = MMatrix(currentA);
// 				}
// 				newMat[3][0] = newPoint.x;
// 				newMat[3][1] = newPoint.y;
// 				newMat[3][2] = newPoint.z;
// 				iter->addPreStop(newMat);
// 			}
// 		}
// 		lastA = iter->endApproach();
// 		lastPoint = MPoint(lastA[3][0], lastA[3][1], lastA[3][2]);
// 	}
// }




// private


void Stroke::reverseArray(MDoubleArray &arr)
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
                                  Stroke::DirectionMethod strokeDirection)
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

// std::vector<Target>::const_reverse_iterator citer;
// for (citer = other.targets().rbegin() ; citer != other.targets().rend(); citer++) {
// 	m_targets.push_back(*citer);
// }
// std::vector<Target>::const_iterator citer;
// for (citer = other.targets().begin() ; citer != other.targets().end(); citer++) {
// 	m_targets.push_back(*citer);
// }
