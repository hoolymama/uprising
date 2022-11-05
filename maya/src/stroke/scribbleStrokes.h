
#ifndef _scribbleStrokes_H
#define _scribbleStrokes_H
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
class scribbleStrokes : public strokeMutator
{
public:
  scribbleStrokes();
  virtual ~scribbleStrokes();

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

  enum ColorPropagation
  {
    kOverride,
    kInterpolate
  };

  void getMappedValues(
      const MObject &attribute,
      MDataBlock &data,
      MFloatPointArray &points,
      MFloatVectorArray &result) const;

  void getMappedValues(
      const MObject &attribute,
      MDataBlock &data,
      MFloatPointArray &points,
      MFloatArray &result) const;

  void getMappedValues(
      const MObject &attribute,
      MDataBlock &data,
      MFloatPointArray &points,
      float scale,
      MFloatVectorArray &result) const;

  void getMappedValues(
      const MObject &attribute,
      MDataBlock &data,
      MFloatPointArray &points,
      float scale,
      MFloatArray &result) const;

  void calculateScribbleTransforms(
      const Stroke &stroke,
      int mapIndex,
      const MFloatArray &tilts,
      const MFloatArray &banks,
      const MFloatArray &twists,
      PaintingEnums::BrushRotateOrder   order,
      std::vector<MFloatMatrix> &scribbleTransforms) const;

  void getFollowMatrix(const Target &target, MFloatMatrix &mat) const;

  void applyTilt(float angle, MFloatMatrix &mat) const;
  void applyBank(float angle, MFloatMatrix &mat) const;
  void applyTwist(float angle, MFloatMatrix &mat) const;

  static MObject aTargetRotationMatrix;
  static MObject aPointDensity;
  static MObject aMinimumPoints;
  static MObject aAngle;
  static MObject aRadiusGain;
  static MObject aRadiusOffset;

  static MObject aTilt;
  static MObject aBank;
  static MObject aTwist;

  static MObject aProjectToPlane;
  static MObject aProjectionPlaneMatrix;
  

  static MObject aTiltMap;
  static MObject aBankMap;
  static MObject aTwistMap;
  static MObject aRotateOrder;

  static MObject aColorPropagation;
  static MObject aColorOverride;
  static MObject aWhiteOverride;
  static MObject aWeightOverride;
};

#endif