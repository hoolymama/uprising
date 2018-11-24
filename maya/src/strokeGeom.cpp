
#include "strokeGeom.h"
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);

MFloatPoint extractfPos(const MMatrix &mat) {
	return MFloatPoint(mat[3][0], mat[3][1], mat[3][2]);
}

MPoint extractPos(const MMatrix &mat) {
	return MPoint(mat[3][0], mat[3][1], mat[3][2]);
}

MVector extractRotation(const MMatrix &mat,
                        MTransformationMatrix::RotationOrder order,
                        MAngle::Unit unit)
{
	double rotValue[3];
	MTransformationMatrix tMat(mat);
	tMat.reorderRotation(order);

	MTransformationMatrix::RotationOrder throwAway;
	tMat.getRotation( rotValue, throwAway );
	if (unit == MAngle::kDegrees) {
		rotValue[0] *= rad_to_deg;
		rotValue[1] *= rad_to_deg;
		rotValue[2] *= rad_to_deg;
	}
	return MVector(rotValue[0], rotValue[1], rotValue[2]);
}
// strokeGeom::strokeGeom():
// 	m_id(-1),
// 	m_parentId(-1),
// 	hasSortStack(false),
// 	m_pivot(MPoint(1, 2, 3))
// {}
strokeGeom::strokeGeom(int id, const Stroke &src, short brushId, short paintId,
                       int layerId, bool force):
	m_id(id),
	m_parentId(0),
	m_startApproach(),
	m_endApproach(),
	m_targets(),
	m_tangents(),
	m_planeNormal(src.planeNormal()),
	m_direction(src.direction()),
	m_arcLength(src.arcLength()),
	m_pivot(src.pivot()),
	m_repeatId(src.repeatId()),
	m_brushId(brushId),
	m_paintId(paintId),
	m_layerId(layerId),
	m_forceDip(force),
	m_preStops(),
	m_sortColor(),
	m_sortStack(),
	m_u(0.0f),
	m_v(0.0f)
	// m_globalId()
	// hasSortStack(false)

{
	src.appendTargets(m_targets);
	src.appendTangents(m_tangents);
	src.getApproachTargets(m_startApproach, m_endApproach);
	// cerr << "stroke geom direction" << src.direction() << endl;
}

strokeGeom::~strokeGeom() {}

const MPoint &strokeGeom::pivot() const {
	return m_pivot;
}

bool strokeGeom::overlapsPlane(const MMatrix &inversePlaneMatrix) const {
	unsigned len = m_targets.length();
	for (int i = 0; i < len; ++i)
	{
		MPoint pt = extractPos(m_targets[i]);
		if (pt.x > -1 && pt.x < 1 && pt.y > -1 && pt.y < 1) {
			return true;
		}
	}
	return false;
}

const MMatrix &strokeGeom::startApproach() const {
	return m_startApproach;
}
const MMatrix &strokeGeom::endApproach() const {
	return m_endApproach;
}
const MMatrixArray &strokeGeom::targets() const {
	return m_targets;
}
const MVectorArray &strokeGeom::tangents() const {
	return m_tangents;
}
double strokeGeom::arcLength() const {
	return m_arcLength;
}
short strokeGeom::direction() const {
	return m_direction;
}
const MVector &strokeGeom::planeNormal() const {
	return m_planeNormal;
}

short strokeGeom::brushId() const {
	return m_brushId;
}
short strokeGeom::paintId() const {
	return m_paintId;
}


void strokeGeom::setBrushId(short val) {
	m_brushId = val;
}
void strokeGeom::setPaintId(short val) {
	m_paintId = val;
}

void strokeGeom::setSortColor(const MFloatVector &color)
{
	m_sortColor = color;
}
void strokeGeom::setFilterColor(const MFloatVector &color)
{
	m_filterColor = color;
}

void strokeGeom::appendIdToSortStack(bool ascending) {
	int val = ascending ? m_id : -m_id;
	m_sortStack.append(val);
}

void strokeGeom::appendParentIdToSortStack(bool ascending) {
	int val = ascending ? m_parentId : -m_parentId;
	m_sortStack.append(val);
}

void strokeGeom::appendBrushIdToSortStack(bool ascending) {
	int val = ascending ? int(m_brushId) : -int(m_brushId);
	m_sortStack.append(val);
}

void strokeGeom::appendPaintIdToSortStack(bool ascending) {
	int val = ascending ? int(m_paintId) : -int(m_paintId);
	m_sortStack.append(val);
}

void strokeGeom::appendLayerIdToSortStack(bool ascending) {
	int val = ascending ? int(m_layerId) : -int(m_layerId);
	m_sortStack.append(val);
}

void strokeGeom::appendRepeatIdToSortStack(bool ascending) {
	int val = ascending ? int(m_repeatId) : -int(m_repeatId);
	m_sortStack.append(val);
}


void strokeGeom::appendCustomBrushIdToSortStack(bool ascending) {
	int val = ascending ? int(m_customBrushId) : -int(m_customBrushId);
	m_sortStack.append(val);
}

void strokeGeom::appendCustomPaintIdToSortStack(bool ascending) {
	int val = ascending ? int(m_customPaintId) : -int(m_customPaintId);
	m_sortStack.append(val);
}

void strokeGeom::appendMapRedIdToSortStack(bool ascending) {
	int val = ascending ? int(m_sortColor.x * 256) : -int(m_sortColor.x * 256);
	m_sortStack.append(val);
}
void strokeGeom::appendMapGreenIdToSortStack(bool ascending) {
	int val = ascending ? int(m_sortColor.y * 256) : -int(m_sortColor.y * 256);
	m_sortStack.append(val);
}
void strokeGeom::appendMapBlueIdToSortStack(bool ascending) {
	int val = ascending ? int(m_sortColor.z * 256) : -int(m_sortColor.z * 256);
	m_sortStack.append(val);
}

bool strokeGeom::testAgainstValue(int lhs, StrokeFilterOperator op, int rhs ) const
{
	switch (op)
	{
		case strokeGeom::kLessThan:
			return lhs < rhs;
		case strokeGeom::kGreaterThan:
			return lhs > rhs;
		case strokeGeom::kEqualTo:
			return lhs == rhs;
		case strokeGeom::kNotEqualTo:
			return lhs != rhs;
		default:
			return false;
	}
}



bool strokeGeom::testId(StrokeFilterOperator op, int value) const {return  testAgainstValue(m_id, op, value);}
bool strokeGeom::testParentId(StrokeFilterOperator op, int value) const {return  testAgainstValue(m_parentId, op, value);}
bool strokeGeom::testBrushId(StrokeFilterOperator op, int value) const {
	return  testAgainstValue(int(m_brushId), op, value);
}
bool strokeGeom::testPaintId(StrokeFilterOperator op, int value) const {
	return  testAgainstValue(int(m_paintId), op, value);
}
bool strokeGeom::testLayerId(StrokeFilterOperator op, int value) const {return  testAgainstValue(m_layerId, op, value);}
bool strokeGeom::testRepeatId(StrokeFilterOperator op, int value) const {return  testAgainstValue(m_repeatId, op, value);}

bool strokeGeom::testCustomBrushId(StrokeFilterOperator op, int value) const {return  testAgainstValue(int(m_customBrushId), op, value);}
bool strokeGeom::testCustomPaintId(StrokeFilterOperator op, int value) const {return  testAgainstValue(int(m_customPaintId), op, value);}


bool strokeGeom::testMapRedId(StrokeFilterOperator op, int value) const {return  testAgainstValue(int(m_filterColor.x * 256), op, value);}
bool strokeGeom::testMapGreenId(StrokeFilterOperator op, int value) const {return  testAgainstValue(int(m_filterColor.y * 256), op, value);}
bool strokeGeom::testMapBlueId(StrokeFilterOperator op, int value) const {return  testAgainstValue(int(m_filterColor.z * 256), op, value);}




bool strokeGeom::forceDip() const {
	return m_forceDip;
}

int strokeGeom::id() const
{
	return m_id;
}
int strokeGeom::parentId() const
{
	return m_parentId;
}

int strokeGeom::repeatId() const
{
	return m_repeatId;
}
int strokeGeom::layerId() const
{
	return m_layerId;
}


void strokeGeom::setParentId(int parentId)
{
	m_parentId = parentId;
}


// int strokeGeom: globalId() const {
// 	return m_globalId;
// }

// void strokeGeom: setGlobalId(int globalId)  {
// 	m_globalId = globalId;
// }


const MIntArray &strokeGeom::sortStack() const {
	return m_sortStack;
}

void strokeGeom::clearSortStack()
{
	m_sortStack.clear();
}

// friend bool strokeGeom::operator < (const strokeGeom &rhs) const
// {
// 	int len = m_sortStack.length();
// 	if (len !=  rhs.m_sortStack.length()) {
// 		return false;
// 	}
// 	for (int i = 0; i < len; ++i)
// 	{
// 		if (m_sortStack[i] < rhs.m_sortStack[i]) {
// 			return true;
// 		}
// 		if (m_sortStack[i] > rhs.m_sortStack[i]) {
// 			return false;
// 		}
// 	}
// 	return true;
// }

void strokeGeom::getHead(MFloatPoint &result, bool withLift,
                         double stackHeight) const {
	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);
	int first = withLift ? 0 : 1;
	result = extractfPos(m_targets[first]) + stackOffset;
}



void strokeGeom::getDirectionMatrices(MMatrixArray &result, bool withLift,
                                      double stackHeight) const {
	MVector stackOffset = m_planeNormal * stackHeight;
	unsigned len = m_targets.length();
	int first = withLift ? 0 : 1;
	int end = withLift ? len : len - 1;
	for (int i = first; i < end; ++i)
	{
		const MVector &front = m_tangents[i];
		const MVector &up = m_planeNormal;
		MVector side = (up ^ front).normal();
		MPoint p = extractPos(m_targets[i]) + stackOffset;

		MMatrix res;
		res[0][0] = front.x; res[0][1] = front.y; res[0][2] = front.z; res[0][3] = 0.0;
		res[1][0] = side.x; res[1][1] = side.y; res[1][2] = side.z; res[1][3] = 0.0;
		res[2][0] = up.x; res[2][1] = up.y; res[2][2] = up.z; res[2][3] = 0.0;
		res[3][0] = p.x; res[3][1] = p.y; res[3][2] = p.z; res[3][3] = 1.0;



		result.append(res);
	}
}



void strokeGeom::getPoints(MFloatPointArray &result, bool withLift,
                           double stackHeight) const {
	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);
	unsigned len = m_targets.length();
	int first = withLift ? 0 : 1;
	int end = withLift ? len : len - 1;
	for (int i = first; i < end; ++i)
	{
		result.append(extractfPos(m_targets[i]) + stackOffset);
	}
}

void strokeGeom::getXAxes(MFloatVectorArray &result, bool withLift) const {
	unsigned len = m_targets.length();
	int first = withLift ? 0 : 1;
	int end = withLift ? len : len - 1;
	for (int i = first; i < end; ++i)
	{
		result.append( (MFloatVector(MVector::xAxis * m_targets[i])) );
	}
}

void strokeGeom::getYAxes(MFloatVectorArray &result, bool withLift) const {
	unsigned len = m_targets.length();
	int first = withLift ? 0 : 1;
	int end = withLift ? len : len - 1;
	for (int i = first; i < end; ++i)
	{
		result.append( (MFloatVector(MVector::yAxis * m_targets[i])) );
	}
}

void strokeGeom::getZAxes(MFloatVectorArray &result, bool withLift) const {
	unsigned len = m_targets.length();
	int first = withLift ? 0 : 1;
	int end = withLift ? len : len - 1;
	for (int i = first; i < end; ++i)
	{
		result.append( (MFloatVector(MVector::zAxis * m_targets[i])));
	}
}


void strokeGeom::getStopPoints(MFloatPointArray &result, double stackHeight) const {
	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);
	unsigned len = m_preStops.length();
	result.setLength(len);
	for (int i = 0; i < len; ++i)
	{
		result.set(extractfPos(m_preStops[i]) + stackOffset , i);
	}
}

void strokeGeom::getStopXAxes(MFloatVectorArray &result) const {
	unsigned len = m_preStops.length();
	result.setLength(len);
	for (int i = 0; i < len; ++i)
	{
		result.set( (MFloatVector(MVector::xAxis * m_preStops[i])) , i);
	}
}

void strokeGeom::getStopYAxes(MFloatVectorArray &result) const {
	unsigned len = m_preStops.length();
	result.setLength(len);
	for (int i = 0; i < len; ++i)
	{
		result.set( (MFloatVector(MVector::yAxis * m_preStops[i])) , i);
	}
}

void strokeGeom::getStopZAxes(MFloatVectorArray &result) const {
	unsigned len = m_preStops.length();
	result.setLength(len);
	for (int i = 0; i < len; ++i)
	{
		result.set( (MFloatVector(MVector::zAxis * m_preStops[i])) , i);
	}
}




void strokeGeom::getBorders(MFloatPointArray &lefts, MFloatPointArray &rights,
                            const Brush &brush,
                            // double brushWidth,
                            bool withLift, double stackHeight) const {
	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);
	unsigned len = m_targets.length();
	lefts.clear();
	rights.clear();
	double width = brush.width * 0.5;
	int first = 0;
	int end = len;

	if (!withLift) {
		first = 1;
		end = len - 1;
	}


	if (brush.shape == Brush::kFlat) {
		for (int i = first; i < end; ++i)
		{
			MFloatVector projectedX = MFloatVector(((MVector::xAxis * m_targets[i]) ^ m_planeNormal)^
			                                       m_planeNormal).normal();
			const MMatrix &m = m_targets[i];
			MFloatPoint c = extractfPos(m_targets[i]) + stackOffset;
			projectedX *= width;
			lefts.append( c + projectedX);
			rights.append(c - projectedX);
		}
	}
	else {
		for (int i = first; i < end; ++i)
		{

			MFloatVector xAxis = MFloatVector(m_tangents[i] ^ m_planeNormal).normal();
			const MMatrix &m = m_targets[i];
			MFloatPoint c = extractfPos(m_targets[i]) + stackOffset;

			xAxis *= width;
			lefts.append( c + xAxis);
			rights.append(c - xAxis);
		}

	}
}



void strokeGeom::getApproaches(MFloatPointArray &startApproachPoints,
                               MFloatPointArray &endApproachPoints, double stackHeight) const {

	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);

	startApproachPoints.setLength(3);
	endApproachPoints.setLength(3);

	startApproachPoints.set(extractfPos(m_targets[1]) + stackOffset, 0);
	startApproachPoints.set(extractfPos(m_targets[0]) + stackOffset, 1);
	startApproachPoints.set(extractfPos(m_startApproach) + stackOffset, 2);

	unsigned len = m_targets.length();
	endApproachPoints.set(extractfPos(m_targets[(len - 2)]) + stackOffset, 0);
	endApproachPoints.set(extractfPos(m_targets[(len - 1)]) + stackOffset, 1);
	endApproachPoints.set(extractfPos(m_endApproach) + stackOffset, 2);

}

void strokeGeom::getFullPath(MFloatPointArray &points, double stackHeight) const {
	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);


	unsigned len = m_targets.length();

	points.append(extractfPos(m_startApproach) + stackOffset);
	for (int i = 0; i < len; ++i)
	{
		points.append(extractfPos(m_targets[i]) + stackOffset);
	}
	points.append(extractfPos(m_endApproach) + stackOffset);
}


void strokeGeom::getAllPositions(const MMatrix &worldMatrix, MPointArray &result) const
{
	result.append(extractPos(m_startApproach * worldMatrix));
	unsigned len = m_targets.length();
	for (int i = 0; i < len; ++i)
	{
		result.append(extractPos(m_targets[i]*worldMatrix));
	}
	result.append(extractPos(m_endApproach * worldMatrix));
}

void strokeGeom::getAllRotations(
  const MMatrix &worldMatrix,
  MTransformationMatrix::RotationOrder order,
  MAngle::Unit unit,
  MVectorArray &result ) const
{

	unsigned len = m_targets.length();

	result.append(extractRotation(m_startApproach * worldMatrix, order, unit));
	for (int i = 0; i < len; ++i)
	{
		result.append(extractRotation(m_targets[i]*worldMatrix, order, unit));
	}
	result.append(extractRotation(m_endApproach * worldMatrix, order, unit));
}



void strokeGeom::getAllTangents(const MMatrix &worldMatrix, MVectorArray &result) const
{
	unsigned len = m_tangents.length();
	result.append(m_tangents[0]*worldMatrix);
	for (int i = 0; i < len; ++i)
	{
		result.append(m_tangents[i]*worldMatrix);
	}
	result.append(m_tangents[(len - 1)]*worldMatrix);
}


void strokeGeom::setCustomSortData(const Brush &brush,  const Paint &paint)
{
	m_customBrushId = brush.customId;
	m_customPaintId =	paint.customId;
}


void strokeGeom::setUV(
  const MMatrix &inversePlaneMatrix)
{
	MPoint p = ((m_pivot * inversePlaneMatrix) * 0.5) + MVector(0.5, 0.5, 0.0);
	m_u = p.x;
	m_v = p.y;
}
void strokeGeom::getUV( float &u, float &v) {
	u = m_u;
	v = m_v;
}





void strokeGeom::displaceMatrix( MFnMesh &meshFn, MMeshIsectAccelParams &ap,
                                 MMatrix &mat)
{
	MFloatPoint hitPoint;
	MFloatPoint rayOrigin =  extractfPos(mat);
	rayOrigin.z = 0.0f;
	float dist = 0;
	bool hit = meshFn.closestIntersection( rayOrigin, MFloatVector::zAxis, 0, 0, false,
	                                       MSpace::kWorld,
	                                       100.0f, true, &ap, hitPoint, &dist, 0, 0, 0, 0);
	if (hit) {
		mat[3][2] += dist;
	}
}


void strokeGeom::displace( MFnMesh &meshFn, MMeshIsectAccelParams &ap)
{
	// const MFloatVector rayDirection(MFloatVector::zAxis);
	unsigned len = m_targets.length();
	MFloatPoint hitPoint;

	for (int i = 0; i < len; ++i)
	{
		displaceMatrix(meshFn, ap, m_targets[i]);
	}

	len = m_preStops.length();
	for (int i = 0; i < len; ++i)
	{
		displaceMatrix(meshFn, ap, m_preStops[i]);
	}

	displaceMatrix(meshFn, ap, m_startApproach);
	displaceMatrix(meshFn, ap, m_endApproach);

}


void strokeGeom::getStopPositions(const MMatrix &worldMatrix, MPointArray &result) const
{
	unsigned len = m_preStops.length();
	for (int i = 0; i < len; ++i)
	{
		result.append(extractPos(m_preStops[i]*worldMatrix));
	}
}

void strokeGeom::getStopRotations(
  const MMatrix &worldMatrix,
  MTransformationMatrix::RotationOrder order,
  MAngle::Unit unit,
  MVectorArray &result ) const
{

	unsigned len = m_preStops.length();

	for (int i = 0; i < len; ++i)
	{
		result.append(extractRotation(m_preStops[i]*worldMatrix, order, unit));
	}
}

void strokeGeom::addPreStop(const MMatrix &mat)
{
	m_preStops.append(mat);
}




// /* override */
// strokeGeom &strokeGeom::operator=( const strokeGeom &other )
// {
// 	if ( &other != this ) {
// 		m_id = other.m_id;
// 		m_parentId = other.m_parentId;
// 		m_startApproach = other.m_startApproach;
// 		m_endApproach = other.m_endApproach;
// 		m_targets = MMatrixArray(other.m_targets);
// 		m_tangents = MVectorArray(other.m_tangents);
// 		m_arcLength = other.m_arcLength;
// 		m_direction = other.m_direction;;
// 		m_planeNormal = other.m_planeNormal;
// 		m_brushId = other.m_brushId;
// 		m_paintId = other.m_paintId;
// 		m_pivot = other.m_pivot;
// 		m_forceDip = other.m_forceDip;
// 		m_sortColor = other.m_sortColor;;
// 		m_preStops = MMatrixArray(other.m_preStops);
// 		m_sortStack = MIntArray(other.m_sortStack);
// 		m_repeatId = other.m_repeatId;

// 	}
// 	return *this;
// }

ostream &operator<<(ostream &os, const strokeGeom &g)
{
	MString direction = (g.m_direction == 1) ? "forward" : "back"  ;
	os << " Id:" <<  g.m_id << ",";
	os << " Direction:" <<  direction << ",";
	os << " Targets:" << g.m_targets.length() << ",";
	os << " Arc length:" <<  g.m_arcLength;

	return os;
}



