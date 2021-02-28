
#ifndef _lightPaintingNODE_H
#define _lightPaintingNODE_H

#include <maya/MPxLocatorNode.h>
#include <maya/MFloatArray.h>
#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MAngle.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "lightPaintingData.h"

#include "brush.h"
#include "stroke.h"

class lightPainting : public MPxLocatorNode
{
public:
  lightPainting();

  virtual ~lightPainting();

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

  static MObject aInMatrix;

private:

MStatus  addStrokes( MDataBlock &data, std::vector<Stroke> *outStrokeGeom );

  static MObject aStrokes;

  static MObject aLinearSpeed;             // cm/sec
  static MObject aAngularSpeed;            // per sec
  static MObject aApproximationDistance;   // cm

  static MObject aBrush;

  lightPaintingData *m_pd;

  // Public because it's needed by lightPaintingtDrawOverride
public:
  static MObject aReassignParentId;
  static MObject aPointSize;
  static MObject aLineLength;
  static MObject aLineThickness;
  static MObject aDisplayTargets;

  static MObject aDisplayTargetColors;

  static MObject aDisplayIds;
  static MObject aDisplayParentIds;
  static MObject aDisplayLayerIds;

  static MObject aIdDisplayOffset;

  static MObject aArrowheadSize;

  static MObject aDrawParam;

  static MObject aOutput;
};

namespace lightPaintingCallback
{
  static MCallbackId id;

  static void makeDefaultConnections(MObject &node, void *clientData)
  {

    MPlug wmPlugmulti(node, lightPainting::worldMatrix);
    MPlug wm(wmPlugmulti.elementByLogicalIndex(0));
    MPlug mt(node, lightPainting::aInMatrix);

    MDGModifier mod;
    mod.connect(wm, mt);
    MStatus stat = mod.doIt();
    if (stat != MS::kSuccess)
    {
      stat.perror("lightPainting ERROR :: callback unable to make matrix connections");
    }
  }
} // namespace lightPaintingCallback

#endif
