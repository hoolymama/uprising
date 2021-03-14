
#ifndef _strokeCreator_H
#define _strokeCreator_H

#include <strokeNodeBase.h>
#include <maya/MPxNode.h>
#include "stroke.h"

/**
 * @brief A node with attributes common to al nodes that generate strokes. 
 *
 * It inherits from strokeNodeBase and adds point density and initial rot matrix.
 */
class strokeCreator : public strokeNodeBase
{
public:
  strokeCreator();
  virtual ~strokeCreator();
  virtual bool isAbstractClass() const { return true; }

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  virtual MStatus generateStrokeGeometry(
      MDataBlock &data,
      std::vector<Stroke> *pStrokes) const;

  static MTypeId id;

protected:

  static MObject aTargetRotationMatrix; ///> Initial matrix for generated targets.
  static MObject aPointDensity; ///< The minimum number of targets per centimeter.
};

#endif