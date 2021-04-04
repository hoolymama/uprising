
#ifndef _meshStrokeNode_H
#define _meshStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeCreator.h>
#include "stroke.h"

#include "toonGraph/tNode.h"
#include "toonGraph/tGraph.h"

/**
 * @brief A node to generatestrokes from the features of a mesh.
 */
class meshStrokeNode : public strokeCreator
{

public: 


  meshStrokeNode();

  virtual ~meshStrokeNode();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  static MTypeId id;

private:

  /**
   * @brief Generate some strokes.
   * 
   * @param data Access to all node's data block
   * @param pStrokes A pointer to the vectmor of strokes to be filled
   * @return MStatus 
   */
  MStatus generateStrokeGeometry(
      MDataBlock &data,
      std::vector<Stroke> *pStrokes) ;


/**
 * @brief Generate a view dependent graph of edges from a mesh 
 * 
 * @param dInMesh ///> The mesh MObject.
 * @param viewPoint ///> The point to consider as the camera.
 * @param pGraph ///> A pointer to the resulting graph.
 * @return MStatus
 */
MStatus  buildGraph(
    MObject& dInMesh,
    const MPoint &viewPoint,
    tGraph *pGraph)const ;


/**
 * @brief insert points so there are at least density points per cm
 * 
 * @param inPoints The sparse points describing a degree 1 curve
 * @param density The  minimum number of points per cm
 * @param outPoints The result
 */
void   subdivide(
    const MFloatPointArray & inPoints, 
    float density,
    MFloatPointArray & outPoints ) const ;

  static MObject aMesh; ///< The input mesh
  static MObject aViewpoint; ///< The location of the camera
  static MObject aTargetRotationMatrix; ///> Initial matrix for generated targets.

};

#endif
