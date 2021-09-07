
#ifndef _bulgeStrokes_H
#define _bulgeStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

class bulgeStrokes : public strokeMutator
{
public:
  bulgeStrokes();
  virtual ~bulgeStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(
    const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;
  static MTypeId id;

private:

  static MObject aMult;
  static MObject aRamp;


};

#endif