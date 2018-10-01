
#ifndef _projectStroke_H
#define _projectStroke_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <strokeNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "stroke.h"






class projectStroke : public strokeNode
{
public:
  projectStroke();
  virtual ~projectStroke();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool    isAbstractClass() const {return false;}

  MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId   id;

  enum Spac { kParametric, kArcLen };


protected:

  // unsigned int  getStrokeBoundaries(
  //   MDataBlock &data,
  //   MVectorArray &result
  // ) const  ;

  // void setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
  //                  double approachStart, double approachMid, double approachEnd) const;


  // MStatus generateStrokeGeometry(MDataBlock &data,
  //                                std::vector<strokeGeom> *geom) const;



  static  MObject  aDensity;
  static  MObject  aDensityMap;
  static  MObject  aOutPoints;
  static  MObject aProjectionMatrix;
  // static  MObject  aSeed;

  // static MObject aForceDip;
  // static MObject aBrushRampScope;

};

#endif
