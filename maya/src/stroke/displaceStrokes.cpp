
#include <maya/MIOStream.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MMeshIntersector.h>

#include "displaceStrokes.h"
#include <jMayaIds.h>
#include "errorMacros.h"
 
MObject displaceStrokes::aCanvasMatrix;
MObject displaceStrokes::aMesh;

MTypeId displaceStrokes::id(k_displaceStrokes);

displaceStrokes::displaceStrokes() {}

displaceStrokes::~displaceStrokes() {}

void *displaceStrokes::creator()
{
  return new displaceStrokes();
}

/// Post constructor
void displaceStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

 

MStatus displaceStrokes::initialize()
{
  MStatus st;
  MString method("displaceStrokes::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnTypedAttribute tAttr;
  MFnMatrixAttribute mAttr;

  aMesh = tAttr.create("mesh", "msh", MFnData::kMesh);
  tAttr.setReadable(false);
  tAttr.setHidden(false);
   tAttr.setKeyable(true);
 
  addAttribute(aMesh);

  MFloatMatrix identity;
  identity.setToIdentity();
  aCanvasMatrix = mAttr.create("canvasMatrix", "cmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  mAttr.setDefault(identity);
  addAttribute(aCanvasMatrix);

  st = attributeAffects(aMesh, aOutput);
  st = attributeAffects(aCanvasMatrix, aOutput);

  return (MS::kSuccess);
}

MStatus displaceStrokes::mutate(
  const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const
{
  MStatus st;
  MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();
  
  MFloatVector normal = (MFloatVector::zAxis * canvasMatrix).normal();
  MFloatMatrix canvasMatrixInv = canvasMatrix.inverse();

  MObject dMesh = data.inputValue(aMesh).asMeshTransformed();
  MFnMesh meshFn(dMesh, &st); msert;
  
  MMeshIsectAccelParams ap = meshFn.autoUniformGridParams();

  std::vector<Stroke>::iterator current_stroke = strokes->begin();
  for (unsigned i = 0; current_stroke != strokes->end(); current_stroke++)
  {
    Stroke::target_iterator current_target = current_stroke->targets_begin();
    for (; current_target != current_stroke->targets_end(); current_target++)
    {
      MFloatPoint rayOrigin =  current_target->position() * canvasMatrixInv;
      rayOrigin[2]=0.0f;
      rayOrigin *= canvasMatrix;
      MFloatPoint hitPoint;

	    bool hit = meshFn.closestIntersection(
			 rayOrigin, normal, 0, 0, false,
			 MSpace::kWorld,
			 100.0f, true, &ap, hitPoint, 0, 0, 0, 0, 0);
      if (hit)
      {
        current_target->offsetBy(hitPoint-rayOrigin);
      }
    }
  }
  return MS::kSuccess;
}