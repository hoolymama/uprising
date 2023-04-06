
#ifndef _projectStrokes_H
#define _projectStrokes_H
#include <vector>

#include <maya/MRampAttribute.h>

#include "strokeMutator.h"
#include "stroke.h"
#include "target.h"

/**
 * @brief Map a solid texture to the color property of all targets.
 *
 * An occlusion mesh can be connected, which will set any occluded targets to black.
 *
 */
class projectStrokes : public strokeMutator
{
public:
  projectStrokes();
  virtual ~projectStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }

  virtual MStatus mutate(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *strokes) const;

  static MTypeId id;

private:

  static MObject aTargetRotationMatrix;
  static MObject aProjectionMatrix;
  static MObject aProjectionPlaneDistance;
  static MObject aMinimumTargetDistance;
  static MObject aPointDensity;
  static MObject aMinimumPoints;

};

#endif