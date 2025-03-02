/**
 * @file colorSpreadNode.h
 * @brief Defines the colorSpreadNode class for generating variations of colors.
 * 
 * This file contains the colorSpreadNode class which provides functionality for
 * generating variations of colors based on hue, saturation, and value ranges.
 * It supports different propagation methods for creating color variations.
 */

#ifndef _colorSpreadNode_H
#define _colorSpreadNode_H

#include <maya/MPxNode.h>

/**
 * @class colorSpreadNode
 * @brief A node for generating variations of colors.
 * 
 * The colorSpreadNode class extends MPxNode to create a specialized node that
 * generates variations of colors based on hue, saturation, and value ranges.
 * It provides functionality for:
 * - Creating color variations with controlled ranges
 * - Supporting different propagation methods
 * - Generating random variations with seed control
 */
class colorSpreadNode : public MPxNode
{
public:
  /**
   * @brief Default constructor.
   */
  colorSpreadNode();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~colorSpreadNode();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new colorSpreadNode object.
   */
  static void *creator();
  
  /**
   * @brief Initialize attributes and connections for the node.
   * @return MStatus indicating success or failure.
   */
  static MStatus initialize();
  
  /**
   * @brief Post-constructor setup for the node.
   */
  virtual void postConstructor();
  
  /**
   * @brief Compute method for calculating the node's outputs.
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /**
   * @brief Gets the IDs of connected inputs.
   * @param result Output array to store the connection IDs.
   * @return Number of connected inputs.
   */
  int getConnectionIds(MIntArray &result) const;

  /** @brief Input color attribute. */
  static MObject aColor;

  /** @brief Hue range attribute. */
  static MObject aHueRange;
  
  /** @brief Hue propagation method attribute. */
  static MObject aHuePropagation;

  /** @brief Saturation range attribute. */
  static MObject aSaturationRange;
  
  /** @brief Saturation propagation method attribute. */
  static MObject aSaturationPropagation;

  /** @brief Value range attribute. */
  static MObject aValueRange;
  
  /** @brief Value propagation method attribute. */
  static MObject aValuePropagation;

  /** @brief Random seed attribute. */
  static MObject aSeed;

  /** @brief Output color attribute. */
  static MObject aOutColor;

  /**
   * @enum Propagation
   * @brief Enumeration for color propagation methods.
   * 
   * Defines different methods for propagating color variations.
   */
  enum Propagation
  {
    kSpread,           /**< Spread values evenly across the range. */
    kForwardLinear,    /**< Linear progression from start to end. */
    kForwardRandom     /**< Random progression with forward bias. */
  };
};

#endif