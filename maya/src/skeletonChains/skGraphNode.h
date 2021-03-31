
#ifndef _skGraphNode_H
#define _skGraphNode_H

#define cimg_display 0

#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
#include <maya/MVectorArray.h>

#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include "skGraph.h"

// using namespace cimg_library;

class skGraphNode : public MPxLocatorNode
{
public:
  skGraphNode();
  virtual ~skGraphNode();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  virtual bool isBounded() const;

  virtual MBoundingBox boundingBox() const;

  virtual void draw(
      M3dView &view,
      const MDagPath &path,
      M3dView::DisplayStyle style,
      M3dView::DisplayStatus status);

  MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;
  static MString drawDbClassification;
  static MString drawRegistrantId;

  static MObject aPointSize;
  static MObject aColor1;
  static MObject aColor2;

  static MObject aOutPoints;
  static MObject aOutRadius;
  static MObject aOutParams;
  static MObject aOutCounts;

  static MObject aRadiusMult;
  static MObject aDrawEdges;
  static MObject aDrawPoints;
  static MObject aDrawCircles;
  static MObject aRandomChainColor;

private:
  MStatus extract(
      MDataBlock &data, MVectorArray &points, MDoubleArray &params,
      MDoubleArray &radius, MIntArray &counts) const;

  static MObject aChains;
};

#endif
