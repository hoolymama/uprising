
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
      float circleStampRadius,
      float span,
      float cmToPixels,
      int maxSteps,
      CImg<unsigned char> *pOutImage,
      skChain &chain) const;



  // bool stampAndChain(
  //   const MVector& point,
  //   const MFloatVector & coord,
  //   float radiusCm,
  //   float radiusPixels,
  //   CImg<unsigned char> *pOutImage,
  //   skChain &chain
  // ) const;

  float  getCmToPixels(MDataBlock &data, const CImg<unsigned char> *pImage) const;
  MFloatMatrix  getTransformation(MDataBlock &data , int w, int h) const;

  static MObject aImage;

  static MObject aSeedPoints;
  static MObject aFields;
  static MObject aSeedChainMaxSteps;
  static MObject aSeedChainSpan;
  static MObject aSeedChainStampWidth;
  static MObject aSeedChainWidth;
 
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

// inline bool skChainNode::stampAndChain(
//   const MVector& point,
//   const MFloatVector & coord,
//   float radiusCm,
//   float stampRadiusPixels,
//   CImg<unsigned char> *pOutImage,
//   skChain &chain
// ) const {
//     unsigned char color[] = {0};
    
//     int w = pOutImage->width();
//     int h = pOutImage->height();
    
//     if ( int(coord.x  + stampRadiusPixels)  < 0) {
//       return false;
//     }
//     if ( int(coord.y  + stampRadiusPixels)  < 0) {
//       return false;
//     }
//     if ( int(coord.x  - stampRadiusPixels) > w) {
//       return false;
//     }
//     if ( int(coord.y  - stampRadiusPixels) > h) {
//       return false;
//     }

//     bool onWhite = true;
//     if ( 
//       coord.x < 0 || 
//       coord.x >= w || 
//       coord.y  <  0 || 
//       coord.y >= h ||
//       (*pOutImage)(coord.x,coord.y) == 0
//     )
//     {
//       onWhite  = false;
//     }

//     chain.add(skPoint(point.x, point.y, radiusCm));

//     pOutImage->draw_circle(
//       coord.x, coord.y, 
//       stampRadiusPixels, 
//       color
//     );
//     return onWhite;

// }



#endif