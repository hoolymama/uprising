
#ifndef _strokeCurve_H
#define _strokeCurve_H

#include <maya/MFloatArray.h>
#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MAngle.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "stroke.h"


// #include "forwardstroke.h"
// #include "backstroke.h"

// #include "brush.h"
// #include "paint.h"



class strokeCurve : public MPxNode
{
public:
  strokeCurve();
  virtual ~strokeCurve();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  // virtual bool            isBounded() const;

  // virtual MBoundingBox    boundingBox() const;

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId   id;

  enum Spac { kParametric, kArcLen };


private:


  MStatus setData(MDataBlock &block, MObject &attribute,
                  const MVectorArray &data) ;

  MStatus setData(MDataBlock &block, MObject &attribute,
                  const MDoubleArray &data) ;

  MStatus setData(MDataBlock &block, MObject &attribute,
                  const MMatrixArray &data) ;

  MStatus setData(MDataBlock &block, MObject &attribute,
                  const MIntArray &data) ;

  MStatus setData(MDataBlock &block, MObject &attribute,
                  const MMatrix &data);

  MStatus getData( MObject &attribute,  MIntArray &array);

  MStatus getData( MObject &attribute,  MDoubleArray &array);

  MStatus getData( MObject &attribute,  MVectorArray &array);


  unsigned int  getStrokeBoundaries(
    const MObject &curve,
    double strokeLength,
    double randomLengthFactor,
    double overlap,
    double randomOverlapFactor,
    double subcurveMin,
    double subcurveMax,
    double countFactor,
    MVectorArray &result
  ) const  ;

  // MStatus getBrushes(MDataBlock &data, std::map<short, Brush> &brushes ) const;

  // MStatus getPaints(MDataBlock &data, std::map<short, Paint> &paints ) const ;

  MStatus generateStrokes(MDataBlock &data,
                          std::vector<std::unique_ptr<Stroke> > &strokes  ) const;

  MVector  binormal(const MMatrix &p1, const MMatrix &p2, const MVector &normal) const;


  MStatus getTextureName(const MObject &attribute,
                         MString &name) const;
  MStatus sampleUVTexture(const MObject &attribute,  MFloatArray &uVals,
                          MFloatArray &vVals, MFloatArray &result) const;

  MStatus sampleUVGradient(const MObject &attribute, float sampleDistance,
                           MFloatArray &uVals,
                           MFloatArray &vVals, MFloatVectorArray &result) const;
  // curves
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
  static MObject aStrokeRotationTexture;
  static MObject aStrokeTranslationTexture;
  static MObject aStrokeTranslationSampleDistance;

  // output
  static MObject  aOutTargets;
  static MObject  aOutTangents;
  static MObject  aOutPositions;

  // one per stroke
  static MObject  aOutCounts;
  static MObject  aOutArcLengths;
};

#endif




















