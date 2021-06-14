
#ifndef _paintingBaseNODE_H
#define _paintingBaseNODE_H

#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>


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
  static MObject aInMatrix;
  // static MObject aViewMatrix;
  static MObject aReassignParentId;
  static MObject aPointSize;
  static MObject aLineLength;
  static MObject aLineThickness;
  static MObject aDisplayTargets;

  static MObject aDisplayIds;
  static MObject aDisplayParentIds;
  static MObject aDisplayLayerIds;

  static MObject aWireColor; 

  static MObject aIdDisplayOffset;
  static MObject aArrowheadSize;
  static MObject aDrawParam;

protected:

  static MObject aStrokes;
  static MObject aLinearSpeed;             // cm/sec
  static MObject aAngularSpeed;            // per sec
  static MObject aApproximationDistance;   // cm

};

namespace paintingBaseCallback
{
  static MCallbackId id;

  static void makeDefaultConnections(MObject &node, void *clientData)
  {

    MPlug wmPlugmulti(node, paintingBase::worldMatrix);
    MPlug wm(wmPlugmulti.elementByLogicalIndex(0));
    MPlug mt(node, paintingBase::aInMatrix);

    MDGModifier mod;
    mod.connect(wm, mt);
    MStatus stat = mod.doIt();
    if (stat != MS::kSuccess)
    {
      stat.perror("painting ERROR :: callback unable to make matrix connections");
    }
  }
} // namespace paintingBaseCallback

#endif
