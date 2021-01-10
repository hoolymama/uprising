
#ifndef _tiltStrokes_H
#define _tiltStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

class tiltStrokes : public strokeMutator
{
public:
  tiltStrokes();
  virtual ~tiltStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(MDataBlock &data, std::vector<Stroke> *geom) const;
  static MTypeId id;

private:

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

  int countTargets(const std::vector<Stroke> *geom) const;

  
  static MObject aProjection;
  static MObject aGlobalTiltTexture;
  static MObject aGlobalTiltSampleDistance;
  static MObject aGlobalTiltAmount;

};

#endif