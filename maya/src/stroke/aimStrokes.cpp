
#include <maya/MIOStream.h>
#include <maya/MFnNumericAttribute.h>
#include "aimStrokes.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "texUtils.h"

const double rad_to_deg = (180 / 3.1415927);
const double deg_to_rad = (3.1415927 / 180);


MObject aimStrokes::aPoint;

MTypeId aimStrokes::id(k_aimStrokes);

aimStrokes::aimStrokes() {}

aimStrokes::~aimStrokes() {}

void *aimStrokes::creator()
{
  return new aimStrokes();
}

/// Post constructor
void aimStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus aimStrokes::initialize()
{
  MStatus st;
  MString method("aimStrokes::initialize");

	inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;

  aPoint = nAttr.createPoint("point", "pt");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aPoint);

  st = attributeAffects(aPoint, aOutput);

  return (MS::kSuccess);
}


MStatus aimStrokes::mutate(
  const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const 
{
  float3 & fpoint = data.inputValue(aPoint).asFloat3();
  MFloatPoint point(fpoint[0], fpoint[1], fpoint[2]);
  MFloatMatrix centerMat;
  unsigned j=0;
  std::vector<Stroke>::iterator siter = strokes->begin();
  for (unsigned i = 0; siter != strokes->end(); siter++)
  {
 
    Stroke::target_iterator titer = siter->targets_begin();
    for (; titer != siter->targets_end(); titer++)
    {
      const MFloatMatrix & mat =  titer->matrix();

      	MVector z1 = (MVector(point) - MVector(mat[3][0], mat[3][1], mat[3][2])).normal();

        MVector lance( mat[2][0],  mat[2][1],  mat[2][2]); // local zaxis
        
        MFloatMatrix rotMat = MFloatMatrix(MQuaternion(lance, z1).asMatrix().matrix);
      
      centerMat.setToIdentity();
      centerMat[3][0] = -mat[3][0];
      centerMat[3][1] = -mat[3][1];
      centerMat[3][2] = -mat[3][2];

      MFloatMatrix result  = mat * centerMat * rotMat * centerMat.inverse();
      titer->setMatrix(result);
    }
  }
  return MS::kSuccess;
}