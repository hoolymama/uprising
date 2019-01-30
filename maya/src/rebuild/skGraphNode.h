
#ifndef _skGraphNode_H
#define _skGraphNode_H

#define cimg_display 0

#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include "skGraph.h"
#include "CImg.h"

using namespace cimg_library;

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

  virtual void draw(  M3dView &view,
                      const MDagPath &path,
                      M3dView::DisplayStyle style,
                      M3dView::DisplayStatus status  );

  MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId   id;


  // static  MObject aProjectionMatrix;
private:

  // void makeDots(MDataBlock &data, std::vector<dotData> &dots);
  // void relaxDots(MDataBlock &data, std::vector<dotData> &dots);
  // void cullDots(MDataBlock &data, std::vector<dotData> &dots);


  // CImg<float> *getImage(MDataBlock &data, MObject &attribute );

  // coord findWhitePixel(const coord &start,  const CImg<bool> &image );

  static MObject aProjectionMatrix;



  static  MObject aSkeletonImage;
  static  MObject aPruneLength;

  static  MObject aMaxRadius;
  static  MObject aPixelStep;
  static  MObject aMinPixels;

  static  MObject aPointSize;
  static  MObject aColor1;
  static  MObject aColor2;

  static  MObject aOutPoints;
  // static  MObject aOutEdges;
  static  MObject aOutRadius;
  static  MObject aOutParams;

  static  MObject aOutCounts;


  static  MObject aRadiusMult;
  static  MObject aDrawEdges;
  static  MObject aDrawPoints;
  static  MObject aDrawCircles;
  static  MObject aRandomChainColor;






};

// namespace
// {
// static  MCallbackId id;


// static void makeDefaultConnections(  MObject &node, void *clientData )
// {

//   MPlug wmPlugmulti( node, skGraphNode::worldMatrix );
//   MPlug wm( wmPlugmulti.elementByLogicalIndex( 0 ) );
//   MPlug pmt( node, skGraphNode::aProjectionMatrix );

//   MDGModifier mod;
//   mod.connect( wm, pmt );
//   MStatus stat = mod.doIt();
//   if (stat != MS::kSuccess)
//   {
//     stat.perror("painting ERROR :: callback unable to make matrix connections");
//   }
// }
// }


#endif
