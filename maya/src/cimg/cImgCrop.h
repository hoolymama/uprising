/**
 * @file cImgCrop.h
 * @brief Maya node for image cropping and resizing operations.
 *
 * This file implements a Maya node for image cropping and resizing:
 * - Supports dynamic image cropping with adjustable boundaries
 * - Provides resizing functionality with multiple boundary conditions
 * - Handles letterboxing for aspect ratio preservation
 * - Outputs crop factors for downstream node usage
 */

#ifndef _cImgCrop
#define _cImgCrop

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgData.h"

/**
 * @class cImgCrop
 * @brief Maya node for image cropping and resizing.
 *
 * cImgCrop provides image cropping and resizing functionality:
 * - Dynamic cropping with adjustable corner points
 * - Resizing with multiple boundary handling options
 * - Letterboxing support for aspect ratio preservation
 * - Outputs crop factors for downstream node usage
 */
class cImgCrop : public MPxNode
{
public:
    /**
     * @brief Default constructor.
     */
    cImgCrop();

    /**
     * @brief Virtual destructor.
     */
    virtual ~cImgCrop();

    /**
     * @brief Computes the output cropped/resized image.
     * @param plug The plug to compute
     * @param data The data block containing the node's data
     * @return MStatus Success or failure status
     */
    virtual MStatus compute(const MPlug& plug, MDataBlock& data);

    /**
     * @brief Updates dirty state of dependent attributes.
     * @param plugBeingDirtied The plug that is being dirtied
     * @param affectedPlugs Array of plugs affected by the dirty state
     * @return MStatus Success or failure status
     */
    virtual MStatus setDependentsDirty(
        const MPlug& plugBeingDirtied,
        MPlugArray& affectedPlugs);

    /**
     * @brief Creates a new instance of the node.
     * @return Pointer to new cImgCrop instance
     */
    static void* creator();

    /**
     * @brief Initializes the node attributes.
     * @return MStatus Success or failure status
     */
    static MStatus initialize();

    /** @brief Input image data */
    static MObject aInput;
    /** @brief Enable/disable resizing */
    static MObject aResize;
    /** @brief Target resolution for resizing */
    static MObject aResizeResolution;
    /** @brief Boundary handling method */
    static MObject aBoundary;
    /** @brief Enable/disable cropping */
    static MObject aApplyCrop;
    /** @brief Crop region corner points */
    static MObject aCropCorner;
    /** @brief Crop region resolution */
    static MObject aCropResolution;
    /** @brief Letterboxing method */
    static MObject aLetterbox;
    /** @brief Output processed image */
    static MObject aOutput;
    /** @brief Output crop factor */
    static MObject aOutputCropFactor;
    /** @brief Output X offset factor */
    static MObject aOutputOffsetFactorX;
    /** @brief Output Y offset factor */
    static MObject aOutputOffsetFactorY;

    /** @brief Unique Maya type ID for the node */
    static MTypeId id;

private:
    /** @brief Boundary handling methods */
    enum Boundary { 
        kBoundaryDirichlet, ///< Zero padding
        kBoundaryNeumann    ///< Edge replication
    };
    
    /** @brief Letterboxing methods */
    enum Letterbox { 
        kBlack,  ///< Fill with black
        kWhite,  ///< Fill with white
        kRepeat  ///< Repeat edge pixels
    };
};

#endif
