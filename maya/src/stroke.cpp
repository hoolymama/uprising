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


// double Stroke::interpContact(const MDoubleArray &contacts, double uniformParam)
// {
// 	int len = contacts.length();
// 	int rindex = ceil(uniformParam);
// 	if ( len < (rindex + 1))
// 	{
// 		return 1.0;
// 	}
// 	int lindex = floor(uniformParam);

// 	double result = contacts[lindex];
// 	if (lindex != rindex)
// 	{
// 		double w = double(rindex) - uniformParam;
// 		result = contacts[lindex] * w + contacts[rindex] * (1.0 - w);
// 	}
// 	return result;
// }


unsigned Stroke::create(
  const MObject &thisObj,
  const MObject &dCurve,
  const MDoubleArray &contacts,
  bool localContact,
  double curveLength,
  double startDist,
  double endDist,
  double entryLength,
  double exitLength,
  TransitionBlendMethod transBlendMethod,
  double pointDensity,
  int minimumPoints,
  const StrokeRotationSpec &rotSpec,
  const StrokeRepeatSpec &repeatSpec,
  DirectionMethod strokeDirection,
  double pivotParam,
  int strokeId,
  int brushId,
  int paintId,
  int layerId,
  int customBrushId,
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
	  contacts,
	  localContact,
	  curveLength,
	  startDist,
	  endDist,
	  entryLength,
	  exitLength,
	  transBlendMethod,
	  pointDensity,
	  minimumPoints,
	  pivotParam,
	  strokeId,
	  brushId,
	  paintId,
	  layerId,
	  customBrushId,
	  repeatId,
	  backstroke);

	motherStroke.setRotations(thisObj, rotSpec);
	motherStroke.setTransitionContact();

	repeatId++;


	// motherStroke.setPivot(dCurve, pivotParam, startDist, endDist);

	strokes->push_back(motherStroke);

	// const Stroke &mother = stk;

	for (int j = 0; j < repeatSpec.repeats; ++j) {

		double fan = repeatSpec.fan * (j + 1);
		bool	reverse = (repeatSpec.oscillate && (j % 2 == 0));

		double offset = repeatSpec.offset * (j + 1);
		Stroke stk(motherStroke);
		stk.offset(offset, reverse, repeatId++);
		stk.setRotations(thisObj, rotSpec);
		stk.setTransitionContact();
		stk.rotate(fan);
		strokes->push_back(stk);
		// cerr << "repeatSpec.mirror" << repeatSpec.mirror << endl;
		if ( repeatSpec.mirror) {
			Stroke stk(motherStroke);
			stk.offset(-offset, reverse, repeatId++);
			stk.setRotations(thisObj, rotSpec);
			stk.setTransitionContact();
			stk.rotate(-fan);
			strokes->push_back(stk);
		}
	}

	return repeatSpec.count() ;
}


Stroke::Stroke()	:
	m_targets(),
	m_pivot(),
	m_localContact(),
	m_arcLength(0),
	m_entryLength(),
	m_exitLength(),
	m_repeatId(),
	m_arrivals(),
	m_departure()
{}





Stroke::Stroke(
  const MObject &curveObject ,
  const MDoubleArray &contacts,
  bool localContact,
  double curveLength,
  double startDist,
  double endDist,
  double entryLength,
  double exitLength,
  TransitionBlendMethod transBlendMethod,
  double density,
  int minimumPoints,
  double pivotParam,
  int strokeId,
  int brushId,
  int paintId,
  int layerId,
  int customBrushId,
  int repeatId,
  bool backstroke):
	m_targets(),
	m_pivot(),
	m_localContact(localContact),
	m_arcLength(),
	m_entryLength(entryLength),
	m_exitLength(exitLength),
	m_transitionBlendMethod(transBlendMethod),
	m_strokeId(strokeId),
	m_brushId(brushId),
	m_paintId(paintId),
	m_layerId(layerId),
	m_customBrushId(customBrushId),
	m_repeatId(repeatId),
	m_backstroke(backstroke),
	m_arrivals(),
	m_departure()

{

	MFnNurbsCurve curveFn(curveObject);
	double strokeRange = endDist - startDist; // can be negative
	unsigned numPoints = unsigned(density * fabs(strokeRange));
	if (minimumPoints < 2) { minimumPoints = 2; }
	if (numPoints < minimumPoints) { numPoints = minimumPoints; }
	double gap = strokeRange / (numPoints - 1) ; // can be negative

	for (unsigned i = 0; i < numPoints; i++) {


		double dist = startDist + (i * gap);
		double strokeParam = (dist - startDist) / strokeRange;
		double curveParam = dist / curveLength;
		double uniformParam = curveFn.findParamFromLength(dist);
		MVector tangent = curveFn.tangent(uniformParam);
		tangent.z = 0;
		tangent.normalize();
		double contact = Stroke::interpContact(contacts, uniformParam);

		MPoint pt;
		curveFn.getPointAtParam(uniformParam, pt, MSpace::kObject);

		m_targets.push_back(
		  Target(pt, tangent, strokeParam, curveParam, contact)
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

	std::vector<Target>::iterator iter;
	m_repeatId = repeatId;
	if (reverse) {
		m_backstroke =  !m_backstroke;
		std::reverse(m_targets.begin(), m_targets.end());
		for (iter = m_targets.begin(); iter != m_targets.end(); iter++) {
			iter->reverseParam();
		}
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

bool operator<(const Stroke &a, const Stroke &b)
{
	const MIntArray &lstack = a.sortStack();
	const MIntArray &rstack = b.sortStack();

	int len = lstack.length();
	// commented because it should never happen
	// if (len !=  rstack.length()) {
	//   return false;
	// }


	for (int i = 0; i < len; ++i)
	{
		if (lstack[i] < rstack[i]) {
			return true;
		}
		if (lstack[i] > rstack[i]) {
			return false;
		}
	}

	/*
	If we get this far, then everything is the same.
	However we must conform to strict weak ordering,
	so use the order of creation, comprised of strokeId and,
	repeatId.
	*/

	if (a.strokeId() < b.strokeId()) {
		return true;
	}

	if (a.strokeId() > b.strokeId()) {
		return false;
	}
	if (a.repeatId() < b.repeatId()) {
		return true;
	}
	return false;

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
int Stroke::customBrushId() const
{
	return m_customBrushId;
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
void Stroke::setCustomBrushId(int val) {
	m_customBrushId = val;
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


void Stroke::appendCustomBrushIdToSortStack(bool ascending)
{
	int val = ascending ? int(m_customBrushId) : -int(m_customBrushId);
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

void Stroke::appendTargetCountToSortStack(bool ascending)
{
	int num = m_targets.size();
	int val = ascending ? num : -num;
	m_sortStack.append(val);
}


// void Stroke::appendCustomBrushIdToSortStack(bool ascending)
// {
// 	int val = ascending ? int(m_customBrushId) : -int(m_customBrushId);
// 	m_sortStack.append(val);
// }

// void Stroke::appendCustomPaintIdToSortStack(bool ascending)
// {
// 	int val = ascending ? int(m_customPaintId) : -int(m_customPaintId);
// 	m_sortStack.append(val);
// }

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

bool Stroke::testCustomBrushId(FilterOperator op, int value) const
{
	return  testAgainstValue(m_customBrushId, op, value);
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
bool Stroke::testTargetCount(FilterOperator op, int value) const
{
	return  testAgainstValue(m_targets.size(), op, value);
}

// bool Stroke::testCustomBrushId(FilterOperator op, int value) const
// {
// 	return  testAgainstValue(int(m_customBrushId), op, value);
// }
// bool Stroke::testCustomPaintId(FilterOperator op, int value) const
// {
// 	return  testAgainstValue(int(m_customPaintId), op, value);
// }
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

	result.clear();
	MPointArray pts;
	getPoints(pts, stackHeight, withTraversal);
	for (int i = 0; i < pts.length(); ++i)
	{
		result.append(pts[i]);
	}
}


void Stroke::getPoints(MPointArray &result, double stackHeight,
                       bool withTraversal) const {


	MVector stackOffset =  MVector::zAxis * stackHeight;
	std::vector<Target>::const_iterator citer;
	if (withTraversal)
	{
		for (citer = m_arrivals.begin() ; citer != m_arrivals.end(); citer++) {
			result.append(citer->position() + stackOffset);
		}
	}
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(citer->position() + stackOffset);
	}
	if (withTraversal)
	{
		result.append(m_departure.position() + stackOffset);
	}
}



void Stroke::transform(const MVector &vec, MFloatVectorArray &result,
                       bool withTraversal) const {
	result.clear();
	MVectorArray vecs;
	transform(vec, vecs, withTraversal);
	for (int i = 0; i < vecs.length(); ++i)
	{
		result.append(vecs[i]);
	}
}

void Stroke::transform(const MVector &vec, MVectorArray &result,
                       bool withTraversal) const {
	std::vector<Target>::const_iterator citer;
	if (withTraversal)
	{
		for (citer = m_arrivals.begin() ; citer != m_arrivals.end(); citer++) {
			result.append(citer->transform(vec));
		}
	}
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(citer->transform(vec));
	}
	if (withTraversal)
	{
		result.append(m_departure.transform(vec));
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

void Stroke::getXAxes(MVectorArray &result, bool withTraversal) const {
	transform(MVector::xAxis, result, withTraversal);
}
void Stroke::getYAxes(MVectorArray &result, bool withTraversal) const {
	transform(MVector::yAxis, result, withTraversal);
}
void Stroke::getZAxes(MVectorArray &result, bool withTraversal) const {
	transform(MVector::zAxis, result, withTraversal);
}



// void Stroke::getHead(MFloatPoint &result,  float stackHeight) const {
// 	MFloatVector stackOffset = MFloatVector::zAxis * stackHeight;
// 	result = MFloatPoint(m_targets[0].position()) + stackOffset;
// }


MPoint Stroke::getHead( double stackHeight) const {
	MFloatVector stackOffset = MVector::zAxis * stackHeight;
	return m_targets[0].position() + stackOffset;
}




void Stroke::getBorders(
  MPointArray &lefts,
  MPointArray &rights,
  const Brush &brush,
  double stackHeight,
  bool displayContactWidth) const
{
	unsigned len = m_targets.size();
	lefts.setLength(len);
	rights.setLength(len);

	MFloatVector stackOffset =  MVector::zAxis * stackHeight;
	double width = brush.width() * 0.5;

	bool flat = (brush.shape() == Brush::kFlat ) ;
	std::vector<Target>::const_iterator citer;
	unsigned i = 0;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++, i++) {
		MPoint &left = lefts[i];
		MPoint &right = rights[i];
		citer->getBorderPoints(left, right, width, flat, displayContactWidth);
		left += stackOffset;
		right += stackOffset;
	}
}

void Stroke::getBorderLoop(
  const Brush &brush,
  double stackHeight,
  MPointArray &result,
  bool displayContactWidth) const
{
	unsigned len = m_targets.size() * 2;

	result.setLength(len);

	MVector stackOffset =  MVector::zAxis * stackHeight;
	double width = brush.width() * 0.5;
	bool flat = (brush.shape() == Brush::kFlat  ) ;
	std::vector<Target>::const_iterator citer;
	unsigned i = 0;
	unsigned j = len - 1;

	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++, i ++, j--) {
		citer->getBorderPoints(result[i], result[j], width, flat, displayContactWidth);
		result[i] += stackOffset;
		result[j] += stackOffset;
	}
}


void Stroke::getTriangleStrip(
  const Brush &brush,
  double stackHeight,
  MPointArray &result,
  bool displayContactWidth) const
{
	unsigned len = m_targets.size();
	result.setLength(len * 2);

	MVector stackOffset =  MVector::zAxis * stackHeight;
	double width = brush.width() * 0.5;

	bool flat = (brush.shape() == Brush::kFlat  ) ;
	std::vector<Target>::const_iterator citer;
	unsigned i = 0;

	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++, i += 2) {
		citer->getBorderPoints(result[i], result[i + 1], width, flat, displayContactWidth);
		result[i] += stackOffset;
		result[i + 1] += stackOffset;
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


void Stroke::arrivalPositions(const MMatrix &space, MPointArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_arrivals.begin() ; citer != m_arrivals.end(); citer++) {
		result.append(citer->position(space));
	}
}

void Stroke::arrivalRotations(
  const MMatrix &space,
  MTransformationMatrix::RotationOrder order,
  MAngle::Unit unit,
  MVectorArray &result ) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_arrivals.begin() ; citer != m_arrivals.end(); citer++) {
		result.append(citer->rotation(order, unit, space));
	}
}

void Stroke::departurePosition(const MMatrix &space, MPoint &result) const
{
	result = m_departure.position(space);
}

void Stroke::departureRotation(
  const MMatrix &space,
  MTransformationMatrix::RotationOrder order,
  MAngle::Unit unit,
  MVector &result ) const
{
	result = m_departure.rotation(order, unit, space) ;
}


void Stroke::tangents(const MMatrix &space, MVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin() ; citer != m_targets.end(); citer++) {
		result.append(citer->tangent() * space);
	}
}


// void Stroke::setCustomSortData(const Brush &brush,  const Paint &paint)
// {
// 	m_customBrushId = brush.customId;
// 	m_customPaintId =	paint.customId;
// }


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




void Stroke::setTransitionContact( )
{
	double m_entry_param = m_entryLength / m_arcLength;
	double m_exit_param = m_exitLength / m_arcLength;

	double total_param = m_entry_param + m_exit_param;

	if (total_param > 1.0)
	{
		m_entry_param = m_entry_param / total_param;
		m_exit_param = m_exit_param / total_param;
	}
	m_exit_param = 1.0 - m_exit_param;


	// now have entry and exit params for the transitions
	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin() ; iter != m_targets.end(); iter++) {
		const double &param = iter->param();
		if (param > m_entry_param ) {
			break;
		}
		double contact = param / m_entry_param;

		if (m_transitionBlendMethod == Stroke::kTransitionMin) {
			contact = fmin(contact, iter->contact());
		}
		else if (m_transitionBlendMethod == Stroke::kTransitionMax)
		{
			contact = fmax(contact, iter->contact());
		}
		else { // kTransitionBlend
			contact =  (contact + iter->contact()) * 0.5 ;
		}
		iter->setContact(contact);
	}

	std::vector<Target>::reverse_iterator riter;

	for (riter = m_targets.rbegin() ; riter != m_targets.rend(); riter++) {
		const double &param = riter->param();
		if (param <= m_exit_param ) {
			break;
		}
		double contact = (1.0 - param) / (1.0 - m_exit_param);
		if (m_transitionBlendMethod == Stroke::kTransitionMin) {
			contact = fmin(contact, riter->contact());
		}
		else if (m_transitionBlendMethod == Stroke::kTransitionMax)
		{
			contact = fmax(contact, riter->contact());
		}
		else { // kTransitionBlend
			contact =  (contact + riter->contact()) * 0.5 ;
		}
		riter->setContact(contact);
	}
}



void Stroke::offsetBrushContact(const Brush &brush)
{
	float height = brush.transHeight();
	float power = brush.contactPower();
	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin() ; iter != m_targets.end(); iter++) {
		float dist = (1.0 - iter->contact());
		dist = pow(dist, power) * height;
		if (m_localContact) {
			iter->offsetLocalZ(-dist);
		}
		else {
			iter->offsetBy(MVector::zAxis * dist);
		}
	}
}




// void Stroke::offsetBrushContact(const Brush &brush)
// {
// 	const double &height = brush.transHeight;
// 	const double &power = brush.transPower;
// 	double tip = brush.tip;

// 	if (tip < 0.00001) {
// 		tip = 0.00001;
// 	}

// 	double m_entry_param = m_entryLength / m_arcLength;
// 	double m_exit_param = (m_exitLength / m_arcLength);

// 	double total_param = m_entry_param + m_exit_param;

// 	if (total_param > 1.0)
// 	{
// 		m_entry_param = m_entry_param / total_param;
// 		m_exit_param = m_exit_param / total_param;
// 	}
// 	m_exit_param = 1.0 - m_exit_param;

// 	// now have entry and exit params for the transitions
// 	std::vector<Target>::iterator iter;
// 	for (iter = m_targets.begin() ; iter != m_targets.end(); iter++) {
// 		const double &param = iter->param();
// 		if (param > m_entry_param ) {
// 			break;
// 		}
// 		float dist = pow((1.0 - (param / m_entry_param)) , power) * height;
// 		iter->offsetBy(MVector(0.0, 0.0, dist));

// 		double contact = (1.0 - (dist / tip));
// 		iter->setContact(contact);
// 	}


// 	std::vector<Target>::reverse_iterator riter;

// 	for (riter = m_targets.rbegin() ; riter != m_targets.rend(); riter++) {
// 		const double &param = riter->param();
// 		if (param <= m_exit_param ) {
// 			break;
// 		}
// 		float dist = pow (((param - m_exit_param) / (1.0 - m_exit_param)), power) * height;
// 		riter->offsetBy(MVector(0.0, 0.0, dist));

// 		double contact = (1.0 - (dist / tip));
// 		riter->setContact(contact);
// 	}
// }


const Target &Stroke::departure() const
{
	return m_departure;
}

void Stroke::setDeparture(double offset)
{
	MVector offsetVector(0.0, 0.0, offset);
	m_departure = Target(m_targets.back());
	m_departure.offsetBy(offsetVector);
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



void Stroke::reverseArray(const MDoubleArray &arr,  MDoubleArray &result)
{
	for (int i = arr.length() - 1; i >= 0 ; --i)
	{
		result.append(arr[i]);
	}
}

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

void Stroke::rotate(double rotation) {
	MMatrix rmat;
	double sz = sin(rotation);
	double cz = cos(rotation);
	rmat[0][0] = cz; rmat[0][1] = sz;
	rmat[1][0] = -sz; rmat[1][1] = cz;

	const MPoint &pos = m_pivot.position();

	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin() ; iter != m_targets.end(); iter++) {
		iter->rotate(pos, rmat);
	}

	// if arivals and departure have already been set (unlkikely)
	// then we better rotate them too
	if (m_arrivals.size()) {
		for (iter = m_arrivals.begin() ; iter != m_arrivals.end(); iter++) {
			iter->rotate(pos, rmat);
		}
		m_departure.rotate(pos, rmat);
	}
}


void Stroke::translate(const MVector &translation) {

	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin() ; iter != m_targets.end(); iter++) {
		iter->offsetBy(translation);
	}
	if (m_arrivals.size()) {
		for (iter = m_arrivals.begin() ; iter != m_arrivals.end(); iter++) {

			iter->offsetBy(translation);
		}
		m_departure.offsetBy(translation );
	}

}




