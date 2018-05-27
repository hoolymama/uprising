
#ifndef _strokeFactoryNODE_H
#define _strokeFactoryNODE_H


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



class strokeFactory : public MPxLocatorNode
{
public:
  strokeFactory();

  virtual ~strokeFactory();

  static void *creator();

  static MStatus initialize();

  virtual void postConstructor();

  virtual bool            isBounded() const;

  virtual MBoundingBox    boundingBox() const;

  virtual void draw(  M3dView &view,
                      const MDagPath &path,
                      M3dView::DisplayStyle style,
                      M3dView:: DisplayStatus status  );

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId   id;

  static MObject aInMatrix;

  enum Spac { kParametric, kArcLen };

private:

  // unsigned int getStrokeBoundaries(
  //   const MObject &curve, double &strokeLength,
  //   MVectorArray &boundaries
  // ) const;

  unsigned int getStrokeBoundaries(
    const MObject &curve, double strokeLength, double randomLengthFactor, double overlap,
    double randomOverlapFactor, MVectorArray &result
  ) const ;

  MStatus getBrushes(MDataBlock &data, std::map<short, Brush> &brushes ) const;

  MStatus getPaints(MDataBlock &data, std::map<short, Paint> &paints ) const ;

  MStatus getStrokes(MDataBlock &data,
                     const MVector &normal,
                     const MMatrix &inversePlaneMatrix,
                     const   std::map<short, Brush> brushes,
                     const   std::map<short, Paint> paints,
                     std::vector<stroke> &strokes ) const;

  MStatus getTextureName(const MObject &attribute,
                         MString &name) const;
  MStatus sampleUVTexture(const MObject &attribute,  MFloatArray &uVals,
                          MFloatArray &vVals, MFloatArray &result) const;

  MStatus sampleUVGradient(const MObject &attribute, float sampleDistance,
                           MFloatArray &uVals,
                           MFloatArray &vVals, MFloatVectorArray &result) const;
  // curves
  static MObject aCurve;
  static MObject aPointDensity;
  static MObject aStrokeLength;
  static MObject aRandomLengthFactor;
  static MObject aRandomOverlapFactor;


  static MObject aAttack;
  static MObject aLift;
  static MObject aElevation;
  static MObject aBrushId;
  static MObject aPaintId;
  static MObject aActive;
  static MObject aOverlap;
  static MObject aAlternate;
  static MObject aStrokeRotation;
  static MObject aStrokeTranslation;
  static MObject aPivotFraction;



  static MObject aCurves;

  //  brushes
  static MObject  aBrushWidth;
  static MObject  aBrushName;
  static MObject  aBrushTcp;
  static MObject  aBrushTip;
  static MObject  aBrushes;

  // paint
  static MObject  aPaintColorR;
  static MObject  aPaintColorG;
  static MObject  aPaintColorB;
  static MObject  aPaintColor;
  static MObject  aPaintName;
  static MObject  aPaintOpacity;
  static MObject  aPaintMaxArcLength;
  static MObject  aPaints;

  // general
  // static MObject aNormal;
  static MObject aPlaneMatrix;
  static MObject aStrokeRotationTexture;
  static MObject aStrokeTranslationTexture;
  static MObject aStrokeTranslationSampleDistance;
  static MObject aStrokeCountFactor;

  // output
  static MObject  aOutCounts;
  static MObject  aOutBrushIds;
  static MObject  aOutPaintIds;
  static MObject  aOutPointsWorld;
  static MObject  aOutPointsLocal;
  static MObject  aOutNormalsWorld;
  static MObject  aOutNormalsLocal;
  static MObject  aOutBrushWidths;
  static MObject  aOutPaintColors;
  static MObject  aOutPaintOpacities;
  static MObject  aOutArcLengths;


  // display
  static MObject aDisplayPoints;
  static MObject aDisplayNormals;

  static MObject aDisplayAttackLift;
  static MObject aDisplaySegmentOutlines;
  static MObject aSegmentOutlineThickness;

  static MObject aDisplaySegments;
  static MObject aNormalLength;
  static MObject aPointSize;

  static MObject aWireColor;
  static MObject aStackGap;

};


namespace strokeFactoryCallback
{
static  MCallbackId id;

static void makeDefaultConnections(  MObject &node, void *clientData )
{

  MPlug wmPlugmulti( node, strokeFactory::worldMatrix );
  MPlug wm( wmPlugmulti.elementByLogicalIndex( 0 ) );
  MPlug mt( node, strokeFactory::aInMatrix );

  MDGModifier mod;
  mod.connect( wm, mt );
  MStatus stat = mod.doIt();
  if (stat != MS::kSuccess)
  { stat.perror("strokeFactory ERROR :: callback unable to make matrix connections"); }
}
}


#endif




















