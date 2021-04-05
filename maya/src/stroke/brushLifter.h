
#ifndef _brushLifter_H
#define _brushLifter_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief Rotate all target matrices so they aim their z (lance) towards a point.
 * 
 */
class brushLifter : public strokeMutator
{
public:
  brushLifter();
  virtual ~brushLifter();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(MDataBlock &data, std::vector<Stroke> *strokes) const;

  static MTypeId id;

private:

  static MObject aBrushes; ///> The list of available brushes. 

  MStatus collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const;

};

#endif