
#ifndef _skeletonStrokeNode_H
#define _skeletonStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeNode.h>
#include "stroke.h"
#include "skGraph.h"

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
  unsigned int getStrokeBoundaries(
    const MObject  &dCurve,
    float strokeLength,
    float overlap,
    MVectorArray &result
  ) const  ;

  CImg<float> *getImage(MDataBlock &data, MObject &attribute )const;

  void setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                   double approachStart, double approachMid, double approachEnd) const;

  MStatus generateStrokeGeometry(MDataBlock &data,
                                 std::vector<Stroke> *geom) const;

  std::vector< std::pair<int, float> > getBrushRadii(
    MDataBlock &data) const;

  static MObject aSkeletonImage;

  static MObject aBrushFilter;/**/
  static MObject aMinPixels;
  static MObject aSpanPixels;
  static MObject aProjectionMatrix;

  static MObject aBrushes;
  static MObject aBrushWidth;
  static MObject aBrushActive;

  static MObject aStrokeLength;
  static MObject aOverlap;
  static MObject aBrushRampScope;

};

#endif
