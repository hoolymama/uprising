/**
 * @file curveBoundaries.h
 * @brief Defines the CurveBoundaries class for managing stroke boundaries along curves.
 * 
 * This file contains the CurveBoundaries class and Boundary struct which provide
 * functionality for managing stroke boundaries along curves. It includes methods for
 * calculating boundaries based on stroke length, minimum advance, overlap, and split
 * angles, which are useful for creating strokes that follow curves with appropriate
 * spacing and segmentation.
 */

#ifndef _CurveBoundaries_H
#define _CurveBoundaries_H

#include <iostream>
#include <maya/MObject.h>

/**
 * @struct Boundary
 * @brief A structure to hold boundary values for a stroke.
 * 
 * The Boundary struct encapsulates the start and end positions along a curve
 * for a stroke, along with the maximum coil value when relevant. It provides
 * methods for calculating the length of the boundary and for output streaming.
 */
struct Boundary
{
    float start;    /**< Start position along the curve. */
    float end;      /**< End position along the curve. */
    float maxCoil;  /**< Maximum coil value for the stroke. */

    /**
     * @brief Constructor with default values.
     * @param oStart Start position (default: 0).
     * @param oEnd End position (default: 0).
     * @param oMaxCoil Maximum coil value (default: 0).
     */
    Boundary(float oStart = 0, float oEnd = 0, float oMaxCoil = 0)
        : start(oStart), end(oEnd), maxCoil(oMaxCoil) {}

    /**
     * @brief Calculates the length of the boundary.
     * @return The length (end - start).
     */
    float length() const
    {
        return end - start;
    }

    /**
     * @brief Output stream operator for Boundary objects.
     * @param os The output stream.
     * @param b The Boundary object to output.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Boundary &b)
    {
        os << "{ start:" << b.start << ", end:" << b.end << ", maxCoil" << b.maxCoil << " }";
        return os;
    }
};

/**
 * @class CurveBoundaries
 * @brief A class for managing stroke boundaries along curves.
 * 
 * The CurveBoundaries class provides functionality for calculating and managing
 * stroke boundaries along curves. It can determine appropriate boundaries based on
 * stroke length, minimum advance, overlap, and split angles, which are useful for
 * creating strokes that follow curves with appropriate spacing and segmentation.
 */
class CurveBoundaries
{
public:
    /**
     * @brief Default constructor.
     */
    CurveBoundaries();

    /**
     * @brief Constructor with a curve object.
     * @param dCurve The curve object to calculate boundaries for.
     */
    CurveBoundaries(const MObject &dCurve);

    /**
     * @brief Destructor.
     */
    ~CurveBoundaries();

    /**
     * @brief Calculates boundaries based on stroke length, minimum advance, and overlap.
     * @param strokeLength The length of the stroke.
     * @param minimumAdvance The minimum distance to advance along the curve.
     * @param overlap The amount of overlap between adjacent strokes.
     * @param boundaries Output vector to store the calculated boundaries.
     */
    void boundaries(
        float strokeLength,
        float minimumAdvance,
        float overlap,
        std::vector<Boundary> &boundaries);

    /**
     * @brief Calculates boundaries with split angle consideration.
     * @param strokeLength The length of the stroke.
     * @param minimumAdvance The minimum distance to advance along the curve.
     * @param overlap The amount of overlap between adjacent strokes.
     * @param splitAngle The angle at which to split the stroke.
     * @param splitTestInterval The interval for testing split angles.
     * @param canvasNormal The normal vector of the canvas.
     * @param boundaries Output vector to store the calculated boundaries.
     */
    void boundaries(
        float strokeLength,
        float minimumAdvance,
        float overlap,
        float splitAngle,
        float splitTestInterval,
        const MFloatVector &canvasNormal,
        std::vector<Boundary> &boundaries);
        
    /**
     * @brief Gets the next boundary along the curve.
     * @param strokeLength The length of the stroke.
     * @param minimumAdvance The minimum distance to advance along the curve.
     * @param overlap The amount of overlap between adjacent strokes.
     * @param resultBoundary Output parameter to store the next boundary.
     * @return MStatus indicating success or failure.
     */
    MStatus getNextBoundary(
        float strokeLength,
        float minimumAdvance,
        float overlap,
        Boundary &resultBoundary);

    /**
     * @brief Splits a boundary based on angle criteria.
     * @param splitAngle The angle at which to split the boundary.
     * @param splitTestInterval The interval for testing split angles.
     * @param canvasNormal The normal vector of the canvas.
     * @param boundary The boundary to split (modified in place).
     * @return MStatus indicating success or failure.
     */
    MStatus splitBoundary(
        float splitAngle,
        float splitTestInterval,
        const MFloatVector &canvasNormal,
        Boundary &boundary);
private:
    MObject m_dCurve;       /**< The curve object. */
    double m_curveLength;   /**< The length of the curve. */
    Boundary m_lastBoundary; /**< The last boundary calculated. */
    bool m_hasStarted;      /**< Flag indicating if boundary calculation has started. */
};
#endif
