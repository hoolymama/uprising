
#ifndef _decimateStrokes_H
#define _decimateStrokes_H

#include <maya/MRampAttribute.h>
#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief Map a solid texture to the color property of all targets.
 *
 * An occlusion mesh can be connected, which will set any occluded targets to black.
 *
 */
class decimateStrokes : public strokeMutator
{
public:
  decimateStrokes();
  virtual ~decimateStrokes();

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
  void getDensities(
      MDataBlock &data,
      MFloatPointArray &points,
      MFloatArray &densities) const;

void removeSpans(
    std::vector<Stroke> *strokes,
    const MIntArray &spanMask) const;

  void dither(int smooth, const MFloatArray &densities, MIntArray &spanMask) const;

  static MObject aDensity; ///> The solid texture whose red value will be sampled to remove strokes.
  static MObject aSeed;
  static MObject aSmooth;
};

#endif