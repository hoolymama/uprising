/**
 * @file aimStrokes.h
 * @brief Defines the aimStrokes class for orienting strokes towards a point.
 * 
 * This file contains the aimStrokes class which extends strokeMutator to provide
 * functionality for orienting strokes towards a specific point in space. It rotates
 * all target matrices within strokes so that their Z (lance) axis points towards
 * the specified point, creating a radial arrangement of strokes.
 */

#ifndef _aimStrokes_H
#define _aimStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief A node for rotating all target matrices to aim their Z (lance) axis towards a point.
 * 
 * The aimStrokes class extends strokeMutator to create a specialized node that
 * rotates all target matrices within strokes so that their Z (lance) axis points
 * towards a specified point in space. This creates a radial arrangement of strokes,
 * which can be useful for creating effects like strokes emanating from or pointing
 * towards a central point.
 */
class aimStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  aimStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~aimStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new aimStrokes object.
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
   * @brief Rotates the targets in the input strokes to aim towards the specified point.
   * @param plug The plug being computed.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to be modified.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus mutate(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /** @brief Point attribute specifying the target point to aim towards. */
  static MObject aPoint;
};

#endif