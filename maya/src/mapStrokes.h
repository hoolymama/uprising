
#ifndef _mapStrokes_H
#define _mapStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

class mapStrokes : public strokeMutator
{
public:
  mapStrokes();
  virtual ~mapStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(MDataBlock &data, std::vector<Stroke> *geom) const;
  static MTypeId id;

private:
 
  void calcUVs(MDataBlock &data, std::vector<Stroke> *geom, MFloatArray &u, MFloatArray &v) const;

  bool speedMap(
      MDataBlock &data,
      std::vector<Stroke> *geom,
      MFloatArray &uVals,
      MFloatArray &vVals) const;

  static MObject aProjection;
  static MObject aLinearSpeedTexture;
  static MObject aAngularSpeedTexture;

};

#endif