/**
 * @file cImgChoice.h
 * @brief Maya node for selecting between multiple input images
 * 
 * This class implements a Maya node that acts as a multiplexer for image inputs,
 * allowing dynamic selection between multiple input images based on an index.
 * Useful for creating image switching networks or conditional image processing paths.
 */

#ifndef _cImgChoice
#define _cImgChoice

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

#include "cImgData.h"

/**
 * @class cImgChoice
 * @brief Image selection/switching node
 * 
 * This node provides functionality to switch between multiple input images:
 * - Accepts an array of input images
 * - Uses an integer selector to choose which image to output
 * - Dynamically updates when selector or inputs change
 * - Supports Maya's array functionality for variable number of inputs
 */
class cImgChoice : public MPxNode
{
public:
    /**
     * @brief Default constructor
     */
    cImgChoice();

    /**
     * @brief Virtual destructor
     */
    virtual ~cImgChoice();

    /**
     * @brief Compute method called by Maya to process the node
     * @param plug The plug to compute
     * @param data The data block containing the node's data
     * @return MStatus Success or failure status
     */
    virtual MStatus compute(const MPlug &plug, MDataBlock &data);

    /**
     * @brief Creator method for Maya plugin system
     * @return Pointer to a new instance of cImgChoice
     */
    static void *creator();

    /**
     * @brief Initialize method for Maya plugin system
     * @return MStatus Success or failure status
     */
    static MStatus initialize();

    static MTypeId id;               ///< Maya type ID for this node

protected:
    static MObject aSelector;        ///< Index of the input image to output (-1 for none)
    static MObject aInput;           ///< Array of input images
    static MObject aOutput;          ///< Selected image output
};

#endif
