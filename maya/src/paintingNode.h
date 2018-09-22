
#ifndef _paintingNODE_H
#define _paintingNODE_H


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

#include "paintingData.h"
#include "brush.h"
#include "paint.h"
#include "strokeGeom.h"


class painting : public MPxLocatorNode
{
public:
  painting();

  virtual ~painting();

  static void *creator();

  static MStatus initialize();

  virtual void postConstructor();

  virtual bool isBounded() const;

  virtual MBoundingBox boundingBox() const;

  virtual void draw(  M3dView &view,
                      const MDagPath &path,
                      M3dView::DisplayStyle style,
                      M3dView:: DisplayStatus status  );

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);


  static  MTypeId   id;


  enum Spac { kParametric, kArcLen };
  enum TargetDisplay {kTargetsNone, kTargetsPoint, kTargetsLine, kTargetsMatrix };

  // enum StrokeSort { kNoSort,
  //                   kBrushAscPaintAsc,
  //                   kBrushAscPaintDesc,
  //                   kBrushDescPaintAsc,
  //                   kBrushDescPaintDesc,
  //                   kPaintAscBrushAsc,
  //                   kPaintAscBrushDesc,
  //                   kPaintDescBrushAsc,
  //                   kPaintDescBrushDesc
  //                 };

  enum StrokeSortFilterKey {
    kId,
    kParentId,
    kBrushId,
    kPaintId,
    kRepeatId,
    kMapRed,
    kMapGreen,
    kMapBlue
  };


  enum StrokeSortDirection { kSortAscending,  kSortDescending};

  static MObject aInMatrix;

private:

  bool findInSortDefinition(StrokeSortFilterKey key,
                            const std::vector< std::pair <StrokeSortFilterKey, StrokeSortDirection> >
                            &sortDefinition);
  // bool  findInFilterDefinition( StrokeSortFilterKey key,
  //                               const  std::vector< std::tuple <StrokeSortFilterKey, strokeGeom::StrokeFilterOperator, int> >
  //                               &filterDefinition);

  MStatus populateStrokePool(MDataBlock &data,
                             std::vector<strokeGeom> &strokePool);

  void setWireDrawColor(M3dView &view,  M3dView::DisplayStatus status);

  void drawWireframeTargets(const paintingGeom &geom, M3dView &view,
                            const MDagPath &path, M3dView::DisplayStatus status );

  void drawWireframeBorders( const paintingGeom &geom, M3dView &view,
                             const MDagPath &path, M3dView::DisplayStatus status );

  void drawWireframe(const paintingGeom &geom, M3dView &view, const MDagPath &path,
                     M3dView::DisplayStatus status ) ;

  void drawWireframeApproach( const paintingGeom &geom, M3dView &view, const MDagPath &path,
                              M3dView:: DisplayStatus status );

  void drawWireframeClusterPath(
    const paintingGeom &geom, M3dView &view,
    const MDagPath &path, M3dView::DisplayStatus status );

  void  drawWireframeStops(
    const paintingGeom &geom, M3dView &view,
    const MDagPath &path,
    M3dView:: DisplayStatus status );

  void drawShaded(const paintingGeom &geom, M3dView &view, const MDagPath &path,
                  M3dView::DisplayStatus status ) ;


  MStatus overrideBrushIds(MDataBlock &data,  MFloatArray &uVals,
                           MFloatArray &vVals, std::vector<strokeGeom> &strokePool);
  MStatus  overridePaintIds(MDataBlock &data,  MFloatArray &uVals,
                            MFloatArray &vVals, std::vector<strokeGeom> &strokePool);

  MStatus filterStrokes(MDataBlock &data,  MFloatArray &uVals,
                        MFloatArray &vVals, std::vector<strokeGeom> &strokePool);
  MStatus sortStrokes(MDataBlock &data,  MFloatArray &uVals,
                      MFloatArray &vVals, std::vector<strokeGeom> &strokePool);
  // MStatus setData(MDataBlock &block, MObject &attribute,
  //                 const MMatrixArray &data) ;

  // MStatus getData( MObject &attribute,  MIntArray &array);

  // MStatus getData( MObject &attribute,  MDoubleArray &array);

  // MStatus getData( MObject &attribute,  MVectorArray &array);
  // MStatus getData( MArrayDataHandle &ha,  strokeCurveGeom *result);



  MStatus getTextureName(const MObject &attribute,
                         MString &name) const ;

  MStatus sampleUVTexture(const MObject &attribute,  MFloatArray &uVals,
                          MFloatArray &vVals, MIntArray &result, short range) const;

  MStatus sampleUVTexture(const MObject &attribute,  MFloatArray &uVals,
                          MFloatArray &vVals, MFloatVectorArray &result) const ;

  static MObject aStrokeCurves;


  static MObject aBrushIdTexture;
  static MObject aPaintIdTexture;

  // static MObject aStrokeSort;


  static MObject aStrokeSortKey;
  static MObject aStrokeSortDirection;
  static MObject aStrokeSortList;
  static MObject aStrokeSortTexture;

  static MObject aStrokeFilterKey;
  static MObject aStrokeFilterOperator;
  static MObject aStrokeFilterOperand;
  static MObject aStrokeFilterList;
  static MObject aStrokeFilterTexture;


  // static MObject aStrokeGate;


  // static MObject aStrokeSorts;


  static MObject aBrushIdTextureRange;
  static MObject aPaintIdTextureRange;


  static MObject aRotateOrder;
  static MObject aOutputUnit;



  static MObject aPlaneMatrix;

  // static MObject aDipApproachObject;
  // static MObject aToolChangeApproachObject;
  // static MObject aHomeApproachObject;

  static MObject aLinearSpeed; // cm/sec
  static MObject aAngularSpeed; // per sec
  static MObject aApproximationDistance; // cm
  static MObject aMaxPointToPointDistance; // cm


  static MObject  aBrushMatrix;
  static MObject  aBrushRetention;
  static MObject  aBrushWidth;
  static MObject  aBrushShape;
  static MObject  aBrushes;

  static MObject  aPaintColorR;
  static MObject  aPaintColorG;
  static MObject  aPaintColorB;
  static MObject  aPaintColor;
  static MObject  aPaintOpacity;
  static MObject  aPaintTravel;
  static MObject  aPaints;

  static MObject  aPointSize;
  static MObject  aLineLength;
  static MObject  aLineThickness;
  static MObject  aDisplayIds;
  static MObject  aDisplayTargets;
  static MObject  aDisplayLift;
  static MObject  aDisplayApproach;
  static MObject  aDisplayClusterPath;
  static MObject  aDisplayStops;

  static MObject  aStackGap;

  // static MObject  aOutTargets; // local

  static MObject aOutput;

  // output

  // for actual output

  // static MObject  aOutCounts;
  // static MObject  aOutBrushIds;
  // static MObject  aOutPaintIds;
  // static MObject  aOutCurveIds;

  // static MObject  aOutClusters;
  // static MObject  aOutTangents; // local

  // static MObject  aOutPosition; // world
  // static MObject  aOutRotation; // world


  // static MObject  aOutBrushWidths;
  // static MObject  aOutPaintColors;
  // static MObject  aOutPaintOpacities;
  // static MObject  aOutForceDips;

  // static MObject  aOutArcLengths;

  // static MObject  aOutApproachStarts;
  // static MObject  aOutApproachEnds;

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

  paintingData *m_pd;


};


namespace paintingCallback
{
static  MCallbackId id;

static void makeDefaultConnections(  MObject &node, void *clientData )
{

  MPlug wmPlugmulti( node, painting::worldMatrix );
  MPlug wm( wmPlugmulti.elementByLogicalIndex( 0 ) );
  MPlug mt( node, painting::aInMatrix );

  MDGModifier mod;
  mod.connect( wm, mt );
  MStatus stat = mod.doIt();
  if (stat != MS::kSuccess)
  {
    stat.perror("painting ERROR :: callback unable to make matrix connections");
  }
}
}


#endif




















