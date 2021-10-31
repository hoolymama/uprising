
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
  MStatus generate(
    MDataBlock &data, 
    std::vector<skChain> *geom, 
    CImg<unsigned char> *pInkImage) const;



int  getFields(MObjectArray &result) const;

  MStatus generateSeedChains(
    MDataBlock &data, 
    std::vector<skChain> *geom, 
    CImg<unsigned char> *pInkImage) const;

  MStatus generateFillerChains(
    MDataBlock &data, 
    std::vector<skChain> *geom, 
    CImg<unsigned char> *pInkImage) const;


  void propagateAndStamp(
    const MVector &seedPoint,
    const MObjectArray &fields,
    const MFloatMatrix &transformation,
    const MFloatMatrix &flowTransformation,
    const MFloatMatrix &flowTransformationInverse,
    float circleRadius,
    float circleStampRadius,
    float span,
    float cmToPixels,
    int maxSteps,
    float forceThreshold,
    float angle,
    CImg<unsigned char> *pInkImage,
    CImg<float> *pFlowImage,
    skChain &chain) const;

  float  getCmToPixels(MDataBlock &data, const CImg<unsigned char> *pImage) const;
  MFloatMatrix  getTransformProjToImage(MDataBlock &data , int w, int h) const;

  static MObject aImage;

  static MObject aSeedPoints;
  static MObject aFields;
  static MObject aFlowImage;
  static MObject aSeedChainMaxSteps;
  static MObject aSeedChainSpan;
  static MObject aSeedChainStampWidth;
  static MObject aSeedChainWidth;
 
  static MObject aForceThreshold;
  static MObject aFlowRotation;

  static MObject aDoSeedChains;
  static MObject aDoFillerChains;


  static MObject aMaxIterations;

  // static MObject aRadiusMult;
  static MObject aOffsetWidth;
  static MObject aMaxWidth;
  static MObject aMaxStampWidth;

  static MObject aMinBranchTwigLength;
  static MObject aMinLooseTwigLength;

  static MObject aSpan;

  static MObject aProjectionMatrix;

 
  static MObject aTrigger;
  static MObject aMaxChainsPerOutput;
  static MObject aOutputCount;
  static MObject aOutputImage;
  static MObject aOutputs;
};


#endif