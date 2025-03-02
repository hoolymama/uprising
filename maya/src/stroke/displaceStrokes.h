#ifndef _displaceStrokes_H
#define _displaceStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief A node for displacing strokes based on a mesh surface.
 * 
 * The displaceStrokes class extends strokeMutator to provide functionality for
 * displacing strokes based on a mesh surface. It projects the targets within
 * strokes onto the surface of a mesh, allowing strokes to conform to the shape
 * of the mesh. This is useful for creating strokes that follow the contours of
 * a 3D surface.
 */
class displaceStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  displaceStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~displaceStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new displaceStrokes object.
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
   * @brief Displaces the input strokes based on the mesh surface.
   * @param plug The plug being computed.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to be modified.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus mutate(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /** @brief Mesh attribute for the surface to displace onto. */
  static MObject aMesh; 
  
  /** @brief Canvas matrix attribute for defining the displacement space. */
  static MObject aCanvasMatrix;
};

#endif