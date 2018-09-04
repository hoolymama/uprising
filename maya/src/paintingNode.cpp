
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>


#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>
#include <maya/MFnPluginData.h>




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
#include "strokeCurveData.h"
#include "paintingNode.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

#include "brush.h"
#include "paint.h"


const double rad_to_deg = (180 / 3.1415927);

const int LEAD_COLOR            = 18;
const int ACTIVE_COLOR          = 15;
const int ACTIVE_AFFECTED_COLOR = 8;
const int DORMANT_COLOR         = 4;
const int HILITE_COLOR          = 17;
const int RED_COLOR          = 12;





MTypeId painting::id( k_painting );

painting::painting() {
  m_pd = new paintingData;
}

painting::~painting() {
  if (m_pd) {
    delete m_pd;
    m_pd = 0;
  }
}

// painting::painting() {}

// painting::~painting() {}

void *painting::creator() {
  return new painting();
}

const double epsilon = 0.0001;

MObject painting::aPlaneMatrix;
MObject painting::aInMatrix;

MObject painting::aStrokeCurves;
MObject painting::aRotateOrder;
MObject painting::aOutputUnit;
// MObject painting::aApproachDistanceStart;
// MObject painting::aApproachDistanceMid;
// MObject painting::aApproachDistanceEnd;
// MObject painting::aApproachDistance;

MObject painting::aDipApproachObject;
MObject painting::aToolChangeApproachObject;
MObject painting::aHomeApproachObject;
MObject painting::aLinearSpeed; // cm/sec
MObject painting::aAngularSpeed; // per sec
MObject painting::aApproximationDistance; // cm


MObject painting::aBrushMatrix;
MObject painting::aBrushRetention;
MObject painting::aBrushWidth;
MObject painting::aBrushShape;
MObject painting::aBrushes;

MObject painting::aPaintColorR;
MObject painting::aPaintColorG;
MObject painting::aPaintColorB;
MObject painting::aPaintColor;
MObject painting::aPaintOpacity;
MObject painting::aPaintTravel;
MObject painting::aPaints;


MObject painting::aPointSize;
MObject painting::aLineLength;
MObject painting::aLineThickness;

MObject painting::aDisplayIds;
MObject painting::aDisplayTargets;
MObject painting::aDisplayLift;
MObject painting::aDisplayApproach;
MObject painting::aDisplayClusterPath;
MObject painting::aStackGap;
MObject painting::aOutTargets; // local
MObject painting::aOutput;


MStatus painting::initialize()
{
  MStatus st;
  MString method("painting::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
  MFnCompoundAttribute cAttr;
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;
  MFnMessageAttribute msgAttr;

  MMatrix identity;
  identity.setToIdentity();


  aInMatrix = mAttr.create( "inMatrix", "imat",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);
  addAttribute(aInMatrix);

  aPlaneMatrix = mAttr.create( "planeMatrix", "pmat",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);
  addAttribute(aPlaneMatrix);

  // APPROACH OBJECTS
  aDipApproachObject =  msgAttr.create("dipApproachObject", "dao");
  addAttribute(aDipApproachObject);
  aToolChangeApproachObject =  msgAttr.create("toolChangeApproachObject", "tcao");
  addAttribute(aToolChangeApproachObject);
  aHomeApproachObject =  msgAttr.create("homeApproachObject", "hmao");
  addAttribute(aHomeApproachObject);

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

  aRotateOrder = eAttr.create( "rotateOrder", "ro", MTransformationMatrix::kZYX);
  eAttr.addField("xyz", MTransformationMatrix::kXYZ);
  eAttr.addField("yzx", MTransformationMatrix::kYZX);
  eAttr.addField("zxy", MTransformationMatrix::kZXY);
  eAttr.addField("xzy", MTransformationMatrix::kXZY);
  eAttr.addField("yxz", MTransformationMatrix::kYXZ);
  eAttr.addField("zyx", MTransformationMatrix::kZYX);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute( aRotateOrder ); er;

  aOutputUnit = eAttr.create( "outAngularUnit", "oang", MAngle::kDegrees);
  eAttr.addField("radians", MAngle::kRadians);
  eAttr.addField("degrees", MAngle::kDegrees);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute( aOutputUnit ); er;


  aStrokeCurves = tAttr.create( "strokeCurves", "scrvs", strokeCurveData::id );
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aStrokeCurves);

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



  aBrushShape = eAttr.create( "brushShape", "bshp", Brush::kRound);
  eAttr.addField("flat", Brush::kFlat);
  eAttr.addField("round", Brush::kRound);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute( aBrushShape ); er;



  aBrushes = cAttr.create("brushes", "bsh");
  cAttr.addChild(aBrushWidth);
  cAttr.addChild(aBrushShape);
  cAttr.addChild(aBrushRetention);
  cAttr.addChild(aBrushMatrix);
  cAttr.setArray( true );
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute( aBrushes ); er;

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

  aPaints = cAttr.create("paints", "pts");
  cAttr.addChild(aPaintColor);
  cAttr.addChild(aPaintOpacity);
  cAttr.addChild(aPaintTravel);
  cAttr.setArray( true );
  cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  cAttr.setReadable(false);
  st = addAttribute( aPaints ); er;

  aOutTargets = tAttr.create("outTargets", "otg", MFnData::kMatrixArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutTargets ); er;


  aOutput = tAttr.create("output", "out", paintingData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  // tAttr.setCached(false);
  addAttribute(aOutput);



  ////////



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




  aDisplayIds = nAttr.create( "displayIds", "dids", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayIds );

  aDisplayTargets = eAttr.create( "displayTargets", "dtg");
  eAttr.addField( "none",    painting::kTargetsNone);
  eAttr.addField( "point",    painting::kTargetsPoint);
  eAttr.addField( "line",   painting::kTargetsLine );
  eAttr.addField( "matrix",   painting::kTargetsMatrix );
  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setDefault(painting::kTargetsNone);
  addAttribute(aDisplayTargets );

  aDisplayLift = nAttr.create( "displayLift", "dal", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayLift );

  aDisplayApproach = nAttr.create( "displayApproach", "dapp", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayApproach );


  aDisplayClusterPath = nAttr.create( "displayClusterPath", "dcpt",
                                      MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayClusterPath );



  aStackGap = nAttr.create( "stackGap", "sgap", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1);
  nAttr.setDefault(0);
  addAttribute(aStackGap);


  ///////


  // st = attributeAffects(aPlaneMatrix, aOutTargets);
  // st = attributeAffects(aStrokeCurves, aOutTargets);
  // st = attributeAffects(aDipApproachObject, aOutTargets);
  // st = attributeAffects(aToolChangeApproachObject, aOutTargets);
  // st = attributeAffects(aHomeApproachObject, aOutTargets);
  // st = attributeAffects(aLinearSpeed, aOutTargets);
  // st = attributeAffects(aAngularSpeed, aOutTargets);
  // st = attributeAffects(aApproximationDistance, aOutTargets);

  // st = attributeAffects(aBrushes, aOutTargets);
  // st = attributeAffects(aPaints, aOutTargets);


  st = attributeAffects(aPlaneMatrix, aOutput);
  st = attributeAffects(aStrokeCurves, aOutput);
  // st = attributeAffects(aDipApproachObject, aOutput);
  // st = attributeAffects(aToolChangeApproachObject, aOutput);
  // st = attributeAffects(aHomeApproachObject, aOutput);
  st = attributeAffects(aLinearSpeed, aOutput);
  st = attributeAffects(aAngularSpeed, aOutput);
  st = attributeAffects(aApproximationDistance, aOutput);
  st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aPaints, aOutput);

  return ( MS::kSuccess );

}

MStatus painting::setData(MDataBlock &block, MObject &attribute,
                          const MMatrixArray &data) {
  MDataHandle h = block.outputValue(attribute);
  MFnMatrixArrayData fn;
  MObject d = fn.create(data);
  h.set(d);
  return block.setClean( attribute );
}

MStatus painting::getData( MObject &attribute,  MIntArray &array) {
  MStatus st;
  MFnIntArrayData fn;
  MPlug plug(thisMObject(), attribute);
  MObject d;
  st = plug.getValue(d); ert;
  fn.setObject(d);
  array = fn.array(&st); ert;
  return MS::kSuccess;
}


MStatus painting::getData( MObject &attribute,  MDoubleArray &array) {
  MStatus st;
  MFnDoubleArrayData fn;
  MPlug plug(thisMObject(), attribute);
  MObject d;
  st = plug.getValue(d); ert;
  fn.setObject(d);
  array = fn.array(&st); ert;
  return MS::kSuccess;

}

MStatus painting::getData( MObject &attribute,  MVectorArray &array) {
  MStatus st;
  MFnVectorArrayData fn;
  MPlug plug(thisMObject(), attribute);
  MObject d;
  st = plug.getValue(d); ert;
  fn.setObject(d);
  array = fn.array(&st); ert;
  return MS::kSuccess;

}



MStatus painting::getData( MArrayDataHandle &ha,  strokeCurveGeom *result) {
  MStatus st;
  MDataHandle h = ha.inputValue(&st );
  MObject d = h.data();
  MFnPluginData fn ( d , &st); ert;
  strokeCurveData *scData = (strokeCurveData *)fn.data();
  result = scData->fGeometry;
  return MS::kSuccess;
}

MStatus painting::compute( const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  MString method("painting::compute");
  if (!(
        /* plug == aOutTargets  || */ plug == aOutput  )) {
    return ( MS::kUnknownParameter );
  }


  // cerr << "plug name: " << plug.name() << endl;

  // MMatrixArray outTargets; // outTargets.clear();

  MDataHandle mh = data.inputValue(aInMatrix, &st); er;
  MMatrix wm = mh.asMatrix();
  MMatrix planeMatrix = data.inputValue(painting::aPlaneMatrix).asMatrix();
  MMatrix inversePlaneMatrix = planeMatrix.inverse();
  MVector planeNormal = (MVector::zAxis * planeMatrix).normal();



  // PAINTING DATA COMMENTED OUT

  MArrayDataHandle hBrushes = data.inputArrayValue(aBrushes, &st ); ert;
  std::map<short, Brush> brushes = Brush::factory(
                                     hBrushes,
                                     painting::aBrushWidth,
                                     painting::aBrushRetention,
                                     painting::aBrushShape
                                   );

  MArrayDataHandle hPaints = data.inputArrayValue(aPaints, &st ); ert;
  std::map<short, Paint> paints = Paint::factory(
                                    hPaints,
                                    painting::aPaintColor,
                                    painting::aPaintOpacity,
                                    painting::aPaintTravel
                                  );

  // MAKE NEW DATA
  // cerr <<  "m_pd->create" << endl;
  m_pd->create( data, painting::aStrokeCurves, brushes, paints );
  // cerr << " m_pd->geometry()" <<  m_pd->geometry()->clusters().size() << endl;

  MFnPluginData fnOut;
  MTypeId kdid(paintingData::id);
  MObject dOut = fnOut.create(kdid , &st ); er;
  paintingData *outGeometryData = (paintingData *)fnOut.data(&st); er;
  if (m_pd) {
    *outGeometryData = (*m_pd);
  }

  // cerr <<  "PAINTING NODE COMPUTE - GEO FULL" << endl;
  // cerr << *(outGeometryData->geometry()) << endl;



  MDataHandle outputHandle = data.outputValue(aOutput, &st ); er;
  st = outputHandle.set(outGeometryData); er;
  data.setClean( plug );


  // MDataHandle hOutput = data.outputValue(aOutput);
  // MFnPluginData fnOutPaintingData;
  // MTypeId kdid(paintingData::id);
  // MObject dOutPaintingData = fnOutPaintingData.create(kdid , &st );
  // paintingData *pPaintingData = (paintingData *)fnOutPaintingData.data(&st); er;
  // paintingGeom *pPaintingGeom = pPaintingData->fGeometry;


  // pPaintingGeom->clear();
  // pPaintingGeom->setPaints(paints);
  // pPaintingGeom->setBrushes(brushes);








  // MArrayDataHandle hStrokeCurves = data.inputValue(aStrokeCurves, &st); ert;
  // unsigned nCurves = hStrokeCurves.elementCount();



  // for (unsigned i = 0; i < nCurves; i++, hStrokeCurves.next()) {
  //   // JPMDBG;
  //   short index = short(hStrokeCurves.elementIndex(&st));

  //   MDataHandle hStrokeCurve = hStrokeCurves.inputValue(&st );
  //   MObject dStrokeCurve = hStrokeCurve.data();
  //   MFnPluginData fnStrokeCurves( dStrokeCurve , &st); ert;
  //   strokeCurveData *scData = (strokeCurveData *)fnStrokeCurves.data();
  //   strokeCurveGeom *geom = scData->fGeometry;
  //   // strokeCurveGeom *pScGeom = 0;
  //   // st = getData(hStrokeCurves, pScGeom);



  //   cerr << "POINTER: " << geom << endl;
  //   cerr << *geom << endl;

  //   // pPaintingGeom->addStrokeCurve(*geom);

  //   // if (geom) {
  //   //   cerr << "num strokes" << geom->m_strokes.size() << endl;
  //   //   cerr << "m_forceDip" << geom->m_forceDip << endl;
  //   //   cerr << "m_brushId" << geom->m_brushId << endl;
  //   //   cerr << "m_paintId" << geom->m_paintId << endl;
  //   // }

  // }





  // cerr << *pPaintingGeom << endl;

  // st = setData(data, painting::aOutTargets, outTargets); er;

  // hOutput.set( pPaintingData );
  // data.setClean( plug );



  // aStrokeRotationTexture
  // aStrokeTranslationTexture
  // aStrokeTranslationSampleDistance
  // sample rotation texture and translation gradient

  // std::vector<Stroke>::const_iterator citer;
  // std::vector<Stroke>::iterator iter;
  // // // JPMDBG;

  // unsigned nStrokes = strokes.size();
  // MFloatArray uVals(nStrokes);
  // MFloatArray vVals(nStrokes);
  // // // JPMDBG;

  // citer = strokes.begin();
  // for (unsigned i = 0; citer != strokes.end(); citer++, i++) {
  //   float &u = uVals[i];
  //   float &v = vVals[i];
  //   citer->getPivotUVs(inversePlaneMatrix, u, v);
  // }
  // MFloatArray rotations;
  // MFloatVectorArray translations;
  // // // JPMDBG;




  // citer = strokes.begin();
  // for (; citer != strokes.end(); citer++) {
  //   const MMatrixArray &targets = citer->targets();
  //   const MVectorArray &tangents = citer->tangents();
  //   const unsigned len = targets.length();

  //   outCounts.append(len);
  //   outBrushIds.append(citer->brush().id);
  //   outPaintIds.append(citer->paint().id);
  //   outArcLengths.append(citer->arcLength());
  //   outBrushWidths.append(citer->brush().width);
  //   const MColor &c = citer->paint().color;
  //   outPaintColors.append(MVector(c.r, c.g, c.b));
  //   outPaintOpacities.append(citer->paint().opacity);
  //   outForceDips.append(citer->forceDip());
  //   outCurveIds.append(citer->curveId());
  //   outApproachStarts.append(citer->approachStart());
  //   outApproachEnds.append(citer->approachEnd());

  //   for (int i = 0; i < len; ++i)
  //   {
  //     outTargets.append(targets[i]);
  //     outTangents.append(tangents[i]);
  //     // outNormals.append(nrm[i]);
  //   }
  // }

  // // JPMDBG;

  // unsigned len = outTargets.length();
  // cerr << "len: " << len << endl;

  // MMatrixArray outTargetsWorld(len);
  // MVectorArray normalsWorld(len);
  // MVectorArray outPosition(len);
  // MVectorArray outRotation(len);

  // MTransformationMatrix::RotationOrder order =  MTransformationMatrix::RotationOrder(
  //       data.inputValue(aRotateOrder).asShort());

  // MAngle::Unit outUnit = MAngle::Unit(data.inputValue(aOutputUnit).asShort());


  // for (int i = 0; i < len; ++i)
  // {
  //   double rotValue[3];
  //   MTransformationMatrix tMat( outTargets[i] * wm );
  //   tMat.reorderRotation(order);

  //   MTransformationMatrix::RotationOrder tmpOrd;
  //   tMat.getRotation( rotValue, tmpOrd );
  //   if (outUnit == MAngle::kDegrees) {
  //     rotValue[0] *= rad_to_deg;
  //     rotValue[1] *= rad_to_deg;
  //     rotValue[2] *= rad_to_deg;
  //   }
  //   outPosition[i] = tMat.getTranslation( MSpace::kWorld);
  //   outRotation[i] = MVector(rotValue[0], rotValue[1], rotValue[2]);
  // }


  // st = setData(data, painting::aOutTangents, outTangents); er;
  // st = setData(data, painting::aOutPosition, outPosition); er;
  // st = setData(data, painting::aOutRotation, outRotation); er;
  // st = setData(data, painting::aOutCounts, outCounts); er;
  // st = setData(data, painting::aOutBrushIds, outBrushIds); er;
  // st = setData(data, painting::aOutPaintIds, outPaintIds); er;
  // st = setData(data, painting::aOutCurveIds, outCurveIds); er;

  // st = setData(data, painting::aOutBrushWidths, outBrushWidths); er;
  // st = setData(data, painting::aOutPaintColors, outPaintColors); er;
  // st = setData(data, painting::aOutPaintOpacities, outPaintOpacities); er;
  // st = setData(data, painting::aOutForceDips, outForceDips); er;
  // st = setData(data, painting::aOutArcLengths, outArcLengths); er;


  // st = setData(data, painting::aOutApproachStarts, outApproachStarts); er;
  // st = setData(data, painting::aOutApproachEnds, outApproachEnds); er;

  // st = setData(data, painting::aOutPlaneMatrixWorld, outPlaneMatrixWorld); er;


  // MDataHandle hOutMat = data.outputValue(aOutPlaneMatrixWorld);
  // hOutMat.set(outPlaneMatrixWorld);
  // st = data.setClean( aOutPlaneMatrixWorld ); er;



  return MS::kSuccess;

}


void painting::setWireDrawColor(M3dView &view,  M3dView::DisplayStatus status)
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

void painting::drawWireframeTargets(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{
  setWireDrawColor(view, status);

  MObject thisObj = thisMObject();

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
  if (targetDisplayStyle == painting::kTargetsNone) {
    return;
  }

  if (targetDisplayStyle == painting::kTargetsPoint) {
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

  if (targetDisplayStyle == painting::kTargetsLine) {
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



  if (targetDisplayStyle == painting::kTargetsMatrix) {
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
  }
}



void painting::drawWireframeBorders(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{

  setWireDrawColor(view, status);
  MObject thisObj = thisMObject();

  double lineThickness;
  MPlug(thisObj, aLineThickness).getValue(lineThickness);

  bool displayLift;
  MPlug(thisObj, aDisplayLift).getValue(displayLift);

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);
  double stackHeight = 0.0;

  glPushAttrib(GL_LINE_BIT);
  glLineWidth(GLfloat(lineThickness));
  glBegin(GL_LINES);
  for (auto cluster : geom.clusters())
  {
    double brushWidth = geom.brushFromId(cluster.brushId()).width;
    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;

      MFloatPointArray lefts;
      MFloatPointArray rights;

      stroke.getBorders(lefts, rights, brushWidth, displayLift, stackHeight);

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




void painting::drawWireframeApproach(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{

  setWireDrawColor(view, status);
  MObject thisObj = thisMObject();

  /* Don't draw approach if cluster path visible as it will interfere */
  bool doDisplayClusterPath;
  MPlug(thisObj, aDisplayClusterPath).getValue(doDisplayClusterPath);
  if (doDisplayClusterPath ) {
    return;
  }

  bool doDisplayApproach;
  MPlug(thisObj, aDisplayApproach).getValue(doDisplayApproach);
  if (! doDisplayApproach ) {
    return;
  }




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
    double brushWidth = geom.brushFromId(cluster.brushId()).width;
    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;
      /* three points each */
      MFloatPointArray starts;
      MFloatPointArray ends;

      stroke.getApproaches(starts, ends, stackHeight);

      // cerr << starts << endl;
      // cerr << ends << endl;



      glVertex3f( starts[0].x , starts[0].y , starts[0].z );
      glVertex3f( starts[1].x , starts[1].y , starts[1].z );

      glVertex3f( starts[1].x , starts[1].y , starts[1].z );
      glVertex3f( starts[2].x , starts[2].y , starts[2].z );

      glVertex3f( ends[0].x , ends[0].y , ends[0].z );
      glVertex3f( ends[1].x , ends[1].y , ends[1].z );

      glVertex3f( ends[1].x , ends[1].y , ends[1].z );
      glVertex3f( ends[2].x , ends[2].y , ends[2].z );

    }
  }
  glEnd();
  glPopAttrib();
}



void painting::drawWireframeClusterPath(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{

  // view.setDrawColor( RED_COLOR, M3dView::kActiveColors );
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
      stroke.getFullPath(points, stackHeight);
      unsigned len = points.length();
      for (int i = 0; i < len; ++i)
      {
        glVertex3f( points[i].x , points[i].y , points[i].z );
      }
    }
    glEnd();
  }
  glPopAttrib();
}




void painting::drawWireframe(const paintingGeom &geom, M3dView &view,
                             const MDagPath &path,
                             M3dView:: DisplayStatus status ) {

  view.beginGL();
  drawWireframeTargets(geom, view, path, status);
  drawWireframeBorders(geom, view, path, status);
  drawWireframeApproach(geom, view, path, status);
  drawWireframeClusterPath(geom, view, path, status);




  view.endGL();


}



//   glPushAttrib(GL_CURRENT_BIT);
//   glEnable(GL_BLEND);
//   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//   unsigned i = 0;
//   for (int j = 0; j < numStrokes; ++j)
//   {
//     unsigned numPoints = counts[j];
//     double width = brushWidths[j] * 0.5;
//     const MVector    &pcv = paintColors[j];


//     float opac = float(paintOpacities[j]);
//     MColor pcolor = MColor(pcv.x, pcv.y, pcv.z, opac);

//     glColor4f(pcolor.r , pcolor.g , pcolor.b, opac);

//     glBegin(GL_TRIANGLE_STRIP);

//     unsigned first = 0;
//     unsigned end = numPoints;

//     if (! doDisplayBrushLift) {i++;  numPoints -= 2;}

//     for (int k = 0; k < numPoints ; ++k)
//     {
//       MVector side = (tangents[i] ^ planeNormal).normal() * width;


//       MFloatVector a(targets[i][3][0] + side.x, targets[i][3][1] + side.y,
//                      targets[i][3][2] + side.z);
//       MFloatVector b(targets[i][3][0] - side.x, targets[i][3][1] - side.y,
//                      targets[i][3][2] - side.z);

//       glVertex3f( a.x , a.y , a.z );
//       glVertex3f( b.x , b.y, b.z);
//       i++;
//     }

//     if (! doDisplayBrushLift) { i++; }

void painting::drawShaded(const paintingGeom &geom, M3dView &view, const MDagPath &path,
                          M3dView:: DisplayStatus status ) {



  MObject thisObj = thisMObject();

  bool displayLift;
  MPlug(thisObj, aDisplayLift).getValue(displayLift);

  double stackGap;
  MPlug(thisObj, aStackGap).getValue(stackGap);
  double stackHeight = 0.0;


  glPushAttrib(GL_CURRENT_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (auto cluster : geom.clusters())
  {
    double brushWidth = geom.brushFromId(cluster.brushId()).width;
    MColor color = geom.paintFromId(cluster.paintId()).color;
    glColor4f(color.r , color.g , color.b, color.a);


    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;

      MFloatPointArray lefts;
      MFloatPointArray rights;

      stroke.getBorders(lefts, rights, brushWidth, displayLift, stackHeight);

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
void painting::draw( M3dView &view,
                     const MDagPath &path,
                     M3dView::DisplayStyle style,
                     M3dView:: DisplayStatus status  )
{

  // cerr << "painting::DRAW" << endl;
  MStatus st;
  // JPMDBG;
  MObject thisObj = thisMObject();

  //   MPlug pointSizePlug (thisObj, aPointSize);
  //   double pointSize;
  //   pointSizePlug.getValue(pointSize);

  // COMMENTED PAINTING DATA
  // paintingData *ptd = 0;
  MPlug plugPaintingData( thisObj, aOutput );
  MObject dPaintingData;
  st = plugPaintingData.getValue(dPaintingData);
  MFnPluginData fnPaintingData(dPaintingData);
  paintingData *ptd  =  (paintingData *)fnPaintingData.data();
  if (! ptd)  {
    // cerr << "ptd: " << ptd << endl;
    return;
  }



  paintingGeom *pGeom = ptd->geometry();
  if (! (pGeom && pGeom->clusters().size())) {
    return;
  }
  const paintingGeom &geom = *pGeom;


  // drawWireframe(geom, view, path, status);


  if (style ==  M3dView::kWireFrame ) {
    drawWireframe(geom, view, path, status);
  }
  else if ((style ==  M3dView::kGouraudShaded) || (style ==  M3dView::kFlatShaded)) {
    drawShaded(geom, view, path, status);
    bool wfos = view.wireframeOnShaded() ;
    if (status == M3dView::kActive || status == M3dView::kLead
        || status == M3dView::kHilite || wfos ) {
      drawWireframe(geom, view, path, status);
    }
  }



  // cerr << "draw: pGeom " << pGeom << endl;


  /*
  paintingData *pPaintingData = (paintingData *)fnPaintingData.data(&st); er;
  paintingGeom *pPaintingGeom = pPaintingData->fGeometry;



  */



  MFnMatrixArrayData fnXA;

  // MFnVectorArrayData fnV;
  // MFnDoubleArrayData fnD;
  // MFnIntArrayData fnI;
  // cerr << "HERE draw" << endl;
  // JPMDBG;

  MFnMatrixData fnX;
  MObject dX;
  MPlug plugX( thisObj, aPlaneMatrix );
  st = plugX.getValue(dX);
  MMatrix pmat;
  if (st.error()) {
    pmat.setToIdentity();
  }
  else {
    fnX.setObject(dX);
    pmat = fnX.matrix(&st);
    if (st.error()) {
      pmat.setToIdentity();
    }
  }
  MVector planeNormal = (MVector::zAxis * pmat).normal();



  // MMatrix ipmat = pmat.inverse();
  // JPMDBG;

  // get sample positions from output
  // MPlug targetsPlug(thisObj, aOutTargets);
  // MObject dTargets;
  // st = targetsPlug.getValue(dTargets); er;
  // fnXA.setObject(dTargets);
  // MMatrixArray tmpTargets = fnXA.array(&st); er;
  // unsigned pl = tmpTargets.length();
  // JPMDBG;

  // double stackGap;
  // MPlug(thisObj, aStackGap).getValue(stackGap);
  // bool doStack = stackGap > 0;

  // MMatrixArray targets(tmpTargets);
  // JPMDBG;

  // MPlug tangentsPlug(thisObj, aOutTangents);
  // MObject dTangents;
  // st = tangentsPlug.getValue(dTangents); er;
  // fnV.setObject(dTangents);
  // MVectorArray tangents = fnV.array(&st); er;

  // MPlug countsPlug(thisObj, aOutCounts);
  // MObject dCounts;
  // st = countsPlug.getValue(dCounts); er;
  // fnI.setObject(dCounts);
  // MIntArray counts = fnI.array(&st); er;
  // unsigned numStrokes = counts.length();

  // MPlug widthsPlug(thisObj, aOutBrushWidths);
  // MObject dBrushWidths;
  // st = widthsPlug.getValue(dBrushWidths); er;
  // fnD.setObject(dBrushWidths);
  // MDoubleArray brushWidths = fnD.array(&st); er;

  // MPlug paintColorsPlug(thisObj, aOutPaintColors);
  // MObject dPaintColors;
  // st = paintColorsPlug.getValue(dPaintColors); er;
  // fnV.setObject(dPaintColors);
  // MVectorArray paintColors = fnV.array(&st); er;

  // MPlug paintOpacitiesPlug(thisObj, aOutPaintOpacities);
  // MObject dPaintOpacities;
  // st = paintOpacitiesPlug.getValue(dPaintOpacities); er;
  // fnD.setObject(dPaintOpacities);
  // MDoubleArray paintOpacities = fnD.array(&st); er;





  // if (stackGap > 0) {
  //   MMatrixArray stackTargets(pl);
  //   unsigned i = 0;
  //   for (int j = 0; j < numStrokes; ++j)
  //   {
  //     MVector z(planeNormal * stackGap * j);
  //     unsigned numPoints = counts[j];
  //     for (int k = 0; k < numPoints ; ++k)
  //     {
  //       stackTargets[i] = tmpTargets[i];
  //       stackTargets[i][3][0] = stackTargets[i][3][0] + z.x;
  //       stackTargets[i][3][1] = stackTargets[i][3][1] + z.y;
  //       stackTargets[i][3][2] = stackTargets[i][3][2] + z.z;
  //       i++;
  //     }
  //   }
  //   targets = stackTargets;
  // }
  // else {
  //   targets = tmpTargets;
  // }

  // bool doDisplayPoints;
  // bool doDisplayIds;
  // bool doDisplaySegments;
  // bool doDisplayBrushLift;
  // bool doDisplayApproach;

  // MPlug(thisObj, aDisplayPoints).getValue(doDisplayPoints);
  // MPlug(thisObj, aDisplayIds).getValue(doDisplayIds);

  // MPlug(thisObj, aDisplaySegments).getValue(doDisplaySegments);
  // MPlug(thisObj, aDisplayBrushLift).getValue(doDisplayBrushLift);
  // MPlug(thisObj, aDisplayApproach).getValue(doDisplayApproach);

  // short brushDisp;
  // MPlug(thisObj, aDisplayBrush).getValue(brushDisp);

  // short displaySegmentOutlines;
  // MPlug(thisObj, aDisplaySegmentOutlines).getValue(displaySegmentOutlines);
  // OutlineDisplay od = OutlineDisplay(displaySegmentOutlines);

  // bool doArrows = od == painting::kOutlinesArrows
  //                 || od == painting::kOutlinesBoth;
  // bool doBorders = od == painting::kOutlinesBorders
  //                  || od == painting::kOutlinesBoth;

  // MPlug wireColorPlug (thisObj, aWireColor);
  // float wireColorRed, wireColorGreen, wireColorBlue;
  // wireColorPlug.child(0).getValue(wireColorRed);
  // wireColorPlug.child(1).getValue(wireColorGreen);
  // wireColorPlug.child(2).getValue(wireColorBlue);

  // MColor wireColor( MColor::kRGB, wireColorRed, wireColorGreen, wireColorBlue);

  // double thickness;
  // MPlug(thisObj, aSegmentOutlineThickness).getValue(thickness);


  // double approachDistanceStart, approachDistanceMid, approachDistanceEnd;
  // MPlug(thisObj, aStrokeApproachDistanceStart).getValue(approachDistanceStart);
  // MPlug(thisObj, aStrokeApproachDistanceMid).getValue(approachDistanceMid);
  // MPlug(thisObj, aStrokeApproachDistanceEnd).getValue(approachDistanceEnd);



  // JPMDBG;


  view.beginGL();


  // // POINTS
  // if (doDisplayPoints)
  // {
  //   MPlug pointSizePlug (thisObj, aPointSize);
  //   double pointSize;
  //   pointSizePlug.getValue(pointSize);

  //   view.setDrawColor(wireColor );

  //   glPushAttrib(GL_CURRENT_BIT);
  //   glPointSize(float(pointSize));

  //   glBegin( GL_POINTS );

  //   unsigned i = 0;
  //   for (int j = 0; j < numStrokes; ++j)
  //   {
  //     unsigned numPoints = counts[j];
  //     if (! doDisplayBrushLift) {i++;  numPoints -= 2;}
  //     for (int k = 0; k < numPoints ; ++k)
  //     {
  //       glVertex3f(float(targets[i][3][0]), float(targets[i][3][1]), float(targets[i][3][2]));
  //       i++;
  //     }
  //     if (! doDisplayBrushLift) { i++; }
  //   }

  //   glEnd();
  //   glPopAttrib();
  // }

  // // SEGMENTS
  // if (doDisplaySegments)
  // {

  //   glPushAttrib(GL_CURRENT_BIT);

  //   glEnable(GL_BLEND);
  //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //   unsigned i = 0;
  //   for (int j = 0; j < numStrokes; ++j)
  //   {
  //     unsigned numPoints = counts[j];
  //     double width = brushWidths[j] * 0.5;
  //     const MVector    &pcv = paintColors[j];


  //     float opac = float(paintOpacities[j]);
  //     MColor pcolor = MColor(pcv.x, pcv.y, pcv.z, opac);

  //     glColor4f(pcolor.r , pcolor.g , pcolor.b, opac);

  //     glBegin(GL_TRIANGLE_STRIP);

  //     unsigned first = 0;
  //     unsigned end = numPoints;

  //     if (! doDisplayBrushLift) {i++;  numPoints -= 2;}

  //     for (int k = 0; k < numPoints ; ++k)
  //     {
  //       MVector side = (tangents[i] ^ planeNormal).normal() * width;


  //       MFloatVector a(targets[i][3][0] + side.x, targets[i][3][1] + side.y,
  //                      targets[i][3][2] + side.z);
  //       MFloatVector b(targets[i][3][0] - side.x, targets[i][3][1] - side.y,
  //                      targets[i][3][2] - side.z);

  //       glVertex3f( a.x , a.y , a.z );
  //       glVertex3f( b.x , b.y, b.z);
  //       i++;
  //     }

  //     if (! doDisplayBrushLift) { i++; }

  //     glEnd();
  //   }
  //   glPopAttrib();
  // }




  // if (doDisplayApproach) {

  //   MDoubleArray approachStarts, approachEnds;
  //   getData( painting::aOutApproachStarts, approachStarts);
  //   getData( painting::aOutApproachEnds, approachEnds);

  //   // MFloatVector approachVec = MFloatVector(planeNormal * approachDistance);

  //   // cerr << "approachVec length : " << approachVec.length() << endl;
  //   glPushAttrib(GL_LINE_BIT);

  //   unsigned i = 0;


  //   view.setDrawColor(wireColor );


  //   glLineWidth(GLfloat(thickness));
  //   glBegin(GL_LINES);

  //   for (int j = 0; j < numStrokes; ++j)
  //   {

  //     MVector approachVec;
  //     approachVec = MFloatVector(planeNormal *  approachStarts[j] ); ;
  //     // MVector approachEnd = MFloatVector(planeNormal * approachEnds[j] ); ;


  //     unsigned numPoints = counts[j];
  //     MFloatVector lift, stroke, approach;


  //     // start
  //     lift = MFloatVector(targets[i][3][0], targets[i][3][1], targets[i][3][2]);
  //     stroke = MFloatVector(targets[(i + 1)][3][0], targets[(i + 1)][3][1],
  //                           targets[(i + 1)][3][2]);
  //     approach = MFloatVector(lift + approachVec);

  //     glVertex3f(lift.x , lift.y, lift.z);
  //     glVertex3f(approach.x , approach.y , approach.z);

  //     glVertex3f(stroke.x , stroke.y, stroke.z);
  //     glVertex3f(lift.x , lift.y , lift.z);

  //     i += numPoints - 1;


  //     approachVec = MFloatVector(planeNormal *  approachEnds[j] ); ;

  //     lift = MFloatVector(targets[i][3][0], targets[i][3][1], targets[i][3][2]);
  //     stroke = MFloatVector(targets[(i - 1)][3][0], targets[(i - 1)][3][1],
  //                           targets[(i - 1)][3][2]);
  //     approach = MFloatVector(lift + approachVec);

  //     glVertex3f(lift.x , lift.y, lift.z);
  //     glVertex3f(approach.x , approach.y , approach.z);

  //     glVertex3f(stroke.x , stroke.y, stroke.z);
  //     glVertex3f(lift.x , lift.y , lift.z);

  //     i++;

  //   }
  //   glEnd();
  //   glPopAttrib();
  // }

  // // SEGMENT OUITLINES
  // if (doArrows || doBorders)
  // {


  //   glPushAttrib(GL_LINE_BIT);

  //   unsigned i = 0;
  //   for (int j = 0; j < numStrokes; ++j)
  //   {
  //     unsigned numPoints = counts[j];
  //     double width = brushWidths[j] * 0.5;
  //     view.setDrawColor(wireColor );

  //     glLineWidth(GLfloat(thickness));
  //     glBegin(GL_LINES);

  //     MFloatVector last_a;
  //     MFloatVector last_b;

  //     if (! doDisplayBrushLift) {i++;  numPoints -= 2;}

  //     for (int k = 0; k < numPoints ; ++k)
  //     {
  //       MVector side = (tangents[i] ^ planeNormal).normal() * width;

  //       MFloatVector p(targets[i][3][0], targets[i][3][1], targets[i][3][2]);
  //       MFloatVector a(p + side);
  //       MFloatVector b(p - side);

  //       if (! doArrows) {
  //         // draw the ends unless arrows
  //         if (k == 0 || k == (numPoints - 1)) {
  //           glVertex3f(a.x , a.y, a.z);
  //           glVertex3f(b.x , b.y , b.z);
  //         }
  //       }
  //       if (k > 0) {
  //         if (doBorders) {
  //           glVertex3f( last_a.x , last_a.y, last_a.z);
  //           glVertex3f( a.x , a.y , a.z );
  //           glVertex3f( last_b.x , last_b.y, last_b.z);
  //           glVertex3f( b.x , b.y , b.z );

  //         }
  //         if (doArrows)  {
  //           glVertex3f( last_a.x , last_a.y, last_a.z);
  //           glVertex3f( p.x , p.y , p.z );
  //           glVertex3f( last_b.x , last_b.y, last_b.z);
  //           glVertex3f( p.x , p.y , p.z );
  //         }
  //       }


  //       last_a = a;
  //       last_b = b;
  //       i++;
  //     }
  //     if (! doDisplayBrushLift) { i++; }

  //     glEnd();
  //   }
  //   glPopAttrib();
  // }




  // if ( BrushDisplay(brushDisp) == painting::kBrushLine)
  // {

  //   double lineLength;
  //   MPlug(thisObj, aNormalLength).getValue(lineLength);

  //   view.setDrawColor(MColor(0.0, 0.0, 1.0) );

  //   glPushAttrib(GL_LINE_BIT);
  //   glLineWidth(GLfloat(thickness));
  //   glBegin( GL_LINES );


  //   unsigned i = 0;
  //   for (int j = 0; j < numStrokes; ++j)
  //   {
  //     unsigned numPoints = counts[j];
  //     if (! doDisplayBrushLift) {i++;  numPoints -= 2;}
  //     for (int k = 0; k < numPoints ; ++k)
  //     {
  //       MFloatVector start(targets[i][3][0], targets[i][3][1], targets[i][3][2]);
  //       // MFloatVector end =  MFloatVector::zAxis;
  //       MFloatVector end( MFloatVector( MVector::zNegAxis * lineLength * targets[i]));
  //       end += start;

  //       glVertex3f( start.x , start.y , start.z );
  //       glVertex3f( end.x , end.y, end.z);
  //       i++;
  //     }
  //     if (! doDisplayBrushLift) { i++; }
  //   }

  //   glEnd();
  //   glPopAttrib();
  // }



  // if ( BrushDisplay(brushDisp) == painting::kBrushMatrix)
  // {

  //   double lineLength;
  //   MPlug(thisObj, aNormalLength).getValue(lineLength);

  //   // view.setDrawColor(wireColor );

  //   glPushAttrib(GL_LINE_BIT);
  //   glLineWidth(GLfloat(thickness));
  //   glBegin( GL_LINES );


  //   unsigned i = 0;
  //   for (int j = 0; j < numStrokes; ++j)
  //   {
  //     unsigned numPoints = counts[j];
  //     if (! doDisplayBrushLift) {i++;  numPoints -= 2;}
  //     for (int k = 0; k < numPoints ; ++k)
  //     {
  //       MFloatVector start(targets[i][3][0], targets[i][3][1], targets[i][3][2]);

  //       MFloatVector endX( MFloatVector(MVector::xAxis * lineLength  * targets[i]) )   ;
  //       MFloatVector endY(  MFloatVector(MVector::yAxis * lineLength  * targets[i]) )   ;
  //       MFloatVector endZ(  MFloatVector(MVector::zAxis * lineLength  * targets[i]) )   ;
  //       endX += start;
  //       endY += start;
  //       endZ += start;


  //       glColor3f(1.0f , 0.0f, 0.0f );
  //       glVertex3f( start.x , start.y , start.z );
  //       glVertex3f( endX.x , endX.y, endX.z);

  //       glColor3f(0.0f , 1.0f, 0.0f );
  //       glVertex3f( start.x , start.y , start.z );
  //       glVertex3f( endY.x , endY.y, endY.z);

  //       glColor3f(0.0f , 0.0f, 1.0f );
  //       glVertex3f( start.x , start.y , start.z );
  //       glVertex3f( endZ.x , endZ.y, endZ.z);
  //       i++;
  //     }
  //     if (! doDisplayBrushLift) { i++; }
  //   }

  //   glEnd();
  //   glPopAttrib();
  // }

  // // IDs
  // if (doDisplayIds)
  // {
  //   glPushAttrib(GL_CURRENT_BIT);

  //   view.setDrawColor(wireColor );
  //   unsigned i = 0;
  //   for (int j = 0; j < numStrokes; ++j)
  //   {
  //     int k = i + 1;
  //     MString txt  = "";
  //     txt += j;
  //     MPoint textPos =  MPoint( targets[k][3][0], targets[k][3][1], targets[k][3][2]);
  //     view.drawText( txt , textPos,  M3dView::kRight);
  //     i +=  counts[j];
  //   }
  //   glPopAttrib();

  // }
  // JPMDBG;

  view.endGL();

}

bool painting::isBounded() const
{
  return false;
}

MBoundingBox painting::boundingBox() const
{
  return MBoundingBox();
  //   MStatus st;

  //   MObject thisObj = thisMObject();

  //   MFnMatrixData fnX;

  //   MObject dX;
  //   MPlug plugX( thisObj, aPlaneMatrix );
  //   st = plugX.getValue(dX);
  //   MMatrix pmat;
  //   if (st.error()) {
  //     pmat.setToIdentity();
  //   }
  //   else {
  //     fnX.setObject(dX);
  //     pmat = fnX.matrix(&st);
  //     if (st.error()) {
  //       pmat.setToIdentity();
  //     }
  //   }
  //   MVector planeNormal = (MVector::zAxis * pmat).normal();

  //   MFnMatrixArrayData fnXA;
  //   MFnIntArrayData fnI;
  //   // get sample positions from output
  //   MPlug targetsPlug(thisObj, aOutTargets);
  //   MObject dTargets;
  //   st = targetsPlug.getValue(dTargets); er;
  //   fnXA.setObject(dTargets);
  //   MMatrixArray targets = fnXA.array(&st); er;
  //   unsigned pl = targets.length();
  //   if (! pl ) {
  //     return MBoundingBox();
  //   }

  //   double stackGap;
  //   MPlug(thisObj, aStackGap).getValue(stackGap);

  //   MPlug countsPlug(thisObj, aOutCounts);
  //   MObject dCounts;
  //   st = countsPlug.getValue(dCounts); er;
  //   fnI.setObject(dCounts);
  //   MIntArray counts = fnI.array(&st); er;
  //   unsigned numStrokes = counts.length();


  //   MVector zn(planeNormal * stackGap);
  //   MVector z = MPoint(targets[0][3][0], targets[0][3][1], targets[0][3][2]);
  //   MBoundingBox bb(z, z);

  //   unsigned i = 0;
  //   for (int j = 0; j < numStrokes; ++j)
  //   {
  //     MVector zj(zn * j);
  //     unsigned numPoints = counts[j];
  //     z = MPoint(targets[i][3][0], targets[i][3][1], targets[i][3][2]) + zj;
  //     bb.expand(z);
  //     i += numPoints - 1;
  //     z = MPoint(targets[i][3][0], targets[i][3][1], targets[i][3][2]) + zj;
  //     bb.expand(z);
  //     i++;
  //   }

  //   return bb;
}

void painting::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());
  nodeFn.setName("paintingShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}



