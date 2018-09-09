
#ifndef _strokeCurve_H
#define _strokeCurve_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "stroke.h"


class strokeCurve : public MPxNode
{
public:
  strokeCurve();
  virtual ~strokeCurve();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId   id;

  enum Spac { kParametric, kArcLen };


private:

  unsigned int  getStrokeBoundaries(
    MDataBlock &data,
    MVectorArray &result
  ) const  ;

  void setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                   double approachStart, double approachMid, double approachEnd) const;

  MStatus generateStrokes(MDataBlock &data,
                          std::vector<std::unique_ptr<Stroke> > &strokes  ) const;

  static MObject aCurve;
  static MObject aSubcurveMin;
  static MObject aSubcurveMax;
  static MObject aSubcurve;
  static MObject aPointDensity;
  static MObject aStrokeLength;
  static MObject aRandomLengthFactor;
  static MObject aRandomOverlapFactor;
  static MObject aBackstroke;
  static MObject aRepeats;
  static MObject aRepeatOffset;
  static MObject aRepeatMirror;
  static MObject aRepeatOscillate;
  static MObject aSeed;

  static MObject aLiftLength;
  static MObject aLiftBias;
  static MObject aLiftHeight;
  static MObject aLift;
  static MObject aStrokeProfileRamp;
  static MObject aStrokeProfileScaleMin;
  static MObject aStrokeProfileScaleMax;
  static MObject aStrokeProfileScale;


  static MObject aBrushId;
  static MObject aPaintId;
  static MObject aActive;
  static MObject aStrokeCountFactor;

  static MObject aOverlap;
  static MObject aPivotFraction;
  static MObject aRepeatPivot;


  static MObject  aBrushRampScope;
  static MObject  aBrushTiltRamp;
  static MObject  aBrushBankRamp;
  static MObject  aBrushTwistRamp;

  static MObject  aBrushFollowStroke;
  static MObject  aForceDip;

  static MObject  aApproachDistanceStart;
  static MObject  aApproachDistanceMid;
  static MObject  aApproachDistanceEnd;
  static MObject  aApproachDistance;

  static MObject aPlaneMatrix;

  static MObject aOutput;
};

#endif




















