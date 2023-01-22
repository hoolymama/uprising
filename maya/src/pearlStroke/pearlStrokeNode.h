
#ifndef _pearlStrokeNode_H
#define _pearlStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeCreator.h>
#include "stroke.h"

#include "pearlChain.h"
#include "brush.h"

class pearlStrokeNode : public strokeCreator
{
public:
  pearlStrokeNode();
  virtual ~pearlStrokeNode();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }

  virtual MStatus generateStrokeGeometry(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

  static MTypeId id;

private:
  static MObject aPearlChains;
  static MObject aInputMatrix;

  static MObject aSmoothNeighbors;
  static MObject aSmoothPositions;
  static MObject aSmoothWeights;

  static MObject aTargetRotationMatrix;

  static MObject aMinimumPoints;

};


#endif
