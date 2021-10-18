
#ifndef _colorSpreadNode_H
#define _colorSpreadNode_H

#include <maya/MPxNode.h>

class colorSpreadNode : public MPxNode
{
public:
  colorSpreadNode();
  virtual ~colorSpreadNode();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;

private:
  int getConnectionIds(MIntArray &result) const;

  static MObject aColor;

  static MObject aHueRange;
  static MObject aHuePropagation;

  static MObject aSaturationRange;
  static MObject aSaturationPropagation;

  static MObject aValueRange;
  static MObject aValuePropagation;

  static MObject aSeed;

  static MObject aOutColor;

  enum Propagation
  {
    kSpread,
    kForwardLinear,
    kForwardRandom
  };
};

#endif