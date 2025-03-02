/**
 * @file paletteNode.h
 * @brief Defines the paletteNode class for representing a palette of paints in Maya.
 * 
 * This file contains the paletteNode class which represents a palette of paints
 * in Maya. It provides functionality for displaying and managing a collection of
 * paints with customizable visualization options.
 */

#ifndef _paletteNODE_H
#define _paletteNODE_H

#include <maya/MPxLocatorNode.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include <maya/MPxNode.h>

#include "paletteData.h"

/**
 * @class paletteNode
 * @brief A locator node that represents a palette of paints in Maya.
 * 
 * The paletteNode class extends MPxLocatorNode to create a specialized node that
 * represents a palette of paints in Maya. It provides functionality for:
 * - Displaying a collection of paints in the viewport
 * - Customizing the visualization of paints
 * - Managing paint properties such as color, opacity, and travel distance
 * - Supporting different display modes for paint information
 */
class paletteNode : public MPxLocatorNode
{
public:
  /**
   * @brief Default constructor.
   */
  paletteNode();

  /**
   * @brief Virtual destructor.
   */
  virtual ~paletteNode();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new paletteNode object.
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
   * @brief Indicates whether the node has a boundable volume.
   * @return true if the node is bounded, false otherwise.
   */
  virtual bool isBounded() const;

  /**
   * @brief Returns the bounding box for the node.
   * @return MBoundingBox representing the node's bounds.
   */
  virtual MBoundingBox boundingBox() const;

  /**
   * @brief Legacy viewport drawing method.
   * @param view The 3D view to draw in.
   * @param path The DAG path to this node.
   * @param style The display style to use.
   * @param status The display status to use.
   */
  virtual void draw(M3dView &view,
                    const MDagPath &path,
                    M3dView::DisplayStyle style,
                    M3dView::DisplayStatus status);

  /**
   * @brief Compute method for calculating the node's outputs.
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;
  
  /** @brief Classification string for the draw override. */
  static MString drawDbClassification;
  
  /** @brief Registrant ID for the draw override. */
  static MString drawRegistrantId;

  /** @brief Width attribute. */
  static MObject aWidth;
  
  /** @brief Height attribute. */
  static MObject aHeight;
  
  /** @brief X position attribute. */
  static MObject aXPos;
  
  /** @brief Y position attribute. */
  static MObject aYPos;

  /** @brief Display ID flag attribute. */
  static MObject aDisplayId;
  
  /** @brief Display name flag attribute. */
  static MObject aDisplayName;
  
  /** @brief Display pot ID flag attribute. */
  static MObject aDisplayPotId;
  
  /** @brief Display RGB flag attribute. */
  static MObject aDisplayRGB;
  
  /** @brief Output palette attribute. */
  static MObject aOutput;
  
private:
  /** @brief Color attribute. */
  static MObject aColor;
  
  /** @brief Opacity attribute. */
  static MObject aOpacity;
  
  /** @brief Travel distance attribute. */
  static MObject aTravel;
  
  /** @brief Pot ID attribute. */
  static MObject aPotId;
  
  /** @brief Name attribute. */
  static MObject aName;

  /** @brief Input palette attribute. */
  static MObject aInput;
};

#endif
