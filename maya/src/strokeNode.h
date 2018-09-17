
#ifndef _strokeNode_H
#define _strokeNode_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "stroke.h"



class strokeNode : public MPxNode
{
public:
  strokeNode();
  virtual ~strokeNode();
  virtual bool    isAbstractClass() const {return true;}

  static void *creator();
  static MStatus initialize();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId id;

protected:

  virtual MStatus generateStrokeGeometry(MDataBlock &data,
                                         std::vector<strokeGeom> *geom) const;

  static MObject aPointDensity;
  static MObject aStrokeLength;
  static MObject aRandomLengthFactor;
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

  static MObject aPivotFraction;
  static MObject aRepeatPivot;

  static MObject aBrushTiltRamp;
  static MObject aBrushTiltRangeMin;
  static MObject aBrushTiltRangeMax;
  static MObject aBrushTiltRange;

  static MObject aBrushBankRamp;
  static MObject aBrushBankRangeMin;
  static MObject aBrushBankRangeMax;
  static MObject aBrushBankRange;

  static MObject aBrushTwistRamp;
  static MObject aBrushTwistRangeMin;
  static MObject aBrushTwistRangeMax;
  static MObject aBrushTwistRange;

  static MObject  aBrushFollowStroke;

  static MObject  aApproachDistanceStart;
  static MObject  aApproachDistanceMid;
  static MObject  aApproachDistanceEnd;
  static MObject  aApproachDistance;

  static MObject aPlaneMatrix;

  static MObject aOutput;
};

#endif
