
#ifndef _hatchStrokes_H
#define _hatchStrokes_H
#include <vector>

#include <maya/MFloatPointArray.h>
#include <maya/MDataBlock.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MObject.h>

#include "strokeMutator.h"
#include "stroke.h"
#include "target.h"

/**
 * @brief Map a solid texture to the color property of all targets.
 *
 * An occlusion mesh can be connected, which will set any occluded targets to black.
 *
 */
class hatchStrokes : public strokeMutator
{
public:
  hatchStrokes();
  virtual ~hatchStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  static MTypeId id;

  virtual MStatus mutate(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *strokes) const;

  // void getFlow(
  //     const MFloatPointArray &points,
  //     MDataBlock &data,
  //     MFloatVectorArray &flow) const;

  // void addHatchSet(
  //     MDataHandle &h,
  //     const MVector &planeNormal,
  //     int numPoints,
  //     const MFloatMatrix &targetRotationMatrix,
  //     const MFloatPointArray &points,
  //     const MFloatVectorArray &flow,
  //     const MFloatVectorArray &tangents,
  //     const MColorArray &colors,
  //     const MColorArray &rampColors,
  //     std::vector<Stroke> *strokes) const;

void addHatchSet(
    MDataHandle &handle,
    unsigned numPointsSide,
    // const MVector &planeNormal,
    const MFloatMatrix &targetRotationMatrix,
    const MFloatPointArray &points,
    // const MFloatVectorArray &tangents,
    // const MFloatVectorArray &flow,
    const MColorArray &colors,
    const MColorArray &rampColors,
    const MFloatMatrix &projectionWorldToUv,
    const MFloatMatrix &projectionUvToWorld,
    const MFloatMatrix &projectionUvToImage,
    const MFloatMatrix &projectionImageToUv,
    const CImg<float> *pImage,
    std::vector<Stroke> *strokes) const;


void getFlowPoints(
    const MFloatPoint &point,
    unsigned numPointsSide,
    float hatchLength,
    float flowAttraction,
    const MFloatMatrix &projectionWorldToUv,
    const MFloatMatrix &projectionUvToWorld,
    const MFloatMatrix &projectionUvToImage,
    const MFloatMatrix &projectionImageToUv,
    const CImg<float> *pImage,
    MFloatPointArray &flowPoints) const;


private:
  static MObject aTargetRotationMatrix;
  static MObject aFlowImage;
  
  static MObject aNumPointsSide;
  static MObject aFlowProjection;

  static MObject aHatchLengthMax;
  static MObject aHatchLengthMin;
  static MObject aHatchLength;

  static MObject aHatchAngleMin;
  static MObject aHatchAngleMax;
  static MObject aHatchAngle;

  static MObject aTangentBlend;
  static MObject aFlowAttraction;

  
  static MObject aColorRamp;

  static MObject aHatch;

  static MObject aSeed;
};

#endif