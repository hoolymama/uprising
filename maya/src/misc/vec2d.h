#ifndef _JVEC2D
#define _JVEC2D

#include <math.h>

/**
 * @file vec2d.h
 * @brief Defines 2D vector and point classes for mathematical operations.
 * 
 * This file contains the JVector2D and JPoint2D classes which provide functionality
 * for 2D vector and point operations. These classes support various mathematical
 * operations such as addition, subtraction, multiplication, normalization, and
 * geometric transformations.
 */

/**
 * @class JVector2D
 * @brief A 2D vector class with mathematical operations.
 * 
 * The JVector2D class represents a 2D vector with x and y components and provides
 * a comprehensive set of mathematical operations including:
 * - Vector addition and subtraction
 * - Scalar multiplication and division
 * - Dot product
 * - Normalization
 * - Rotation
 * - Length calculations
 */
class JVector2D
{
public:

    /** @brief X component of the vector. */
    float   x;
    
    /** @brief Y component of the vector. */
    float   y;

    /**
     * @brief Default constructor.
     * Initializes x and y to zero.
     */
    JVector2D():x(),y() {}

    /**
     * @brief Constructor with initial values.
     * @param r Initial value for x component.
     * @param s Initial value for y component.
     */
    JVector2D(float r, float s)
    {
        x = r;
        y = s;
    }

    /**
     * @brief Sets the vector components.
     * @param r Value for x component.
     * @param s Value for y component.
     * @return Reference to this vector.
     */
    JVector2D &set(float r, float s)
    {
        x = r;
        y = s;
        return (*this);
    }

    /**
     * @brief Array subscript operator.
     * @param k Index (0 for x, 1 for y).
     * @return Reference to the component.
     */
    float &operator [](int k)
    {
        return ((&x)[k]);
    }

    /**
     * @brief Const array subscript operator.
     * @param k Index (0 for x, 1 for y).
     * @return Const reference to the component.
     */
    const float &operator [](int k) const
    {
        return ((&x)[k]);
    }

    /**
     * @brief Addition assignment operator.
     * @param v Vector to add.
     * @return Reference to this vector.
     */
    JVector2D &operator +=(const JVector2D &v)
    {
        x += v.x;
        y += v.y;
        return (*this);
    }

    /**
     * @brief Subtraction assignment operator.
     * @param v Vector to subtract.
     * @return Reference to this vector.
     */
    JVector2D &operator -=(const JVector2D &v)
    {
        x -= v.x;
        y -= v.y;
        return (*this);
    }

    /**
     * @brief Scalar multiplication assignment operator.
     * @param t Scalar value.
     * @return Reference to this vector.
     */
    JVector2D &operator *=(float t)
    {
        x *= t;
        y *= t;
        return (*this);
    }

    /**
     * @brief Scalar division assignment operator.
     * @param t Scalar value.
     * @return Reference to this vector.
     */
    JVector2D &operator /=(float t)
    {
        float f = 1.0F / t;
        x *= f;
        y *= f;
        return (*this);
    }

    /**
     * @brief Component-wise multiplication assignment operator.
     * @param v Vector to multiply by.
     * @return Reference to this vector.
     */
    JVector2D &operator &=(const JVector2D &v)
    {
        x *= v.x;
        y *= v.y;
        return (*this);
    }

    /**
     * @brief Unary negation operator.
     * @return Negated vector.
     */
    JVector2D operator -(void) const
    {
        return (JVector2D(-x, -y));
    }

    /**
     * @brief Addition operator.
     * @param v Vector to add.
     * @return Result of addition.
     */
    JVector2D operator +(const JVector2D &v) const
    {
        return (JVector2D(x + v.x, y + v.y));
    }

    /**
     * @brief Subtraction operator.
     * @param v Vector to subtract.
     * @return Result of subtraction.
     */
    JVector2D operator -(const JVector2D &v) const
    {
        return (JVector2D(x - v.x, y - v.y));
    }

    /**
     * @brief Scalar multiplication operator.
     * @param t Scalar value.
     * @return Result of multiplication.
     */
    JVector2D operator *(float t) const
    {
        return (JVector2D(x * t, y * t));
    }

    /**
     * @brief Scalar division operator.
     * @param t Scalar value.
     * @return Result of division.
     */
    JVector2D operator /(float t) const
    {
        float f = 1.0F / t;
        return (JVector2D(x * f, y * f));
    }

    /**
     * @brief Dot product operator.
     * @param v Vector to dot with.
     * @return Dot product result.
     */
    float operator *(const JVector2D &v) const
    {
        return (x * v.x + y * v.y);
    }

    /**
     * @brief Calculates the projection of a vector onto this vector.
     * @param v Vector to project.
     * @return Projected vector.
     */
    JVector2D projection(const JVector2D &v)
    {
        JVector2D n = this->normal();
        return n * (v * n);
    }

    /**
     * @brief Equality operator.
     * @param v Vector to compare with.
     * @return true if vectors are equal, false otherwise.
     */
    bool operator ==(const JVector2D &v) const
    {
        return ((x == v.x) && (y == v.y));
    }

    /**
     * @brief Inequality operator.
     * @param v Vector to compare with.
     * @return true if vectors are not equal, false otherwise.
     */
    bool operator !=(const JVector2D &v) const
    {
        return ((x != v.x) || (y != v.y));
    }

    /**
     * @brief Returns a normalized copy of this vector.
     * @return Normalized vector.
     */
    JVector2D normal() const
    {
        return ((*this) / sqrtf(x * x + y * y));
    }
    
    /**
     * @brief Normalizes this vector in place.
     */
    void normalize()
    {   
        float len = sqrtf(x * x + y * y);
        if (len > 0.0f)
        {
            float f = 1.0F / len;
            x *= f;
            y *= f;
        }
    }

    /**
     * @brief Rotates this vector 90 degrees clockwise in place.
     */
    void rotateBy90(void)
    {
        float  tmp = y ;
        y = -x;
        x = tmp;
    }

    /**
     * @brief Rotates this vector 90 degrees counter-clockwise in place.
     */
    void rotateByNeg90(void)
    {
        float  tmp = x ;
        x = -y;
        y = tmp;
    }
    
    /**
     * @brief Returns a copy of this vector rotated 90 degrees clockwise.
     * @return Rotated vector.
     */
    JVector2D getRotateBy90(void) const
    {
        return JVector2D(y,-x);
    }

    /**
     * @brief Returns a copy of this vector rotated 90 degrees counter-clockwise.
     * @return Rotated vector.
     */
    JVector2D getRotateByNeg90(void) const
    {
        return JVector2D(-y, x);
    }

    /**
     * @brief Calculates the length of this vector.
     * @return Length of the vector.
     */
    float length(void) const
    {
        return (sqrtf(x * x + y * y));
    }

    /**
     * @brief Calculates the squared length of this vector.
     * @return Squared length of the vector.
     */
    float sqlength(void) const
    {
        return (x * x + y * y);
    }

    /**
     * @brief Checks if this vector is approximately zero.
     * @param epsilon Threshold for comparison (default: 0.00001).
     * @return true if the vector is approximately zero, false otherwise.
     */
    float isZero(const float epsilon=0.00001f) const
    {
        return (fabsf(x) < epsilon && fabsf(y) < epsilon);
    }

};

/**
 * @class JPoint2D
 * @brief A 2D point class derived from JVector2D.
 * 
 * The JPoint2D class extends JVector2D to represent a 2D point. It provides
 * additional operations specific to points, such as:
 * - Point-vector addition and subtraction
 * - Point-point subtraction (resulting in a vector)
 * - Scalar multiplication and division
 */
class JPoint2D : public JVector2D
{
public:
    /**
     * @brief Default constructor.
     */
    JPoint2D() {}

    /**
     * @brief Constructor with initial values.
     * @param r Initial value for x component.
     * @param s Initial value for y component.
     */
    JPoint2D(float r, float s) : JVector2D(r, s) {}

    /**
     * @brief Assignment operator from vector.
     * @param v Vector to assign from.
     * @return Reference to this point.
     */
    JPoint2D &operator =(const JVector2D &v)
    {
        x = v.x;
        y = v.y;
        return (*this);
    }

    /**
     * @brief Scalar multiplication assignment operator.
     * @param t Scalar value.
     * @return Reference to this point.
     */
    JPoint2D &operator *=(float t)
    {
        x *= t;
        y *= t;
        return (*this);
    }

    /**
     * @brief Scalar division assignment operator.
     * @param t Scalar value.
     * @return Reference to this point.
     */
    JPoint2D &operator /=(float t)
    {
        float f = 1.0F / t;
        x *= f;
        y *= f;
        return (*this);
    }

    /**
     * @brief Unary negation operator.
     * @return Negated point.
     */
    JPoint2D operator -(void) const
    {
        return (JPoint2D(-x, -y));
    }

    /**
     * @brief Point-vector addition operator.
     * @param v Vector to add.
     * @return Result of addition.
     */
    JPoint2D operator +(const JVector2D &v) const
    {
        return (JPoint2D(x + v.x, y + v.y));
    }

    /**
     * @brief Point-vector subtraction operator.
     * @param v Vector to subtract.
     * @return Result of subtraction.
     */
    JPoint2D operator -(const JVector2D &v) const
    {
        return (JPoint2D(x - v.x, y - v.y));
    }

    /**
     * @brief Point-point subtraction operator.
     * @param p Point to subtract.
     * @return Vector from p to this point.
     */
    JVector2D operator -(const JPoint2D &p) const
    {
        return (JVector2D(x - p.x, y - p.y));
    }

    /**
     * @brief Scalar multiplication operator.
     * @param t Scalar value.
     * @return Result of multiplication.
     */
    JPoint2D operator *(float t) const
    {
        return (JPoint2D(x * t, y * t));
    }

    /**
     * @brief Scalar division operator.
     * @param t Scalar value.
     * @return Result of division.
     */
    JPoint2D operator /(float t) const
    {
        float f = 1.0F / t;
        return (JPoint2D(x * f, y * f));
    }
};

#endif
