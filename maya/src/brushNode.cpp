
#include <maya/MIOStream.h>
#include <math.h>
// #include <algorithm>
#include <map>
// #include <utility>
// #include <tuple>
#include <string>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>
#include <maya/MFnPluginData.h>
#include <maya/MRenderUtil.h>
#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnMessageAttribute.h>


#include <maya/MArrayDataHandle.h>
#include <maya/MAngle.h>

#include <maya/MPlugArray.h>


#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>

#include <maya/MFnVectorArrayData.h>

#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MTransformationMatrix.h>

#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnEnumAttribute.h>


#include "paintingGeom.h"
// #include "strokeData.h"
// #include "paintingNode.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

#include "brushData.h"
#include "brushNode.h"


const double rad_to_deg = (180 / 3.1415927);

const int LEAD_COLOR            = 18;
const int ACTIVE_COLOR          = 15;
const int ACTIVE_AFFECTED_COLOR = 8;
const int DORMANT_COLOR         = 4;
const int HILITE_COLOR          = 17;
const int RED_COLOR             = 12;



MTypeId brushNode::id( k_brushNode );

brushNode::brushNode() {

}

brushNode::~brushNode() {
}

void *brushNode::creator() {
  return new brushNode();
}

const double epsilon = 0.0001;

MObject brushNode::aPhysicalId;
MObject brushNode::aWidth;
MObject brushNode::aTip;
MObject brushNode::aBristleHeight;
MObject brushNode::aPaintingParam;
MObject brushNode::aDipParam;
MObject brushNode::aWipeParam;
MObject brushNode::aRetention;
MObject brushNode::aShape;
MObject brushNode::aTransHeightParam;
MObject brushNode::aContactPower;



MObject brushNode::aLineLength;
MObject brushNode::aLineThickness;

MObject brushNode::aOutPaintBrush;
MObject brushNode::aOutDipBrush;
MObject brushNode::aOutWipeBrush;

MObject brushNode::aCustomId;


MStatus brushNode::initialize()
{
  MStatus st;
  MString method("brushNode::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
  MFnCompoundAttribute cAttr;
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;
  MFnMessageAttribute msgAttr;

  MFloatMatrix identity;
  identity.setToIdentity();



  aPhysicalId  = nAttr.create("physicalId", "pid", MFnNumericData::kInt);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0);
  addAttribute(aPhysicalId);


  aTip = nAttr.create( "tip", "tip", MFnNumericData::k3Float );
  nAttr.setStorable(true);
  nAttr.setHidden(false);
  nAttr.setWritable(true);
  nAttr.setKeyable(true);
  st = addAttribute( aTip ); mser;



  aBristleHeight =  nAttr.create( "bristleHeight", "bht", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  // nAttr.setMin(0.00f);
  // nAttr.setSoftMax(20f);
  // nAttr.setDefault(2.0f);
  nAttr.setKeyable(true);
  addAttribute(aBristleHeight);

  aPaintingParam =  nAttr.create( "paintingParam", "ppm", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  // nAttr.setMin(0.00f);
  // nAttr.setSoftMax(20f);
  // nAttr.setDefault(2.0f);
  nAttr.setKeyable(true);
  addAttribute(aPaintingParam);


  aDipParam =  nAttr.create( "dipParam", "dpm", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  // nAttr.setMin(0.00f);
  // nAttr.setSoftMax(20f);
  // nAttr.setDefault(2.0f);
  nAttr.setKeyable(true);
  addAttribute(aDipParam);


  aWipeParam =  nAttr.create( "wipeParam", "wpm", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  // nAttr.setMin(0.00);
  // nAttr.setSoftMax(20);
  // nAttr.setDefault(2.0);
  nAttr.setKeyable(true);
  addAttribute(aWipeParam);


  aWidth =  nAttr.create( "width", "wid", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  // nAttr.setMin(0.00);
  // nAttr.setSoftMax(20);
  // nAttr.setDefault(2.0);
  nAttr.setKeyable(true);
  addAttribute(aWidth);

  aRetention =  nAttr.create( "retention", "ret", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  // nAttr.setMin(0.00);
  // nAttr.setSoftMax(100.0);
  // nAttr.setDefault(20.0);
  nAttr.setKeyable(true);
  addAttribute(aRetention);


  aShape = eAttr.create( "shape", "shp", Brush::kRound);
  eAttr.addField("flat", Brush::kFlat);
  eAttr.addField("round", Brush::kRound);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute( aShape ); mser;


  aTransHeightParam =  nAttr.create( "transHeightParam", "thp", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aTransHeightParam);

  aContactPower =  nAttr.create( "contactPower", "ctp", MFnNumericData::kFloat);
  nAttr.setMin(0.00f);
  nAttr.setDefault(1.0);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aContactPower);


  aLineLength = nAttr.create( "lineLength", "lln", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  // nAttr.setMin(0.00f);
  // nAttr.setSoftMax(20.0f);
  // nAttr.setDefault(1.0f);
  addAttribute(aLineLength);

  aLineThickness = nAttr.create( "lineThickness", "ltk", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  // nAttr.setMin(0.00f);
  // nAttr.setSoftMax(20.0f);
  // nAttr.setDefault(5.0f);
  addAttribute(aLineThickness);



  aOutPaintBrush = tAttr.create("outPaintBrush", "opb", brushData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutPaintBrush);

  aOutDipBrush = tAttr.create("outDipBrush", "odb", brushData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutDipBrush);

  aOutWipeBrush = tAttr.create("outWipeBrush", "owb", brushData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutWipeBrush);



  aCustomId  = nAttr.create("customId", "cid", MFnNumericData::kInt);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0);
  addAttribute(aCustomId);


  attributeAffects(aPhysicalId, aOutPaintBrush);
  attributeAffects(aWidth, aOutPaintBrush);
  attributeAffects(aTip, aOutPaintBrush);
  attributeAffects(aBristleHeight, aOutPaintBrush);
  attributeAffects(aPaintingParam, aOutPaintBrush);
  attributeAffects(aDipParam, aOutPaintBrush);
  attributeAffects(aWipeParam, aOutPaintBrush);
  attributeAffects(aRetention, aOutPaintBrush);
  attributeAffects(aShape, aOutPaintBrush);
  attributeAffects(aTransHeightParam, aOutPaintBrush);
  attributeAffects(aContactPower, aOutPaintBrush);
  attributeAffects(aCustomId, aOutPaintBrush);

  attributeAffects(aPhysicalId, aOutDipBrush);
  attributeAffects(aWidth, aOutDipBrush);
  attributeAffects(aTip, aOutDipBrush);
  attributeAffects(aBristleHeight, aOutDipBrush);
  attributeAffects(aPaintingParam, aOutDipBrush);
  attributeAffects(aDipParam, aOutDipBrush);
  attributeAffects(aWipeParam, aOutDipBrush);
  attributeAffects(aRetention, aOutDipBrush);
  attributeAffects(aShape, aOutDipBrush);
  attributeAffects(aTransHeightParam, aOutDipBrush);
  attributeAffects(aContactPower, aOutDipBrush);
  attributeAffects(aCustomId, aOutDipBrush);

  attributeAffects(aPhysicalId, aOutWipeBrush);
  attributeAffects(aWidth, aOutWipeBrush);
  attributeAffects(aTip, aOutWipeBrush);
  attributeAffects(aBristleHeight, aOutWipeBrush);
  attributeAffects(aPaintingParam, aOutWipeBrush);
  attributeAffects(aDipParam, aOutWipeBrush);
  attributeAffects(aWipeParam, aOutWipeBrush);
  attributeAffects(aRetention, aOutWipeBrush);
  attributeAffects(aShape, aOutWipeBrush);
  attributeAffects(aTransHeightParam, aOutWipeBrush);
  attributeAffects(aContactPower, aOutWipeBrush);
  attributeAffects(aCustomId, aOutWipeBrush);


  return ( MS::kSuccess );

}



MStatus brushNode::outputData(MDataBlock &data, MObject &attribute, const Brush &brush )
{
  MStatus st;
  MDataHandle hOutput = data.outputValue(attribute);
  MFnPluginData fnOut;
  MTypeId kdid(brushData::id);


  MObject dOut = fnOut.create(kdid, & st);  mser;
  brushData *newData = (brushData * )fnOut.data(&st); mser;

  *(newData->fGeometry) = brush ;

  hOutput.set(newData);
  hOutput.setClean();
  return MS:: kSuccess;
}


MStatus brushNode::compute( const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  MString method("brushNode::compute");

  if (! (
        (plug == aOutPaintBrush) ||
        (plug == aOutDipBrush) ||
        (plug == aOutWipeBrush)
      )  ) { return ( MS::kUnknownParameter ); }

  MFloatVector tip = data.inputValue( aTip ).asFloatVector();
  cerr << "brushNode::compute TIP" << tip <<  endl;

  int physicalId = data.inputValue( aPhysicalId).asInt();

  float width = data.inputValue( aWidth).asFloat();
  float bristleHeight = data.inputValue( aBristleHeight).asFloat();
  // cerr << "compute bristleHeight " <<  bristleHeight << endl;
  float paintingParam = data.inputValue( aPaintingParam).asFloat();
  float dipParam = data.inputValue( aDipParam).asFloat();
  float wipeParam = data.inputValue( aWipeParam).asFloat();

  float retention = data.inputValue( aRetention).asFloat();
  Brush::Shape shape =  Brush::Shape(data.inputValue( aShape).asShort());
  float transHeightParam = data.inputValue( aTransHeightParam).asFloat();
  float contactPower = data.inputValue( aContactPower).asFloat();

  int customId = data.inputValue( aCustomId).asInt();

  Brush paintingBrush( physicalId,
                       customId,
                       tip,
                       bristleHeight,
                       paintingParam,
                       width,
                       shape,
                       retention,
                       transHeightParam,
                       contactPower);

  Brush dipBrush( physicalId,
                  customId,
                  tip,
                  bristleHeight,
                  dipParam,
                  width,
                  shape,
                  99999999.0,
                  transHeightParam,
                  contactPower);

  Brush wipeBrush( physicalId,
                   customId,
                   tip,
                   bristleHeight,
                   wipeParam,
                   width,
                   shape,
                   99999999.0,
                   transHeightParam,
                   contactPower);

  outputData(data, aOutPaintBrush, paintingBrush);
  outputData(data, aOutDipBrush, dipBrush);
  outputData(data, aOutWipeBrush, wipeBrush);

  return MS::kSuccess;

}

MStatus brushNode::getBrush(MObject &attribute, Brush &brush)
{
  MStatus st;
  MObject thisObj = thisMObject();
  MPlug plug( thisObj, attribute);
  MObject d;
  st = plug.getValue(d);
  MFnPluginData fnP(d);
  brushData *bdata  =  (brushData *)fnP.data();
  if (! bdata)  {
    return MS::kFailure;
  }
  brush = *(bdata->fGeometry);
  return MS::kSuccess;
}


void brushNode::draw( M3dView &view,
                      const MDagPath &path,
                      M3dView::DisplayStyle style,
                      M3dView:: DisplayStatus status  )
{

  MStatus st;
  MObject thisObj = thisMObject();
  float lineThickness;
  MPlug(thisObj, aLineThickness).getValue(lineThickness);
  float lineLength;
  MPlug(thisObj, aLineLength).getValue(lineLength);


  std::map< std::string, Brush > brushes;
  Brush p, d, w;
  st = getBrush(brushNode::aOutPaintBrush, p); mser;
  st = getBrush(brushNode::aOutDipBrush, d); mser;
  st = getBrush(brushNode::aOutWipeBrush, w); mser;
  brushes["paint"] =  p;
  brushes["dip"] =  d;
  brushes["wipe"] =  w;

  glPushAttrib(GL_CURRENT_BIT);
  glPushAttrib(GL_LINE_BIT);
  glLineWidth(GLfloat(lineThickness));
  glBegin(GL_LINES);
  for (std::map< std::string,  Brush  >::const_iterator iter = brushes.begin();
       iter != brushes.end(); iter++)
  {
    MFloatMatrix tcp = iter->second.tcp();

    MFloatPoint start = MFloatPoint::origin * tcp;
    MFloatPoint x = MFloatPoint(1.0, 0.0, 0.0) * tcp;
    MFloatPoint y = MFloatPoint(0.0, 1.0, 0.0) * tcp;
    MFloatPoint z = MFloatPoint(0.0, 0.0, 1.0) * tcp;

    glColor3f(1.0f , 0.0f, 0.0f );
    glVertex3f( start.x , start.y , start.z );
    glVertex3f( x.x , x.y, x.z);

    glColor3f(0.0f , 1.0f, 0.0f );
    glVertex3f( start.x , start.y , start.z );
    glVertex3f( y.x , y.y, y.z);

    glColor3f(0.0f , 0.0f, 1.0f );
    glVertex3f( start.x , start.y , start.z );
    glVertex3f( z.x , z.y, z.z);


  }
  glEnd();
  glPopAttrib();
  glPopAttrib();


}

bool brushNode::isBounded() const
{
  return false;
}

MBoundingBox brushNode::boundingBox() const
{
  return MBoundingBox();
}

void brushNode::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());
  nodeFn.setName("brushShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}



