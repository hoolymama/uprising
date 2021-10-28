#include <maya/MIOStream.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include "strokeData.h"
#include "paintingNodeBase.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"


MTypeId paintingBase::id(k_paintingBase);

paintingBase::paintingBase(){}

paintingBase::~paintingBase(){}

void *paintingBase::creator()
{
  return new paintingBase();
}


MObject paintingBase::aStrokes;
MObject paintingBase::aLinearSpeed;           // cm/sec
MObject paintingBase::aAngularSpeed;          // per sec
MObject paintingBase::aApproximationDistance; // cm

MObject paintingBase::aReassignParentId;

MObject paintingBase::aPointSize;
MObject paintingBase::aLineLength;
MObject paintingBase::aLineThickness;
MObject paintingBase::aDisplayTargets;
MObject paintingBase::aWireColor; 

MObject paintingBase::aDisplayIds;
MObject paintingBase::aDisplayParentIds;
MObject paintingBase::aDisplayLayerIds;
MObject paintingBase::aIdDisplayOffset;
MObject paintingBase::aArrowheadSize;
MObject paintingBase::aDrawParam;
 
MStatus paintingBase::initialize()
{
  MStatus st;
  MString method("paintingBase::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
 
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;
 
  MMatrix identity;
  identity.setToIdentity();

  aLinearSpeed = nAttr.create("linearSpeed", "lnsp", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(200.0f);
  nAttr.setDefault(100.00f);
  nAttr.setKeyable(true);
  addAttribute(aLinearSpeed);

  const double minAngSpeed = mayaMath::single_pi / 900.00;
  const double defaultAngSpeed = mayaMath::single_pi / 9.00;
  aAngularSpeed = uAttr.create("angularSpeed", "agsp", MFnUnitAttribute::kAngle);
  uAttr.setStorable(true);
  uAttr.setReadable(true);
  uAttr.setMin(minAngSpeed);
  uAttr.setMax((mayaMath::single_pi));
  addAttribute(aAngularSpeed);

  aApproximationDistance = nAttr.create("approximationDistance", "apxd",
                                        MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.0f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  nAttr.setKeyable(true);
  addAttribute(aApproximationDistance);

  aStrokes = tAttr.create("strokes", "stks", strokeData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setIndexMatters(true);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aStrokes);
  aReassignParentId = nAttr.create("reassignParentId", "rpi",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(false);
  addAttribute(aReassignParentId);

  aPointSize = nAttr.create("pointSize", "psi", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  addAttribute(aPointSize);

  aLineLength = nAttr.create("lineLength", "lln", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  addAttribute(aLineLength);

  aLineThickness = nAttr.create("lineThickness", "ltk", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  addAttribute(aLineThickness);

  aArrowheadSize = nAttr.create("arrowheadSize", "arsz", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  addAttribute(aArrowheadSize);

  aDisplayTargets = eAttr.create("displayTargets", "dtg");
  eAttr.addField("none", PaintingEnums::kTargetsNone);
  eAttr.addField("point", PaintingEnums::kTargetsPoint);
  eAttr.addField("line", PaintingEnums::kTargetsLine);
  eAttr.addField("matrix", PaintingEnums::kTargetsMatrix);
  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setDefault(PaintingEnums::kTargetsNone);
  addAttribute(aDisplayTargets);

  aDisplayIds = nAttr.create("displayIds", "did",
                             MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayIds);

  aDisplayParentIds = nAttr.create("displayParentIds", "dprid",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayParentIds);

  aDisplayLayerIds = nAttr.create("displayLayerIds", "dlyid",
                                  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayLayerIds);

  aIdDisplayOffset = nAttr.create("idDisplayOffset", "iddo", MFnNumericData::k3Float);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.0f, 0.0f, 1.0f);
  addAttribute(aIdDisplayOffset);

  aDrawParam = nAttr.create("drawParam", "dprm", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.0f);
  nAttr.setSoftMax(1.0f);
  nAttr.setDefault(1.0f);
  addAttribute(aDrawParam);

	aWireColor = nAttr.createColor("wireframeColor", "wfcol");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aWireColor);
	mser;

  return (MS::kSuccess);
}

MStatus paintingBase::compute(const MPlug &plug, MDataBlock &data)
{
  return MS::kUnknownParameter;
}

void paintingBase::draw(M3dView &view,
                    const MDagPath &path,
                    M3dView::DisplayStyle style,
                    M3dView::DisplayStatus status)
{
  return;
}

bool paintingBase::isBounded() const
{
  return false;
}

MBoundingBox paintingBase::boundingBox() const
{
  return MBoundingBox();
}

void paintingBase::postConstructor(){}
