
#ifndef _displaceStrokes_H
#define _displaceStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief Rotate all target matrices so they aim their z (lance) towards a point.
 * 
 */
class displaceStrokes : public strokeMutator
{
public:
  displaceStrokes();
  virtual ~displaceStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;

  static MTypeId id;

private:


    static MObject aMesh; 
    static MObject aCanvasMatrix; ///> The space in which to calculate the displacement.
};

#endif