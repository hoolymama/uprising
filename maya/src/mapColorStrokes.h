
#ifndef _mapColorStrokes_H
#define _mapColorStrokes_H

#include "strokeMutator.h"
// #include <maya/MPxNode.h>
#include "stroke.h"

class mapColorStrokes : public strokeMutator
{
public:
  mapColorStrokes();
  virtual ~mapColorStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(MDataBlock &data, std::vector<Stroke> *geom) const;
  static MTypeId id;

private:

 
void getColors(
    MDataBlock &data,
    std::vector<Stroke> *geom,
    MFloatPointArray &points,
    MFloatVectorArray &colors,
    MFloatArray &whites
     ) const;


MStatus occludeColors(
  MDataBlock &data,
  const MFloatPointArray &points,
  MFloatVectorArray &colors, 
  MFloatArray &whites
  ) const ;

void applyColors(   
    std::vector<Stroke> *geom,
    const MFloatVectorArray &colors,
    const MFloatArray &whites) const;
 
  static MObject aRGB;
  static MObject aWhite;

 
  static MObject aMesh;
  static MObject aPoint;
  static MObject aBias;
  static MObject aDoOcclusion;
  

 
};

#endif