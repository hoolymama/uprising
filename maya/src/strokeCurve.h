
#ifndef _strokeCurveNODE_H
#define _strokeCurveNODE_H


#include <maya/MPxLocatorNode.h>
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
#include "brush.h"
#include "paint.h"



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


  unsigned int getStrokeBoundaries(
    const MObject &curve,
    double strokeLength,
    double randomLengthFactor,
    double overlap,
    double randomOverlapFactor,
    double subcurveMin,
    double subcurveMax,
    MVectorArray &result
  ) const ;

  // MStatus getBrushes(MDataBlock &data, std::map<short, Brush> &brushes ) const;

  // MStatus getPaints(MDataBlock &data, std::map<short, Paint> &paints ) const ;

  MStatus getStrokes(MDataBlock &data,
                     const MVector &normal,
                     const MMatrix &inversePlaneMatrix,
                     const std::map<short, Brush> brushes,
                     const std::map<short, Paint> paints,
                     std::vector<Stroke> &strokes ) const;

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



  static MObject aLiftLength;
  static MObject aLiftBias;
  static MObject aLiftHeight;
  // static MObject aLiftPower;
  static MObject aStrokeProfile;

  static MObject aBrushMatrix;


  // static MObject aRotateOrder;
  // static MObject aOutputUnit;

  static MObject aBrushId;
  static MObject aPaintId;
  static MObject aActive;
  static MObject aStrokeCountFactor;

  static MObject aOverlap;
  static MObject aPivotFraction;
  // static MObject aStrokeRotation;
  // static MObject aStrokeTranslation;
  // static MObject aBrushAlignment;


  // static MObject  aBrushRotateTilt;
  // static MObject  aBrushRotateBank;
  // static MObject  aBrushRotateTwist;
  // static MObject  aBrushRotate;
  static MObject  aBrushRotateRampRange;
  static MObject  aBrushRotateTiltRamp;
  static MObject  aBrushRotateBankRamp;
  static MObject  aBrushRotateTwistRamp;

  static MObject  aBrushFollowStroke;
  static MObject  aForceDip;


  // static MObject  aBrushMatrix;


  static MObject  aApproachDistanceStart;
  static MObject  aApproachDistanceMid;
  static MObject  aApproachDistanceEnd;
  static MObject  aApproachDistance;

  // static MObject  aBrushFrontAxis;
  // static MObject  aBrushUpAxis;


  // static MObject aCurves;

  // //  brushes
  // static MObject  aBrushLiftLength;
  // static MObject  aBrushLiftHeight;
  // static MObject  aBrushLiftBias;
  // static MObject  aBrushLift;




  // static MObject  aBrushTcpX;
  // static MObject  aBrushTcpY;
  // static MObject  aBrushTcpZ;
  // static MObject  aBrushMatrix;

  // static MObject  aBrushRetention;
  // static MObject  aBrushWidth;
  // // static MObject  aBrushName;
  // static MObject  aBrushes;

  // // paint
  // static MObject  aPaintColorR;
  // static MObject  aPaintColorG;
  // static MObject  aPaintColorB;
  // static MObject  aPaintColor;
  // static MObject  aPaintName;
  // static MObject  aPaintOpacity;
  // static MObject  aPaintMaxArcLength;
  // static MObject  aPaints;

  // general
  // static MObject aNormal;

  static MObject aPlaneMatrix;
  static MObject aStrokeRotationTexture;
  static MObject aStrokeTranslationTexture;
  static MObject aStrokeTranslationSampleDistance;

  // static MObject aStrokeApproachDistance;
  // static MObject aClusterApproachObject;
  // static MObject aToolChangeApproachObject;
  // static MObject aHomeApproachObject;

  // static MObject aLinearSpeed; // cm/sec
  // static MObject aAngularSpeed; // per sec
  // static MObject aApproximationDistance; // cm



  // output
  static MObject  aOutTargets;
  static MObject  aOutTangents;

  // one per stroke
  static MObject  aOutCounts;
  static MObject  aOutArcLengths;

  // static MObject  aOutApproachStarts;
  // static MObject  aOutApproachEnds;

  // static MObject  aOutBrushIds;
  // static MObject  aOutPaintIds;
  // static MObject  aOutCurveIds;

  // static MObject  aOutPosition; // world
  // static MObject  aOutRotation; // world

  // static MObject  aOutBrushWidths;
  // static MObject  aOutPaintColors;
  // static MObject  aOutPaintOpacities;
  // static MObject  aOutForceDips;


  // static MObject  aOutPlaneMatrixWorld;


  // display
  // static MObject aDisplayPoints;
  // static MObject aDisplayBrush;
  // static MObject aDisplayApproach;
  // static MObject aDisplayIds;

  // static MObject aDisplayBrushLift;
  // static MObject aDisplaySegmentOutlines;
  // static MObject aSegmentOutlineThickness;

  // static MObject aDisplaySegments;
  // static MObject aNormalLength;
  // static MObject aPointSize;
  // static MObject aWireColor;


  // static MObject aStackGap;

};


// namespace strokeCurveCallback
// {
// static  MCallbackId id;

// static void makeDefaultConnections(  MObject &node, void *clientData )
// {

//   MPlug wmPlugmulti( node, strokeCurve::worldMatrix );
//   MPlug wm( wmPlugmulti.elementByLogicalIndex( 0 ) );
//   MPlug mt( node, strokeCurve::aInMatrix );

//   MDGModifier mod;
//   mod.connect( wm, mt );
//   MStatus stat = mod.doIt();
//   if (stat != MS::kSuccess)
//   { stat.perror("strokeCurve ERROR :: callback unable to make matrix connections"); }
// }
// }


#endif




















