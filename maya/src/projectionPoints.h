
#ifndef _projectionPoints_H
#define _projectionPoints_H

#include <maya/MObject.h>
#include <maya/MPxNode.h>

#include "dot_data.h"


class projectionPoints : public MPxNode
{
public:
  projectionPoints();
  virtual ~projectionPoints();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool    isAbstractClass() const {return false;}

  MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId   id;

  enum Spac { kParametric, kArcLen };


protected:

  void makeDots(MDataBlock &data, std::vector<dotData> &dots);

  static  MObject aDensity;
  static  MObject aRadius;
  static  MObject aDensityMap;
  static  MObject aNeighbors;
  static  MObject aIterations;
  static  MObject aMagnitude;
  static  MObject aProjectionMatrix;
  static  MObject aSeed;
  static  MObject aOutPoints;

};

#endif
