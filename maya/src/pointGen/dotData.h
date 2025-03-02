/**
 * @file dotData.h
 * @brief Defines the dotData class for representing 2D points with radius and other properties.
 * 
 * This file contains the dotData class which represents a 2D point with associated properties
 * such as radius, gradient, and density. It provides methods for manipulating and querying
 * these points, which are used in various point generation and manipulation algorithms.
 */

#ifndef _dotData
#define _dotData

#include <vector>
#include <map>

#include <maya/MFloatVector.h>
#include <maya/MFloatMatrix.h>

// #include "errorMacros.h"
#include "mayaMath.h"
#include "vec2d.h"

typedef mayaMath::axis axis;

/**
 * @class dotData
 * @brief Represents a 2D point with radius and other properties.
 * 
 * The dotData class encapsulates a 2D point with associated properties such as
 * radius, gradient, density, and unique identifier. It provides methods for
 * manipulating and querying these points, including position access, transformation,
 * containment testing, and force application.
 */
class dotData
{
public:
	/**
     * @brief Constructor with position, density, radius, and ID.
     * @param u U coordinate of the point.
     * @param v V coordinate of the point.
     * @param density Density value of the point.
     * @param radius Radius of the point.
     * @param id Unique identifier for the point.
     */
	dotData(float u, float v, float density,
			float radius, int id);
    
    /**
     * @brief Copy constructor.
     * @param rhs The dotData object to copy.
     */
	dotData(const dotData &rhs);

	/**
     * @brief Sets the UV coordinates based on normalized points (0->1).
     */
	void setUV();
    
    /**
     * @brief Sets the gradient vector for this point.
     * @param x X component of the gradient.
     * @param y Y component of the gradient.
     */
	void setGradient(float x, float y);

    /**
     * @brief Destructor.
     */
	~dotData();

    /**
     * @brief Sets the unique identifier for this point.
     * @param id The new identifier value.
     */
	void setId(int id);

    /**
     * @brief Gets the 2D position of this point.
     * @return Constant reference to the 2D position.
     */
	const JPoint2D &position() const;
    
    /**
     * @brief Gets a specific coordinate of this point.
     * @param a The axis (X or Y) to get the coordinate for.
     * @return Constant reference to the coordinate value.
     */
	const float &position(axis a) const;
    
    /**
     * @brief Gets the radius of this point.
     * @return Constant reference to the radius value.
     */
	const float &radius() const;
    
    /**
     * @brief Gets the minimum bound along a specific axis.
     * @param a The axis (X or Y) to get the minimum bound for.
     * @return Constant reference to the minimum bound value.
     */
	const float &min(axis a) const;
    
    /**
     * @brief Gets the maximum bound along a specific axis.
     * @param a The axis (X or Y) to get the maximum bound for.
     * @return Constant reference to the maximum bound value.
     */
	const float &max(axis a) const;

    /**
     * @brief Gets the U coordinate of this point.
     * @return Constant reference to the U coordinate.
     */
	const float &u() const;
    
    /**
     * @brief Gets the V coordinate of this point.
     * @return Constant reference to the V coordinate.
     */
	const float &v() const;

    /**
     * @brief Converts this point to a Maya vector.
     * @return MVector representation of this point.
     */
	MVector asVector() const;
    
    /**
     * @brief Transforms this point using a matrix.
     * @param mat The transformation matrix to apply.
     * @return MVector containing the transformed point.
     */
	MVector transformed(const MMatrix &mat) const;
    
    /**
     * @brief Gets the gradient vector of this point.
     * @return MVector containing the gradient.
     */
	MVector gradient() const;
    
    /**
     * @brief Gets the gradient vector transformed by a matrix.
     * @param mat The transformation matrix to apply.
     * @return MVector containing the transformed gradient.
     */
	MVector gradientTransformed(const MMatrix &mat) const;

    /**
     * @brief Tests if this point contains another point.
     * @param other Pointer to the other dotData object to test.
     * @return True if this point contains the other point.
     */
	bool contains(const dotData *other) const;

    /**
     * @brief Applies a force to this point.
     * @param force The 2D force vector to apply.
     */
	void applyForce(const JVector2D &force);
	
    /**
     * @brief Sets a parameter used for sorting points.
     * @param worldMatrix The world transformation matrix.
     * @param direction The direction vector for sorting.
     */
	void setSortParam(const MMatrix &worldMatrix, const MVector&direction);
    
    /**
     * @brief Gets the sort parameter value.
     * @return Constant reference to the sort parameter.
     */
	const float & sortParam() const; 
    
    /**
     * @brief Gets the unique identifier of this point.
     * @return Constant reference to the identifier value.
     */
	const int &id() const;
    
    /**
     * @brief Less-than operator for comparing dotData objects.
     * @param a First dotData object.
     * @param b Second dotData object.
     * @return True if a should be ordered before b.
     */
	friend bool operator<(const dotData &a, const dotData &b);

private:
	JPoint2D m_p;         /**< 2D position of the point. */
	JVector2D m_gradient; /**< Gradient vector of the point. */
	JPoint2D m_min;       /**< Minimum bounds of the point. */
	JPoint2D m_max;       /**< Maximum bounds of the point. */
	float m_radius;       /**< Radius of the point. */
	float m_density;      /**< Density value of the point. */
	float m_u;            /**< U coordinate of the point. */
	float m_v;            /**< V coordinate of the point. */
	int m_id;             /**< Unique identifier of the point. */
	float m_sortParam;    /**< Parameter used for sorting points. */

};

/** @brief Type definition for a list of dotData pointers. */
typedef std::vector<dotData *> PT_LIST;

#endif
