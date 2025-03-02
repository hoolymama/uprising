/**
 * @file paintingData.h
 * @brief Defines the paintingData class for storing painting data in Maya.
 * 
 * This file contains the paintingData class which provides a custom Maya data type
 * for storing painting data. It encapsulates a paintingGeom object that can be
 * passed through Maya's dependency graph.
 */

#ifndef _paintingData
#define _paintingData

#include <map>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "paintingGeom.h"

#include "brush.h"
#include "paint.h"

/**
 * @class paintingData
 * @brief A custom Maya data type for storing painting data.
 * 
 * The paintingData class extends MPxData to create a custom Maya data type for
 * storing painting data. It encapsulates a paintingGeom object, allowing painting
 * geometry data to flow through Maya's dependency graph.
 */
class paintingData : public MPxData
{
public:
	/**
	 * @brief Default constructor.
	 */
	paintingData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~paintingData();

	/**
	 * @brief Clears the painting data.
	 */
	void clear();

	/**
	 * @brief Creates an empty painting geometry.
	 * @return MStatus indicating success or failure.
	 */
	MStatus create();

	/**
	 * @brief Creates painting geometry from data block and maps.
	 * @param data The data block containing the node's data.
	 * @param strokeCurvesAtt The stroke curves attribute.
	 * @param brushes Map of brushes to use.
	 * @param paints Map of paints to use.
	 * @return MStatus indicating success or failure.
	 */
	MStatus create(
	  MDataBlock &data,
	  MObject &strokeCurvesAtt,
	  const std::map<short, Brush> &brushes,
	  const std::map<short, Paint> &paints);

	/**
	 * @brief Copies data from another MPxData object.
	 * @param other The source data object to copy from.
	 */
	virtual void copy(const MPxData &other);
	
	/**
	 * @brief Assignment operator.
	 * @param otherData The source data object to assign from.
	 * @return Reference to this object.
	 */
	paintingData &operator=(const paintingData &otherData);
	
	/**
	 * @brief Gets the painting geometry.
	 * @return Pointer to the painting geometry.
	 */
	paintingGeom *geometry() const;

	/**
	 * @brief Returns the type ID of this data type.
	 * @return MTypeId representing the data type.
	 */
	virtual MTypeId typeId() const;
	
	/**
	 * @brief Returns the name of this data type.
	 * @return MString containing the data type name.
	 */
	virtual MString name() const;

	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new paintingData object.
	 */
	static void *creator();

public:
	/** @brief Type name for this data type. */
	static const MString typeName;
	
	/** @brief Type ID for this data type. */
	static const MTypeId id;

	/** @brief Pointer to the painting geometry. */
	paintingGeom *m_pGeometry;
};
#endif

