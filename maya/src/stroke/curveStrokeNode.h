
#ifndef _curveStrokeNode_H
#define _curveStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <paintStrokeCreator.h>
#include "stroke.h"
#include "brush.h"

class curveStrokeNode : public paintStrokeCreator
{
public:
  curveStrokeNode();
  virtual ~curveStrokeNode();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  MStatus generateStrokeGeometry(const MPlug &plug,MDataBlock &data, std::vector<Stroke> *pOutStrokes);
 
  static MTypeId id;

private:
  static MObject aCurve;
  static MObject aBrushId;
  static MObject aBrush;

  unsigned createStrokeData(
      const MObject &dCurve,
      float startDist,
      float endDist,
      float density,
      int minimumPoints,
      MDoubleArray &curveParams) const;

  Stroke createStroke(
      const MObject &dCurve,
      const std::pair<int, Brush> &brushPair,
      const MFloatMatrix &canvasMatrix,
      const MDoubleArray &curveParams,
      bool followStroke,
      bool applyBrushBias,
      float entryTransitionLength,
      float exitTransitionLength) const;
};

#endif
