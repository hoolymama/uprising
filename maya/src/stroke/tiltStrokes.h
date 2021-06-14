
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
  virtual MStatus mutate(
    const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;
  static MTypeId id;

private:

  static MObject aCanvasMatrix;
  static MObject aGradientTexture;
  static MObject aSampleDistance;
  static MObject aStrength;

};

#endif