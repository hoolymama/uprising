
#ifndef _diptych_H
#define _diptych_H

// #define cimg_display 0

#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
#include <maya/MPointArray.h>
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
  MMatrix calculatePaintingMatrix(MDataBlock &data, double xScale, double yScale) const;

  MStatus generateMeshAndProbes(
      MDataBlock &data,
      const MMatrix &boardMatrix,
      const MMatrix &paintingMatrix,
      MPointArray & outProbePoints,
      MObject &outGeom
     ) const;

  void createPatch(
      int numX,
      int numY,
      int offset,
      const MMatrix &matrix,
      const MDoubleArray &displacement,
      MPointArray &points,
      MIntArray &faceCounts,
      MIntArray &connectivity) const;

  void createBorder(
      int numX,
      int numY,
      int offset,
      const MMatrix &matrix,
      MPointArray &points,
      MIntArray &faceCounts,
      MIntArray &connectivity) const;

  void getDisplacement(
      MDataBlock &data,
      int numPoints,
      MDoubleArray &rightDisplacement,
      MDoubleArray &leftDisplacement) const;

  void appendProbePoints(
      int numX,
      int numY,
      const MMatrix &matrix,
      MPointArray &points) const;

      static MObject aBoardHeight;

  static MObject aGap;

  static MObject aApplyCrop;
  static MObject aCropCorner;
  static MObject aCropResolution;
  static MObject aImageResolution;

  static MObject aPinOffset;

  static MObject aPin0;
  static MObject aPinX;
  static MObject aPinY;
  static MObject aPinPainting;

  // FOR DISP
  static MObject aDisplacement;
  static MObject aOutMesh;

public:
  static MObject aOutSquareMatrix;
  static MObject aOutBoardMatrix;
  static MObject aOutPinMatrix;
  static MObject aOutPaintingMatrix;

  static MObject aOutProbePoints;

  static MObject aNumProbes;
  static MObject aProbeBorder;

  static MObject aBoardColor;
  static MObject aSquareColor;
  static MObject aPinColor;
  static MObject aProbeColor;

  static MObject aMirror;
  static MObject aPaintingLocatorSize;

  static MObject aDisplaySquareMatrix;
  static MObject aDisplayBoardMatrix;
  static MObject aDisplayPinMatrix;
  static MObject aDisplayPaintingMatrix;
  static MObject aDisplayProbes;
  static MObject aProbePointSize;
};

#endif
