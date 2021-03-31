
#ifndef _collectStrokes_H
#define _collectStrokes_H

#include <maya/MVector.h>
 
#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
 
#include "stroke.h"

#include "strokeNodeBase.h"

class collectStrokes : public strokeNodeBase
{
public:
  collectStrokes();
  virtual ~collectStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void    postConstructor();

  virtual bool isAbstractClass() const { return false; }
 
  virtual MStatus generateStrokeGeometry(
      MDataBlock &data,
      std::vector<Stroke> *pStrokes) ;


  static  MTypeId id;

private:


  static MObject  aStrokes;

};



#endif