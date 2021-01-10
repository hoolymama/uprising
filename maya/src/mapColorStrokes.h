
#ifndef _mapColorStrokes_H
#define _mapColorStrokes_H

#include "strokeMutator.h"
// #include <maya/MPxNode.h>
#include "stroke.h"

class mapColorStrokes : public strokeMutator
{
public:
  mapColorStrokes();
  virtual ~mapColorStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(MDataBlock &data, std::vector<Stroke> *geom) const;
  static MTypeId id;

private:

  void getPoints(MDataBlock &data, std::vector<Stroke> *geom, MFloatPointArray &points) const;

  void applyColor(
    MDataBlock &data,
    std::vector<Stroke> *geom,
    MFloatPointArray &points ) const;


  // static MObject aStrokes;

  static MObject aRGB;
  // static MObject aR;
  // static MObject aG;
  // static MObject aB;
  static MObject aWhite;

  // static MObject aOutput;
};

#endif