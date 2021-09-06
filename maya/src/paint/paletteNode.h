
#ifndef _paletteNODE_H
#define _paletteNODE_H

#include <maya/MPxLocatorNode.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include <maya/MPxNode.h>

#include "paletteData.h"

class paletteNode : public MPxLocatorNode
{
public:
  paletteNode();

  virtual ~paletteNode();

  static void *creator();

  static MStatus initialize();

  virtual void postConstructor();

  virtual bool isBounded() const;

  virtual MBoundingBox boundingBox() const;

  virtual void draw(M3dView &view,
                    const MDagPath &path,
                    M3dView::DisplayStyle style,
                    M3dView::DisplayStatus status);

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;
  static MString drawDbClassification;
  static MString drawRegistrantId;
 

  static MObject aWidth;
  static MObject aHeight;
  static MObject aXPos;
  static MObject aYPos;
    
  static MObject aOutput;
private:

 
  static MObject aColor;
  static MObject aPot;
  static MObject aOpacity;
  static MObject aTravel;
  static MObject aInput;



};

#endif
