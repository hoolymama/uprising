
#ifndef _particleStrokeNode_H
#define _particleStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeCreator.h>
#include "stroke.h"
 
class particleStrokeNode : public strokeCreator
{

public:
  particleStrokeNode();

  virtual ~particleStrokeNode();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  static MTypeId id;

private:
  MStatus generateStrokeGeometry(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

  static MObject aViewpoint;
  static MObject aTargetRotationMatrix;
  static MObject aTrails;
};

#endif
