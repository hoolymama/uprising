
#ifndef _meshStrokeNode_H
#define _meshStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeNodeBase.h>
#include "stroke.h"

/**
 * @brief A node to generatestrokes from the features of a mesh.
 */
class meshStrokeNode : public strokeNodeBase
{

public: 


  meshStrokeNode();

  virtual ~meshStrokeNode();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  static MTypeId id;

protected:

  /**
   * @brief Generate some strokes.
   * 
   * @param data Access to all node's data block
   * @param pStrokes A pointer to the vectmor of strokes to be filled
   * @return MStatus 
   */
  MStatus generateStrokeGeometry(
      MDataBlock &data,
      std::vector<Stroke> *pStrokes) const;

  static MObject aMesh; ///< The input mesh
  static MObject aViewpoint; ///< The location of the camera
  static MObject aPointDensity; ///< The minimum number of targets per centimeter.

};

#endif
