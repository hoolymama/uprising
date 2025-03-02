/**
 * @file curveStrokeNode.h
 * @brief Defines the curveStrokeNode class for generating strokes from curves in Maya.
 * 
 * This file contains the curveStrokeNode class which extends paintStrokeCreator to provide
 * functionality for generating strokes from NURBS curves. It includes methods for
 * creating stroke data from curves, handling curve parameters, and creating strokes
 * with appropriate properties.
 */

#ifndef _curveStrokeNode_H
#define _curveStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <paintStrokeCreator.h>
#include "stroke.h"
#include "brush.h"

/**
 * @class curveStrokeNode
 * @brief A node for generating strokes from NURBS curves.
 * 
 * The curveStrokeNode class extends paintStrokeCreator to create a specialized node that
 * generates strokes from NURBS curves. It samples points along the curves based on
 * density settings and creates strokes with appropriate properties, including width,
 * orientation, and pivot placement.
 */
class curveStrokeNode : public paintStrokeCreator
{
public:
  /**
   * @brief Default constructor.
   */
  curveStrokeNode();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~curveStrokeNode();
  
  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new curveStrokeNode object.
   */
  static void *creator();
  
  /**
   * @brief Initialize attributes and connections for the node.
   * @return MStatus indicating success or failure.
   */
  static MStatus initialize();
  
  /**
   * @brief Post-constructor initialization.
   */
  virtual void postConstructor();
  
  /**
   * @brief Indicates whether this class is abstract.
   * @return False, as this is a concrete implementation.
   */
  virtual bool isAbstractClass() const { return false; }
  
  /**
   * @brief Generates stroke geometry from curves.
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @param pOutStrokes Pointer to a vector to store the generated strokes.
   * @return MStatus indicating success or failure.
   */
  MStatus generateStrokeGeometry(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *pOutStrokes);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /** @brief Curves attribute for the input curves. */
  static MObject aCurves;
  
  /** @brief Width attribute for the stroke width. */
  static MObject aWidth;
  
  /** @brief Split angle attribute for determining when to split strokes. */
  static MObject aSplitAngle;
  
  /** @brief Linear curve flag attribute. */
  static MObject aLinearCurve;
  
  /** @brief Pivot attribute for determining pivot placement. */
  static MObject aPivot;

  /**
   * @enum Pivot
   * @brief Defines options for pivot placement.
   */
  enum Pivot
  {
    kFirstTarget, /**< Place pivot at the first target. */
    kCurveStart   /**< Place pivot at the curve start. */
  };

  /**
   * @brief Creates stroke data from a curve.
   * @param dCurve The curve object.
   * @param startDist The start distance along the curve.
   * @param endDist The end distance along the curve.
   * @param density The point density.
   * @param minimumPoints The minimum number of points.
   * @param curveParams Output array to store the curve parameters.
   * @return The number of points created.
   */
  unsigned createStrokeData(
      const MObject &dCurve,
      float startDist,
      float endDist,
      float density,
      int minimumPoints,
      MDoubleArray &curveParams) const;

  /**
   * @brief Creates a stroke from a curve and parameters.
   * @param dCurve The curve object.
   * @param curveParams Array of curve parameters.
   * @param radius The stroke radius.
   * @return The created stroke.
   */
  Stroke createStroke(
      const MObject &dCurve,
      const MDoubleArray &curveParams,
      float radius) const;
};

#endif
