#include <vector>
#include <maya/MFnNurbsCurve.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>

#include "curveBoundaries.h"
const float epsilon = 0.0001f;

CurveBoundaries::CurveBoundaries(const MObject &dCurve)
    : m_dCurve(dCurve),
      m_lastBoundary(),
      m_hasStarted(false)
{
    MFnNurbsCurve curveFn(dCurve);
    m_curveLength = curveFn.length();
}

CurveBoundaries::~CurveBoundaries()
{
}

// Get the boundaries of strokes in terms of distances along a curve.
void CurveBoundaries::boundaries(
    float strokeLength,
    float minimumAdvance,
    float overlap,
    std::vector<Boundary> &boundaries)
{
    while (true)
    {
        Boundary boundary;
        MStatus status = getNextBoundary(strokeLength, minimumAdvance, overlap, boundary);
        if (status != MStatus::kSuccess)
        {
            break;
        }
        boundaries.push_back(boundary);
        m_lastBoundary = boundary;
    }
}

// Get the boundaries of strokes in terms of distances along the curve. If the boundary rotation
// exceded a given angle at any point, end it early.
void CurveBoundaries::boundaries(
    float strokeLength,
    float minimumAdvance,
    float overlap,
    float splitAngle,
    float splitTestInterval,
    const MFloatVector &canvasNormal,
    std::vector<Boundary> &boundaries)
{
    bool doSplitTest = (splitAngle > epsilon && splitTestInterval > epsilon);

    while (true)
    {
        Boundary boundary;
        MStatus status = getNextBoundary(strokeLength, minimumAdvance, overlap, boundary);

        if (status != MStatus::kSuccess)
            break;

        // Check if the boundary needs to be split.
        if (doSplitTest)
        {
            status = splitBoundary(splitAngle, splitTestInterval, canvasNormal, boundary);
            if (status != MStatus::kSuccess)
                break;
        }

        boundaries.push_back(boundary);
        m_lastBoundary = boundary;
    }
}

MStatus CurveBoundaries::getNextBoundary(
    float strokeLength,
    float minimumAdvance,
    float overlap,
    Boundary &resultBoundary)
{

    MFnNurbsCurve curveFn(m_dCurve);
    resultBoundary.start = m_lastBoundary.end - overlap;

    if (m_hasStarted)
    {
        if (m_lastBoundary.end + epsilon > m_curveLength)
        {
            return MStatus::kFailure;
        }

        resultBoundary.start = m_lastBoundary.end - overlap;
        if (resultBoundary.start - m_lastBoundary.start < minimumAdvance)
        {
            resultBoundary.start = m_lastBoundary.start + minimumAdvance;
        }
    }
    else
    {
        resultBoundary.start = 0.0;
    }

    if (double(resultBoundary.start) >= m_curveLength)
    {
        // Past the end of the curve so don't make any more boundaries
        return MStatus::kFailure;
    }

    resultBoundary.end = std::min((resultBoundary.start + strokeLength), float(m_curveLength));

    if (resultBoundary.length() < epsilon)
    {
        // The boundary is too small, don't make any more boundaries.
        return MStatus::kFailure;
    }

    m_hasStarted = true;
    return MStatus::kSuccess;
}

MStatus CurveBoundaries::splitBoundary(
    float splitAngle,
    float splitTestInterval,
    const MFloatVector &canvasNormal,
    Boundary &boundary)
{
    MFnNurbsCurve curveFn(m_dCurve);

    float endDist = boundary.end;
    float startDist = boundary.start;
    float leftExtent = 0;
    float rightExtent = 0;
    float accumAngle = 0;
    double param = curveFn.findParamFromLength(startDist);
    MVector lastTangent = curveFn.tangent(param);
    float currDist = startDist;

    double angle;
    MVector axis;
    bool foundEnd = false;
    float outMaxCoil = 0.0f;
    do
    {
        currDist += splitTestInterval;
        if (currDist >= boundary.end)
        {
            // We got to the end without splitting the boundary.
            return MS::kSuccess;
        }
        param = curveFn.findParamFromLength(currDist);
        MVector tangent = curveFn.tangent(param);
        MQuaternion q(lastTangent, tangent);
        lastTangent = tangent;
        bool rotated = q.getAxisAngle(axis, angle);
        if (!rotated)
        {
            continue;
        }

        float direction = (MFloatVector(axis) * canvasNormal < 0) ? -1.0f : 1.0f;
        accumAngle += (direction * angle);
        if (accumAngle < leftExtent)
        {
            leftExtent = accumAngle;
        }
        if (accumAngle > rightExtent)
        {
            rightExtent = accumAngle;
        }
        float coil = rightExtent - leftExtent;
        foundEnd = (coil > splitAngle);
        if (coil > outMaxCoil)
        {
            outMaxCoil = coil;
        }

    } while (!foundEnd);

    boundary.end = currDist;
    boundary.maxCoil = outMaxCoil;

    return MStatus::kSuccess;
}
