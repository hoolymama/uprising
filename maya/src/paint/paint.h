/**
 * @file paint.h
 * @brief Defines the Paint class for representing paint properties.
 * 
 * This file contains the Paint class which represents paint properties in the
 * painting system, including color, opacity, pot ID, travel distance, and name.
 */

#ifndef _paint_
#define _paint_

#include <maya/MFloatVector.h>
#include <maya/MColor.h>
#include <maya/MString.h>

/**
 * @class Paint
 * @brief Represents paint properties in the painting system.
 * 
 * The Paint class encapsulates properties of paint in the painting system, including:
 * - Color and opacity
 * - Pot ID for identifying the paint container
 * - Travel distance for determining how far the paint can be applied
 * - Name for identifying the paint
 */
class Paint
{
public:
	/**
	 * @brief Default constructor.
	 * Creates a paint object with default properties.
	 */
	Paint();
 
	/**
	 * @brief Constructor with color, pot ID, travel, and name.
	 * @param color The color of the paint.
	 * @param pot The pot ID of the paint.
	 * @param travel The travel distance of the paint.
	 * @param name The name of the paint.
	 */
	Paint( const MColor &color, int pot, float travel, const MString &name);
	
	/**
	 * @brief Constructor with color vector, opacity, pot ID, travel, and name.
	 * @param color The color vector of the paint.
	 * @param opacity The opacity of the paint.
	 * @param pot The pot ID of the paint.
	 * @param travel The travel distance of the paint.
	 * @param name The name of the paint.
	 */
	Paint( const MFloatVector &color, float opacity, int pot, float travel, const MString &name );

	/**
	 * @brief Destructor.
	 */
	~Paint();

	/**
	 * @brief Gets the color of the paint.
	 * @return The color of the paint.
	 */
	const MColor& color() const;
	
	/**
	 * @brief Gets the pot ID of the paint.
	 * @return The pot ID of the paint.
	 */
	int pot() const;
	
	/**
	 * @brief Gets the travel distance of the paint.
	 * @return The travel distance of the paint.
	 */
	float travel() const;
	
	/**
	 * @brief Gets the name of the paint.
	 * @return The name of the paint.
	 */
	const MString& name() const;

	/**
	 * @brief Stream output operator for Paint objects.
	 * @param os The output stream.
	 * @param b The Paint object to output.
	 * @return The output stream.
	 */
	friend ostream &operator<<(ostream &os, const Paint &b);

private:
	/** @brief The color of the paint. */
	MColor m_color;
	
	/** @brief The travel distance of the paint. */
	float m_travel;
	
	/** @brief The name of the paint. */
	MString m_name;
	
	/** @brief The pot ID of the paint. */
	int m_pot;

	// compat
};

#endif
