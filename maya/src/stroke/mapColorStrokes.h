
#ifndef _mapColorStrokes_H
#define _mapColorStrokes_H

#include <maya/MRampAttribute.h>
#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief Map a solid texture to the color property of all targets.
 * 
 * An occlusion mesh can be connected, which will set any occluded targets to black.
 * 
 */
class mapColorStrokes : public strokeMutator
{
public:
  mapColorStrokes();
  virtual ~mapColorStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }

  virtual  MStatus mutate(
     const MPlug &plug, 
    MDataBlock &data,
    std::vector<Stroke> *strokes) const;

  static MTypeId id;

private:

void getColors(
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatVectorArray &colors,
    MFloatArray &whites
     ) const;


bool getWaits(
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatArray &waits) const;


MStatus occludeColors(
  MDataBlock &data,
  const MFloatPointArray &points,
  MFloatVectorArray &colors, 
  MFloatArray &whites
  ) const ;

void applyColors(   
    std::vector<Stroke> *geom,
    const MFloatVectorArray &colors,
    const MFloatArray &whites) const;
 
MStatus flattenStrokeColors(
    const std::vector<Stroke> *strokes,
    const MFloatVectorArray &strokeColors, 
    const MFloatArray &strokeWhites, 
    MFloatVectorArray &colors, 
    MFloatArray &whites) const;

void applyWaits(
    std::vector<Stroke> *strokes,
    const MFloatArray &waits,
    MRampAttribute & rampAttr,
    float startEndAngle
    ) const;

void removeBlackSpans(std::vector<Stroke> *strokes, float thresh) const;

  static MObject aRGB;  ///> The solid texture whose color will be mapped to the RGB component of targets. 
  static MObject aWhite;  ///> The solid texture whose red channel will be mapped to the white component of targets.
  static MObject aWait;  ///> The solid texture whose red channel will be mapped to the wait component of targets.
  static MObject aAngleWaitRemap;
  static MObject aStartEndAngle;
  
  static MObject aPivotSampleOnly;

  
  static MObject aMesh;  ///> The occlusion mesh
  static MObject aPoint; ///> The point from which occlusions are calculated. 
  static MObject aBias; ///> An offset on the occlusion ray test.
  static MObject aDoOcclusion;
  static MObject aRemoveBlackSpans;
  static MObject aBlackSpanThreshold;

};

#endif