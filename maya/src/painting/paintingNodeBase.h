
#ifndef _paintingBaseNODE_H
#define _paintingBaseNODE_H

#include <map>
#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
#include "brush.h"
#include "brushShop.h"
#include <maya/MFnMatrixData.h>

class paintingBase : public MPxLocatorNode
{
public:
  paintingBase();

  virtual ~paintingBase();
  virtual bool isAbstractClass() const { return true; }

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

  static MObject aReassignParentId;
  static MObject aPointSize;
  static MObject aLineLength;
  static MObject aLineThickness;
  static MObject aDisplayTargets;

  static MObject aDisplayIds;
  static MObject aDisplayParentIds;
  static MObject aDisplayBrushIds;
  static MObject aDisplayLayerIds;

  static MObject aWireColor; 

  static MObject aIdDisplayOffset;
  static MObject aArrowheadSize;
  static MObject aDrawParam;

protected:

  virtual void collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const;
  MStatus getBrushShop(MDataBlock &data, BrushShop &brushShop) const;

  static MObject aStrokes;
  static MObject aBrushes;
  static MObject aBrushShop;
  

  static MObject aLinearSpeed;             // cm/sec
  static MObject aAngularSpeed;            // per sec
  static MObject aApproximationDistance;   // cm

};
 
#endif
