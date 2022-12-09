
#ifndef _mapIdStrokes_H
#define _mapIdStrokes_H

#include <map>
#include <maya/MRampAttribute.h>
#include "strokeMutator.h"
#include "brushShop.h"
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
  void getIds(
      MDataBlock &data,
      MFloatPointArray &points,
      int quantizeLevels,
      MIntArray &resultIds) const;

  void applyPaintIds(
      std::vector<Stroke> *strokes,
      const MIntArray &ids) const;

  MStatus assignPaintIds(MDataBlock &data, std::vector<Stroke> *strokes) const;
  MStatus assignBrushIds(MDataBlock &data, std::vector<Stroke> *strokes) const;
  MStatus assignBrushModelIds(MDataBlock &data, std::vector<Stroke> *strokes) const;
 
  MStatus getBrushShop(MDataBlock &data, BrushShop &brushShop) const;
  MStatus assignWidthBandLayerIds(MDataBlock &data, std::vector<Stroke> *strokes) const;

  static MObject aSampleParam;

  static MObject aDoBrushId;
  static MObject aBrushIdMap;
  static MObject aBrushIdMapQuantizeLevel; // 256 is normal
  static MObject aBrushIdOffset;



  static MObject aDoPaintId;
  static MObject aPaintIdMap;
  static MObject aPaintIdMapQuantizeLevel; // 256 is normal
  static MObject aPaintIdOffset;
  static MObject aPalette;

  static MObject aDoBrushModelId;
  static MObject aBrushShop;
  static MObject aSeed;


  static MObject aDoWidthBandLayerIds;
  static MObject aWidthBandLevel;

};

#endif