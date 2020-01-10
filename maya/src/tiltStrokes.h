
#ifndef _tiltStrokes_H
#define _tiltStrokes_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "cImgData.h"
#include "stroke.h"

class tiltStrokes : public MPxNode
{
public:
  tiltStrokes();
  virtual ~tiltStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;

private:
  MStatus copyStrokes(MDataBlock &data, std::vector<Stroke> *geom);

  void assignTargetUVs(MDataBlock &data, std::vector<Stroke> *geom);

  void appendTargetUVsTo(
      MFloatArray &uVals,
      MFloatArray &vVals,
      const std::vector<Stroke> *geom) const;

  bool tilt(
      MFloatArray &uVals,
      MFloatArray &vVals,
      MDataBlock &data,
      std::vector<Stroke> *geom) const;

  bool speedMap(
      MFloatArray &uVals,
      MFloatArray &vVals,
      MDataBlock &data,
      std::vector<Stroke> *geom) const;

  int countTargets(const std::vector<Stroke> *geom) const;

  static MObject aStrokes;
  static MObject aProjection;

  static MObject aGlobalTiltTexture;
  static MObject aGlobalTiltSampleDistance;
  static MObject aGlobalTiltAmount;

  static MObject aLinearSpeedTexture;
  static MObject aAngularSpeedTexture;

  static MObject aOutput;
};

#endif