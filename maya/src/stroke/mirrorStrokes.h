
#ifndef _mirrorStrokes_H
#define _mirrorStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief Rotate all target matrices so they aim their z (lance) towards a point.
 *
 */
class mirrorStrokes : public strokeMutator
{
public:
  mirrorStrokes();
  virtual ~mirrorStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;

  static MTypeId id;

private:
  static MObject aTranslate;
  static MObject aMirror;

  // static MObject aPoint; ///> The point to aim towards.
};

#endif
