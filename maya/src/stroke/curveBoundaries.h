#ifndef _CurveBoundaries_H
#define _CurveBoundaries_H

#include <iostream>
#include <maya/MObject.h>

/**
 * A struct to hold the boundary values for a stroke, along with the maximum coil value when
 * relevant.
 */
struct Boundary
{
    float start;
    float end;
    float maxCoil;

    Boundary(float oStart = 0, float oEnd = 0, float oMaxCoil = 0)
        : start(oStart), end(oEnd), maxCoil(oMaxCoil) {}

    float length() const
    {
        return end - start;
    }

    friend std::ostream &operator<<(std::ostream &os, const Boundary &b)
    {
        os << "{ start:" << b.start << ", end:" << b.end << ", maxCoil" << b.maxCoil << " }";
        return os;
    }
};

class CurveBoundaries
{
public:
    CurveBoundaries();

    CurveBoundaries(const MObject &dCurve);

    ~CurveBoundaries();

    MStatus getNextBoundary(
        double strokeLength,
        double minimumAdvance,
        double overlap,
        Boundary &resultBoundary);

    void boundaries(
        double strokeLength,
        double minimumAdvance,
        double overlap,
        float extendEntry,
        float extendExit,
        std::vector<Boundary> &boundaries);

private:
    MObject m_dCurve;
    double m_length;
    Boundary m_lastBoundary;
    bool m_started;
};
#endif
