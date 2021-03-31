
#ifndef _strokeMutator_H
#define _strokeMutator_H

#include <strokeNodeBase.h>
#include <maya/MPxNode.h>
#include "stroke.h"

/**
 * @brief A node that takes input strokes, mutates them, and puts them on the
 * output. 
 *
 * It inherits from strokeNodeBase, which as no input strokes, and it adds the
 * input attribute (aStrokes).
 */
class strokeMutator : public strokeNodeBase
{
public:
  strokeMutator();
  virtual ~strokeMutator();
  virtual bool isAbstractClass() const { return true; }

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  virtual MStatus generateStrokeGeometry(
      MDataBlock &data,
      std::vector<Stroke> *pStrokes) ;

  static MTypeId id;

protected:

  /**
   * @brief Derived calsses should implement this and change the given strokes. 
   * @param data The node's data block.
   * @param strokes a mutable vector of strokes.
   * @return MStatus 
   */
  virtual MStatus mutate(
    MDataBlock &data,
    std::vector<Stroke> *strokes) const;

  static MObject aStrokes; ///> Input strokes attribute
  static MObject aTrigger; ///> A multi that can be used to trigger the compute.
};

#endif