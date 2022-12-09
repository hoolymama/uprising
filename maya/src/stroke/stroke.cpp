#include <maya/MFnNurbsCurve.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnNurbsCurveData.h>
#include <maya/MAngle.h>
#include <maya/MMatrixArray.h>
#include <maya/MQuaternion.h>
#include <algorithm>
#include <vector>
const double rad_to_deg = (180 / 3.1415927);
#include "errorMacros.h"
#include "mayaMath.h"
#include <stroke.h>
#include <enums.h>

const double epsilon = 0.0001;

Stroke::Stroke()
	: m_targets(),
	  m_pivot(),
	  m_parentId(0),
	  m_repeatId(0),
	  m_layerId(0),
	  m_brushId(0),
	  m_sortColor(),
	  m_filterColor(),
	  m_brushModelId(),
	  m_sortStack(),
	  m_arrivals(),
	  m_departure(),
	  m_linearSpeed(0.0),
	  m_angularSpeed(0.0),
	  m_approximationDistance(0.0),
	  m_coil(0.0f),
	  m_maxRadius(1.0f),
	  m_brushStrokeSpec(),
	  m_creatorName(),
	  m_creatorId(),
	  m_ditherProbability(1.0)
{
}

Stroke::Stroke(
	const MFloatPointArray &points,
	const MFloatMatrix &rotationMat)
	: Stroke()
{
	MStatus st;
	int len = points.length();
	MFloatMatrix mat = rotationMat;
	for (size_t i = 0; i < len; i++)
	{
		mat[3][0] = points[i].x;
		mat[3][1] = points[i].y;
		mat[3][2] = points[i].z;
		m_targets.push_back(Target(mat, 1.0));
	}
	m_pivot = Target(m_targets[0]);
	resetTangents();
}

Stroke::Stroke(
	const MFloatPointArray &points,
	const MColorArray &colors,
	const MFloatMatrix &rotationMat)
	: Stroke(points, rotationMat)
{
	std::vector<Target>::iterator iter = m_targets.begin();
	for (int i = 0; iter != m_targets.end(); iter++, i++)
	{
		iter->setColor(colors[i]);
	}
}

Stroke::Stroke(
	const std::vector<MFloatMatrix> &matrices)
	: Stroke()
{
	
	MStatus st;
	unsigned i = 0;
	std::vector<MFloatMatrix>::const_iterator current_matrix = matrices.begin();
	for (; current_matrix != matrices.end(); current_matrix++, i++)
	{
		m_targets.push_back(
			Target(*current_matrix, 1.0));
	}

	m_pivot = Target(m_targets[0]);
	resetTangents();
	
}

Stroke::Stroke(
	const MFloatPointArray &points,
	const MFloatArray &weights,
	const MFloatMatrix &rotationMat)
	: Stroke()
{

	MStatus st;
	int len = points.length();

	MFloatMatrix mat = rotationMat;
	for (size_t i = 0; i < len; i++)
	{
		mat[3][0] = points[i].x;
		mat[3][1] = points[i].y;
		mat[3][2] = points[i].z;

		m_targets.push_back(Target(mat, weights[i]));
	}

	m_pivot = Target(m_targets[0]);
	resetTangents();
}

Stroke::Stroke(
	const std::vector<MFloatMatrix> &matrices,
	const MFloatArray &weights)
	: Stroke()
{
	MStatus st;

	unsigned i = 0;
	std::vector<MFloatMatrix>::const_iterator current_matrix = matrices.begin();
	for (; current_matrix != matrices.end(); current_matrix++, i++)
	{
		m_targets.push_back(
			Target(*current_matrix, weights[i]));
	}

	m_pivot = Target(m_targets[0]);
	resetTangents();
}

Stroke::Stroke(
	const Stroke &instroke,
	unsigned start,
	unsigned count)
	: Stroke()
{

	if ((start + count) > instroke.targets().size() || count < 2)
	{
		return;
	}
	MStatus st;

	std::vector<Target>::const_iterator iter = instroke.targets().begin() + start;
	std::vector<Target>::const_iterator enditer = iter + count;

	for (; iter != enditer; iter++)
	{
		m_targets.push_back(Target(*iter));
	}
	m_pivot = instroke.pivot();
	resetTangents(); // because the end tangents will change.
}

Stroke::Stroke(
	const MPointArray &editPoints,
	float resampleDensity,
	int minimumPoints,
	const MFloatMatrix &rotationMat)
	: Stroke()
{

	MStatus st;
	MFnNurbsCurve curveFn;
	MFnNurbsCurveData dataCreator;
	MObject curveData = dataCreator.create(&st);
	mser;
	if (st.error())
		return;

	MObject curveObject = curveFn.createWithEditPoints(
		editPoints, 3, MFnNurbsCurve::kOpen,
		false,
		false,
		false,
		curveData,
		&st);
	mser;
	if (st.error())
		return;

	MDoubleArray knotVals;
	st = curveFn.getKnots(knotVals);
	int numKnots = knotVals.length();
	double recip = 1.0 / knotVals[(numKnots - 1)];
	for (int i = 0; i < numKnots; ++i)
	{
		knotVals[i] = knotVals[i] * recip;
	}
	curveFn.setKnots(knotVals, 0, (numKnots - 1));

	double curveLength = curveFn.length(epsilon);
	int numPoints = int(resampleDensity * fabs(curveLength));
	numPoints = std::max(numPoints, minimumPoints);
	float gap = curveLength / (numPoints - 1);

	MFloatPointArray points;

	for (unsigned i = 0; i < numPoints; i++)
	{
		float curveDist = i * gap;

		double curveParam = curveFn.findParamFromLength(curveDist);
		MPoint point;
		st = curveFn.getPointAtParam(curveParam, point, MSpace::kObject);
		mser;
		if (st.error())
			return;

		points.append(point);

	}

	MFloatMatrix mat = rotationMat;
	for (size_t i = 0; i < numPoints; i++)
	{
		mat[3][0] = points[i].x;
		mat[3][1] = points[i].y;
		mat[3][2] = points[i].z;
		m_targets.push_back(Target(mat));
	}
	m_pivot = Target(m_targets[0]);
	resetTangents();
}

Stroke::Stroke(
	const MPointArray &editPoints,
	MFloatArray radii,
	float resampleDensity,
	int minimumPoints,
	const MFloatMatrix &rotationMat)
	: Stroke()
{

	MStatus st;
	MFnNurbsCurve curveFn;
	MFnNurbsCurveData dataCreator;
	MObject curveData = dataCreator.create(&st);
	mser;
	if (st.error())
		return;

	MObject curveObject = curveFn.createWithEditPoints(
		editPoints, 3, MFnNurbsCurve::kOpen,
		false,
		false,
		false,
		curveData,
		&st);
	mser;
	if (st.error())
		return;

	MDoubleArray knotVals;
	st = curveFn.getKnots(knotVals);
	int numKnots = knotVals.length();
	double recip = 1.0 / knotVals[(numKnots - 1)];
	for (int i = 0; i < numKnots; ++i)
	{
		knotVals[i] = knotVals[i] * recip;
	}
	curveFn.setKnots(knotVals, 0, (numKnots - 1));

	double curveLength = curveFn.length(epsilon);
	int numPoints = int(resampleDensity * fabs(curveLength));
	numPoints = std::max(numPoints, minimumPoints);
	float gap = curveLength / (numPoints - 1);

	MFloatPointArray points;
	MFloatArray weights;

	for (unsigned i = 0; i < numPoints; i++)
	{
		float curveDist = i * gap;

		double curveParam = curveFn.findParamFromLength(curveDist);
		MPoint point;
		st = curveFn.getPointAtParam(curveParam, point, MSpace::kObject);
		mser;
		if (st.error())
			return;

		float weight = Stroke::interpFloat(radii, curveParam);

		points.append(point);
		weights.append(weight);
	}

	MFloatMatrix mat = rotationMat;
	for (size_t i = 0; i < numPoints; i++)
	{
		mat[3][0] = points[i].x;
		mat[3][1] = points[i].y;
		mat[3][2] = points[i].z;
		m_targets.push_back(Target(mat, weights[i]));
	}
	m_pivot = Target(m_targets[0]);
	resetTangents();
}






Stroke::Stroke(
	const MPointArray &editPoints,
	const MFloatArray &originalWeights,
	const MColorArray &originalColors,
	float resampleDensity,
	int minimumPoints,
	const MFloatMatrix &rotationMat)
	: Stroke()
{

	MStatus st;
	MFnNurbsCurve curveFn;
	MFnNurbsCurveData dataCreator;
	MObject curveData = dataCreator.create(&st);
	mser;
	if (st.error())
		return;

	MObject curveObject = curveFn.createWithEditPoints(
		editPoints, 3, MFnNurbsCurve::kOpen,
		false,
		false,
		false,
		curveData,
		&st);
	mser;
	if (st.error())
		return;

	MDoubleArray knotVals;
	st = curveFn.getKnots(knotVals);
	int numKnots = knotVals.length();
	double recip = 1.0 / knotVals[(numKnots - 1)];
	for (int i = 0; i < numKnots; ++i)
	{
		knotVals[i] = knotVals[i] * recip;
	}
	curveFn.setKnots(knotVals, 0, (numKnots - 1));

	double curveLength = curveFn.length(epsilon);
	int numPoints = int(resampleDensity * fabs(curveLength));
	numPoints = std::max(numPoints, minimumPoints);
	float gap = curveLength / (numPoints - 1);

	MFloatPointArray points;
	MFloatArray weights;
	MColorArray colors;
	

	for (unsigned i = 0; i < numPoints; i++)
	{
		float curveDist = i * gap;

		double curveParam = curveFn.findParamFromLength(curveDist);
		MPoint point;
		st = curveFn.getPointAtParam(curveParam, point, MSpace::kObject);
		mser;
		if (st.error())
			return;

		float weight = Stroke::interpFloat(originalWeights, curveParam);
		MColor color = Stroke::interpColor(originalColors, curveParam);
		points.append(point);
		weights.append(weight);
		colors.append(color);
	}

	MFloatMatrix mat = rotationMat;
	for (size_t i = 0; i < numPoints; i++)
	{
		mat[3][0] = points[i].x;
		mat[3][1] = points[i].y;
		mat[3][2] = points[i].z;
		m_targets.push_back(Target(mat, weights[i], colors[i]));
	}
	m_pivot = Target(m_targets[0]);
	resetTangents();
}



Stroke::Stroke(
	const MPointArray &editPoints,
	const MColorArray &colors,
	int numPoints,
	const MFloatMatrix &rotationMat
	)
	: Stroke()
{

	MStatus st;
	MFnNurbsCurve curveFn;
	MFnNurbsCurveData dataCreator;
	MObject curveData = dataCreator.create(&st);
	mser;
	if (st.error())
		return;

	MObject curveObject = curveFn.createWithEditPoints(
		editPoints, 3, MFnNurbsCurve::kOpen,
		false,
		false,
		false,
		curveData,
		&st);
	mser;
	if (st.error())
		return;

	MDoubleArray knotVals;
	st = curveFn.getKnots(knotVals);
	int numKnots = knotVals.length();
	double recip = 1.0 / knotVals[(numKnots - 1)];
	for (int i = 0; i < numKnots; ++i)
	{
		knotVals[i] = knotVals[i] * recip;
	}
	curveFn.setKnots(knotVals, 0, (numKnots - 1));

	double curveLength = curveFn.length(epsilon);
	float gap = curveLength / (numPoints - 1);

	MFloatPointArray points;
	MFloatArray weights;
	// MColorArray colors;
	

	for (unsigned i = 0; i < numPoints; i++)
	{
		float curveDist = i * gap;

		double curveParam = curveFn.findParamFromLength(curveDist);
		MPoint point;
		st = curveFn.getPointAtParam(curveParam, point, MSpace::kObject);
		mser;
		if (st.error())
			return;
 
		points.append(point);
		weights.append(1.0f);
	}

	MFloatMatrix mat = rotationMat;
	for (size_t i = 0; i < numPoints; i++)
	{
		mat[3][0] = points[i].x;
		mat[3][1] = points[i].y;
		mat[3][2] = points[i].z;
		m_targets.push_back(Target(mat, weights[i], colors[i]));
	}
	m_pivot = Target(m_targets[0]);
	resetTangents();
}



void Stroke::setCreator(const MString &creatorName, int creatorId)
{
	m_creatorName = creatorName;
	m_creatorId = creatorId;
}
const MString & Stroke::creatorName() const
{
	return m_creatorName;
}

int Stroke::creatorId() const
{
	return m_creatorId;
}

float Stroke::ditherProbability() const
{
	return m_ditherProbability;
}
void Stroke::setDitherProbability(float probability)
{
	m_ditherProbability = probability;
}

void Stroke::resetTangents()
{

	std::vector<Target>::iterator citer;
	std::vector<Target>::const_iterator piter;
	std::vector<Target>::const_iterator niter;

	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		piter = citer == m_targets.begin() ? citer : std::prev(citer);
		niter = std::next(citer) == m_targets.end() ? citer : std::next(citer);
		citer->setDrawTangent((niter->position() - piter->position()).normal());
	}
}

void Stroke::setStrokeId(unsigned rhs)
{
	m_strokeId = rhs;
}

float Stroke::calculateArcLength() const
{
	float result = 0.0f;

	std::vector<Target>::const_iterator previter = m_targets.begin();
	std::vector<Target>::const_iterator iter = std::next(previter);
	std::vector<Target>::const_iterator enditer = m_targets.end();

	for (; iter != enditer; iter++, previter++)
	{
		result += iter->distanceTo(*previter);
	}
	return result;
}

void Stroke::calculateParams(MFloatArray &result) const
{

	float param = 0.0f;
	result.append(param);
	std::vector<Target>::const_iterator previter = m_targets.begin();
	std::vector<Target>::const_iterator iter = std::next(previter);
	std::vector<Target>::const_iterator enditer = m_targets.end();

	for (; iter != enditer; iter++, previter++)
	{
		param += iter->distanceTo(*previter);
		result.append(param);
	}

	float paramRecip = 1.0 / param;
	for (size_t i = 1; i < result.length(); i++)
	{
		result[i] *= paramRecip;
	}
}

void Stroke::getPointAtParam(float param, MFloatPoint &result) const
{
	param = std::max(0.0f, std::min(param, 1.0f));

	if (param ==0.0f)
	{
		result = m_targets.front().position();
		return;
	}
	if (param == 1.0f)
	{
		result = m_targets.back().position();
		return;
	}

	MFloatArray params;
	calculateParams(params);
	for (int i = 0; i < params.length() - 1; i++)
	{
		if (param >= params[i] && param <= params[i+1])
		{
			float t = (param - params[i]) / (params[i+1] - params[i]);
			MPoint p0 = m_targets[i].position();
			MPoint p1 = m_targets[i+1].position();
			result = p0 + (p1 - p0) * t;
			return;
		}
	}
}
	
	void Stroke::setBrushStrokeSpec(const BrushStrokeSpec &rhs)
	{
		m_brushStrokeSpec = rhs;
	}
	
	const BrushStrokeSpec &  Stroke::brushStrokeSpec() const
	{
		return m_brushStrokeSpec  ;
	}
	


void Stroke::smoothTargets(int neighbors, bool doPositions, bool doWeights)
{
	if (neighbors < 1)
	{
		return;
	}
	int count = m_targets.size();
	std::vector<Target>::iterator iter = m_targets.begin();
	for (int i = 0; iter != m_targets.end(); iter++, i++)
	{
		int fromStart = i;
		int fromEnd = (count - 1) - i;
		int n = std::min(neighbors, std::min(fromStart, fromEnd));
		if (n < 1)
		{
			continue;
		}
		float denom = (2 * n) + 1;

		if (doWeights)
		{
			float meanWeight = 0.0;
			for (int j = i - n; j < i + n + 1; j++)
			{
				meanWeight += m_targets[j].radius();
			}
			meanWeight = meanWeight / denom;
			iter->setRadius(meanWeight);
		}

		if (doPositions)
		{
			MFloatVector meanPoint;
			for (int j = i - n; j < i + n + 1; j++)
			{
				const MFloatMatrix mat = m_targets[j].matrix();
				meanPoint += MFloatVector(mat[3][0], mat[3][1], mat[3][2]);
			}
			meanPoint = meanPoint / denom;
			iter->setPosition(MFloatPoint(meanPoint));
		}
	}
}

void Stroke::setCoil(float rhs)
{
	m_coil = rhs;
}

float Stroke::coil() const
{
	return m_coil;
}

const Target &Stroke::pivot() const
{
	return m_pivot;
}


void Stroke::setPivotPosition(const MFloatPoint &rhs)
{
	m_pivot.setPosition(rhs);
}


void Stroke::setPivotMatrix(const MFloatMatrix &rhs)
{
	m_pivot.setMatrix(rhs);
}



Stroke::~Stroke() {}

// bool Stroke::backstroke() const
// {
// 	return m_backstroke;
// }

bool operator<(const Stroke &a, const Stroke &b)
{
	const MIntArray &lstack = a.sortStack();
	const MIntArray &rstack = b.sortStack();

	int len = lstack.length();

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

// void Stroke::appendTangents(MVectorArray &result) const
// {
// 	std::vector<Target>::const_iterator citer;
// 	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
// 	{
// 		result.append(citer->tangent());
// 	}
// }

const std::vector<Target> &Stroke::targets() const
{
	return m_targets;
}

unsigned Stroke::valid() const
{
	return m_targets.size() > 1;
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

float Stroke::maxRadius() const
{
	return m_maxRadius;
}


void Stroke::setParentId(int parentId)
{
	m_parentId = parentId;
}

void Stroke::setRepeatId(int rhs)
{
	m_repeatId = rhs;
}

void Stroke::setMaxRadius(float rhs)
{
	m_maxRadius = rhs;
}


void Stroke::setLayerId(int rhs)
{
	m_layerId = rhs;
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
void Stroke::setBrushId(int val)
{
	m_brushId = val;
}

int Stroke::paintId() const
{
	return m_paintId;
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

void Stroke::setBrushModelId(int rhs)
{
	m_brushModelId = rhs;
}

int Stroke::brushModelId() const
{
	return m_brushModelId;
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

// void Stroke::appendCustomBrushIdToSortStack(bool ascending)
// {
// 	int val = ascending ? int(m_customBrushId) : -int(m_customBrushId);
// 	m_sortStack.append(val);
// }

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

void Stroke::appendBrushModelIdToSortStack(bool ascending)
{
	int val = ascending ? m_brushModelId : -m_brushModelId;
	m_sortStack.append(val);
}

void Stroke::appendBrushShapeToSortStack(bool ascending)
{
	int val = int(m_brushStrokeSpec.shape);
	val = ascending ? val : -val;
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

bool Stroke::testBrushModelId(FilterOperator op, int value) const
{
	return testAgainstValue(m_brushModelId, op, value);
}

bool Stroke::testBrushShape(FilterOperator op, int value) const
{
	return testAgainstValue(int(m_brushStrokeSpec.shape), op, value);
}


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

void Stroke::getPoints(
	MFloatPointArray &result,
	bool withTraversal) const
{

	std::vector<Target>::const_iterator citer;
	if (withTraversal)
	{
		for (citer = m_arrivals.begin(); citer != m_arrivals.end(); citer++)
		{
			result.append(citer->position());
		}
	}
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->position());
	}
	if (withTraversal)
	{
		result.append(m_departure.position());
	}
}

void Stroke::transform(const MFloatVector &vec, MFloatVectorArray &result,
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
	transform(MFloatVector::xAxis, result, withTraversal);
}
void Stroke::getYAxes(MFloatVectorArray &result, bool withTraversal) const
{
	transform(MFloatVector::yAxis, result, withTraversal);
}
void Stroke::getZAxes(MFloatVectorArray &result, bool withTraversal) const
{
	transform(MFloatVector::zAxis, result, withTraversal);
}

MFloatPoint Stroke::getHead(
	const MFloatVector &planeNormal /* ,float stackHeight */) const
{
	// MFloatVector stackOffset = planeNormal * stackHeight;
	return m_targets[0].position() /*  + stackOffset */;
}

void Stroke::getBorders(
	const MFloatVector &planeNormal,
	const Brush &brush,
	// float stackHeight,
	MFloatPointArray &lefts,
	MFloatPointArray &rights,
	bool scaleWidthByWeight,
	int maxSegments) const
{
	if (maxSegments == 0)
	{
		return;
	}
	unsigned len = m_targets.size();
	if (maxSegments > -1)
	{
		if (maxSegments + 1 < len)
		{
			len = maxSegments + 1;
		}
	}

	lefts.setLength(len);
	rights.setLength(len);

	// MFloatVector stackOffset = planeNormal * stackHeight;
	float width = brush.width() * 0.5f;
	bool flat = (brush.shape() == Brush::kFlat);
	std::vector<Target>::const_iterator citer;

	unsigned i = 0;
	for (citer = m_targets.begin();
		 citer != m_targets.end() && i < len;
		 citer++, i++)
	{

		MFloatMatrix mat = citer->viewMatrix(planeNormal);
		float weight = citer->weight();
		if (flat || (!scaleWidthByWeight))
		{
			weight = 1.0f;
		}
		MFloatPoint offset = MFloatPoint(MFloatVector::yAxis * width * weight);
		lefts[i] = (offset * mat) /* + stackOffset */;
		rights[i] = ((offset * -1.0) * mat) /* + stackOffset */;
	}
}

void Stroke::getBorderLoop(
	const MFloatVector &planeNormal,
	const Brush &brush,
	// float stackHeight,
	MFloatPointArray &result,
	bool scaleWidthByWeight,
	int maxSegments) const
{

	MFloatPointArray lefts;
	MFloatPointArray rights;
	getBorders(planeNormal, brush /* , stackHeight */, lefts, rights, scaleWidthByWeight, maxSegments);

	result.clear();
	for (size_t i = 0; i < lefts.length(); i++)
	{
		result.append(lefts[i]);
	}

	for (size_t i = rights.length(); i > 0; i--)
	{
		result.append(rights[(i - 1)]);
	}
}

void Stroke::getTriangleStrip(
	const MFloatVector &planeNormal,
	const Brush &brush,
	// float stackHeight,
	MFloatPointArray &result,
	bool scaleWidthByWeight,
	int maxSegments) const
{

	MFloatPointArray lefts, rights;
	getBorders(planeNormal, brush /* , stackHeight */, lefts, rights, scaleWidthByWeight, maxSegments);

	result.clear();
	for (size_t i = 0; i < lefts.length(); i++)
	{
		result.append(lefts[i]);
		result.append(rights[i]);
	}
}

void Stroke::getTargetBorderColors(
	MColorArray &result,
	int maxSegments,
	PaintingEnums::TargetColorsDisplay displayMode) const
{
	result.clear();
	if (maxSegments == 0)
	{
		return;
	}
	unsigned len = m_targets.size();
	if (maxSegments > -1)
	{
		if (maxSegments + 1 < len)
		{
			len = maxSegments + 1;
		}
	}

	result.setLength(len * 2);

	std::vector<Target>::const_iterator citer;
	unsigned i = 0;
	unsigned j = 0;
	MColor color;
	if (displayMode == PaintingEnums::kTargetColorsBlend)
	{
		for (citer = m_targets.begin(); (citer != m_targets.end()) && (j < len); citer++, i += 2, j++)
		{
			const MColor &tc = citer->color();
			color = MColor((tc.r + tc.a), (tc.g + tc.a), (tc.b + tc.a)) * 0.5f;
			result.set(color, i);
			result.set(color, (i + 1));
		}
	}
	else if (displayMode == PaintingEnums::kTargetColorsWhite)
	{

		for (citer = m_targets.begin(); (citer != m_targets.end()) && (j < len); citer++, i += 2, j++)
		{
			const MColor &tc = citer->color();
			color = MColor(tc.a, tc.a, tc.a);
			result.set(color, i);
			result.set(color, (i + 1));
		}
	}
	else if (displayMode == PaintingEnums::kTargetColorsRGB)
	{
		for (citer = m_targets.begin(); (citer != m_targets.end()) && (j < len); citer++, i += 2, j++)
		{
			const MColor &tc = citer->color();
			color = MColor(tc.r, tc.g, tc.b);
			result.set(color, i);
			result.set(color, (i + 1));
		}
	}
	else
	{ // wait
		for (citer = m_targets.begin(); (citer != m_targets.end()) && (j < len); citer++, i += 2, j++)
		{
			const float &wait = citer->wait();
			color = MColor(wait, wait, wait);
			result.set(color, i);
			result.set(color, (i + 1));
		}
	}
}
///////// ///////// /////////

///////// ///////// /////////

void Stroke::positions(
	const MFloatMatrix &space,
	MFloatPointArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->position(space));
	}
}





void Stroke::rotations(
	const MFloatMatrix &space,
	MTransformationMatrix::RotationOrder order,
	MAngle::Unit unit,
	MFloatVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->rotation(order, unit, space));
	}
}

void Stroke::arrivalPositions(
	const MFloatMatrix &space,
	MFloatPointArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_arrivals.begin(); citer != m_arrivals.end(); citer++)
	{
		result.append(citer->position(space));
	}
}

void Stroke::arrivalRotations(
	const MFloatMatrix &space,
	MTransformationMatrix::RotationOrder order,
	MAngle::Unit unit,
	MFloatVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_arrivals.begin(); citer != m_arrivals.end(); citer++)
	{
		result.append(citer->rotation(order, unit, space));
	}
}

void Stroke::departurePosition(
	const MFloatMatrix &space,
	MFloatPoint &result) const
{
	result = m_departure.position(space);
}

void Stroke::departureRotation(
	const MFloatMatrix &space,
	MTransformationMatrix::RotationOrder order,
	MAngle::Unit unit,
	MFloatVector &result) const
{
	result = m_departure.rotation(order, unit, space);
}

void Stroke::drawTangents(
	const MFloatMatrix &space,
	MFloatVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->drawTangent() * space);
	}
}

void Stroke::drawTangents(MFloatVectorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->drawTangent());
	}
}

void Stroke::colors(MColorArray &result) const
{
	std::vector<Target>::const_iterator citer;
	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
	{
		result.append(citer->color());
	}
}

float Stroke::angularSpeed(MAngle::Unit unit) const
{
	if (unit == MAngle::kRadians)
	{
		return m_angularSpeed;
	}
	return m_angularSpeed * rad_to_deg;
}

void Stroke::setAngularSpeed(float val)
{
	m_angularSpeed = val;
}

float Stroke::linearSpeed() const
{
	return m_linearSpeed;
}

void Stroke::setLinearSpeed(float val)
{
	m_linearSpeed = val;
}

float Stroke::approximationDistance() const
{
	return m_approximationDistance;
}

void Stroke::setApproximationDistance(float val)
{
	m_approximationDistance = val;
}

const Target &Stroke::departure() const
{
	return m_departure;
}

void Stroke::setDeparture(const MFloatVector &offset)
{
	m_departure = Target(m_targets.back());
	m_departure.offsetBy(offset);
}

void Stroke::setArrival(const MFloatVector &offset)
{
	Target arrival(m_targets.front());
	arrival.offsetBy(offset);
	m_arrivals.push_back(arrival);
}

void Stroke::setArrival(
	const MFloatVector &offset,
	float threshold,
	const Stroke &prev)
{
	const Target &departure = prev.departure();
	// MVector offsetVector(0.0, 0.0, offset);
	Target arrival(m_targets.front());
	arrival.offsetBy(offset);

	float dist = departure.distanceTo(arrival);
	MFloatPoint departurePos = departure.position();
	MFloatPoint arrivalPos = arrival.position();

	if (dist > threshold)
	{
		int num_inbetweens = int(dist / threshold);
		// cerr << "Stroke::setArrival num_inbetweens: " << num_inbetweens << endl;
		for (int i = 0; i < num_inbetweens; ++i)
		{
			float fraction = (i + 1) / double(num_inbetweens + 1);
			MFloatPoint newPoint((departurePos * (1.0 - fraction)) + (arrivalPos * fraction));

			Target stop(arrival);
			stop.setPosition(newPoint);
			m_arrivals.push_back(stop);
			// if (fraction < 0.5)
			// {
			// 	stop = Target(departure);
			// }
			// else
			// {
			// 	stop = Target(arrival);
			// }
			// stop.setPosition(newPoint);
			// m_arrivals.push_back(stop);
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

// void Stroke::calculateTubeMatrices(const MMatrix&initialMatrix ,const  MVector & initialTangent, MMatrixArray & tubeMatrices) const {

// 	MVector lastTangent = initialTangent;
// 	MMatrix rMatrix;
// 	rMatrix[3][0] = 0.0;
// 	rMatrix[3][1] = 0.0;
// 	rMatrix[3][2] = 0.0;

// 	std::vector<Target>::const_iterator citer;
// 	for (citer = m_targets.begin(); citer != m_targets.end(); citer++)
// 	{
// 		const MVector &tangent = citer->drawTangent();
// 		MMatrix deltaRotation = MQuaternion(lastTangent, tangent).asMatrix();
// 		rMatrix *= deltaRotation;

// 		MMatrix tf(rMatrix); // copy

// 		const MFloatMatrix &targetMatrix = citer->matrix();
// 		tf[3][0] = targetMatrix[3][0];
// 		tf[3][1] = targetMatrix[3][1];
// 		tf[3][2] = targetMatrix[3][2];
// 		tubeMatrices.append(tf);
// 		lastTangent = tangent;
// 	}
// }



ostream &operator<<(ostream &os, const Stroke &s)
{

	const std::vector<Target> &targets = s.targets();
	os << "TTT: Size" << targets.size() << endl;
	std::vector<Target>::const_iterator citer;
	for (citer = targets.begin(); citer != targets.end(); citer++)
	{
		os << "TTT: p" << citer->position() << " t:" << citer->drawTangent() << " contact: " << citer->weight() << endl;
	}

	return os;
}
