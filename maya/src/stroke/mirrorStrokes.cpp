
#include <maya/MIOStream.h>
#include <maya/MFnNumericAttribute.h>
#include "mirrorStrokes.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "texUtils.h"

// const double rad_to_deg = (180 / 3.1415927);
// const double deg_to_rad = (3.1415927 / 180);

MObject mirrorStrokes::aTranslate;
MObject mirrorStrokes::aMirror;

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
  MFnNumericAttribute nAttr;

  aTranslate = nAttr.create("translate", "tra", MFnNumericData::k3Float);
  nAttr.setDefault(0.0f, 0.0f, 0.0f);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setReadable(true);
  addAttribute(aTranslate);

  aMirror = nAttr.create("mirror", "mir", MFnNumericData::kBoolean);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setReadable(true);
  addAttribute(aMirror);

  attributeAffects(aTranslate, aOutput);
  attributeAffects(aMirror, aOutput);

  return (MS::kSuccess);
}

MStatus mirrorStrokes::mutate(
    const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const
{

  const float3 &translate = data.inputValue(aTranslate).asFloat3();
  bool mirror = data.inputValue(aMirror).asBool();
  MFloatVector offset(translate[0], translate[1], translate[2]);

  if (mirror)
  {

    MFloatMatrix globalMirror;
    globalMirror.setToIdentity();
    globalMirror[0][0] = -1;

    MFloatMatrix localMirror;
    localMirror.setToIdentity();
    localMirror[0][0] = -1;

    MFloatMatrix centerMat;
    unsigned j = 0;
    std::vector<Stroke>::iterator siter = strokes->begin();
    for (unsigned i = 0; siter != strokes->end(); siter++)
    {

      Stroke::target_iterator titer = siter->targets_begin();
      for (; titer != siter->targets_end(); titer++)
      {
        const MFloatMatrix &mat = titer->matrix();

        MFloatMatrix result = localMirror * mat * globalMirror;
        titer->setMatrix(result);
      }

      const MFloatMatrix &pivotMat = siter->pivot().matrix();
      MFloatMatrix pivotResult = localMirror * pivotMat * globalMirror;
      siter->setPivotMatrix(pivotResult);

      siter->resetTangents();
    }
  }

  if (! offset.isEquivalent(MFloatVector::zero)) {
    unsigned j = 0;
    std::vector<Stroke>::iterator siter = strokes->begin();
    for (unsigned i = 0; siter != strokes->end(); siter++)
    {
      Stroke::target_iterator titer = siter->targets_begin();
      for (; titer != siter->targets_end(); titer++)
      {
        titer->offsetBy(offset);
      }
      
      MFloatMatrix pivotMat = siter->pivot().matrix();
      pivotMat[3][0] += offset[0];
      pivotMat[3][1] += offset[1];
      pivotMat[3][2] += offset[2];
      siter->setPivotMatrix(pivotMat);
    }
  }


  return MS::kSuccess;
}