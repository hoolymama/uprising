
#ifndef _mapIdStrokes_H
#define _mapIdStrokes_H

#include <maya/MRampAttribute.h>
#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief Map a solid texture to the brushId property of all strokes.
 *
 */
class mapIdStrokes : public strokeMutator
{
public:
  mapIdStrokes();
  virtual ~mapIdStrokes();

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

  enum SampleAt
  {
    kFirstTarget,
    kPivot
  };


  void getBrushIds(
      MDataBlock &data,
      MFloatPointArray &points,
      int numBrushes,
      MIntArray &brushIds) const;

  void applyBrushIds(
      std::vector<Stroke> *strokes,
      const MIntArray &brushIds) const;

  static MObject aBrushId;         ///> The solid texture whose red channel will be mapped to the brushId property of strokes.
  static MObject aNumberOfBrushes; /// normalize
  static MObject aSampleAt;
  
};

#endif