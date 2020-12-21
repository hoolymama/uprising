
#ifndef _aimStrokes_H
#define _aimStrokes_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "cImgData.h"
#include "stroke.h"

class aimStrokes : public MPxNode
{
public:
  aimStrokes();
  virtual ~aimStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;

private:
  MStatus copyStrokes(MDataBlock &data, std::vector<Stroke> *geom);

  void aim(  MDataBlock &data,  std::vector<Stroke> *geom) const;

  static MObject aPoint;
  static MObject aStrokes;
  static MObject aOutput;
};

#endif