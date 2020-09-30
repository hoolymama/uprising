#include <maya/MFnNurbsCurve.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnNurbsCurveData.h>

#include <maya/MQuaternion.h>
#include <algorithm>
#include <vector>

// #include <typeinfo>
#include "errorMacros.h"
#include "mayaMath.h"
#include <stroke.h>

const double epsilon = 0.0001;

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
	double paintFlow,
	int strokeId,
	int brushId,
	int paintId,
	int layerId,
	int customBrushId,
	std::vector<Stroke> *strokes)
{
	/* This is the motherstroke. Determine whether forward or back */
	bool backstroke = shouldMakeBackstroke(dCurve, startDist, endDist, strokeDirection);

	if (backstroke)
	{
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
		paintFlow,
		strokeId,
		brushId,
		paintId,
		layerId,
		customBrushId,
		repeatId,
		backstroke);

	motherStroke.setRotations(thisObj, rotSpec);
	motherStroke.setTransitionContact();

	strokes->push_back(motherStroke);
	int count = 1;

	double fan;
	double tangentOffset;
	double normalOffset;

	for (int j = 0; j < repeatSpec.repeats; ++j)
	{
		bool reverse = (repeatSpec.oscillate && (j % 2 == 0));
		repeatId = (j * 2) + 1;
		if (drand48() < repeatSpec.probability)
		{
			fan = repeatSpec.calcFan(j);
			repeatSpec.calcOffsets(j, tangentOffset, normalOffset);

			Stroke stk(motherStroke);
			stk.offset(tangentOffset, normalOffset, reverse, repeatId);
			stk.setRotations(thisObj, rotSpec);
			stk.setTransitionContact();
			stk.rotate(fan);
			strokes->push_back(stk);
			count++;
		}

		// cerr << "repeatSpec.mirror" << repeatSpec.mirror << endl;
		if (repeatSpec.mirror && (drand48() < repeatSpec.probability))
		{
			repeatId = (j * 2) + 2;
			fan = repeatSpec.calcFan(j);
			repeatSpec.calcOffsets(j, tangentOffset, normalOffset);

			Stroke stk(motherStroke);
			stk.offset(tangentOffset, -normalOffset, reverse, repeatId);
			stk.setRotations(thisObj, rotSpec);
			stk.setTransitionContact();
			stk.rotate(-fan);
			strokes->push_back(stk);
			count++;
		}
	}

	return count;
}

Stroke::Stroke() : m_targets(),
				   m_pivot(),
				   m_localContact(),
				   m_arcLength(0),
				   m_entryLength(),
				   m_exitLength(),
				   m_paintFlow(1.0),
				   m_repeatId(),
				   m_parentId(-1),
				   m_arrivals(),
				   m_departure(),
				   m_follow(true),
				   m_linearSpeed(1.0),
				   m_angularSpeed(1.0)
{
}

Stroke::Stroke(
	const MObject &curveObject,
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
	double paintFlow,
	int strokeId,
	int brushId,
	int paintId,
	int layerId,
	int customBrushId,
	int repeatId,
	bool backstroke) : m_targets(),
					   m_pivot(),
					   m_localContact(localContact),
					   m_arcLength(),
					   m_entryLength(entryLength),
					   m_exitLength(exitLength),
					   m_transitionBlendMethod(transBlendMethod),
					   m_paintFlow(paintFlow),
					   m_strokeId(strokeId),
					   m_brushId(brushId),
					   m_paintId(paintId),
					   m_layerId(layerId),
					   m_parentId(-1),
					   m_customBrushId(customBrushId),
					   m_repeatId(repeatId),
					   m_backstroke(backstroke),
					   m_arrivals(),
					   m_departure(),
					   m_follow(true),
					   m_linearSpeed(1.0),
					   m_angularSpeed(1.0)

{

	MFnNurbsCurve curveFn(curveObject);
	double strokeRange = endDist - startDist; // can be negative
	unsigned numPoints = unsigned(density * fabs(strokeRange));
	if (minimumPoints < 2)
	{
		minimumPoints = 2;
	}
	if (numPoints < minimumPoints)
	{
		numPoints = minimumPoints;
	}
	double gap = strokeRange / (numPoints - 1); // can be negative

	for (unsigned i = 0; i < numPoints; i++)
	{

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
			Target(pt, tangent, strokeParam, curveParam, contact));
	}

	double pivotDist = startDist + (strokeRange * pivotParam);
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
	if (reverse)
	{
		m_backstroke = !m_backstroke;
		std::reverse(m_targets.begin(), m_targets.end());
		for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
		{
			iter->reverseParam();
		}
	}

	MVector offsetVec;
	if (fabs(offset) > epsilon)
	{
		std::vector<Target>::iterator iter;
		for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
		{
			offsetVec = (iter->tangent() ^ MVector::zAxis) * offset;
			iter->offsetBy(offsetVec);
		}
		offsetVec = (m_pivot.tangent() ^ MVector::zAxis) * offset;
		m_pivot.offsetBy(offsetVec);
	}
	setArcLength();
}

void Stroke::offset(
	double tangentOffset,
	double normalOffset,
	bool reverse,
	int repeatId)
{

	std::vector<Target>::iterator iter;
	m_repeatId = repeatId;
	if (reverse)
	{
		m_backstroke = !m_backstroke;
		std::reverse(m_targets.begin(), m_targets.end());
		for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
		{
			iter->reverseParam();
		}
	}

	MVector offsetVec;
	if ((fabs(tangentOffset) > epsilon) || (fabs(normalOffset) > epsilon))
	{
		std::vector<Target>::iterator iter;
		for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
		{
			offsetVec = (iter->tangent() ^ MVector::zAxis) * normalOffset;
			offsetVec += iter->tangent() * tangentOffset;
			iter->offsetBy(offsetVec);
		}
		offsetVec = (m_pivot.tangent() ^ MVector::zAxis) * normalOffset;
		offsetVec += m_pivot.tangent() * tangentOffset;
		m_pivot.offsetBy(offsetVec);
	}
	setArcLength();
}

const Target &Stroke::pivot() const
{
	return m_pivot;
}

Stroke::~Stroke() {}

void Stroke::setRotations(
	const MObject &thisObj,
	const StrokeRotationSpec &rotSpec)
{
	m_follow = rotSpec.followStroke;

	MDoubleArray sampleVals;
	if (rotSpec.rampScope == StrokeRotationSpec::kCurve)
	{
		this->getCurveParams(sampleVals);
	}
	else
	{
		this->getParams(sampleVals);
		if (rotSpec.rampScope == StrokeRotationSpec::kTravelStroke && m_backstroke)
		{ // reverse them
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
				 tiltMax);
	doRampLookup(thisObj, rotSpec.bankRampAtt, sampleVals, outBank, 0.0, 1.0, bankMin,
				 bankMax);
	doRampLookup(thisObj, rotSpec.twistRampAtt, sampleVals, outTwist, 0.0, 1.0, twistMin,
				 twistMax);

	std::vector<Target>::iterator iter = m_targets.begin();
	unsigned i = 0;
	for (; iter != m_targets.end(); iter++, i++)
	{
		iter->setRotation(outTilt[i], outBank[i], outTwist[i], m_follow, m_backstroke);
	}
}

void Stroke::setArcLength()
{
	m_arcLength = 0;
	std::vector<Target>::const_iterator previter = m_targets.begin();
	std::vector<Target>::const_iterator iter = std::next(previter);
	std::vector<Target>::const_iterator enditer = m_targets.end();
	unsigned i = 0;
	for (; iter != enditer; iter++, previter++, i++)
	{
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
		if (lstack[i] < rstack[i])
		{
			return true;
		}
		if (lstack[i] > rstack[i])
		{
			return false;
		}
	}

	/*
	If we get this far, then everything is the same.
	However we must conform to strict weak ordering,
	so use the order of creation, comprised of strokeId and,
	repeatId.
	*/

	if (a.strokeId() < b.strokeId())
	{
		return true;
	}

	if (a.strokeId() > b.strokeId())
	{
		return false;
	}
	if (a.repeatId() < b.repeatId())
	{
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

void Stroke::getParams(MDoubleArray &result) const
{
	std::vector<Target>::const_iterator citer = m_targets.begin();
	for (; citer != m_targets.end(); citer++)
	{
		result.append(citer->param());
	}
}

void Stroke::getCurveParams(MDoubleArray &result) const
{
	std::vector<Target>::const_iterator citer = m_targets.begin();
	for (; citer != m_targets.end(); citer++)
	{
		result.append(citer->curveParam());
	}
}

void Stroke::appendTargets(MMatrixArray &result) const
{
	std::vector<Target>::const_iterator citer;
	unsigned i = 0;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++, i++)
	{
		result.append(citer->matrix());
	}
}

void Stroke::appendTangents(MVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->tangent());
	}
}

const std::vector<Target> &Stroke::targets() const
{
	return m_targets;
}

const double &Stroke::arcLength() const
{
	return m_arcLength;
}

const double &Stroke::paintFlow() const
{
	return m_paintFlow;
}

unsigned Stroke::size(bool withTraversal) const
{
	if (withTraversal)
	{
		return m_targets.size() + m_arrivals.size() + 1;
	}
	return m_targets.size();
}

int Stroke::repeatId() const
{
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

int Stroke::brushId() const
{
	return m_brushId;
}
int Stroke::paintId() const
{
	return m_paintId;
}

void Stroke::setBrushId(int val)
{
	m_brushId = val;
}
void Stroke::setCustomBrushId(int val)
{
	m_customBrushId = val;
}
void Stroke::setPaintId(int val)
{
	m_paintId = val;
}

/////////////////////// SORT AND FILTER ///////////////////////

const MIntArray &Stroke::sortStack() const
{
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

bool Stroke::testAgainstValue(int lhs, FilterOperator op, int rhs) const
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
	return testAgainstValue(m_strokeId, op, value);
}
bool Stroke::testParentId(FilterOperator op, int value) const
{
	return testAgainstValue(m_parentId, op, value);
}
bool Stroke::testBrushId(FilterOperator op, int value) const
{
	return testAgainstValue(m_brushId, op, value);
}

bool Stroke::testCustomBrushId(FilterOperator op, int value) const
{
	return testAgainstValue(m_customBrushId, op, value);
}

bool Stroke::testPaintId(FilterOperator op, int value) const
{
	return testAgainstValue(m_paintId, op, value);
}
bool Stroke::testLayerId(FilterOperator op, int value) const
{
	return testAgainstValue(m_layerId, op, value);
}
bool Stroke::testRepeatId(FilterOperator op, int value) const
{
	return testAgainstValue(m_repeatId, op, value);
}
bool Stroke::testTargetCount(FilterOperator op, int value) const
{
	return testAgainstValue(m_targets.size(), op, value);
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
	return testAgainstValue(int(m_filterColor.x * 256), op, value);
}
bool Stroke::testMapGreenId(FilterOperator op, int value) const
{
	return testAgainstValue(int(m_filterColor.y * 256), op, value);
}
bool Stroke::testMapBlueId(FilterOperator op, int value) const
{
	return testAgainstValue(int(m_filterColor.z * 256), op, value);
}

//////////////////////////////////////////////

/////////////////////// FOR DRAWING ///////////////////////

void Stroke::getDirectionMatrices(MMatrixArray &result, double stackHeight) const
{
	MVector stackOffset = MVector::zAxis * stackHeight;
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		MMatrix mat = citer->directionMatrix(m_backstroke);
		mat[3][2] += stackHeight;
		result.append(mat);
	}
}

void Stroke::getPoints(MFloatPointArray &result, double stackHeight,
					   bool withTraversal) const
{

	result.clear();
	MPointArray pts;
	getPoints(pts, stackHeight, withTraversal);
	for (int i = 0; i < pts.length(); ++i)
	{
		result.append(pts[i]);
	}
}

void Stroke::getPoints(MPointArray &result, double stackHeight,
					   bool withTraversal) const
{

	MVector stackOffset = MVector::zAxis * stackHeight;
	std::vector<Target>::const_iterator citer;
	if (withTraversal)
	{
		for (citer = m_arrivals.begin(); citer != m_arrivals.end(); citer++)
		{
			result.append(citer->position() + stackOffset);
		}
	}
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->position() + stackOffset);
	}
	if (withTraversal)
	{
		result.append(m_departure.position() + stackOffset);
	}
}

void Stroke::transform(const MVector &vec, MFloatVectorArray &result,
					   bool withTraversal) const
{
	result.clear();
	MVectorArray vecs;
	transform(vec, vecs, withTraversal);
	for (int i = 0; i < vecs.length(); ++i)
	{
		result.append(vecs[i]);
	}
}

void Stroke::transform(const MVector &vec, MVectorArray &result,
					   bool withTraversal) const
{
	std::vector<Target>::const_iterator citer;
	if (withTraversal)
	{
		for (citer = m_arrivals.begin(); citer != m_arrivals.end(); citer++)
		{
			result.append(citer->transform(vec));
		}
	}
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->transform(vec));
	}
	if (withTraversal)
	{
		result.append(m_departure.transform(vec));
	}
}

void Stroke::getXAxes(MFloatVectorArray &result, bool withTraversal) const
{
	transform(MVector::xAxis, result, withTraversal);
}
void Stroke::getYAxes(MFloatVectorArray &result, bool withTraversal) const
{
	transform(MVector::yAxis, result, withTraversal);
}
void Stroke::getZAxes(MFloatVectorArray &result, bool withTraversal) const
{
	transform(MVector::zAxis, result, withTraversal);
}

void Stroke::getXAxes(MVectorArray &result, bool withTraversal) const
{
	transform(MVector::xAxis, result, withTraversal);
}
void Stroke::getYAxes(MVectorArray &result, bool withTraversal) const
{
	transform(MVector::yAxis, result, withTraversal);
}
void Stroke::getZAxes(MVectorArray &result, bool withTraversal) const
{
	transform(MVector::zAxis, result, withTraversal);
}

// void Stroke::getHead(MFloatPoint &result,  float stackHeight) const {
// 	MFloatVector stackOffset = MFloatVector::zAxis * stackHeight;
// 	result = MFloatPoint(m_targets[0].position()) + stackOffset;
// }

MPoint Stroke::getHead(double stackHeight) const
{
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

	MFloatVector stackOffset = MVector::zAxis * stackHeight;
	double width = brush.width() * 0.5;

	bool flat = (brush.shape() == Brush::kFlat);
	std::vector<Target>::const_iterator citer;
	unsigned i = 0;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++, i++)
	{
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

	MVector stackOffset = MVector::zAxis * stackHeight;
	double width = brush.width() * 0.5;
	bool flat = (brush.shape() == Brush::kFlat);
	std::vector<Target>::const_iterator citer;
	unsigned i = 0;
	unsigned j = len - 1;

	for (citer = m_targets.begin(); citer != m_targets.end(); citer++, i++, j--)
	{
		citer->getBorderPoints(result[i], result[j], width, flat, displayContactWidth);
		result[i] += stackOffset;
		result[j] += stackOffset;
	}
}

void Stroke::getTriangleStrip(
	const Brush &brush,
	double stackHeight,
	MPointArray &result,
	bool displayContactWidth,
	int maxSegments) const
{
	if (maxSegments == 0) {
		result.setLength(0);
		return;
	}
	unsigned len = m_targets.size();
	if (maxSegments>-1)
	{
	 	if (maxSegments+1 < len) {
			len = maxSegments+1;
		}
	}

	result.setLength(len * 2);

	MVector stackOffset = MVector::zAxis * stackHeight;
	double width = brush.width() * 0.5;

	bool flat = (brush.shape() == Brush::kFlat);
	std::vector<Target>::const_iterator citer;
	unsigned i = 0;
	unsigned j=0;

	for (citer = m_targets.begin(); (citer != m_targets.end()) && (j < len)  ; citer++, i += 2, j++)
	{
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
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->position(space));
	}
}

void Stroke::rotations(
	const MMatrix &space,
	MTransformationMatrix::RotationOrder order,
	MAngle::Unit unit,
	MVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->rotation(order, unit, space));
	}
}

void Stroke::arrivalPositions(const MMatrix &space, MPointArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_arrivals.begin(); citer != m_arrivals.end(); citer++)
	{
		result.append(citer->position(space));
	}
}

void Stroke::arrivalRotations(
	const MMatrix &space,
	MTransformationMatrix::RotationOrder order,
	MAngle::Unit unit,
	MVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_arrivals.begin(); citer != m_arrivals.end(); citer++)
	{
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
	MVector &result) const
{
	result = m_departure.rotation(order, unit, space);
}

void Stroke::tangents(const MMatrix &space, MVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->tangent() * space);
	}
}

void Stroke::setUV(
	const MMatrix &inversePlaneMatrix)
{
	MPoint p = ((m_pivot.position() * inversePlaneMatrix) * 0.5) + MVector(0.5, 0.5, 0.0);
	m_u = p.x;
	m_v = p.y;
}
void Stroke::getUV(float &u, float &v) const
{
	u = m_u;
	v = m_v;
}
void Stroke::assignTargetUVs(const MMatrix &inversePlaneMatrix)
{
	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
	{
		iter->setUV(inversePlaneMatrix);
	}

	for (iter = m_arrivals.begin(); iter != m_arrivals.end(); iter++)
	{
		iter->setUV(inversePlaneMatrix);
	}
	m_departure.setUV(inversePlaneMatrix);
}
void Stroke::appendTargetUVsTo(MFloatArray &uVals, MFloatArray &vVals) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_arrivals.begin(); citer != m_arrivals.end(); citer++)
	{
		citer->appendUVsTo(uVals, vVals);
	}
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		citer->appendUVsTo(uVals, vVals);
	}
	m_departure.appendUVsTo(uVals, vVals);
}

int Stroke::applyGlobalTilt(const MFloatVectorArray &gradients, int index)
{

	std::vector<Target>::iterator iter;
	for (iter = m_arrivals.begin(); iter != m_arrivals.end(); iter++, index++)
	{
		iter->applyGlobalTilt(gradients[index]);
	}
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++, index++)
	{
		iter->applyGlobalTilt(gradients[index]);
	}
	m_departure.applyGlobalTilt(gradients[index]);
	index++;
	return index;
}

void Stroke::displace(MFnMesh &meshFn, MMeshIsectAccelParams &ap)
{
	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
	{
		MFloatPoint rayOrigin = MFloatPoint(iter->position());
		rayOrigin.z = 0.0f;
		float dist = 0;
		MFloatPoint hitPoint;
		bool hit = meshFn.closestIntersection(
			rayOrigin, MFloatVector::zAxis, 0, 0, false,
			MSpace::kWorld,
			100.0f, true, &ap, hitPoint, &dist, 0, 0, 0, 0);
		if (hit)
		{
			iter->offsetBy(MVector(0.0, 0.0, dist));
		}
	}
}

void Stroke::setTransitionContact()
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
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
	{
		const double &param = iter->param();
		if (param >= m_entry_param)
		{
			break;
		}
		// cerr << "m_entry_param" << m_entry_param << endl;
		double contact = param / m_entry_param;

		if (m_transitionBlendMethod == Stroke::kTransitionMin)
		{
			contact = fmin(contact, iter->contact());
		}
		else if (m_transitionBlendMethod == Stroke::kTransitionMax)
		{
			contact = fmax(contact, iter->contact());
		}
		else
		{ // kTransitionBlend
			contact = (contact + iter->contact()) * 0.5;
		}
		iter->setContact(contact);
	}

	std::vector<Target>::reverse_iterator riter;

	for (riter = m_targets.rbegin(); riter != m_targets.rend(); riter++)
	{
		const double &param = riter->param();
		if (param <= m_exit_param)
		{
			break;
		}
		// cerr << " (1.0 - m_exit_param)" <<  (1.0 - m_exit_param) << endl;
		double contact = (1.0 - param) / (1.0 - m_exit_param);
		if (m_transitionBlendMethod == Stroke::kTransitionMin)
		{
			contact = fmin(contact, riter->contact());
		}
		else if (m_transitionBlendMethod == Stroke::kTransitionMax)
		{
			contact = fmax(contact, riter->contact());
		}
		else
		{ // kTransitionBlend
			contact = (contact + riter->contact()) * 0.5;
		}
		riter->setContact(contact);
	}
}

//
void Stroke::offsetBrushContact(const Brush &brush)
{
	float height = brush.transHeight();
	float power = brush.contactPower();

	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
	{
		float dist = (1.0 - iter->contact());
		dist = pow(dist, power) * height;
		if (m_localContact)
		{
			iter->offsetLocalZ(-dist);
		}
		else
		{
			iter->offsetBy(MVector::zAxis * dist);
		}
	}
}

void Stroke::applyBiases(const Brush &brush, float mult)
{
	applyForwardBias(brush, mult);
	// applyGravityBias(brush, mult);
}

MObject Stroke::generateNurbsCurve(const MPointArray &points, MStatus *st) const
{

	int count = points.length();
	double maxParam = count - 1;
	MFnNurbsCurve curveFn;
	MFnNurbsCurveData dataCreator;
	MObject curveData = dataCreator.create(st);

	MObject dCurve = curveFn.createWithEditPoints(points, 3, MFnNurbsCurve::kOpen, true, false, true, curveData, st);
	if (st->error())
	{
		return dCurve;
	}
	MDoubleArray knotVals;
	curveFn.getKnots(knotVals);
	int numKnots = knotVals.length();
	double maxValRecip = 1.0 / knotVals[(numKnots - 1)];
	for (int i = 0; i < numKnots; ++i)
	{
		knotVals[i] = knotVals[i] * maxValRecip;
	}
	curveFn.setKnots(knotVals, 0, (numKnots - 1));
	return dCurve;
}

void Stroke::applyForwardBias(const Brush &brush, float mult)
{
	MStatus st;
	float forwardBias0 = brush.forwardBias0() * mult;
	float forwardBias1 = brush.forwardBias1() * mult;
	if ((fabs(forwardBias0) < epsilon) && (fabs(forwardBias1) < epsilon))
	{
		return;
	}

	MPointArray editPoints;
	this->getPoints(editPoints, 0.0);
	int count = editPoints.length();
	double maxParam = count - 1;

	// MObject dCurve = this->generateNurbsCurve(editPoints, &st);
	// if (st.error())
	// {
	// 	return;
	// }
	// MFnNurbsCurve curveFn(dCurve);

	MFnNurbsCurve curveFn;
	MFnNurbsCurveData dataCreator;
	MObject curveData = dataCreator.create(&st);

	MObject dCurve = curveFn.createWithEditPoints(editPoints, 3, MFnNurbsCurve::kOpen, true, false, true, curveData, &st);
	if (st.error())
	{
		return;
	}
	MDoubleArray knotVals;
	curveFn.getKnots(knotVals);
	int numKnots = knotVals.length();
	double maxValRecip = 1.0 / knotVals[(numKnots - 1)];
	for (int i = 0; i < numKnots; ++i)
	{
		knotVals[i] = knotVals[i] * maxValRecip;
	}
	curveFn.setKnots(knotVals, 0, (numKnots - 1));
	////////
	double curveLength = curveFn.length(epsilon);
	MVector tangent0 = curveFn.tangent(0.000001).normal();
	MVector tangent1 = curveFn.tangent(.999999).normal();
	std::vector<Target>::iterator iter;
	int i = 0;
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++, i++)
	{
		// get the dist at param.
		double param = double(i) / maxParam;
		double distOnCurve = curveFn.findLengthFromParam(param, &st);
		mser;
		// get the contact and interpolate to get the forward amount
		float c = iter->contact();
		float forwardBias = (forwardBias1 * c) + (forwardBias0 * (1.0f - c));
		distOnCurve += forwardBias;
		MPoint newPoint;
		MVector newTangent;
		if (distOnCurve < 0.0)
		{
			// handle:  before start
			newPoint = editPoints[0] + (tangent0 * distOnCurve);
			newTangent = tangent0;
		}
		else if (distOnCurve > curveLength)
		{
			newPoint = editPoints[(count - 1)] + (tangent1 * (distOnCurve - curveLength));
			newTangent = tangent1;
			// handle : after end
		}
		else
		{
			double newParam = curveFn.findParamFromLength(distOnCurve, &st);
			mser;

			st = curveFn.getPointAtParam(newParam, newPoint, MSpace::kObject);
			newTangent = curveFn.tangent(newParam).normal();
		}
		if (m_backstroke)
		{
			newTangent = -newTangent;
		}
		// cerr << i << " param: " << param << " newPoint: " << newPoint << " newTangent: " << newTangent << endl;

		// bundle these three together.
		MVector offset = newPoint - editPoints[i];
		iter->offsetBy(offset);
		iter->setTangent(newTangent);
		iter->setRotation(m_follow, m_backstroke);
	}
}

void Stroke::applyGravityBias(const Brush &brush, float mult)
{
	MStatus st;

	float gravityBias0 = brush.gravityBias0() * mult;
	float gravityBias1 = brush.gravityBias1() * mult;
	if ((fabs(gravityBias0) < epsilon) && (fabs(gravityBias1) < epsilon))
	{
		return;
	}

	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
	{
		float c = iter->contact();
		float gravityBias = (gravityBias1 * c) + (gravityBias0 * (1.0f - c));
		MVector offset = MVector::yAxis * gravityBias;
		iter->offsetBy(offset);
	}
}

void Stroke::setLinearSpeed(float val)
{
	m_linearSpeed = val;
}
void Stroke::setAngularSpeed(float val)
{
	m_angularSpeed = val;
}

float Stroke::linearSpeed() const
{
	return m_linearSpeed;
}

float Stroke::angularSpeed() const
{
	return m_angularSpeed;
}

const Target &Stroke::departure() const
{
	return m_departure;
}

void Stroke::setDeparture(double offset)
{
	m_departure = Target(m_targets.back());
	MPoint p = m_departure.position();
	p.z = offset;
	m_departure.setPosition(p);
}

/*


MPoint Target::position(const MMatrix &space) const
{
	return MPoint(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]) * space;
}

void Target::setPosition(const MPoint &rhs)
*/

void Stroke::setArrival(double offset)
{
	Target arrival(m_targets.front());
	MPoint p = arrival.position();
	p.z = offset;
	arrival.setPosition(p);
	m_arrivals.push_back(arrival);
}

void Stroke::setArrival(double offset, double threshold, const Stroke &prev)
{
	const Target &departure = prev.departure();
	// MVector offsetVector(0.0, 0.0, offset);
	Target arrival(m_targets.front());
	MPoint p = arrival.position();
	p.z = offset;
	arrival.setPosition(p);

	double dist = departure.distanceTo(arrival);
	MPoint departurePos = departure.position();
	MPoint arrivalPos = arrival.position();

	if (dist > threshold)
	{
		int num_inbetweens = int(dist / threshold);
		for (int i = 0; i < num_inbetweens; ++i)
		{
			double fraction = (i + 1) / double(num_inbetweens + 1);
			MPoint newPoint((departurePos * (1.0 - fraction)) + (arrivalPos * fraction));

			Target stop;
			if (fraction < 0.5)
			{
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

void Stroke::reverseArray(const MDoubleArray &arr, MDoubleArray &result)
{
	for (int i = arr.length() - 1; i >= 0; --i)
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
	if (strokeDirection == Stroke::kForwards)
	{
		return false;
	}
	if (strokeDirection == Stroke::kBackwards)
	{
		return true;
	}

	MFnNurbsCurve curveFn(dCurve);
	double startParam = curveFn.findParamFromLength(startDist);
	double endParam = curveFn.findParamFromLength(endDist);

	if (strokeDirection == Stroke::kStartUppermost || strokeDirection == Stroke::kEndUppermost)
	{

		MVector comparison(MVector::yAxis);

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

	// We must be doing radial for sure

	MPoint startPoint, endPoint;
	curveFn.getPointAtParam(startParam, startPoint, MSpace::kObject);
	curveFn.getPointAtParam(endParam, endPoint, MSpace::kObject);
	// cerr << "start dist " << MVector(startPoint.x, startPoint.y, 0.0).length() << "end dist " << MVector(endPoint.x, endPoint.y, 0.0).length() << endl;
	if (MVector(startPoint.x, startPoint.y, 0.0).length() < MVector(endPoint.x, endPoint.y, 0.0).length())
	{
		// cerr << "stroke is moving outwards" << endl;
		// stroke is moving outwards
		if (strokeDirection == Stroke::kRadialIn)
		{
			// cerr << "dir is Stroke::kRadialIn - need to reverse" << endl;
			// need to reverse
			return true;
		}
		// cerr << "dir is Stroke::kRadialOut - DONT reverse" << endl;
		return false;
	}
	// cerr << "stroke must be moving inwards" << endl;
	// stroke must be moving inwards
	if (strokeDirection == Stroke::kRadialOut)
	{
		// cerr << "dir is Stroke::kRadialOut - need to reverse" << endl;
		// need to reverse
		return true;
	}
	// cerr << "dir is Stroke::kRadialIn - DONT reverse" << endl;

	return false;
}

void Stroke::rotate(double rotation)
{
	MMatrix rmat;
	double sz = sin(rotation);
	double cz = cos(rotation);
	rmat[0][0] = cz;
	rmat[0][1] = sz;
	rmat[1][0] = -sz;
	rmat[1][1] = cz;

	const MPoint &pos = m_pivot.position();

	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
	{
		iter->rotate(pos, rmat);
	}

	// if arivals and departure have already been set (unlkikely)
	// then we better rotate them too
	if (m_arrivals.size())
	{
		for (iter = m_arrivals.begin(); iter != m_arrivals.end(); iter++)
		{
			iter->rotate(pos, rmat);
		}
		m_departure.rotate(pos, rmat);
	}
}

void Stroke::translate(const MVector &translation, bool transformPivot)
{

	std::vector<Target>::iterator iter;
	for (iter = m_targets.begin(); iter != m_targets.end(); iter++)
	{
		iter->offsetBy(translation);
	}
	if (m_arrivals.size())
	{
		for (iter = m_arrivals.begin(); iter != m_arrivals.end(); iter++)
		{

			iter->offsetBy(translation);
		}
		m_departure.offsetBy(translation);
	}
	if (transformPivot)
	{
		m_pivot.offsetBy(translation);
	}
}
