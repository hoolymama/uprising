
#ifndef _projectionPoints_H
#define _projectionPoints_H
#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include <maya/MObject.h>
#include <maya/MPxNode.h>

#include "dot_data.h"
#include "CImg.h"

using namespace cimg_library;

class projectionPoints : public MPxLocatorNode
{
public:
  projectionPoints();
  virtual ~projectionPoints();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  virtual bool isBounded() const;

  virtual MBoundingBox boundingBox() const;

  virtual void draw(  M3dView &view,
                      const MDagPath &path,
                      M3dView::DisplayStyle style,
                      M3dView:: DisplayStatus status  );

  MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId   id;


  static  MObject aProjectionMatrix;
private:

  void makeDots(MDataBlock &data, std::vector<dotData> &dots);
  void relaxDots(MDataBlock &data, std::vector<dotData> &dots);

  // MStatus getImage(MDataBlock &data, MObject &attribute,
  //                  CImg<unsigned char> *image );

  CImg<unsigned char> *getImage(MDataBlock &data, MObject &attribute );

  static  MObject aDensityImage;
  static  MObject aRadiusImage;

  static  MObject aDensityRamp;
  static  MObject aDensityRangeMin;
  static  MObject aDensityRangeMax;
  static  MObject aDensityRange;

  static  MObject aRadiusRamp;
  static  MObject aRadiusRangeMin;
  static  MObject aRadiusRangeMax;
  static  MObject aRadiusRange;

  static  MObject aNeighbors;
  static  MObject aIterations;
  static  MObject aMagnitude;
  static  MObject aSeed;

  static  MObject aOutPoints;
  static  MObject aOutRadius;

  static  MObject aLineThickness;
  static  MObject aPointSize;
  static  MObject aCircleDisplaySize;

  static  MObject aColor;
  static  MObject aDisplayPoints;
  static  MObject aDisplayCircles;

};

namespace
{
static  MCallbackId id;


static void makeDefaultConnections(  MObject &node, void *clientData )
{

  MPlug wmPlugmulti( node, projectionPoints::worldMatrix );
  MPlug wm( wmPlugmulti.elementByLogicalIndex( 0 ) );
  MPlug pmt( node, projectionPoints::aProjectionMatrix );

  MDGModifier mod;
  mod.connect( wm, pmt );
  MStatus stat = mod.doIt();
  if (stat != MS::kSuccess)
  {
    stat.perror("painting ERROR :: callback unable to make matrix connections");
  }
}
}


#endif
