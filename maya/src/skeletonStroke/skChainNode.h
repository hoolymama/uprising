
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
  MStatus generate(MDataBlock &data, std::vector<skChain> *geom, CImg<unsigned char> *pOutImage) const;

  MStatus generateSeedChains(
    MDataBlock &data, 
    std::vector<skChain> *geom, 
    CImg<unsigned char> *pOutImage) const;

  MStatus generateFillerChains(
    MDataBlock &data, 
    std::vector<skChain> *geom, 
    CImg<unsigned char> *pOutImage) const;


  void propagateAndStamp(
      const MVector &seedPoint,
      const MObjectArray &fields,
      const MFloatMatrix &transformation,
      float circleRadius,
      float span,
      float cmToPixels,
      int maxSteps,
      CImg<unsigned char> *pOutImage,
      skChain &chain) const;



  void  stampAndChain(
    const MVector& point,
    const MFloatVector & coord,
    float radiusCm,
    float radiusPixels,
    CImg<unsigned char> *pOutImage,
    skChain &chain
  ) const;

  float  getCmToPixels(MDataBlock &data, const CImg<unsigned char> *pImage) const;


  static MObject aImage;

  static MObject aSeedPoints;
  static MObject aFields;
  static MObject aSeedChainMaxSteps;
  static MObject aSeedChainSpan;
  static MObject aSeedChainStampWidth;
 
  static MObject aDoSeedChains;
  static MObject aDoFillerChains;


  static MObject aMaxIterations;

  static MObject aRadiusMult;
  static MObject aRadiusOffset;
  static MObject aMaxWidth;
  static MObject aMaxStampWidth;

  static MObject aMinBranchTwigLength;
  static MObject aMinLooseTwigLength;

  static MObject aSpan;

  static MObject aProjectionMatrix;

 

  static MObject aMaxChainsPerOutput;
  static MObject aOutputCount;
  static MObject aOutputImage;

  static MObject aOutputs;
};

inline void skChainNode::stampAndChain(
  const MVector& point,
  const MFloatVector & coord,
  float radiusCm,
  float radiusPixels,
  CImg<unsigned char> *pOutImage,
  skChain &chain
) const {
    unsigned char color[] = {0};
    
    int w = pOutImage->width();
    int h = pOutImage->height();
    
    if ( int(coord.x  + radiusPixels)  < 0) {
      return;
    }
    if ( int(coord.y  + radiusPixels)  < 0) {
      return;
    }
    if ( int(coord.x  - radiusPixels) > w) {
      return;
    }
    if ( int(coord.y  - radiusPixels) > h) {
      return;
    }


    chain.add(skPoint(point.x, point.y, radiusCm));

    pOutImage->draw_circle(
    coord.x, coord.y, 
    radiusPixels, 
    color
  );

}



#endif