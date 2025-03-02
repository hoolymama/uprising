/**
 * @file strokeData.h
 * @brief Defines the strokeData class for storing stroke geometry in Maya.
 * 
 * This file contains the strokeData class which extends Maya's MPxData to provide
 * a custom data type for storing stroke geometry. It allows stroke data to be
 * passed between nodes in Maya's dependency graph.
 */

#ifndef _strokeData
#define _strokeData
#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "stroke.h"

/**
 * @class strokeData
 * @brief A custom Maya data type for storing stroke geometry.
 * 
 * The strokeData class extends MPxData to create a custom Maya data type
 * for storing stroke geometry. It encapsulates a vector of Stroke objects,
 * allowing stroke data to flow through Maya's dependency graph.
 */
class strokeData : public MPxData
{
public:
    /**
     * @brief Default constructor.
     */
    strokeData();
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~strokeData();
    
    /**
     * @brief Copies data from another MPxData object.
     * @param other The source data object to copy from.
     */
    virtual void            copy ( const MPxData &other );
    
    /**
     * @brief Returns the type ID of this data type.
     * @return MTypeId representing the data type.
     */
    virtual MTypeId         typeId() const;
    
    /**
     * @brief Returns the name of this data type.
     * @return MString containing the data type name.
     */
    virtual MString         name() const;

    /**
     * @brief Creator function for Maya's plugin system.
     * @return Pointer to a new strokeData object.
     */
    static void *creator();
public:
    /** @brief Type name for this data type. */
    static const MString typeName;
    
    /** @brief Type ID for this data type. */
    static const MTypeId id;

    /** @brief Pointer to a vector of Stroke objects. */
    std::vector<Stroke> *fGeometry;
};
#endif



