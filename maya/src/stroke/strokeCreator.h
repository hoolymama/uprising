/**
 * @file strokeCreator.h
 * @brief Defines the strokeCreator class for generating new strokes in Maya.
 * 
 * This file contains the strokeCreator class which extends strokeNodeBase to provide
 * base functionality for nodes that generate new strokes. It includes methods for
 * subsampling points, applying coats, speeds, and layer IDs to strokes, and other
 * common operations needed when creating strokes.
 */

#ifndef _strokeCreator_H
#define _strokeCreator_H

#include <strokeNodeBase.h>
#include <maya/MPxNode.h>
#include "stroke.h"

/**
 * @brief A node with attributes common to all nodes that generate new strokes.
 *
 * It inherits from strokeNodeBase and adds point density and initial rot matrix.
 * This class provides common functionality for creating and manipulating strokes,
 * including methods for subsampling points, applying coats, speeds, and layer IDs.
 */
class strokeCreator : public strokeNodeBase
{
public:
  /**
   * @brief Default constructor.
   */
  strokeCreator();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~strokeCreator();
  
  /**
   * @brief Indicates whether this class is abstract.
   * @return True, as this is an abstract base class.
   */
  virtual bool isAbstractClass() const { return true; }

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new strokeCreator object.
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
   * @brief Generates stroke geometry based on the node's attributes.
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @param pStrokes Pointer to a vector to store the generated strokes.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus generateStrokeGeometry(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

protected:
  /**
   * @brief Subsamples points based on density.
   * @param inPoints Input array of float points.
   * @param density Minimum number of points per centimeter.
   * @param outPoints Output array to store the subsampled points.
   */
  void subsample(const MFloatPointArray &inPoints,
                 float density,
                 MFloatPointArray &outPoints) const;

  /**
   * @brief Subsamples points based on density.
   * @param inPoints Input array of points.
   * @param density Minimum number of points per centimeter.
   * @param outPoints Output array to store the subsampled points.
   */
  void subsample(
      const MPointArray &inPoints,
      float density,
      MFloatPointArray &outPoints) const;

  /**
   * @brief Subsamples points and colors based on density.
   * @param inPoints Input array of float points.
   * @param inColors Input array of colors.
   * @param density Minimum number of points per centimeter.
   * @param outPoints Output array to store the subsampled points.
   * @param outColors Output array to store the subsampled colors.
   */
  void subsample(const MFloatPointArray &inPoints,
                 const MColorArray &inColors,
                 float density,
                 MFloatPointArray &outPoints,
                 MColorArray &outColors) const;

  /**
   * @brief Applies coat information to strokes.
   * @param data The data block containing the node's data.
   * @param geom Pointer to a vector of strokes to modify.
   */
  void applyCoats(MDataBlock &data, std::vector<Stroke> *geom) const;
  
  /**
   * @brief Applies speed information to strokes.
   * @param data The data block containing the node's data.
   * @param geom Pointer to a vector of strokes to modify.
   */
  void applySpeeds(MDataBlock &data, std::vector<Stroke> *geom) const;
  
  /**
   * @brief Applies layer ID to strokes.
   * @param data The data block containing the node's data.
   * @param geom Pointer to a vector of strokes to modify.
   */
  void applyLayerId(MDataBlock &data, std::vector<Stroke> *geom) const;

  /**
   * @brief Sets the creator information for strokes.
   * @param name The name of the creator.
   * @param geom Pointer to a vector of strokes to modify.
   */
  void applyCreator(
      const MString &name,
      std::vector<Stroke> *geom) const;

  /** @brief Coats attribute. */
  static MObject aCoats;
  /** @brief Point density attribute (minimum number of targets per centimeter). */
  static MObject aPointDensity;
  /** @brief Layer ID attribute. */
  static MObject aLayerId;
  /** @brief Brush tilt start attribute. */
  static MObject aBrushTiltStart;
  /** @brief Brush tilt end attribute. */
  static MObject aBrushTiltEnd;
  /** @brief Brush tilt attribute. */
  static MObject aBrushTilt;
  /** @brief Brush twist start attribute. */
  static MObject aBrushTwistStart;
  /** @brief Brush twist end attribute. */
  static MObject aBrushTwistEnd;
  /** @brief Brush twist attribute. */
  static MObject aBrushTwist;
  /** @brief Brush bank start attribute. */
  static MObject aBrushBankStart;
  /** @brief Brush bank end attribute. */
  static MObject aBrushBankEnd;
  /** @brief Brush bank attribute. */
  static MObject aBrushBank;
  /** @brief Brush rotation order attribute. */
  static MObject aBrushRotateOrder;
  /** @brief Linear speed attribute (cm/sec). */
  static MObject aLinearSpeed;
  /** @brief Angular speed attribute (per sec). */
  static MObject aAngularSpeed;
  /** @brief Approximation distance attribute (cm). */
  static MObject aApproximationDistance;
};

#endif