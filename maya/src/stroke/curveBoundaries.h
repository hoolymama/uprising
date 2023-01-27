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



    void boundaries(
        float strokeLength,
        float minimumAdvance,
        float overlap,
        std::vector<Boundary> &boundaries);

    void boundaries(
        float strokeLength,
        float minimumAdvance,
        float overlap,
        float splitAngle,
        float splitTestInterval,
        const MFloatVector &canvasNormal,
        std::vector<Boundary> &boundaries);
        
    MStatus getNextBoundary(
        float strokeLength,
        float minimumAdvance,
        float overlap,
        Boundary &resultBoundary);

    MStatus splitBoundary(
        float splitAngle,
        float splitTestInterval,
        const MFloatVector &canvasNormal,
        Boundary &boundary);
private:
    MObject m_dCurve;
    double m_curveLength;
    Boundary m_lastBoundary;
    bool m_hasStarted;
};
#endif
