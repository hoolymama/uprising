
#ifndef _brushNODE_H
#define _brushNODE_H


#include <maya/MPxLocatorNode.h>
// #include <maya/MFloatArray.h>
// #include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

// #include <maya/MFnNurbsCurve.h>
// #include <maya/MAngle.h>
// #include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

// #include <maya/MDGMessage.h>
// #include <maya/MDGModifier.h>

#include "brushData.h"

#include "brush.h"
// #include "paint.h"
// #include "stroke.h"
// #include <openGL/glu.h>

class brushNode : public MPxLocatorNode
{
public:
  brushNode();

  virtual ~brushNode();

  static void *creator();

  static MStatus initialize();

  virtual void postConstructor();

  virtual bool isBounded() const;

  virtual MBoundingBox boundingBox() const;

  virtual void draw(  M3dView &view,
                      const MDagPath &path,
                      M3dView::DisplayStyle style,
                      M3dView:: DisplayStatus status  );

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);


  static  MTypeId   id;

private:




  MStatus getBrush(MObject &attribute, Brush &brush);

  MStatus outputData(MDataBlock &data, MObject &attribute, const Brush &brush );

  // static MObject aId;
  static MObject aPhysicalId;
  static MObject aWidth;
  static MObject aTip;
  static MObject aBristleHeight;
  static MObject aPaintingParam;
  static MObject aDipParam;
  static MObject aWipeParam;
  static MObject aRetention;
  static MObject aShape;
  static MObject aTransHeightParam;

  static MObject aLineLength;
  static MObject aLineThickness;


  static MObject aOutPaintBrush;
  static MObject aOutDipBrush;
  static MObject aOutWipeBrush;

  static MObject aCustomId;

};


#endif




















