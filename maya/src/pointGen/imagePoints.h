
#ifndef _projectionPoints_H
#define _projectionPoints_H

#define cimg_display 0

#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include <maya/MObject.h>
#include <maya/MPxNode.h>

#include "dotData.h"
#include "CImg.h"

using namespace cimg_library;

class imagePoints : public MPxLocatorNode
{
public:
  imagePoints();
  virtual ~imagePoints();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  virtual bool isBounded() const;

  virtual MBoundingBox boundingBox() const;

  virtual void draw(M3dView &view,
                    const MDagPath &path,
                    M3dView::DisplayStyle style,
                    M3dView::DisplayStatus status);

  MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;
  static MString drawDbClassification;
  static MString drawRegistrantId;

private:
  void makeDots(MDataBlock &data, std::vector<dotData> &dots);
  void relaxDots(MDataBlock &data, std::vector<dotData> &dots);
  void cullDots(MDataBlock &data, std::vector<dotData> &dots);
  void sortDots(MDataBlock &data, std::vector<dotData> &dots, const MMatrix &worldMatrix);

  CImg<unsigned char> *getImage(MDataBlock &data, MObject &attribute);

  static MObject aDensityImage;
  static MObject aRadiusImage;
  static MObject aMask;

  static MObject aDensityRamp;
  static MObject aDensityRangeMin;
  static MObject aDensityRangeMax;
  static MObject aDensityRange;

  static MObject aRadiusRamp;
  static MObject aRadiusRangeMin;
  static MObject aRadiusRangeMax;
  static MObject aRadiusRange;

  static MObject aNeighbors;
  static MObject aIterations;
  static MObject aMagnitude;
  static MObject aSeed;


  static MObject aSortVector;



  static MObject aOutU;
  static MObject aOutV;
  static MObject aOutPointsWorld;



public:

  static MObject aOutPoints;

  static MObject aOutRadius;
  static MObject aLineThickness;
  static MObject aPointSize;
  static MObject aCircleDisplaySize;
  static MObject aColor;
  static MObject aDisplayPoints;
  static MObject aDisplayCircles;
  static MObject aDisplayOrder;
};


#endif
