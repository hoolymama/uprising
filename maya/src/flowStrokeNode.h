
#ifndef _flowStroke_H
#define _flowStroke_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <strokeNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
#include "cImgData.h"
#include "stroke.h"






class flowStroke : public strokeNode
{
public:
  flowStroke();
  virtual ~flowStroke();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool    isAbstractClass() const {return false;}

  // MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId   id;



protected:

  MStatus generateStrokeGeometry(MDataBlock &data,
                                 std::vector<strokeGeom> *geom) const;


  MStatus getImageChannel(MDataBlock &data,
                          cImgData::Channel channel, CImg<unsigned char> &result) const;

  void setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                   double approachDist) const;

  // MFloatMatrix  getFullProjection(float angle,
  //                                 const MFloatMatrix &projection);


  // int  calcFlowPoints(const MFloatPoint &point, const CImgList<float> &grad,
  //                     const MFloatMatrix &mat, const MFloatMatrix &imat, int count, float gap,
  //                     MFloatPointArray &resultPoints, cImgData::Interpolation interp = cImgData::kBilinear);


  static  MObject aInputImage;
  static  MObject aInterpolation;
  static  MObject aSamplePoints;
  static  MObject aProjectionMatrix;
  static  MObject aChannel;
  static  MObject aBlur;
  static  MObject aRotation;
  static  MObject aSampleDistance;


  static  MObject aBrushRampScope;
  static  MObject aApproachDistance;
};

#endif
