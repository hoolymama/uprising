/**
 * @file cImgLayout.h
 * @brief Maya node for compositing multiple images into a single layout.
 *
 * This file implements a Maya node that arranges multiple input images into a single
 * composite image. The node provides:
 * - Flexible image positioning with offset controls
 * - Cropping capabilities for each input image
 * - Mask generation for compositing
 * - Resolution control for the output image
 * - Isolation mode for debugging individual inputs
 */

#ifndef _cImgLayout
#define _cImgLayout

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

#include "cImgData.h"

/**
 * @class cImgLayout
 * @brief Maya node for compositing multiple images into a single layout.
 *
 * The cImgLayout node provides advanced image compositing capabilities:
 * - Multiple input images with individual positioning
 * - Per-image cropping and offset controls
 * - Mask generation with padding and inversion options
 * - Optional darkening based on mask
 * - Isolation mode for debugging
 * - Real-time preview in Maya viewport
 */
class cImgLayout : public MPxNode
{
public:
    /**
     * @brief Default constructor.
     */
    cImgLayout();

    /**
     * @brief Virtual destructor.
     */
    virtual ~cImgLayout();

    /**
     * @brief Computes the output composite image based on input parameters.
     * @param plug The plug to compute
     * @param data The data block containing the node's data
     * @return MStatus Success or failure status
     */
    virtual MStatus compute(const MPlug& plug, MDataBlock& data);
    // virtual bool    isAbstractClass() const {return true;}

    // virtual MStatus process(MDataBlock &data, const CImgList<unsigned char> &images,
    //                         CImg<unsigned char> &result);

    /**
     * @brief Creates a new instance of the node.
     * @return Pointer to the new cImgLayout instance
     */
    static void* creator();

    /**
     * @brief Initializes the node attributes.
     * @return MStatus Success or failure status
     */
    static MStatus initialize();

    /** @brief Unique Maya type ID for the node */
    static MTypeId id;

protected:
    /** @brief Output resolution for the composite image */
    static MObject aResolution;

    /** @brief Padding amount for the mask */
    static MObject aMaskPadding;

    /** @brief Whether to invert the output mask */
    static MObject aInvertMask;

    /** @brief Whether to darken the output based on the mask */
    static MObject aDarkenByMask;

    /** @brief Index of input to isolate (-1 for no isolation) */
    static MObject aIsolate;

    /** @brief Input image data for each layer */
    static MObject aInputImage;

    /** @brief X offset for image positioning */
    static MObject aOffsetX;

    /** @brief Y offset for image positioning */
    static MObject aOffsetY;

    /** @brief Compound attribute for X/Y offset */
    static MObject aOffset;

    /** @brief Whether the input is active */
    static MObject aActive;

    /** @brief X coordinate of crop corner */
    static MObject aCropCornerX;

    /** @brief Y coordinate of crop corner */
    static MObject aCropCornerY;

    /** @brief Compound attribute for crop corner position */
    static MObject aCropCorner;

    /** @brief X resolution for cropping */
    static MObject aCropResolutionX;

    /** @brief Y resolution for cropping */
    static MObject aCropResolutionY;

    /** @brief Compound attribute for crop resolution */
    static MObject aCropResolution;

    /** @brief Array of input layers (compound attribute) */
    static MObject aInput;

    /** @brief Output composite image */
    static MObject aOutput;

    /** @brief Output mask for compositing */
    static MObject aOutputMask;
};

#endif
