
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <utility>

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
#include "strokeGeometryData.h"
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
const int RED_COLOR             = 12;



// eAttr.addField("brushUpPaintUp", painting::kBrushAscPaintAsc);
// eAttr.addField("brushUpPaintDown", painting::kBrushAscPaintDesc);
// eAttr.addField("brushDownPaintUp", painting::kBrushDescPaintAsc);
// eAttr.addField("brushDownPaintDown", paintig::kBrushDescPaintDesc);
// eAttr.addField("paintUpBrushUp", painting::kPaintAscBrushAsc);
// eAttr.addField("paintUpBrushDown", painting::kPaintAscBrushDesc);
// eAttr.addField("paintDownBrushUp", painting::kPaintDescBrushAsc);
// eAttr.addField("paintDownBrushDown", painting::kPaintDescBrushDesc);

class StrokeCompare
{
public:
  StrokeCompare(painting::StrokeSort sortOrder = painting::kBrushAscPaintAsc)
    : m_sortOrder(sortOrder)
  {}

  bool operator()(const strokeGeom &a, const strokeGeom &b) {
    if (m_sortOrder ==  painting::kBrushAscPaintAsc) {
      if (a.brushId()  < b.brushId() ) { return true; }
      if (b.brushId()  < a.brushId() ) { return false; }
      if (a.paintId()  < b.paintId() ) { return true; }
      if (b.paintId()  < a.paintId() ) { return false; }
      if (a.id()  < b.id() ) { return true; }
      if (b.id()  < a.id() ) { return false; }
      return false;
    }

    if (m_sortOrder ==  painting::kBrushAscPaintDesc) {
      if (a.brushId()  < b.brushId() ) { return true; }
      if (b.brushId()  < a.brushId() ) { return false; }
      if (a.paintId()  > b.paintId() ) { return true; }
      if (b.paintId()  > a.paintId() ) { return false; }
      if (a.id()  < b.id() ) { return true; }
      if (b.id()  < a.id() ) { return false; }
      return false;
    }

    if (m_sortOrder ==  painting::kBrushDescPaintAsc) {
      if (a.brushId()  > b.brushId() ) { return true; }
      if (b.brushId()  > a.brushId() ) { return false; }
      if (a.paintId()  < b.paintId() ) { return true; }
      if (b.paintId()  < a.paintId() ) { return false; }
      if (a.id()  < b.id() ) { return true; }
      if (b.id()  < a.id() ) { return false; }
      return false;
    }

    if (m_sortOrder ==  painting::kBrushDescPaintDesc) {
      if (a.brushId()  > b.brushId() ) { return true; }
      if (b.brushId()  > a.brushId() ) { return false; }
      if (a.paintId()  > b.paintId() ) { return true; }
      if (b.paintId()  > a.paintId() ) { return false; }
      if (a.id()  < b.id() ) { return true; }
      if (b.id()  < a.id() ) { return false; }
      return false;
    }

    if (m_sortOrder ==  painting::kPaintAscBrushAsc) {
      if (a.paintId()  < b.paintId() ) { return true; }
      if (b.paintId()  < a.paintId() ) { return false; }
      if (a.brushId()  < b.brushId() ) { return true; }
      if (b.brushId()  < a.brushId() ) { return false; }
      if (a.id()  < b.id() ) { return true; }
      if (b.id()  < a.id() ) { return false; }
      return false;
    }

    if (m_sortOrder ==  painting::kPaintAscBrushDesc) {
      if (a.paintId()  < b.paintId() ) { return true; }
      if (b.paintId()  < a.paintId() ) { return false; }
      if (a.brushId()  > b.brushId() ) { return true; }
      if (b.brushId()  > a.brushId() ) { return false; }
      if (a.id()  < b.id() ) { return true; }
      if (b.id()  < a.id() ) { return false; }
      return false;
    }

    if (m_sortOrder ==  painting::kPaintDescBrushAsc) {
      if (a.paintId()  > b.paintId() ) { return true; }
      if (b.paintId()  > a.paintId() ) { return false; }
      if (a.brushId()  < b.brushId() ) { return true; }
      if (b.brushId()  < a.brushId() ) { return false; }
      if (a.id()  < b.id() ) { return true; }
      if (b.id()  < a.id() ) { return false; }
      return false;
    }

    if (m_sortOrder ==  painting::kPaintDescBrushDesc) {
      if (a.paintId()  > b.paintId() ) { return true; }
      if (b.paintId()  > a.paintId() ) { return false; }
      if (a.brushId()  > b.brushId() ) { return true; }
      if (b.brushId()  > a.brushId() ) { return false; }
      if (a.id()  < b.id() ) { return true; }
      if (b.id()  < a.id() ) { return false; }
      return false;
    }
    return false;
  }

private:
  painting::StrokeSort m_sortOrder;
};



// struct comp_struct {
//   bool operator() (const strokeGeom &a, const strokeGeom &b) {
//     if (a.paintId()  < b.paintId() ) { return true; }
//     if (b.paintId()  < a.paintId() ) { return false; }
//     if (a.brushId()  < b.brushId() ) { return true; }
//     if (b.brushId()  < a.brushId() ) { return false; }
//     return false;
//   }
// } compare_strokes;




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

void *painting::creator() {
  return new painting();
}

const double epsilon = 0.0001;

MObject painting::aPlaneMatrix;
MObject painting::aInMatrix;

MObject painting::aStrokeCurves;
MObject painting::aRotateOrder;
MObject painting::aOutputUnit;


MObject painting::aBrushIdTexture;
MObject painting::aPaintIdTexture;
MObject painting::aBrushIdTextureRange;
MObject painting::aPaintIdTextureRange;
MObject painting::aStrokeSort;
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
// MObject painting::aOutTargets; // local
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

  // // APPROACH OBJECTS
  // aDipApproachObject =  msgAttr.create("dipApproachObject", "dao");
  // addAttribute(aDipApproachObject);
  // aToolChangeApproachObject =  msgAttr.create("toolChangeApproachObject", "tcao");
  // addAttribute(aToolChangeApproachObject);
  // aHomeApproachObject =  msgAttr.create("homeApproachObject", "hmao");
  // addAttribute(aHomeApproachObject);

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


  aStrokeCurves = tAttr.create( "strokeCurves", "scrvs", strokeGeometryData::id );
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aStrokeCurves);

  aBrushIdTexture = nAttr.create( "brushIdTexture", "bidt",
                                  MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aBrushIdTexture);

  aPaintIdTexture = nAttr.create( "paintIdTexture", "pidt",
                                  MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aPaintIdTexture);

  aBrushIdTextureRange = nAttr.create( "brushIdTextureRange", "bitr",
                                       MFnNumericData::kShort);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aBrushIdTextureRange);

  aPaintIdTextureRange = nAttr.create( "paintIdTextureRange", "pitr",
                                       MFnNumericData::kShort);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aPaintIdTextureRange);


  aStrokeSort = eAttr.create("strokeSort", "stst", painting::kNoSort);
  eAttr.addField("NoSort", painting::kNoSort);
  eAttr.addField("brushUpPaintUp", painting::kBrushAscPaintAsc);
  eAttr.addField("brushUpPaintDown", painting::kBrushAscPaintDesc);
  eAttr.addField("brushDownPaintUp", painting::kBrushDescPaintAsc);
  eAttr.addField("brushDownPaintDown", painting::kBrushDescPaintDesc);
  eAttr.addField("paintUpBrushUp", painting::kPaintAscBrushAsc);
  eAttr.addField("paintUpBrushDown", painting::kPaintAscBrushDesc);
  eAttr.addField("paintDownBrushUp", painting::kPaintDescBrushAsc);
  eAttr.addField("paintDownBrushDown", painting::kPaintDescBrushDesc);
  eAttr.setKeyable(true);
  eAttr.setStorable(true);
  eAttr.setHidden(false);
  st = addAttribute( aStrokeSort ); er;

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

  // aOutTargets = tAttr.create("outTargets", "otg", MFnData::kMatrixArray, &st);
  // er;
  // tAttr.setStorable(false);
  // tAttr.setReadable(true);
  // st = addAttribute(aOutTargets ); er;


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


  st = attributeAffects(aPlaneMatrix, aOutput);
  st = attributeAffects(aStrokeCurves, aOutput);
  st = attributeAffects(aLinearSpeed, aOutput);
  st = attributeAffects(aAngularSpeed, aOutput);
  st = attributeAffects(aApproximationDistance, aOutput);
  st = attributeAffects(aBrushes, aOutput);
  st = attributeAffects(aPaints, aOutput);
  st = attributeAffects(aBrushIdTexture, aOutput);
  st = attributeAffects(aPaintIdTexture, aOutput);
  st = attributeAffects(aBrushIdTextureRange, aOutput);
  st = attributeAffects(aPaintIdTextureRange, aOutput);
  st = attributeAffects(aStrokeSort, aOutput);

  return ( MS::kSuccess );

}


MStatus painting::compute( const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  MString method("painting::compute");
  if (plug != aOutput  ) { return ( MS::kUnknownParameter ); }


  MDataHandle mh = data.inputValue(aInMatrix, &st); er;
  MMatrix wm = mh.asMatrix();
  MMatrix planeMatrix = data.inputValue(painting::aPlaneMatrix).asMatrix();
  MMatrix inversePlaneMatrix = planeMatrix.inverse();
  MVector planeNormal = (MVector::zAxis * planeMatrix).normal();

  painting::StrokeSort sortOrder = painting::StrokeSort(data.inputValue(
                                     aStrokeSort).asShort());

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

  m_pd->create();
  paintingGeom *pGeom = m_pd->geometry();
  pGeom->setBrushes(brushes);
  pGeom->setPaints(paints);


  std::vector<strokeGeom> strokePool;
  populateStrokePool(data,  strokePool);


  std::vector<strokeGeom>::const_iterator citer;
  std::vector<strokeGeom>::iterator iter;

  unsigned nStrokes = strokePool.size();
  MFloatArray uVals(nStrokes);
  MFloatArray vVals(nStrokes);

  citer = strokePool.begin();
  for (unsigned i = 0; citer != strokePool.end(); citer++, i++) {
    float &u = uVals[i];
    float &v = vVals[i];
    citer->getPivotUVs(inversePlaneMatrix, u, v);
  }


  MIntArray brushIds;
  MIntArray paintIds;
  short rangeBrushId = data.inputValue(aBrushIdTextureRange).asShort();
  short rangePaintId = data.inputValue(aPaintIdTextureRange).asShort();

  MFloatVectorArray translations;
  st = sampleUVTexture(painting::aBrushIdTexture, uVals, vVals, brushIds, rangeBrushId);
  if (! st.error()) {
    iter = strokePool.begin();
    for (unsigned i = 0; iter != strokePool.end(); iter++, i++) {
      iter->setBrushId(short(brushIds[i]));
    }
  }
  st = sampleUVTexture(painting::aPaintIdTexture, uVals, vVals, paintIds, rangePaintId);
  if (! st.error()) {
    iter = strokePool.begin();
    for (unsigned i = 0; iter != strokePool.end(); iter++, i++) {
      iter->setPaintId(short(paintIds[i]));
    }
  }


  if (sortOrder !=  painting::kNoSort)
  {
    std::sort(strokePool.begin(), strokePool.end(), StrokeCompare(sortOrder));
  }

  for (citer = strokePool.begin(); citer != strokePool.end(); citer++) {
    pGeom->addStroke(*citer);
  }



  MFnPluginData fnOut;
  MTypeId kdid(paintingData::id);
  MObject dOut = fnOut.create(kdid , &st ); er;
  paintingData *outGeometryData = (paintingData *)fnOut.data(&st); er;
  if (m_pd) {
    *outGeometryData = (*m_pd);
  }


  MDataHandle outputHandle = data.outputValue(aOutput, &st ); er;
  st = outputHandle.set(outGeometryData); er;
  data.setClean( plug );


  return MS::kSuccess;

}



MStatus painting::populateStrokePool(MDataBlock &data,
                                     std::vector<strokeGeom> &strokePool)
{
  MStatus st;
  MArrayDataHandle hStrokeCurves = data.inputValue(aStrokeCurves, &st); ert;
  unsigned nCurves = hStrokeCurves.elementCount();
  unsigned j = 0;
  for (unsigned i = 0; i < nCurves; i++, hStrokeCurves.next()) {
    short index = short(hStrokeCurves.elementIndex(&st));
    MDataHandle hStrokeCurve = hStrokeCurves.inputValue(&st);
    if (st.error()) {
      continue;
    }
    MObject dStrokeCurve = hStrokeCurve.data();
    MFnPluginData fnStrokeCurves( dStrokeCurve , &st);
    if (st.error()) {
      continue;
    }
    strokeGeometryData *scData = (strokeGeometryData *)fnStrokeCurves.data();
    // strokeCurveGeom *scGeom = scData->fGeometry;
    const std::vector<strokeGeom> *scGeom = scData->fGeometry;

    // const std::vector<strokeGeom> &strokes = scGeom->strokes();
    std::vector<strokeGeom>::const_iterator citer;

    for (citer = scGeom->begin(); citer != scGeom->end(); citer++) {
      strokePool.push_back(*citer);
      strokePool.back().setId(j);
      j++;
    }
  }
  return MS::kSuccess;
}

MStatus painting::getTextureName(const MObject &attribute,
                                 MString &name) const {
  MStatus st;
  MPlugArray plugArray;
  MPlug plug(thisMObject(), attribute);
  bool hasConnection = plug.connectedTo(plugArray, 1, 0, &st); ert;
  if (! hasConnection) { return MS::kUnknownParameter; }
  name = plugArray[0].name(&st);
  return MS::kSuccess;
}




MStatus painting::sampleUVTexture(const MObject &attribute,  MFloatArray &uVals,
                                  MFloatArray &vVals, MIntArray &result, short range) const {

  MStatus st;
  MString plugName;
  st = getTextureName(attribute, plugName);
  if (st.error()) {return MS::kFailure; }
  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();
  MFloatVectorArray transparencies;
  MFloatVectorArray colors;

  int n = uVals.length();

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat,
        0, &uVals, &vVals, 0, 0, 0, 0, 0, colors, transparencies); ert;

  result.setLength(n);
  for (int i = 0; i < n; ++i)
  {
    result.set(  int(colors[i].x * float(range)), i);
  }
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

void painting::drawShaded(const paintingGeom &geom, M3dView &view,
                          const MDagPath &path,
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
    drawShaded(geom, view, path, status);
    bool wfos = view.wireframeOnShaded() ;
    if (status == M3dView::kActive || status == M3dView::kLead
        || status == M3dView::kHilite || wfos ) {
      drawWireframe(geom, view, path, status);
    }
  }
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



