
#ifndef _pearlNode_H
#define _pearlNode_H
#define cimg_display 0

#include <maya/MPxLocatorNode.h>
#include <maya/MFloatPointArray.h>

#include "pearlChainData.h"
#include "pearlChain.h"
#include "CImg.h"


using namespace cimg_library;

class pearlNode : public MPxLocatorNode
{
public:
  pearlNode();
  virtual ~pearlNode();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  virtual bool isBounded() const;

  virtual MBoundingBox boundingBox() const;

  virtual void draw(
    M3dView &view,
    const MDagPath &path,
    M3dView::DisplayStyle style,
    M3dView::DisplayStatus status);

  static MTypeId id;
  static MString drawDbClassification;
  static MString drawRegistrantId;



  static MObject aPointSize;
  static MObject aColor1;
  static MObject aColor2;

  static MObject aDrawEdges;
  static MObject aDrawPoints;
  static MObject aDrawCircles;
  static MObject aRandomChainColor;

  static MObject aOutput;



private:
  MStatus generate(
    MDataBlock &data, 
    std::vector<pearlChain> *geom, 
    CImg<unsigned char> *pInkImage) const;

  MFloatMatrix  imageTransform(int w, int h) const;

  static MObject aImage;

  static MObject aMaxIterations;

 
  static MObject aOffsetWidth;
  static MObject aMaxWidth;
  static MObject aMaxStampWidth;

  static MObject aMinBranchTwigLength;

  static MObject aMinLooseTwigLength;

  static MObject aLongestChain;

  static MObject aSpan;

  static MObject aOutputImage;




};


#endif