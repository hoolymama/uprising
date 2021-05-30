
#ifndef _skChainNode_H
#define _skChainNode_H

#include <maya/MPxNode.h>

#include "skChainData.h"
#include "skChain.h"

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


  static MObject aSeedPoints;
  static MObject aFields;


  static MObject aMaxChainsPerOutput;
  static MObject aOutputCount;

  static MObject aOutputs;
};

#endif