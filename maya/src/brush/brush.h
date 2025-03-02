/**
 * @file brush.h
 * @brief Defines the Brush class representing a physical paint brush.
 *
 * This file implements the core Brush class that represents a physical paint brush:
 * - Defines brush properties like shape, width, and retention
 * - Manages brush position and orientation via transformation matrix
 * - Supports different brush shapes (flat, round)
 * - Provides parameters for realistic brush behavior
 */

#ifndef _brush_
#define _brush_

#include <maya/MFloatMatrix.h>
#include <maya/MString.h>

#include "mayaMath.h"

/**
 * @class Brush
 * @brief Represents a physical paint brush with properties and behaviors.
 *
 * The Brush class models a physical paint brush with:
 * - Geometric properties (width, shape, bristle height)
 * - Physical properties (retention, contact power)
 * - Spatial properties (position, orientation)
 * - Behavioral properties (forward/gravity bias)
 */
class Brush
{
public:
	/**
	 * @brief Enumeration of brush shapes.
	 */
	enum Shape
	{
		kFlat,   ///< Flat brush (like a flat watercolor brush)
		kRound,  ///< Round brush (like a round watercolor brush)
		kNone    ///< No specific shape
	};

	/**
	 * @brief Default constructor.
	 */
	Brush();

	/**
	 * @brief Parameterized constructor.
	 * @param matrix Transformation matrix for brush position/orientation
	 * @param inService Whether the brush is currently in service
	 * @param physicalId Unique identifier for the physical brush
	 * @param model Brush model name
	 * @param tip Brush tip position
	 * @param bristleHeight Height of the brush bristles
	 * @param tcpParam Tool center point parameter
	 * @param width Width of the brush
	 * @param shape Shape of the brush (flat, round)
	 * @param retention Paint retention capability
	 * @param transHeightParam Transition height parameter
	 * @param contactPower Power of contact with surface
	 * @param forwardBias0 Forward bias at weight 0
	 * @param forwardBias1 Forward bias at weight 1
	 * @param gravityBias0 Gravity bias at weight 0
	 * @param gravityBias1 Gravity bias at weight 1
	 */
	Brush(
		const MFloatMatrix &matrix,
		bool inService,
		int physicalId,
		const MString &model,
		const MFloatVector &tip,
		float bristleHeight,
		float tcpParam,
		float width,
		Shape shape,
		float retention,
		float transHeightParam,
		float contactPower,
		float forwardBias0,
		float forwardBias1,
		float gravityBias0,
		float gravityBias1);

	/**
	 * @brief Destructor.
	 */
	~Brush();

	/**
	 * @brief Gets the tool center point matrix.
	 * @return Tool center point matrix
	 */
	MFloatMatrix tcp() const;

	/**
	 * @brief Gets triangles representing the brush geometry.
	 * @param triangles Output array of triangle vertices
	 */
	void getTriangles(MFloatPointArray &triangles) const;

	/**
	 * @brief Gets the brush's paint retention capability.
	 * @return Paint retention value
	 */
	const float &retention() const;
	
	/**
	 * @brief Gets the brush's transformation matrix.
	 * @return Transformation matrix
	 */
	const MFloatMatrix &matrix() const;
	
	/**
	 * @brief Checks if the brush is currently in service.
	 * @return True if in service, false otherwise
	 */
	bool inService() const;

	/**
	 * @brief Gets the transition height for the brush.
	 * 
	 * Returns a value representing how far away the brush should start from the ground.
	 * It is the product of the bristle height, the TCP param, and a multiplier (transHeightParam)
	 * 
	 * @return Transition height value
	 */
	float transitionHeight() const;
	
	/**
	 * @brief Gets the contact power of the brush.
	 * @return Contact power value
	 */
	float contactPower() const;

	/**
	 * @brief Gets the forward bias at weight 0.
	 * @return Forward bias value at weight 0
	 */
	float forwardBias0() const;
	
	/**
	 * @brief Gets the forward bias at weight 1.
	 * @return Forward bias value at weight 1
	 */
	float forwardBias1() const;
	
	/**
	 * @brief Gets the gravity bias at weight 0.
	 * @return Gravity bias value at weight 0
	 */
	float gravityBias0() const;
	
	/**
	 * @brief Gets the gravity bias at weight 1.
	 * @return Gravity bias value at weight 1
	 */
	float gravityBias1() const;

	/**
	 * @brief Gets the physical ID of the brush.
	 * @return Physical ID
	 */
	int physicalId() const;

	/**
	 * @brief Gets the custom ID of the brush.
	 * @return Custom ID
	 */
	int customId() const;

	/**
	 * @brief Gets the shape of the brush.
	 * @return Brush shape
	 */
	Brush::Shape shape() const;

	/**
	 * @brief Gets the width of the brush.
	 * @return Brush width
	 */
	const float &width() const;

	/**
	 * @brief Checks if the brush is flat.
	 * @return True if flat, false otherwise
	 */
	bool isFlat() const;

	/**
	 * @brief Gets the model name of the brush.
	 * @return Brush model name
	 */
	const MString & model() const;

	/**
	 * @brief Stream output operator for Brush.
	 * @param os Output stream
	 * @param b Brush to output
	 * @return Reference to output stream
	 */
	friend ostream &operator<<(ostream &os, const Brush &b);

private:
	MFloatMatrix m_matrix;       ///< Transformation matrix
	bool m_inService;            ///< Whether brush is in service
	int m_physicalId;            ///< Physical ID
	MFloatVector m_tip;          ///< Tip position
	float m_width;               ///< Brush width
	float m_bristleHeight;       ///< Bristle height
	float m_retention;           ///< Paint retention capability
	float m_tcpParam;            ///< Tool center point parameter
	Shape m_shape;               ///< Brush shape
	float m_transHeightParam;    ///< Transition height parameter
	float m_contactPower;        ///< Contact power
	float m_forwardBias0;        ///< Forward bias at weight 0
	float m_forwardBias1;        ///< Forward bias at weight 1
	float m_gravityBias0;        ///< Gravity bias at weight 0
	float m_gravityBias1;        ///< Gravity bias at weight 1
	MString m_model;             ///< Brush model name
};

#endif
