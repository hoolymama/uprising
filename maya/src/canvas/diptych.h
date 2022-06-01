
#ifndef _diptych_H
#define _diptych_H

// #define cimg_display 0

#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include <maya/MObject.h>
#include <maya/MPxNode.h>

// #include "dotData.h"
// #include "CImg.h"

// using namespace cimg_library;

class diptych : public MPxLocatorNode
{
public:
  diptych();
  virtual ~diptych();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  virtual bool isBounded() const;

  virtual MBoundingBox boundingBox() const;

  MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;
  static MString drawDbClassification;
  static MString drawRegistrantId;

private:
  static MObject aBoardHeight;

  static MObject aGap;

  static MObject aApplyCrop;
  static MObject aCropCorner;
  static MObject aCropResolution;
  static MObject aImageResolution;

public:
  static MObject aOutBoardOffset;
  static MObject aOutBoardSize;

  static MObject aOutSquareOffset;
  static MObject aOutSquareSize;

  static MObject aOutSquareMatrix;
 


  // static MObject aOutCropFactor;
  // static MObject aOutCropOffsetFactor;
  static MObject aBoardColor;
  static MObject aSquareColor;
  static MObject aMirror;
};

#endif
