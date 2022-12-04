
#ifndef _hatchStrokeNode_H
#define _hatchStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <paintStrokeCreator.h>
#include "stroke.h"

#include "pearlChain.h"
#include "brush.h"

class hatchStrokeNode : public strokeCreator
{
public:
  hatchStrokeNode();
  virtual ~hatchStrokeNode();
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
  static MObject aPoints;
  static MObject aGradients;
  
  static MObject aScaleGradients;
  
  static MObject aLengthRamp;
  static MObject aLengthRangeMin;
  static MObject aLengthRangeMax;
  static MObject aLengthRange;

  static MObject aWidthRamp;
  static MObject aWidthRangeMin;
  static MObject aWidthRangeMax;
  static MObject aWidthRange;


  static MObject aInputMatrix;

  static MObject aSmoothNeighbors;
  static MObject aSmoothPositions;
  static MObject aSmoothWeights;

  static MObject aTargetRotationMatrix;

  static MObject aMinimumPoints;

};


#endif
