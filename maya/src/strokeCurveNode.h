
#ifndef _strokeCurve_H
#define _strokeCurve_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <strokeNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "stroke.h"






class strokeCurve : public strokeNode
{
public:
  strokeCurve();
  virtual ~strokeCurve();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool    isAbstractClass() const {return false;}

  // virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId   id;

  enum Spac { kParametric, kArcLen };

  enum SubcurveMethod { kLength, kBookends };

protected:

  unsigned int  getStrokeBoundaries(
    MDataBlock &data,
    MVectorArray &result
  ) const  ;

  void setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                   double approachStart, double approachMid, double approachEnd) const;


  MStatus generateStrokeGeometry(MDataBlock &data,
                                 std::vector<strokeGeom> *geom) const;



  static MObject aCurve;
  static MObject aSubcurveMin;
  static MObject aSubcurveMax;
  static MObject aSubcurve;
  static MObject aSubcurveMethod;

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
