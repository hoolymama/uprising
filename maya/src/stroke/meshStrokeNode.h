
#ifndef _meshStrokeNode_H
#define _meshStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeNodeBase.h>
#include "stroke.h"

class meshStrokeNode : public strokeNodeBase
{
public:
  meshStrokeNode();
  virtual ~meshStrokeNode();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }

  static MTypeId id;

protected:
  MStatus generateStrokeGeometry(
      MDataBlock &data,
      std::vector<Stroke> *pStrokes) const;

  static MObject aMesh;
  static MObject aViewpoint;
  static MObject aWorldMatrix;
};

#endif
