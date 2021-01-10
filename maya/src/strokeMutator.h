
#ifndef _strokeMutator_H
#define _strokeMutator_H

#include <maya/MPxNode.h>
#include "stroke.h"

class strokeMutator : public MPxNode
{
public:
  strokeMutator();
  virtual ~strokeMutator();
  virtual bool isAbstractClass() const { return true; }

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;

protected:
  virtual MStatus mutate(MDataBlock &data, std::vector<Stroke> *geom) const ;
  void getPoints(MDataBlock &data, std::vector<Stroke> *geom, MFloatPointArray &points) const;

  static MObject aStrokes;
  static MObject aOutput;
  static MObject aTrigger;
};

#endif