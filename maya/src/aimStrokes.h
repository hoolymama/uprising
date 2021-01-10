
#ifndef _aimStrokes_H
#define _aimStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

class aimStrokes : public strokeMutator
{
public:
  aimStrokes();
  virtual ~aimStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(MDataBlock &data, std::vector<Stroke> *geom) const;

  static MTypeId id;

private:

  static MObject aPoint;

};

#endif