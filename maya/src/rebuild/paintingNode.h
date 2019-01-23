
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
#include "stroke.h"


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

  enum StrokeSortFilterKey {
    kId,
    kParentId,
    kBrushId,
    kPaintId,
    kRepeatId,
    kMapRed,
    kMapGreen,
    kMapBlue,
    kLayerId,
    kCustomBrushId,
    kCustomPaintId
  };


  enum StrokeSortDirection { kSortAscending,  kSortDescending};

  static MObject aInMatrix;

private:

  MStatus collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes);

  MStatus addStrokes(MDataBlock &data, paintingGeom *pGeom);

  void setWireDrawColor(M3dView &view,  M3dView::DisplayStatus status);

  void drawWireframeTargets(const paintingGeom &geom, M3dView &view,
                            const MDagPath &path, M3dView::DisplayStatus status );

  void drawWireframeBorders( const paintingGeom &geom, M3dView &view,
                             const MDagPath &path, M3dView::DisplayStatus status );

  void drawWireframe(const paintingGeom &geom, M3dView &view, const MDagPath &path,
                     M3dView::DisplayStatus status ) ;

  void drawWireframeArrows(
    const paintingGeom &geom, M3dView &view,
    const MDagPath &path,
    M3dView:: DisplayStatus status );

  void drawWireframeClusterPath(
    const paintingGeom &geom, M3dView &view,
    const MDagPath &path, M3dView::DisplayStatus status );





  void  drawWireframePivots(
    const paintingGeom &geom, M3dView &view,
    const MDagPath &path,
    M3dView:: DisplayStatus status );

  void drawIds(const paintingGeom &geom, M3dView &view,
               const MDagPath &path,
               M3dView:: DisplayStatus status );

  void drawShaded(const paintingGeom &geom, M3dView &view, const MDagPath &path,
                  M3dView::DisplayStatus status ) ;

  static MObject aStrokes;



  static MObject aApproachDistanceStart;
  static MObject aApproachDistanceMid;
  static MObject aApproachDistanceEnd;
  static MObject aApproachDistance;


  static MObject aRotateOrder;
  static MObject aOutputUnit;



  static MObject aPlaneMatrix;
  static MObject aDisplacementMesh; // cm


  static MObject aLinearSpeed; // cm/sec
  static MObject aAngularSpeed; // per sec
  static MObject aApproximationDistance; // cm
  static MObject aMaxPointToPointDistance; // cm


  // static MObject  aBrushMatrix;
  // static MObject  aBrushRetention;
  // static MObject  aBrushWidth;
  // static MObject  aBrushShape;
  // static MObject  aBrushTip;
  // static MObject  aBrushPhysicalId;
  // static MObject  aBrushCustomId;
  // static MObject  aBrushTransitionHeight;
  // static MObject  aBrushTransitionPower;

  static MObject  aBrushes;

  static MObject  aPaintColorR;
  static MObject  aPaintColorG;
  static MObject  aPaintColorB;
  static MObject  aPaintColor;
  static MObject  aPaintOpacity;
  static MObject  aPaintTravel;
  static MObject  aPaintCustomId;
  static MObject  aPaints;



  static MObject  aPointSize;
  static MObject  aLineLength;
  static MObject  aLineThickness;
  static MObject  aDisplayTargets;
  // static MObject  aDisplayLift;
  // static MObject  aDisplayApproach;
  static MObject  aDisplayClusterPath;
  static MObject  aDisplayPivots;

  static MObject  aDisplayIds;
  static MObject  aDisplayParentIds;
  static MObject  aDisplayLayerIds;
  static MObject  aDisplayBrushIds;
  static MObject  aDisplayPaintIds;
  static MObject  aDisplayRepeatIds;

  static MObject  aIdDisplayOffset;

  static MObject  aArrowheadSize;

  static MObject  aStackGap;

  static MObject aOutput;


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




















