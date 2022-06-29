
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
    kStrokeParam,
    kPivot
  };

  enum Property {
    kBrushId,
    kPaintId,
    kLayerId
  };
  

  void getIds(
      MDataBlock &data,
      MFloatPointArray &points,
      int quantizeLevels,
      MIntArray &resultIds) const;

  void applyIds(
      std::vector<Stroke> *strokes,
      const MIntArray &ids,
       mapIdStrokes::Property idProperty) const;
 
  static MObject aNormalize;  
  static MObject aNumberOfValues; /// normalize

  static MObject aIdProperty;

  static MObject aIdMap;         ///> The solid texture whose red channel will be mapped to the chosen Id property of strokes.
  static MObject aSampleAt;
  static MObject aStrokeParam;
  static MObject aIdOffset;
  
  
};

#endif