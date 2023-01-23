#include <vector>
#include <maya/MFnNurbsCurve.h>

#include "curveBoundaries.h"

CurveBoundaries::CurveBoundaries(const MObject &dCurve)
    : m_dCurve(dCurve),
      m_lastBoundary(),
      m_started(false)
{
    MFnNurbsCurve curveFn(dCurve);
    m_length = curveFn.length();
}

CurveBoundaries::~CurveBoundaries()
{
}

MStatus CurveBoundaries::getNextBoundary(
    double strokeLength,
    double minimumAdvance,
    double overlap,
    Boundary &resultBoundary)
{
    MFnNurbsCurve curveFn(m_dCurve);
    resultBoundary.start = m_lastBoundary.end - overlap;

    if (m_started && ((resultBoundary.start - m_lastBoundary.start) < minimumAdvance))
    {
        // We've already started and the minimum advance hasn't been met, so force it.
        resultBoundary.start = float(m_lastBoundary.start + minimumAdvance);
    }

    if (resultBoundary.start >= m_length)
    {
        // Past the end of the curve so don't make any more boundaries
        return MStatus::kFailure;
    }

    resultBoundary.end = float(std::min((resultBoundary.start + strokeLength), m_length));

    if (resultBoundary.length() < 0.0001)
    {
        // The boundary is too small, don't make any more boundaries.
        return MStatus::kFailure;
    }

    m_lastBoundary = resultBoundary;

    m_started = true;
    return MStatus::kSuccess;
}

void CurveBoundaries::boundaries(
    double strokeLength,
    double minimumAdvance,
    double overlap,
    float extendEntry,
    float extendExit,
    std::vector<Boundary> &boundaries
)
{
    overlap = overlap + extendEntry + extendExit;
    while (true)
    {
        Boundary boundary;
        MStatus status = getNextBoundary(strokeLength, minimumAdvance, overlap, boundary);
        if (status != MStatus::kSuccess)
        {
            break;
        }
        boundaries.push_back(boundary);
    }
}
