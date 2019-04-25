
#ifndef _skeletonStrokeNode_H
#define _skeletonStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeNode.h>
#include "stroke.h"
#include "skGraph.h"
#include "skChain.h"
#include "brush.h"


class skeletonStrokeNode : public strokeNode
{
public:
  skeletonStrokeNode();
  virtual ~skeletonStrokeNode();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool    isAbstractClass() const {return false;}

  static  MTypeId   id;

  enum Spac { kParametric, kArcLen };

  enum SubcurveMethod { kLength, kBookends };

  // enum BrushFilter {kRound, kFlat,  kAll};

private:



  MStatus collectBrushes(
    MDataBlock &data,
    std::vector< std::pair<int, Brush> > &brushes,
    Brush::Shape filter = Brush::kRound) const;

  // int  getContacts(
  //   const skChain &chain,
  //   const std::vector< std::pair<int, Brush> > &brushes,
  //   MDoubleArray &contacts) const;


  void getContacts(
    const skChain &chain,
    const std::pair<int, Brush> &indexedBrush,
    MDoubleArray &contacts) const;

  const std::pair<int, Brush>  selectBrush(
    const skChain &chain,
    const std::vector< std::pair<int, Brush> > &brushes) const;


  unsigned int getStrokeBoundaries(
    const MObject   &dCurve,
    float strokeLength,
    float overlap,
    MVectorArray &result
  ) const  ;

  // CImg<float> *getImage(MDataBlock &data, MObject &attribute )const;

  void setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                   double approachStart, double approachMid, double approachEnd) const;

  MStatus generateStrokeGeometry(MDataBlock &data,
                                 std::vector<Stroke> *geom) const;

  std::vector< std::pair<int, float> > getBrushRadii(
    MDataBlock &data) const;

  // static MObject aSkeletonImage;

  // static MObject aImage;
  // static MObject aThreshold;
  // static MObject aMedian;
  // static MObject aInvert;
  // static MObject aMaxIterations;


  // static MObject aMinBranchTwigLength;
  // static MObject aMinLooseTwigLength;

  // static MObject aSpanPixels;
  // static MObject aMaxWidthPixels;

  // static MObject aProjectionMatrix;

  static MObject aChains;

  static MObject aBrushFilter;
  static MObject aBrushes;

  static MObject aStrokeLength;
  static MObject aOverlap;
  static MObject aBrushRampScope;

};

#endif
