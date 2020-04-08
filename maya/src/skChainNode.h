
#ifndef _skChainNode_H
#define _skChainNode_H

// #include <maya/MVector.h>
// #include <maya/MFnDependencyNode.h>

// #include <maya/MFnNurbsCurve.h>
// #include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

// #include <maya/MDGMessage.h>
// #include <maya/MDGModifier.h>

// #include "cImgData.h"
#include "skChainData.h"
#include "skChain.h"

// #include "skGraph.h"

class skChainNode : public MPxNode
{
public:
  skChainNode();
  virtual ~skChainNode();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;

private:
  MStatus generate(MDataBlock &data, std::vector<skChain> *geom);

  static MObject aImage;
  static MObject aMaxIterations;

  static MObject aRadiusMult;
  static MObject aRadiusOffset;
  static MObject aMaxWidth;
  static MObject aMaxStampWidth;

  static MObject aMinBranchTwigLength;
  static MObject aMinLooseTwigLength;

  static MObject aSpan;

  static MObject aProjectionMatrix;

  static MObject aOutput;

  static MObject aMaxChainsPerOutput;
  static MObject aOutputCount;

  static MObject aOutputs;
};

#endif