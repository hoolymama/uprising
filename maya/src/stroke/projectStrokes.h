#ifndef _projectStrokes_H
#define _projectStrokes_H
#include <vector>

#include <maya/MRampAttribute.h>

#include "strokeMutator.h"
#include "stroke.h"
#include "target.h"

/**
 * @brief A node for projecting strokes onto a plane or surface.
 *
 * The projectStrokes class extends strokeMutator to provide functionality for
 * projecting strokes onto a plane or surface. It transforms the targets within
 * strokes to lie on the specified projection plane, while maintaining their
 * relative positions and orientations as much as possible. This is useful for
 * ensuring strokes conform to a specific surface or plane in 3D space.
 */
class projectStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  projectStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~projectStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new projectStrokes object.
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
   * @brief Projects the input strokes onto the specified plane or surface.
   * @param plug The plug being computed.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to be modified.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus mutate(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *strokes) const;

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /** @brief Target rotation matrix attribute for orienting the projected targets. */
  static MObject aTargetRotationMatrix;
  
  /** @brief Projection matrix attribute defining the projection plane or surface. */
  static MObject aProjectionMatrix;
  
  /** @brief Projection plane distance attribute for offsetting the projection plane. */
  static MObject aProjectionPlaneDistance;
  
  /** @brief Minimum target distance attribute for ensuring targets are not too close. */
  static MObject aMinimumTargetDistance;
  
  /** @brief Point density attribute for controlling the density of projected targets. */
  static MObject aPointDensity;
  
  /** @brief Minimum points attribute for ensuring a minimum number of targets per stroke. */
  static MObject aMinimumPoints;
};

#endif