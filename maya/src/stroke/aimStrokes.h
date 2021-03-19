
#ifndef _aimStrokes_H
#define _aimStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief Rotate all target matrices so they aim their z (lance) towards a point.
 * 
 */
class aimStrokes : public strokeMutator
{
public:
  aimStrokes();
  virtual ~aimStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(MDataBlock &data, std::vector<Stroke> *strokes) const;

  static MTypeId id;

private:

  static MObject aPoint; ///> The point to aim towards. 

};

#endif