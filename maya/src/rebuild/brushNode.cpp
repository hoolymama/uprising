
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <utility>
#include <tuple>

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
// #include "paint.h"


const double rad_to_deg = (180 / 3.1415927);

const int LEAD_COLOR            = 18;
const int ACTIVE_COLOR          = 15;
const int ACTIVE_AFFECTED_COLOR = 8;
const int DORMANT_COLOR         = 4;
const int HILITE_COLOR          = 17;
const int RED_COLOR             = 12;



MTypeId brushNode::id( k_brushNode );

brushNode::painting() {

}

brushNode::~painting() {
}

void *brushNode::creator() {
  return new painting();
}

const double epsilon = 0.0001;



MObject brushNode::aPhysicalId;

MObject brushNode::aPaintTcp;
MObject brushNode::aDipTcp;

MObject brushNode::aWidth;
MObject brushNode::aRetention;
MObject brushNode::aShape;
MObject brushNode::aTransitionHeight;

MObject brushNode::aOutPaintBrush;
MObject brushNode::aOutDipBrush;


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

  aPaintTcp = mAttr.create( "paintTcp", "ptcp",  MFnMatrixAttribute::kFloat );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);
  addAttribute(aPaintTcp);

  aDipTcp = mAttr.create( "dipTcp", "dtcp",  MFnMatrixAttribute::kFloat );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);
  addAttribute(aDipTcp);

  aPhysicalId  = nAttr.create("physicalId", "pid", MFnNumericData::kInt);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0);
  addAttribute(aPhysicalId);

  aWidth =  nAttr.create( "width", "wid", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20);
  nAttr.setDefault(2.0);
  nAttr.setKeyable(true);
  addAttribute(aWidth);

  aRetention =  nAttr.create( "retention", "ret", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(100.0);
  nAttr.setDefault(20.0);
  nAttr.setKeyable(true);
  addAttribute(aRetention);



  aLinearSpeed = nAttr.create( "linearSpeed", "lnsp", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(200);
  nAttr.setDefault(100.00);
  nAttr.setKeyable(true);
  addAttribute(aLinearSpeed);

  const double minAngSpeed = mayaMath::single_pi / 900.00;
  const double defaultAngSpeed = mayaMath::single_pi / 9.00;
  aAngularSpeed = uAttr.create( "angularSpeed", "agsp", MFnUnitAttribute::kAngle );
  uAttr.setStorable(true);
  uAttr.setReadable(true);
  uAttr.setMin(minAngSpeed);
  uAttr.setMax((mayaMath::single_pi));
  addAttribute(aAngularSpeed);


  aApproximationDistance =  nAttr.create( "approximationDistance", "apxd",
                                          MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20);
  nAttr.setDefault(5.0);
  nAttr.setKeyable(true);
  addAttribute(aApproximationDistance);


  aMaxPointToPointDistance =  nAttr.create( "maxPointToPointDistance", "mxptp",
                              MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(3.0);
  nAttr.setSoftMax(100.00);
  nAttr.setDefault(25.00);
  nAttr.setKeyable(true);
  addAttribute(aMaxPointToPointDistance);



  aApproachDistanceStart = nAttr.create( "approachDistanceStart",
                                         "apds", MFnNumericData::kDouble);
  nAttr.setDefault(5.0);
  aApproachDistanceMid = nAttr.create( "approachDistanceMid",
                                       "apdm", MFnNumericData::kDouble);
  nAttr.setDefault(3.0);
  aApproachDistanceEnd = nAttr.create( "approachDistanceEnd",
                                       "apde", MFnNumericData::kDouble);
  nAttr.setDefault(5.0);
  aApproachDistance = nAttr.create( "approachDistance",
                                    "apd", aApproachDistanceStart, aApproachDistanceMid, aApproachDistanceEnd);

  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  addAttribute(aApproachDistance);

  aStrokes = tAttr.create( "strokes", "stks", strokeData::id );
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aStrokes);


  aBrushWidth  = nAttr.create("brushWidth", "brwd", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.1 );

  aBrushMatrix = mAttr.create( "brushMatrix", "bmat",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);

  aBrushRetention  = nAttr.create("brushRetention", "brrt", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 1.0 );


  aBrushPhysicalId  = nAttr.create("brushPhysicalId", "bpid", MFnNumericData::kShort);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( -1);

  aBrushCustomId  = nAttr.create("brushCustomId", "bcid", MFnNumericData::kShort);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( -1);

  aBrushShape = eAttr.create( "brushShape", "bshp", Brush::kRound);
  eAttr.addField("flat", Brush::kFlat);
  eAttr.addField("round", Brush::kRound);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute( aBrushShape ); mser;

  aBrushTip  = nAttr.create("brushTip", "brtp", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.0 );


  aBrushTransitionHeight = nAttr.create("brushTransitionHeight", "brtnh",
                                        MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 1.0 );

  aBrushTransitionPower = nAttr.create("brushTransitionPower", "brtnp",
                                       MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 1.0 );



  aBrushes = cAttr.create("brushes", "bsh");
  cAttr.addChild(aBrushWidth);
  cAttr.addChild(aBrushShape);
  cAttr.addChild(aBrushRetention);
  cAttr.addChild(aBrushTip);
  cAttr.addChild(aBrushPhysicalId);
  cAttr.addChild(aBrushCustomId);
  cAttr.addChild(aBrushTransitionHeight);
  cAttr.addChild(aBrushTransitionPower);
  cAttr.addChild(aBrushMatrix);
  cAttr.setArray( true );
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute( aBrushes ); mser;

  aPaintColorR = nAttr.create( "paintColorR", "pcr", MFnNumericData::kFloat);
  aPaintColorG = nAttr.create( "paintColorG", "pcg", MFnNumericData::kFloat);
  aPaintColorB = nAttr.create( "paintColorB", "pcb", MFnNumericData::kFloat);
  aPaintColor  = nAttr.create( "paintColor",  "pc" , aPaintColorR, aPaintColorG,
                               aPaintColorB);
  nAttr.setStorable(true);
  nAttr.setHidden(false);
  nAttr.setWritable(true);
  nAttr.setUsedAsColor(true);
  addAttribute(aPaintColor);

  aPaintOpacity = nAttr.create("paintOpacity", "pvis", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault(0.5);

  aPaintTravel = nAttr.create( "paintTravel", "ptvl", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aPaintTravel);


  aPaintCustomId  = nAttr.create("paintCustomId", "pcid", MFnNumericData::kShort);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( -1);


  aPaints = cAttr.create("paints", "pts");
  cAttr.addChild(aPaintColor);
  cAttr.addChild(aPaintOpacity);
  cAttr.addChild(aPaintTravel);
  cAttr.addChild(aPaintCustomId);
  cAttr.setArray( true );
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute( aPaints ); mser;


  aOutput = tAttr.create("output", "out", paintingData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  aPointSize = nAttr.create( "pointSize", "psi", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aPointSize);

  aLineLength = nAttr.create( "lineLength", "lln", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aLineLength);

  aLineThickness = nAttr.create( "lineThickness", "ltk", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aLineThickness);

  aArrowheadSize = nAttr.create( "arrowheadSize", "arsz", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  addAttribute(aArrowheadSize);

  aDisplayTargets = eAttr.create( "displayTargets", "dtg");
  eAttr.addField( "none",    brushNode::kTargetsNone);
  eAttr.addField( "point",    brushNode::kTargetsPoint);
  eAttr.addField( "line",   brushNode::kTargetsLine );
  eAttr.addField( "matrix",   brushNode::kTargetsMatrix );
  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setDefault(brushNode::kTargetsNone);
  addAttribute(aDisplayTargets );


  aDisplayClusterPath = nAttr.create( "displayClusterPath", "dcpt",
                                      MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayClusterPath );

  aDisplayPivots = nAttr.create( "displayPivots", "dsp",
                                 MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayPivots );


  aDisplayIds = nAttr.create( "displayIds", "did",
                              MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayIds);

  aDisplayParentIds = nAttr.create( "displayParentIds", "dprid",
                                    MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayParentIds);


  aDisplayLayerIds = nAttr.create( "displayLayerIds", "dlyid",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayLayerIds);

  aDisplayBrushIds = nAttr.create( "displayBrushIds", "dbid",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayBrushIds);

  aDisplayPaintIds = nAttr.create( "displayPaintIds", "dptid",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayPaintIds);

  aDisplayRepeatIds = nAttr.create( "displayRepeatIds", "drpid",
                                    MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayRepeatIds);


  aIdDisplayOffset = nAttr.create( "idDisplayOffset", "iddo",  MFnNumericData::k3Float );
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault( 0.0f, 0.0f, 1.0f );
  addAttribute(aIdDisplayOffset);



  aStackGap = nAttr.create( "stackGap", "sgap", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1);
  nAttr.setDefault(0);
  addAttribute(aStackGap);


  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aLinearSpeed, aOutput);
  st = attributeAffects(aAngularSpeed, aOutput);
  st = attributeAffects(aApproximationDistance, aOutput);
  st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aPaints, aOutput);
  st = attributeAffects(aDisplacementMesh, aOutput);
  st = attributeAffects(aMaxPointToPointDistance, aOutput);
  st = attributeAffects(aApproachDistance, aOutput);


  return ( MS::kSuccess );

}
MStatus brushNode::compute( const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  MString method("brushNode::compute");
  if (plug != aOutput  ) { return ( MS::kUnknownParameter ); }

  MDataHandle mh = data.inputValue(aInMatrix, &st); mser;
  MMatrix wm = mh.asMatrix();



  double ptpThresh = data.inputValue(aMaxPointToPointDistance).asDouble();
  if (ptpThresh < 3.0) {
    ptpThresh = 3.0;
  }

  MArrayDataHandle hBrushes = data.inputArrayValue(aBrushes, &st ); msert;
  std::map<short, Brush> brushes = Brush::factory(
                                     hBrushes,
                                     brushNode::aBrushWidth,
                                     brushNode::aBrushRetention,
                                     brushNode::aBrushTip,
                                     brushNode::aBrushPhysicalId,
                                     brushNode::aBrushShape,
                                     brushNode::aBrushCustomId,
                                     brushNode::aBrushTransitionHeight,
                                     brushNode::aBrushTransitionPower

                                   );

  MArrayDataHandle hPaints = data.inputArrayValue(aPaints, &st ); msert;
  std::map<short, Paint> paints = Paint::factory(
                                    hPaints,
                                    brushNode::aPaintColor,
                                    brushNode::aPaintOpacity,
                                    brushNode::aPaintTravel,
                                    brushNode::aPaintCustomId
                                  );




  m_pd->create();
  paintingGeom *pGeom = m_pd->geometry();

  pGeom->setBrushes(brushes);
  pGeom->setPaints(paints);


  std::vector<Stroke> strokes;
  // collectStrokes(data,  strokes);


  addStrokes(data,  pGeom);


  if (pGeom->clusters().size()) {

    MDataHandle hApproachDistance = data.inputValue(aApproachDistance);
    double approachStart = hApproachDistance.child(aApproachDistanceStart).asDouble();
    double approachMid = hApproachDistance.child(aApproachDistanceMid).asDouble();
    double approachEnd = hApproachDistance.child(aApproachDistanceEnd).asDouble();

    pGeom->setApproaches(approachStart, approachMid, approachEnd, ptpThresh);


    MObject dMesh =  data.inputValue(aDisplacementMesh).asMeshTransformed();
    MFnMesh meshFn(dMesh, &st);
    if (!st.error()) {
      MMeshIsectAccelParams ap = meshFn.autoUniformGridParams();
      pGeom->displace(meshFn, ap);
    }


    // lift up the start and end of each stroke according to
    // brush tip
    pGeom->offsetBrushContact();

  }


  MFnPluginData fnOut;
  MTypeId kdid(paintingData::id);
  MObject dOut = fnOut.create(kdid , &st ); mser;
  paintingData *outGeometryData = (paintingData *)fnOut.data(&st); mser;
  if (m_pd) {
    *outGeometryData = (*m_pd);
  }


  MDataHandle outputHandle = data.outputValue(aOutput, &st ); mser;
  st = outputHandle.set(outGeometryData); mser;
  data.setClean( plug );


  return MS::kSuccess;

}

// void brushNode::collectStrokes(MDataBlock &data,std::vector<Stroke> &strokes)
// {
//   MStatus st;
//   MArrayDataHandle hStrokes = data.inputValue(aStrokes, &st); msert;
//   unsigned nInputs = hStrokes.elementCount();
//   // int gid = 0;
//   for (unsigned i = 0; i < nInputs; i++, hStrokes.next()) {
//     int index = hStrokes.elementIndex(&st);
//     MDataHandle hStrokeInput = hStrokes.inputValue(&st);
//     if (st.error()) {
//       continue;
//     }
//     MObject dStrokeInput = hStrokeInput.data();
//     MFnPluginData fnStrokeInput( dStrokeInput , &st);
//     if (st.error()) {
//       continue;
//     }
//     strokeData *sData = (strokeData *)fnStrokeInput.data();
//     // strokeCurveGeom *strokeGeom = strokeData->fGeometry;
//     const std::vector<Stroke> *strokeGeom = sData->fGeometry;

//     // const std::vector<Stroke> &strokes = strokeGeom->strokes();
//     std::vector<Stroke>::const_iterator citer;
//     for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++) {
//       strokes.push_back(*citer);
//     }
//   }
// }

MStatus brushNode::addStrokes(MDataBlock &data, paintingGeom *pGeom)
{
  MStatus st;
  MArrayDataHandle hStrokes = data.inputValue(aStrokes, &st); msert;
  unsigned nInputs = hStrokes.elementCount();
  // int gid = 0;
  for (unsigned i = 0; i < nInputs; i++, hStrokes.next()) {
    int index = hStrokes.elementIndex(&st);
    MDataHandle hStrokeInput = hStrokes.inputValue(&st);
    if (st.error()) {
      continue;
    }
    MObject dStrokeInput = hStrokeInput.data();
    MFnPluginData fnStrokeInput( dStrokeInput , &st);
    if (st.error()) {
      continue;
    }
    strokeData *sData = (strokeData *)fnStrokeInput.data();
    // strokeCurveGeom *strokeGeom = strokeData->fGeometry;
    const std::vector<Stroke> *strokeGeom = sData->fGeometry;

    // const std::vector<Stroke> &strokes = strokeGeom->strokes();
    std::vector<Stroke>::const_iterator citer;
    for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++) {

      pGeom->addStroke(*citer, index);

    }
  }
  return MS::kSuccess;
}


void brushNode::setWireDrawColor(M3dView &view,  M3dView::DisplayStatus status)
{

  switch ( status )
  {
    case M3dView::kLead :
      view.setDrawColor( LEAD_COLOR, M3dView::kActiveColors );
      break;
    case M3dView::kActive :
      view.setDrawColor( ACTIVE_COLOR, M3dView::kActiveColors );
      break;
    case M3dView::kActiveAffected :
      view.setDrawColor( ACTIVE_AFFECTED_COLOR, M3dView::kActiveColors );
      break;
    case M3dView::kDormant :
      view.setDrawColor( DORMANT_COLOR, M3dView::kDormantColors );
      break;
    case M3dView::kHilite :
      view.setDrawColor( HILITE_COLOR, M3dView::kActiveColors );
      break;
    default:
      break;
  }

}

void brushNode::drawWireframeTargets(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{
  setWireDrawColor(view, status);

  MObject thisObj = thisMObject();

  // bool displayLift;
  // MPlug(thisObj, aDisplayLift).getValue(displayLift);

  double pointSize;
  MPlug(thisObj, aPointSize).getValue(pointSize);

  double lineLength;
  MPlug(thisObj, aLineLength).getValue(lineLength);

  double lineThickness;
  MPlug(thisObj, aLineThickness).getValue(lineThickness);

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);

  short tmp;
  MPlug(thisObj, aDisplayTargets).getValue(tmp);
  TargetDisplay targetDisplayStyle = TargetDisplay(tmp);
  if (targetDisplayStyle == brushNode::kTargetsNone) {
    return;
  }

  if (targetDisplayStyle == brushNode::kTargetsPoint) {
    double stackHeight = 0.0;
    glPushAttrib(GL_CURRENT_BIT);
    glPointSize(float(pointSize));

    glBegin( GL_POINTS );

    for (auto cluster : geom.clusters())
    {
      for (auto stroke : cluster.strokes())
      {
        stackHeight += stackGap;
        MFloatPointArray points;
        stroke.getPoints(points, stackHeight);
        unsigned len = points.length();
        for (int i = 0; i < len; ++i)
        {
          glVertex3f(points[i].x, points[i].y, points[i].z);
        }
      }
    }
    glEnd();
    glPopAttrib();
    return;
  }

  if (targetDisplayStyle == brushNode::kTargetsLine) {
    double stackHeight = 0.0;
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(GLfloat(lineThickness));
    glBegin(GL_LINES);
    for (auto cluster : geom.clusters())
    {
      for (auto stroke : cluster.strokes())
      {
        stackHeight += stackGap;
        MFloatPointArray starts;
        stroke.getPoints(starts, stackHeight);
        MFloatVectorArray ends;
        stroke.getZAxes(ends);

        unsigned len = starts.length();
        for (int i = 0; i < len; ++i)
        {

          const MFloatVector &startPoint = starts[i];
          MFloatVector endPoint = startPoint - (ends[i] * lineLength);
          glVertex3f( startPoint.x , startPoint.y , startPoint.z );
          glVertex3f( endPoint.x , endPoint.y, endPoint.z);
        }
      }
    }
    glEnd();
    glPopAttrib();
    return;
  }

  if (targetDisplayStyle == brushNode::kTargetsMatrix) {
    double stackHeight = 0.0;

    glPushAttrib(GL_CURRENT_BIT);
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(GLfloat(lineThickness));
    glBegin(GL_LINES);
    for (auto cluster : geom.clusters())
    {
      for (auto stroke : cluster.strokes())
      {
        stackHeight += stackGap;
        MFloatPointArray starts;
        stroke.getPoints(starts, stackHeight);

        MFloatVectorArray xAxes;
        stroke.getXAxes(xAxes);

        MFloatVectorArray yAxes;
        stroke.getYAxes(yAxes);

        MFloatVectorArray zAxes;
        stroke.getZAxes(zAxes);

        unsigned len = starts.length();
        for (int i = 0; i < len; ++i)
        {

          const MFloatVector &startPoint = starts[i];


          MFloatVector xPoint = startPoint + (xAxes[i] * lineLength);
          MFloatVector yPoint = startPoint + (yAxes[i] * lineLength);
          MFloatVector zPoint = startPoint + (zAxes[i] * lineLength);


          glColor3f(1.0f , 0.0f, 0.0f );
          glVertex3f( startPoint.x , startPoint.y , startPoint.z );
          glVertex3f( xPoint.x , xPoint.y, xPoint.z);

          glColor3f(0.0f , 1.0f, 0.0f );
          glVertex3f( startPoint.x , startPoint.y , startPoint.z );
          glVertex3f( yPoint.x , yPoint.y, yPoint.z);

          glColor3f(0.0f , 0.0f, 1.0f );
          glVertex3f( startPoint.x , startPoint.y , startPoint.z );
          glVertex3f( zPoint.x , zPoint.y, zPoint.z);
        }
      }
    }
    glEnd();
    glPopAttrib();
    glPopAttrib();
  }
}


void brushNode::drawWireframePivots(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{
  setWireDrawColor(view, status);

  MObject thisObj = thisMObject();

  bool displayPivots;
  MPlug(thisObj, aDisplayPivots).getValue(displayPivots);
  if (! displayPivots) {
    return;
  }

  double pointSize;
  MPlug(thisObj, aPointSize).getValue(pointSize);

  double lineLength;
  MPlug(thisObj, aLineLength).getValue(lineLength);

  double lineThickness;
  MPlug(thisObj, aLineThickness).getValue(lineThickness);

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);

  // draw a vertical line to represent the axis with a dot in the middle

  double stackHeight = 0.0;
  MFloatPointArray pivots;
  for (auto cluster : geom.clusters())
  {
    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;
      pivots.append(stroke.pivot().position() + MFloatVector(0.0f, 0.0f, stackHeight));
    }
  }
  unsigned len = pivots.length();


  glPushAttrib(GL_CURRENT_BIT);
  glPointSize(float(pointSize));

  glBegin( GL_POINTS );

  for (int i = 0; i < len; ++i)
  {
    glVertex3f(pivots[i].x, pivots[i].y, pivots[i].z);
  }
  glEnd();
  glPopAttrib();



  glPushAttrib(GL_LINE_BIT);
  glLineWidth(GLfloat(lineThickness));
  glBegin(GL_LINES);

  MFloatVector offset(0.0f, 0.0f, lineLength * 0.5);
  for (int i = 0; i < len; ++i)
  {
    MFloatPoint startPoint = pivots[i] - offset;
    MFloatPoint endPoint = pivots[i] + offset;
    glVertex3f( startPoint.x , startPoint.y , startPoint.z );
    glVertex3f( endPoint.x , endPoint.y, endPoint.z);
  }

  glEnd();
  glPopAttrib();
}

void brushNode::drawWireframeArrows(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{



  setWireDrawColor(view, status);
  MObject thisObj = thisMObject();

  double arrowheadSize;
  MPlug(thisObj, aArrowheadSize).getValue(arrowheadSize);

  if (arrowheadSize < 0.000001) {
    return;
  }



  MFloatPoint head(MFloatVector::xAxis * arrowheadSize);
  MFloatPoint right(MVector(0.5f, 0.3f, 0.0f) * arrowheadSize );
  MFloatPoint left(MVector(0.5f, -0.3f, 0.0f) * arrowheadSize);




  double lineThickness;
  MPlug(thisObj, aLineThickness).getValue(lineThickness);

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);
  double stackHeight = 0.0;

  glPushAttrib(GL_LINE_BIT);
  glLineWidth(GLfloat(lineThickness));
  glBegin(GL_LINES);
  for (auto cluster : geom.clusters())
  {
    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;
      MMatrixArray mats;
      stroke.getDirectionMatrices(mats, stackHeight);

      unsigned len = mats.length();
      if (! len) {
        continue;
      }






      for (unsigned i = 0; i < len; ++i)
      {
        float floats[4][4];
        mats[i].get(floats);
        MFloatMatrix fmat(floats);

        MFloatPoint c(fmat[3][0], fmat[3][1], fmat[3][2]);
        MFloatPoint h = head * fmat;
        MFloatPoint l = left * fmat;
        MFloatPoint r = right * fmat;


        glVertex3f( c.x , c.y , c.z );
        glVertex3f( h.x , h.y , h.z );


        glVertex3f( h.x , h.y , h.z );
        glVertex3f( l.x , l.y , l.z );

        glVertex3f( l.x , l.y , l.z );
        glVertex3f( r.x , r.y , r.z );

        glVertex3f( r.x , r.y , r.z );
        glVertex3f( h.x , h.y , h.z );


      }

    }
  }

  glEnd();
  glPopAttrib();
}

void brushNode::drawWireframeBorders(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{

  setWireDrawColor(view, status);
  MObject thisObj = thisMObject();

  double lineThickness;
  MPlug(thisObj, aLineThickness).getValue(lineThickness);

  // bool displayLift;
  // MPlug(thisObj, aDisplayLift).getValue(displayLift);

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);
  double stackHeight = 0.0;

  glPushAttrib(GL_LINE_BIT);
  glLineWidth(GLfloat(lineThickness));
  glBegin(GL_LINES);
  for (auto cluster : geom.clusters())
  {
    const Brush &brush = geom.brushFromId(cluster.brushId());
    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;

      MFloatPointArray lefts;
      MFloatPointArray rights;

      stroke.getBorders(lefts, rights, brush, stackHeight);

      unsigned len = lefts.length();
      if (! len) {
        continue;
      }

      glVertex3f( lefts[0].x , lefts[0].y , lefts[0].z );
      glVertex3f( rights[0].x , rights[0].y, rights[0].z);
      unsigned i = 1;
      for ( i = 1; i < len; ++i)
      {
        unsigned prev = i - 1;
        glVertex3f( lefts[i].x , lefts[i].y , lefts[i].z );
        glVertex3f( lefts[prev].x , lefts[prev].y , lefts[prev].z );

        glVertex3f( rights[i].x , rights[i].y , rights[i].z );
        glVertex3f( rights[prev].x , rights[prev].y , rights[prev].z );

      }

      int last = len - 1;

      glVertex3f( lefts[last].x , lefts[last].y , lefts[last].z );
      glVertex3f( rights[last].x , rights[last].y, rights[last].z);



    }
  }
  glEnd();
  glPopAttrib();
}


void brushNode::drawWireframeClusterPath(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{

  MObject thisObj = thisMObject();

  bool doDisplayClusterPath;
  MPlug(thisObj, aDisplayClusterPath).getValue(doDisplayClusterPath);
  if (! doDisplayClusterPath ) {
    return;
  }

  double lineThickness;
  MPlug(thisObj, aLineThickness).getValue(lineThickness);


  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);
  double stackHeight = 0.0;

  glPushAttrib(GL_CURRENT_BIT);
  glPushAttrib(GL_LINE_BIT);
  glLineWidth(GLfloat(lineThickness));
  for (auto cluster : geom.clusters())
  {
    glBegin(GL_LINE_STRIP);
    MColor color = geom.paintFromId(cluster.paintId()).color;
    glColor4f(color.r , color.g , color.b, 1.0f);
    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;
      MFloatPointArray points;

      stroke.getPoints(points, stackHeight, true);
      // cerr << "stroke.getPoints: " << points <<  endl;
      unsigned len = points.length();
      for (int i = 0; i < len; ++i)
      {
        glVertex3f( points[i].x , points[i].y , points[i].z );
      }
    }
    glEnd();
  }
  glPopAttrib();
  glPopAttrib();
}

void brushNode::drawWireframe(const paintingGeom &geom, M3dView &view,
                              const MDagPath &path,
                              M3dView:: DisplayStatus status ) {

  view.beginGL();
  drawWireframeTargets(geom, view, path, status);
  drawWireframeBorders(geom, view, path, status);
  drawWireframeArrows(geom, view, path, status);

  drawWireframeClusterPath(geom, view, path, status);
  drawWireframePivots(geom, view, path, status);

  view.endGL();
}

void brushNode::drawShaded(const paintingGeom &geom, M3dView &view,
                           const MDagPath &path,
                           M3dView:: DisplayStatus status ) {



  MObject thisObj = thisMObject();

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);
  double stackHeight = 0.0;


  glPushAttrib(GL_CURRENT_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (auto cluster : geom.clusters())
  {
    // double brushWidth = geom.brushFromId(cluster.brushId()).width;
    const Brush &brush = geom.brushFromId(cluster.brushId());
    MColor color = geom.paintFromId(cluster.paintId()).color;
    glColor4f(color.r , color.g , color.b, color.a);


    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;

      MFloatPointArray lefts;
      MFloatPointArray rights;

      stroke.getBorders(lefts, rights, brush, stackHeight);

      unsigned len = lefts.length();
      if (! len) {
        // should never happen - maybe remove this check.
        continue;
      }
      glBegin(GL_TRIANGLE_STRIP);

      for (unsigned i = 0; i < len; ++i)
      {
        glVertex3f( lefts[i].x , lefts[i].y , lefts[i].z );
        glVertex3f( rights[i].x , rights[i].y , rights[i].z );
      }
      glEnd();
    }
  }
  glPopAttrib();
}



void brushNode::drawIds(const paintingGeom &geom, M3dView &view,
                        const MDagPath &path,
                        M3dView:: DisplayStatus status )
{




  setWireDrawColor(view, status);
  MObject thisObj = thisMObject();

  MFloatVector offset;

  MPlug offPlug(thisObj, aIdDisplayOffset);

  offPlug.child(0).getValue(offset[0]);
  offPlug.child(1).getValue(offset[1]);
  offPlug.child(2).getValue(offset[2]);


  bool displayIds, displayParentIds, displayLayerIds, displayBrushIds, displayPaintIds,
       displayRepeatIds;

  MPlug(thisObj, aDisplayIds).getValue(displayIds);
  MPlug(thisObj, aDisplayParentIds).getValue(displayParentIds);
  MPlug(thisObj, aDisplayLayerIds).getValue(displayLayerIds);
  MPlug(thisObj, aDisplayBrushIds).getValue(displayBrushIds);
  MPlug(thisObj, aDisplayPaintIds).getValue(displayPaintIds);
  MPlug(thisObj, aDisplayRepeatIds).getValue(displayRepeatIds);


  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);


  if (! (
        displayIds ||
        displayParentIds ||
        displayLayerIds ||
        displayBrushIds ||
        displayPaintIds ||
        displayRepeatIds
      )) { return; }


  { double stackGap; }
  MPlug(thisObj, aStackGap).getValue(stackGap);
  double stackHeight = 0.0;

  glPushAttrib(GL_CURRENT_BIT);



  for (auto cluster : geom.clusters())
  {
    // const Brush &brush = geom.brushFromId(cluster.brushId());
    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;

      MString text("");
      if (displayIds) { text = text + "strokeId:" + stroke.strokeId() + "\n" ;}
      if (displayParentIds) { text = text + "Pr:" + stroke.parentId() + "\n" ;}
      if (displayBrushIds) { text = text + "Br:" + stroke.brushId() + "\n" ;}
      if (displayPaintIds) { text = text + "Pt:" + stroke.paintId() + "\n" ;}
      if (displayLayerIds) { text = text + "Ly:" + stroke.layerId() + "\n" ;}
      if (displayRepeatIds) { text = text + "Rp:" + stroke.repeatId() + "\n" ;}


      MFloatPoint head;
      stroke.getHead(head, stackHeight);
      MPoint textPos =  MPoint(head) + MVector(offset);

      view.drawText( text , textPos,  M3dView::kRight);
    }
  }

  glPopAttrib();

}




void brushNode::draw( M3dView &view,
                      const MDagPath &path,
                      M3dView::DisplayStyle style,
                      M3dView:: DisplayStatus status  )
{


  MStatus st;

  MObject thisObj = thisMObject();


  MPlug plugPaintingData( thisObj, aOutput );
  MObject dPaintingData;
  st = plugPaintingData.getValue(dPaintingData);
  MFnPluginData fnPaintingData(dPaintingData);
  paintingData *ptd  =  (paintingData *)fnPaintingData.data();
  if (! ptd)  {
    return;
  }



  paintingGeom *pGeom = ptd->geometry();
  if (! (pGeom && pGeom->clusters().size())) {
    return;
  }
  const paintingGeom &geom = *pGeom;

  if (style ==  M3dView::kWireFrame ) {
    drawWireframe(geom, view, path, status);
  }

  else if ((style ==  M3dView::kGouraudShaded) || (style ==  M3dView::kFlatShaded)) {

    bool wfos = view.wireframeOnShaded() ;

    bool active =  (status == M3dView::kActive  || status == M3dView::kLead
                    || status == M3dView::kHilite );

    if (active) {
      drawShaded(geom, view, path, status);
      drawWireframe(geom, view, path, status);
    }
    else if (wfos) {   // not active
      drawWireframe(geom, view, path, status);
      drawShaded(geom, view, path, status);
    }
    else {   // not wfos
      drawShaded(geom, view, path, status);
    }

  }
  // always draw any IDs that are enabled
  drawIds(geom, view, path, status);



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
  nodeFn.setName("paintingShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}



