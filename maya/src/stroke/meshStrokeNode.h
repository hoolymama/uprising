#ifndef _meshStrokeNode_H
#define _meshStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeCreator.h>
#include "stroke.h"

#include "tNode.h"
#include "tGraph.h"

/**
 * @brief A node for generating strokes from the features of a mesh.
 * 
 * The meshStrokeNode class extends strokeCreator to provide functionality for
 * generating strokes based on the features of a mesh, such as edges or contours.
 * It creates a view-dependent graph of edges from the mesh and converts these
 * edges into strokes, which can be used for stylized rendering or other purposes.
 */
class meshStrokeNode : public strokeCreator
{

public:
  /**
   * @brief Default constructor.
   */
  meshStrokeNode();

  /**
   * @brief Virtual destructor.
   */
  virtual ~meshStrokeNode();
  
  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new meshStrokeNode object.
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
  
  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /**
   * @brief Generate strokes from the features of a mesh.
   *
   * This method extracts features from the input mesh and converts them into
   * strokes based on the node's attributes.
   *
   * @param plug The plug being computed.
   * @param data The data block containing the node's data.
   * @param pStrokes Pointer to a vector to store the generated strokes.
   * @return MStatus indicating success or failure.
   */
  MStatus generateStrokeGeometry(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

  /**
   * @brief Generate a view dependent graph of edges from a mesh.
   *
   * This method creates a graph representation of the mesh edges that are
   * visible from the specified viewpoint.
   *
   * @param dInMesh The mesh MObject.
   * @param viewPoint The point to consider as the camera position.
   * @param pGraph Pointer to the resulting graph.
   * @return MStatus indicating success or failure.
   */
  MStatus buildGraph(
      MObject &dInMesh,
      const MPoint &viewPoint,
      tGraph *pGraph) const;

  /** @brief Input mesh attribute. */
  static MObject aMesh;
  
  /** @brief Viewpoint attribute defining the camera position. */
  static MObject aViewpoint;
  
  /** @brief Target rotation matrix attribute for orienting the generated targets. */
  static MObject aTargetRotationMatrix;
};

#endif
