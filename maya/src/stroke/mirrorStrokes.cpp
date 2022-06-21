
#include <maya/MIOStream.h>
#include <maya/MFnNumericAttribute.h>
#include "mirrorStrokes.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "texUtils.h"

// const double rad_to_deg = (180 / 3.1415927);
// const double deg_to_rad = (3.1415927 / 180);

// MObject mirrorStrokes::aStrokes;
// MObject mirrorStrokes::aPoint;
// MObject mirrorStrokes::aOutput;

MTypeId mirrorStrokes::id(k_mirrorStrokes);

mirrorStrokes::mirrorStrokes() {}

mirrorStrokes::~mirrorStrokes() {}

void *mirrorStrokes::creator()
{
  return new mirrorStrokes();
}

/// Post constructor
void mirrorStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus mirrorStrokes::initialize()
{
  MStatus st;
  MString method("mirrorStrokes::initialize");

	inheritAttributesFrom("strokeMutator");

  return (MS::kSuccess);
}


MStatus mirrorStrokes::mutate(
  const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const 
{

  MFloatMatrix globalMirror;
  globalMirror.setToIdentity();
  globalMirror[0][0] = -1;

  MFloatMatrix localMirror;
  localMirror.setToIdentity();
  localMirror[0][0] = -1;


  MFloatMatrix centerMat;
  unsigned j=0;
  std::vector<Stroke>::iterator siter = strokes->begin();
  for (unsigned i = 0; siter != strokes->end(); siter++)
  {
 
    Stroke::target_iterator titer = siter->targets_begin();
    for (; titer != siter->targets_end(); titer++)
    {
      const MFloatMatrix & mat =  titer->matrix();

      MFloatMatrix result  = localMirror * mat * globalMirror;
      titer->setMatrix(result);
    }

    const MFloatMatrix & pivotMat =  siter->pivot().matrix();
    MFloatMatrix pivotResult  = localMirror * pivotMat * globalMirror;
    siter->setPivotMatrix(pivotResult);

    siter->resetTangents();
  }
  return MS::kSuccess;
}