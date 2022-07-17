
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
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

  static MTypeId id;

protected:
  void subsample(const MFloatPointArray &inPoints, float density, MFloatPointArray &outPoints) const;
  void subsample(const MFloatPointArray &inPoints, const MColorArray &inColors, float density, MFloatPointArray &outPoints, MColorArray &outColors) const;

  // void applyRotations(MDataBlock &data, std::vector<Stroke> *pOutStrokes) const;
  void applyCoats(MDataBlock &data, std::vector<Stroke> *geom) const;
  void applySpeeds(MDataBlock &data, std::vector<Stroke> *geom) const;

void applyCreator(
  const MString &name,
  std::vector<Stroke> *geom) const;

  static MObject aCoats;
  static MObject aPointDensity; ///< The minimum number of targets per centimeter.
  static MObject aLayerId;

  static MObject aBrushTiltStart;
  static MObject aBrushTiltEnd;
  static MObject aBrushTilt;
  static MObject aBrushTwistStart;
  static MObject aBrushTwistEnd;
  static MObject aBrushTwist;
  static MObject aBrushBankStart;
  static MObject aBrushBankEnd;
  static MObject aBrushBank;
  static MObject aBrushRotateOrder;


  static MObject aLinearSpeed;           // cm/sec
  static MObject aAngularSpeed;          // per sec
  static MObject aApproximationDistance; // cm
};

#endif