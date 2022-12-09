
#ifndef _JVEC2D
#define _JVEC2D

#include <math.h>


class JVector2D
{
public:

    float   x;
    float   y;

    JVector2D():x(),y() {}

    JVector2D(float r, float s)
    {
        x = r;
        y = s;
    }

    JVector2D &set(float r, float s)
    {
        x = r;
        y = s;
        return (*this);
    }

    float &operator [](int k)
    {
        return ((&x)[k]);
    }

    const float &operator [](int k) const
    {
        return ((&x)[k]);
    }

    JVector2D &operator +=(const JVector2D &v)
    {
        x += v.x;
        y += v.y;
        return (*this);
    }

    JVector2D &operator -=(const JVector2D &v)
    {
        x -= v.x;
        y -= v.y;
        return (*this);
    }

    JVector2D &operator *=(float t)
    {
        x *= t;
        y *= t;
        return (*this);
    }

    JVector2D &operator /=(float t)
    {
        float f = 1.0F / t;
        x *= f;
        y *= f;
        return (*this);
    }

    JVector2D &operator &=(const JVector2D &v)
    {
        x *= v.x;
        y *= v.y;
        return (*this);
    }

    JVector2D operator -(void) const
    {
        return (JVector2D(-x, -y));
    }

    JVector2D operator +(const JVector2D &v) const
    {
        return (JVector2D(x + v.x, y + v.y));
    }

    JVector2D operator -(const JVector2D &v) const
    {
        return (JVector2D(x - v.x, y - v.y));
    }

    JVector2D operator *(float t) const
    {
        return (JVector2D(x * t, y * t));
    }

    JVector2D operator /(float t) const
    {
        float f = 1.0F / t;
        return (JVector2D(x * f, y * f));
    }

    // dot
    float operator *(const JVector2D &v) const
    {
        return (x * v.x + y * v.y);
    }

    JVector2D operator &(const JVector2D &v) const
    {
        return (JVector2D(x * v.x, y * v.y));
    }

    bool operator ==(const JVector2D &v) const
    {
        return ((x == v.x) && (y == v.y));
    }

    bool operator !=(const JVector2D &v) const
    {
        return ((x != v.x) || (y != v.y));
    }

    JVector2D &normalize(void)
    {
        return (*this /= sqrtf(x * x + y * y));
    }

    float length(void)
    {
        return (sqrtf(x * x + y * y));
    }

    float sqlength(void)
    {
        return (x * x + y * y);
    }
};


class JPoint2D : public JVector2D
{
public:

    JPoint2D() {}

    JPoint2D(float r, float s) : JVector2D(r, s) {}

    JPoint2D &operator =(const JVector2D &v)
    {
        x = v.x;
        y = v.y;
        return (*this);
    }

    JPoint2D &operator *=(float t)
    {
        x *= t;
        y *= t;
        return (*this);
    }

    JPoint2D &operator /=(float t)
    {
        float f = 1.0F / t;
        x *= f;
        y *= f;
        return (*this);
    }

    JPoint2D operator -(void) const
    {
        return (JPoint2D(-x, -y));
    }

    JPoint2D operator +(const JVector2D &v) const
    {
        return (JPoint2D(x + v.x, y + v.y));
    }

    JPoint2D operator -(const JVector2D &v) const
    {
        return (JPoint2D(x - v.x, y - v.y));
    }

    JVector2D operator -(const JPoint2D &p) const
    {
        return (JVector2D(x - p.x, y - p.y));
    }

    JPoint2D operator *(float t) const
    {
        return (JPoint2D(x * t, y * t));
    }

    JPoint2D operator /(float t) const
    {
        float f = 1.0F / t;
        return (JPoint2D(x * f, y * f));
    }
};
#endif
