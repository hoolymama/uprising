
#ifndef _skTest_H
#define _skTest_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "cImgData.h"
#include "skChainData.h"
#include "skGraph.h"

// #include "stroke.h"


class skTest : public MPxNode
{
public:
  skTest();
  virtual ~skTest();

  static void *creator();
  static MStatus initialize();
  virtual void    postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId id;

private:


  MStatus  generate(MDataBlock &data, std::vector<skChain> *geom);

  static MObject aImage;
  static MObject aMinGate;
  static MObject aMaxGate;
  static MObject aMedian;
  // static MObject aInvert;
  static MObject aMaxIterations;


  static MObject aMult;
  static MObject aOffset;


  static MObject aMinBranchTwigLength;
  static MObject aMinLooseTwigLength;

  static MObject aSpanPixels;
  static MObject aMaxWidthPixels;

  static MObject aProjectionMatrix;

  static MObject  aOutput;
};



#endif