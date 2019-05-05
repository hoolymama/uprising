
#ifndef _curveStrokeNode_H
#define _curveStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeNode.h>
#include "stroke.h"

class curveStrokeNode : public strokeNode
{
public:
  curveStrokeNode();
  virtual ~curveStrokeNode();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool    isAbstractClass() const {return false;}

  static  MTypeId   id;

  // enum Spac { kParametric, kArcLen };

  enum SubcurveMethod { kLength, kBookends };

protected:

  unsigned int  getStrokeBoundaries(
    MDataBlock &data,
    MVectorArray &result
  ) const  ;

  void setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                   double approachStart, double approachMid, double approachEnd) const;


  MStatus generateStrokeGeometry(MDataBlock &data,
                                 std::vector<Stroke> *geom) const;



  static MObject aCurve;
  static MObject aSubcurveMin;
  static MObject aSubcurveMax;
  static MObject aSubcurve;
  static MObject aSubcurveMethod;


  static MObject aStrokeLength;
  static MObject aRandomLengthFactor;

  static MObject aRandomOverlapFactor;
  static MObject aOverlap;

  static MObject aForceDip;
  static MObject aBrushRampScope;

  static MObject  aApproachDistanceStart;
  static MObject  aApproachDistanceMid;
  static MObject  aApproachDistanceEnd;
  static MObject  aApproachDistance;
};

#endif
