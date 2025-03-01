/**
 * @file cImgSmear.h
 * @brief Maya node for image smearing operations.
 *
 * This file implements a Maya node for smear effects:
 * - Directional smearing of image content
 * - Configurable smear amount and direction
 * - Optional dilation control
 * - Extends cImgProcess base functionality
 */

#ifndef _cImgSmear
#define _cImgSmear

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

#include "cImgProcess.h"

/**
 * @class cImgSmear
 * @brief Maya node for image smearing operations.
 *
 * cImgSmear extends cImgProcess with smearing capabilities:
 * - Directional smearing effects
 * - Configurable smear amount
 * - Optional dilation control
 * - Uses reference image for smear direction
 */
class cImgSmear : public cImgProcess
{
public:
    /**
     * @brief Default constructor.
     */
    cImgSmear();

    /**
     * @brief Virtual destructor.
     */
    virtual ~cImgSmear();

    /**
     * @brief Indicates this is not an abstract class.
     * @return false as this class can be instantiated
     */
    virtual bool isAbstractClass() const { return false; }

    /**
     * @brief Processes the input image with smearing.
     * @param data The data block containing the node's data
     * @param image Input image to process
     * @param result Output smeared image
     * @return MStatus Success or failure status
     */
    virtual MStatus process(MDataBlock& data, const CImg<unsigned char>& image,
                          CImg<unsigned char>& result);

    /**
     * @brief Creates a new instance of the node.
     * @return Pointer to new cImgSmear instance
     */
    static void* creator();

    /**
     * @brief Initializes the node attributes.
     * @return MStatus Success or failure status
     */
    static MStatus initialize();

    /** @brief Unique Maya type ID for the node */
    static MTypeId id;

    /** @brief Reference image for smear direction */
    static MObject aSmearImage;
    /** @brief Amount of smearing to apply */
    static MObject aSmearAmount;
    /** @brief Amount of dilation to apply */
    static MObject aDilate;
};

#endif
