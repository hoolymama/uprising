/**
 * @file brushShopNode.h
 * @brief Defines the Maya node for managing brush collections.
 *
 * This file implements a Maya node for brush shop management:
 * - Collects and organizes brushes by shape and width
 * - Provides a central repository for available brushes
 * - Outputs brush shop data for use by other nodes
 * - Integrates with Maya's dependency graph
 */

#ifndef _brushShopNode_H
#define _brushShopNode_H

#include <maya/MPxNode.h>
#include "brush.h"

/**
 * @class brushShopNode
 * @brief Maya node for managing brush collections.
 *
 * brushShopNode provides a central repository for brushes:
 * - Collects and organizes brushes by shape and width
 * - Creates brush racks for different brush shapes
 * - Outputs brush shop data for use by other nodes
 * - Integrates with Maya's dependency graph
 */
class brushShopNode : public MPxNode
{
public:
    /**
     * @brief Default constructor.
     */
    brushShopNode();
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~brushShopNode();

    /**
     * @brief Creates a new instance of the node.
     * @return Pointer to new brushShopNode instance
     */
    static void *creator();
    
    /**
     * @brief Initializes the node attributes.
     * @return MStatus Success or failure status
     */
    static MStatus initialize();
    
    /**
     * @brief Post-constructor setup.
     */
    virtual void postConstructor();
    
    /**
     * @brief Computes the brush shop output.
     * @param plug The plug to compute
     * @param data The data block containing the node's data
     * @return MStatus Success or failure status
     */
    virtual MStatus compute(const MPlug &plug, MDataBlock &data);

    /** @brief Unique Maya type ID for the node */
    static MTypeId id;

private:
    /**
     * @brief Collects brushes from input attributes.
     * @param data Data block containing brush data
     * @param brushes Output map to store collected brushes
     * @return MStatus Success or failure status
     */
    MStatus collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const;

    /** @brief Input array of brushes */
    static MObject aBrushes;
    /** @brief Output brush shop data */
    static MObject aOutput;
};

#endif