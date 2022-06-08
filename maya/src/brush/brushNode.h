
#ifndef _brushNODE_H
#define _brushNODE_H

#include <maya/MPxLocatorNode.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include <maya/MPxNode.h>

#include "brushData.h"

#include "brush.h"
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

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);
 
  static MTypeId id;
  static MString drawDbClassification;
  static MString drawRegistrantId;


  static MObject aWidth;
  static MObject aTip;
  static MObject aBristleHeight;
  static MObject aPaintingParam;
  static MObject aDipParam;
  static MObject aWipeParam;
  static MObject aShape;

private:
  MStatus getBrush(MObject &attribute, Brush &brush);

  MStatus outputData(MDataBlock &data, MObject &attribute, const Brush &brush);

  // static MObject aId;
  static MObject aPhysicalId;
  static MObject aRetention;
  static MObject aTransHeightParam;
  static MObject aContactPower;
  static MObject aForwardBias;
  static MObject aGravityBias;

  static MObject aInitialWait;
  static MObject aInitialWater;
  static MObject aInitialDips;
  static MObject aRetardant;

  static MObject aWipeBarPosition;

  static MObject aLineLength;
  static MObject aLineThickness;

  static MObject aDmx;

  static MObject aOutPaintBrush;
  static MObject aOutDipBrush;
  static MObject aOutWipeBrush;

  static MObject aModel;
};

#endif
