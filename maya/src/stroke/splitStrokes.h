
#ifndef _splitStrokes_H
#define _splitStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief Rotate all target matrices so they aim their z (lance) towards a point.
 * 
 */
class splitStrokes : public strokeMutator
{
public:
  splitStrokes();
  virtual ~splitStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;

  static MTypeId id;



private:
  static MObject aStrokeIds;
  static MObject aSplitCounts;
  static MObject aSplitParams;
  
  void splitStroke(
  const MDoubleArray &splitParams,
  const Stroke *stroke,
  std::vector<Stroke> *strokes) const;


  // static MObject aPoint; ///> The point to aim towards. 

};

#endif