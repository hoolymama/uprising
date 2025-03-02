/**
 * @file paintingNodeBase.h
 * @brief Defines the paintingBase class as a base class for painting nodes in Maya.
 * 
 * This file contains the paintingBase class which serves as a base class for
 * painting nodes in Maya. It provides common functionality for managing brushes,
 * strokes, and visualization options.
 */

#ifndef _paintingBaseNODE_H
#define _paintingBaseNODE_H

#include <map>
#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
#include "brush.h"
#include "brushShop.h"
#include <maya/MFnMatrixData.h>

/**
 * @class paintingBase
 * @brief Abstract base class for painting nodes in Maya.
 * 
 * The paintingBase class extends MPxLocatorNode to create an abstract base class
 * for painting nodes in Maya. It provides common functionality for:
 * - Managing brushes and brush shops
 * - Handling strokes
 * - Controlling visualization options
 * - Supporting linear and angular speeds
 */
class paintingBase : public MPxLocatorNode
{
public:
  /**
   * @brief Default constructor.
   */
  paintingBase();

  /**
   * @brief Virtual destructor.
   */
  virtual ~paintingBase();
  
  /**
   * @brief Indicates whether this class is abstract.
   * @return true, as this is an abstract base class.
   */
  virtual bool isAbstractClass() const { return true; }

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new paintingBase object.
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

  /** @brief Reassign parent ID attribute. */
  static MObject aReassignParentId;
  
  /** @brief Point size attribute. */
  static MObject aPointSize;
  
  /** @brief Line length attribute. */
  static MObject aLineLength;
  
  /** @brief Line thickness attribute. */
  static MObject aLineThickness;
  
  /** @brief Display targets attribute. */
  static MObject aDisplayTargets;

  /** @brief Display IDs flag attribute. */
  static MObject aDisplayIds;
  
  /** @brief Display segment IDs flag attribute. */
  static MObject aDisplaySegmentIds;
  
  /** @brief Display parent IDs flag attribute. */
  static MObject aDisplayParentIds;
  
  /** @brief Display brush IDs flag attribute. */
  static MObject aDisplayBrushIds;
  
  /** @brief Display layer IDs flag attribute. */
  static MObject aDisplayLayerIds;

  /** @brief Wire color attribute. */
  static MObject aWireColor; 

  /** @brief ID display offset attribute. */
  static MObject aIdDisplayOffset;
  
  /** @brief Arrowhead size attribute. */
  static MObject aArrowheadSize;
  
  /** @brief Draw parameter attribute. */
  static MObject aDrawParam;

protected:
  /**
   * @brief Collects brushes from the data block.
   * @param data The data block containing the node's data.
   * @param brushes Output map to store the collected brushes.
   */
  virtual void collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const;
  
  /**
   * @brief Gets the brush shop from the data block.
   * @param data The data block containing the node's data.
   * @param brushShop Output brush shop to store the result.
   * @return MStatus indicating success or failure.
   */
  MStatus getBrushShop(MDataBlock &data, BrushShop &brushShop) const;

  /** @brief Strokes attribute. */
  static MObject aStrokes;
  
  /** @brief Brushes attribute. */
  static MObject aBrushes;
  
  /** @brief Brush shop attribute. */
  static MObject aBrushShop;
  
  /** @brief Linear speed attribute (in cm/sec). */
  static MObject aLinearSpeed;
  
  /** @brief Angular speed attribute (in rotations/sec). */
  static MObject aAngularSpeed;
  
  /** @brief Approximation distance attribute (in cm). */
  static MObject aApproximationDistance;
};
 
#endif
