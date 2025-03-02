/**
 * @file enums.h
 * @brief Defines enumerations used in the painting system.
 * 
 * This file contains the PaintingEnums class which defines various enumerations
 * used throughout the painting system, including display modes for targets,
 * color display options, and brush rotation orders.
 */

#ifndef PaintingEnums_H
#define PaintingEnums_H

/**
 * @class PaintingEnums
 * @brief Container class for enumerations used in the painting system.
 * 
 * The PaintingEnums class serves as a namespace-like container for various
 * enumerations used throughout the painting system. These enumerations define
 * display modes, color options, and rotation orders for brushes.
 */
class PaintingEnums
{

public:
    /**
     * @enum TargetDisplay
     * @brief Enumeration for target display modes.
     * 
     * Defines different modes for displaying targets in the painting system.
     */
    enum TargetDisplay
    {
        kTargetsNone,   /**< Do not display targets. */
        kTargetsPoint,  /**< Display targets as points. */
        kTargetsLine,   /**< Display targets as lines. */
        kTargetsMatrix  /**< Display targets as matrices. */
    };

    /**
     * @enum TargetColorsDisplay
     * @brief Enumeration for target color display modes.
     * 
     * Defines different modes for displaying colors of targets in the painting system.
     */
    enum TargetColorsDisplay
    {
        kTargetColorsOff,    /**< Do not display target colors. */
        kTargetColorsWhite,  /**< Display targets in white. */
        kTargetColorsRGB,    /**< Display targets in RGB colors. */
        kTargetColorsBlend,  /**< Display targets with color blending. */
        kTargetColorsWait    /**< Display targets in wait state color. */
    };

    /**
     * @enum BrushRotateOrder
     * @brief Enumeration for brush rotation orders.
     * 
     * Defines different orders for applying twist, tilt, and bank rotations to brushes.
     */
    enum BrushRotateOrder
    {
        kTwistTiltBank,  /**< Apply rotations in order: twist, tilt, bank. */
        kTiltBankTwist,  /**< Apply rotations in order: tilt, bank, twist. */
        kBankTwistTilt,  /**< Apply rotations in order: bank, twist, tilt. */
        kTiltTwistBank,  /**< Apply rotations in order: tilt, twist, bank. */
        kTwistBankTilt,  /**< Apply rotations in order: twist, bank, tilt. */
        kBankTiltTwist,  /**< Apply rotations in order: bank, tilt, twist. */
    };
};

#endif
