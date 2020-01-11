
#ifndef _mapStrokes_H
#define _mapStrokes_H

#include <maya/MPxNode.h>
#include "stroke.h"

class mapStrokes : public MPxNode
{
public:
  mapStrokes();
  virtual ~mapStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;

private:
  MStatus copyStrokes(MDataBlock &data, std::vector<Stroke> *geom);

  void calcUVs(MDataBlock &data, std::vector<Stroke> *geom, MFloatArray &u, MFloatArray &v) const;

  bool speedMap(
      MDataBlock &data,
      std::vector<Stroke> *geom,
      MFloatArray &uVals,
      MFloatArray &vVals) const;

  static MObject aStrokes;
  static MObject aProjection;

  static MObject aLinearSpeedTexture;
  static MObject aAngularSpeedTexture;

  static MObject aOutput;
};

#endif