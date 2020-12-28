
#ifndef _mapColorStrokes_H
#define _mapColorStrokes_H

#include <maya/MPxNode.h>
#include "stroke.h"

class mapColorStrokes : public MPxNode
{
public:
  mapColorStrokes();
  virtual ~mapColorStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;

private:
  MStatus copyStrokes(MDataBlock &data, std::vector<Stroke> *geom);
  
  void getPoints(MDataBlock &data, std::vector<Stroke> *geom, MFloatPointArray &points) const;
  // void calcUVs(MDataBlock &data, std::vector<Stroke> *geom, MFloatArray &u, MFloatArray &v) const;
 

void applyColor(
    MDataBlock &data,
    std::vector<Stroke> *geom,
    MFloatPointArray &points ) const;


  static MObject aStrokes;

  static MObject aRGB;

  static MObject aWhite;

  static MObject aOutput;
};

#endif