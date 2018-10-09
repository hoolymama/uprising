
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

int clamp(int n, int lower, int upper) {
  return std::max(lower, std::min(n, upper));
}


bool StrokeCompare(const strokeGeom &a, const strokeGeom &b)
{

  const MIntArray &lstack = a.sortStack();
  const MIntArray &rstack = b.sortStack();
  int len = lstack.length();
  if (len !=  rstack.length()) {
    return false;
  }
  for (int i = 0; i < len; ++i)
  {
    if (lstack[i] < rstack[i]) {
      return true;
    }
    if (lstack[i] > rstack[i]) {
      return false;
    }
  }
  /*
  If we get this far, then everything is the same.
  However we must conform to strict weak ordering,
  so use the order of creation, comprised of parentId, id, repeatId.
  */

  if (a.parentId() < b.parentId()) {
    return true;
  }
  if (a.parentId() > b.parentId()) {
    return false;
  }


  if (a.id() < b.id()) {
    return true;
  }
  // if (a.id() > b.id()) {
  //   return false;
  // }


  // if (a.repeatId() < b.repeatId()) {
  //   return true;
  // }
  // if (a.repeatId() > b.repeatId()) {
  return false;
  // }

  // return false;
}




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


MObject painting::aBrushIdTexture;
MObject painting::aPaintIdTexture;
MObject painting::aBrushIdTextureRange;
MObject painting::aPaintIdTextureRange;
// MObject painting::aStrokeSort;



MObject painting::aStrokeSortKey;
MObject painting::aStrokeSortDirection;
MObject painting::aStrokeSortList;
MObject painting::aStrokeSortTexture;
MObject painting::aApplySort;

MObject painting::aStrokeFilterKey;
MObject painting::aStrokeFilterOperator;
MObject painting::aStrokeFilterOperand;
MObject painting::aStrokeFilterList;
MObject painting::aStrokeFilterTexture;
MObject painting::aApplyFilters;




MObject painting::aStartFrom;


// MObject painting::aStrokeGate;
MObject painting::aLinearSpeed; // cm/sec
MObject painting::aAngularSpeed; // per sec
MObject painting::aApproximationDistance; // cm
MObject painting::aMaxPointToPointDistance;

MObject painting::aBrushMatrix;
MObject painting::aBrushRetention;
MObject painting::aBrushWidth;
MObject painting::aBrushShape;
MObject painting::aBrushTip;
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


MObject painting::aDisplayTargets;
MObject painting::aDisplayLift;
MObject painting::aDisplayApproach;
MObject painting::aDisplayClusterPath;

MObject painting::aDisplayStops;

MObject painting::aDisplayIds;
MObject painting::aDisplayParentIds;
MObject painting::aDisplayLayerIds;
MObject painting::aDisplayBrushIds;
MObject painting::aDisplayPaintIds;
MObject painting::aDisplayRepeatIds;
MObject painting::aArrowheadSize;

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


  aMaxPointToPointDistance =  nAttr.create( "maxPointToPointDistance", "mxptp",
                              MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(3.0);
  nAttr.setSoftMax(100.00);
  nAttr.setDefault(25.00);
  nAttr.setKeyable(true);
  addAttribute(aMaxPointToPointDistance);

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




  aStrokeSortKey = eAttr.create("strokeSortKey", "stsk", painting::kBrushId);
  eAttr.addField("Id", painting::kId);
  eAttr.addField("Parent Id", painting::kParentId);
  eAttr.addField("Brush Id", painting::kBrushId);
  eAttr.addField("Paint Id", painting::kPaintId);
  eAttr.addField("Repeat Id", painting::kRepeatId);
  eAttr.addField("Map Red", painting::kMapRed);
  eAttr.addField("Map Green", painting::kMapGreen);
  eAttr.addField("Map Blue", painting::kMapBlue);
  eAttr.addField("Layer Id", painting::kLayerId);


  aStrokeSortDirection = eAttr.create("strokeSortDirection", "stsd",
                                      painting::kSortAscending);
  eAttr.addField("Ascending",  painting::kSortAscending);
  eAttr.addField("Descending", painting::kSortDescending);

  aStrokeSortList = cAttr.create("strokeSortList", "stsl");
  cAttr.addChild(aStrokeSortKey);
  cAttr.addChild(aStrokeSortDirection);
  cAttr.setArray(true);
  st = addAttribute( aStrokeSortList ); mser;


  aStrokeSortTexture = nAttr.createColor( "strokeSortTexture", "stst");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aStrokeSortTexture);



  aApplySort = nAttr.create( "applySort", "apst", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aApplySort );


  aStrokeFilterKey = eAttr.create("strokeFilterKey", "stfk", painting::kBrushId);
  eAttr.addField("Id", painting::kId);
  eAttr.addField("Parent Id", painting::kParentId);
  eAttr.addField("Brush Id", painting::kBrushId);
  eAttr.addField("Paint Id", painting::kPaintId);
  eAttr.addField("Repeat Id", painting::kRepeatId);
  eAttr.addField("Map Red", painting::kMapRed);
  eAttr.addField("Map Green", painting::kMapGreen);
  eAttr.addField("Map Blue", painting::kMapBlue);
  eAttr.addField("Layer Id", painting::kLayerId);

  aStrokeFilterOperator = eAttr.create("strokeFilterOperator", "stfop",
                                       strokeGeom::kGreaterThan);
  eAttr.addField(">", strokeGeom::kGreaterThan);
  eAttr.addField("<", strokeGeom::kLessThan);
  eAttr.addField("==", strokeGeom::kEqualTo);

  aStrokeFilterOperand  = nAttr.create("strokeFilterOperand", "stfod",
                                       MFnNumericData::kInt);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );

  aStrokeFilterList = cAttr.create("strokeFilterList", "stfl");
  cAttr.addChild(aStrokeFilterKey);
  cAttr.addChild(aStrokeFilterOperator);
  cAttr.addChild(aStrokeFilterOperand);
  cAttr.setArray(true);
  st = addAttribute( aStrokeFilterList ); mser;


  aStrokeFilterTexture = nAttr.createColor( "strokeFilterTexture", "stft");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aStrokeFilterTexture);


  aStartFrom = nAttr.create("startFrom", "stfm", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  st = addAttribute(aStartFrom);



  aApplyFilters = nAttr.create( "applyFilters", "apfl", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aApplyFilters );

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
  st = addAttribute( aBrushShape ); mser;

  aBrushTip  = nAttr.create("brushTip", "brtp", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.0 );


  aBrushes = cAttr.create("brushes", "bsh");
  cAttr.addChild(aBrushWidth);
  cAttr.addChild(aBrushShape);
  cAttr.addChild(aBrushRetention);
  cAttr.addChild(aBrushTip);
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

  aPaints = cAttr.create("paints", "pts");
  cAttr.addChild(aPaintColor);
  cAttr.addChild(aPaintOpacity);
  cAttr.addChild(aPaintTravel);
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

  aDisplayStops = nAttr.create( "displayStops", "dstp",
                                MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDisplayStops );


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
  // st = attributeAffects(aStrokeSort, aOutput);


  st = attributeAffects(aStrokeSortKey, aOutput);
  st = attributeAffects(aStrokeSortDirection, aOutput);
  st = attributeAffects(aStrokeSortList, aOutput);
  st = attributeAffects(aStrokeSortTexture, aOutput);

  st = attributeAffects(aStrokeFilterKey, aOutput);
  st = attributeAffects(aStrokeFilterOperator, aOutput);
  st = attributeAffects(aStrokeFilterOperand, aOutput);
  st = attributeAffects(aStrokeFilterList, aOutput);
  st = attributeAffects(aStrokeFilterTexture, aOutput);

  st = attributeAffects(aStartFrom, aOutput);
  st = attributeAffects(aApplyFilters, aOutput);
  st = attributeAffects(aApplySort, aOutput);

  st = attributeAffects(aMaxPointToPointDistance, aOutput);

  return ( MS::kSuccess );

}

bool painting::findInSortDefinition( StrokeSortFilterKey key,
                                     const std::vector< std::pair <StrokeSortFilterKey, StrokeSortDirection> > &sortDefinition)
{
  std::vector< std::pair <StrokeSortFilterKey, StrokeSortDirection> >::const_iterator it =
    sortDefinition.begin();
  for (; it != sortDefinition.end(); ++it)
  {
    if (it->first == key) {
      return true;
    }
  }
  return false;
}

void painting::getUVs(std::vector<strokeGeom> &strokePool, MFloatArray &uVals,
                      MFloatArray &vVals)
{
  int len =  strokePool.size();
  uVals.setLength(len);
  vVals.setLength(len);

  std::vector<strokeGeom>::iterator iter = strokePool.begin();
  for (unsigned i = 0; iter != strokePool.end(); iter++, i++) {
    float &u  = uVals[i];
    float &v  = vVals[i];
    iter->getUV(u, v);
  }
}


MStatus painting::overrideBrushIds(MDataBlock &data,
                                   std::vector<strokeGeom> &strokePool)
{

  if  (! hasTexture(painting::aBrushIdTexture)) {
    return MS::kUnknownParameter;
  }

  MIntArray brushIds;
  short rangeBrushId = data.inputValue(aBrushIdTextureRange).asShort();

  MFloatArray uVals;
  MFloatArray vVals;
  getUVs(strokePool, uVals, vVals);

  MStatus st = sampleUVTexture(painting::aBrushIdTexture, uVals, vVals, brushIds,
                               rangeBrushId);
  if (st.error()) {
    return MS::kUnknownParameter;
  }

  std::vector<strokeGeom>::iterator iter = strokePool.begin();
  for (unsigned i = 0; iter != strokePool.end(); iter++, i++) {
    iter->setBrushId(short(brushIds[i]));
  }
  return MS::kSuccess;
}

MStatus painting::overridePaintIds(MDataBlock &data,  std::vector<strokeGeom> &strokePool)
{

  if  (! hasTexture(painting::aPaintIdTexture)) {
    return MS::kUnknownParameter;
  }

  MIntArray paintIds;
  short rangePaintId = data.inputValue(aPaintIdTextureRange).asShort();

  MFloatArray uVals;
  MFloatArray vVals;
  getUVs(strokePool, uVals, vVals);

  MStatus st = sampleUVTexture(painting::aPaintIdTexture, uVals, vVals, paintIds,
                               rangePaintId);
  if (st.error()) {
    return MS::kUnknownParameter;
  }

  std::vector<strokeGeom>::iterator iter = strokePool.begin();
  for (unsigned i = 0; iter != strokePool.end(); iter++, i++) {
    iter->setPaintId(short(paintIds[i]));
  }
  return MS::kSuccess;
}

MStatus painting::sortStrokes(MDataBlock &data,  std::vector<strokeGeom> &strokePool)

{
  MStatus st;


  bool applySort =  data.inputValue(aApplySort ).asBool();
  if (! applySort) {
    return MS::kSuccess;
  }


  std::vector<strokeGeom>::iterator iter;

  // vector<pair>: This is effectively a map with the order of insertion maintained
  std::vector< std::pair <StrokeSortFilterKey, StrokeSortDirection> > sortDefinition;
  MArrayDataHandle hSortMulti = data.inputArrayValue(aStrokeSortList, &st ); msert;

  unsigned nPlugs = hSortMulti.elementCount();
  bool useSortMap = false;

  for (unsigned i = 0; i < nPlugs; i++, hSortMulti.next()) {
    unsigned index = hSortMulti.elementIndex(&st);

    if (st.error()) {
      continue;
    }

    MDataHandle hComp = hSortMulti.inputValue(&st);
    if (st.error()) {
      continue;
    }

    StrokeSortFilterKey key =  StrokeSortFilterKey(hComp.child(aStrokeSortKey).asShort());
    if (key == painting::kMapRed || key == painting::kMapGreen || key == painting::kMapBlue )
    {
      useSortMap = true;
    }

    StrokeSortDirection direction =  StrokeSortDirection(hComp.child(
                                       aStrokeSortDirection).asShort());

    if (!findInSortDefinition(key, sortDefinition))
    {
      sortDefinition.push_back( std::make_pair(key, direction ) );
    }

  }

  /* Set the mapped colors so they may be used for sorting */
  if (useSortMap)
  {
    MFloatVectorArray sortMapColors;
    if  (hasTexture(painting::aStrokeSortTexture)) {
      MFloatArray uVals;
      MFloatArray vVals;
      getUVs(strokePool, uVals, vVals);
      st = sampleUVTexture(painting::aStrokeSortTexture, uVals, vVals, sortMapColors);
      if (! st.error()) {
        iter = strokePool.begin();
        for (unsigned i = 0; iter != strokePool.end(); iter++, i++) {
          iter->setSortColor(sortMapColors[i]);
        }
      }
    }
  }

  for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {
    iter->clearSortStack();
  }
  if (sortDefinition.size())
  {
    std::vector< std::pair <StrokeSortFilterKey, StrokeSortDirection> >::iterator sortiter;

    for (sortiter = sortDefinition.begin(); sortiter != sortDefinition.end(); ++sortiter)
    {
      // void (strokeGeom::*idSetter) (bool); // function pointer

      bool ascending = (sortiter->second == painting::kSortAscending);
      switch (sortiter->first)
      {
        case kId:
          for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {iter->appendIdToSortStack(ascending);}
          break;
        case kParentId:
          for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {iter->appendParentIdToSortStack(ascending);}
          break;
        case kBrushId:
          for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {iter->appendBrushIdToSortStack(ascending);}
          break;
        case kPaintId:
          for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {iter->appendPaintIdToSortStack(ascending);}
          break;
        case kLayerId:
          for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {iter->appendLayerIdToSortStack(ascending);}
          break;
        case kRepeatId:
          for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {iter->appendRepeatIdToSortStack(ascending);}
          break;
        case kMapRed:
          if (useSortMap) {
            for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {iter->appendMapRedIdToSortStack(ascending);}
          }
          break;
        case kMapGreen:
          if (useSortMap) {
            for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {iter->appendMapGreenIdToSortStack(ascending);}
          }
          break;
        case kMapBlue:
          if (useSortMap) {
            for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {iter->appendMapBlueIdToSortStack(ascending);}
          }
          break;
        default:
          break;
      }
    }

    std::sort(strokePool.begin(), strokePool.end(), StrokeCompare);
  }
  return MS::kSuccess;
}



MStatus painting::filterStrokes(MDataBlock &data,  std::vector<strokeGeom> &strokePool)

{
  MStatus st;


  bool applyFilters =  data.inputValue(aApplyFilters ).asBool();
  if (! applyFilters) {
    return MS::kSuccess;
  }
  std::vector<strokeGeom>::iterator iter;


  // vector<pair>: This is effectively a map with the order of insertion maintained
  std::vector< std::tuple <StrokeSortFilterKey, strokeGeom::StrokeFilterOperator, int> >
  filterDefinition;
  MArrayDataHandle hFilterMulti = data.inputArrayValue(aStrokeFilterList, &st ); msert;


  unsigned nPlugs = hFilterMulti.elementCount();
  bool useFilterMap = false;


  for (unsigned i = 0; i < nPlugs; i++, hFilterMulti.next()) {
    unsigned index = hFilterMulti.elementIndex(&st);

    if (st.error()) {
      continue;
    }

    MDataHandle hComp = hFilterMulti.inputValue(&st);
    if (st.error()) {
      continue;
    }

    StrokeSortFilterKey key =  StrokeSortFilterKey(hComp.child(aStrokeFilterKey).asShort());



    if (key == painting::kMapRed || key == painting::kMapGreen || key == painting::kMapBlue )
    {
      useFilterMap = true;
    }
    int value = hComp.child(aStrokeFilterOperand).asInt();
    strokeGeom::StrokeFilterOperator op =  strokeGeom::StrokeFilterOperator(hComp.child(
        aStrokeFilterOperator).asShort());

    filterDefinition.push_back( std::make_tuple(key, op,  value) );
  }



  /* Set the mapped colors so they may be used for filtering */
  if (useFilterMap)
  {
    MFloatVectorArray filterMapColors;
    if  (hasTexture(painting::aStrokeFilterTexture)) {
      MFloatArray uVals;
      MFloatArray vVals;
      getUVs(strokePool, uVals, vVals);
      st = sampleUVTexture(painting::aStrokeFilterTexture, uVals, vVals, filterMapColors);
      if (! st.error()) {
        iter = strokePool.begin();
        for (unsigned i = 0; iter != strokePool.end(); iter++, i++) {
          iter->setFilterColor(filterMapColors[i]);
        }
      }
    }
  }


  // for ( iter = strokePool.begin(); iter != strokePool.end(); iter++) {
  //   iter->clearSortStack();
  // }
  if (filterDefinition.size())
  {


    std::vector< std::tuple <StrokeSortFilterKey, strokeGeom::StrokeFilterOperator, int> >::iterator
    filteriter;


    for (filteriter = filterDefinition.begin(); filteriter != filterDefinition.end();
         ++filteriter)
    {


      strokeGeom::StrokeFilterOperator op = std::get<1>(*filteriter);
      int value = std::get<2>(*filteriter);
      StrokeSortFilterKey key = std::get<0>(*filteriter);

      std::vector<strokeGeom>::iterator new_end = strokePool.end();

      switch  (key)
      {
        case kId:

          new_end = std::remove_if(strokePool.begin(), strokePool.end(),
                                   [op, value](const strokeGeom & stroke)
          { return stroke.testId(op, value) == false; }   );
          break;
        case kParentId:

          new_end = std::remove_if(strokePool.begin(), strokePool.end(),
                                   [op, value](const strokeGeom & stroke)
          { return stroke.testParentId(op, value) == false; }   );
          break;
        case kBrushId:

          new_end = std::remove_if(strokePool.begin(), strokePool.end(),
                                   [op, value](const strokeGeom & stroke)
          { return stroke.testBrushId(op, value) == false; }   );
          break;
        case kPaintId:

          new_end = std::remove_if(strokePool.begin(), strokePool.end(),
                                   [op, value](const strokeGeom & stroke)
          { return stroke.testPaintId(op, value) == false; }   );
          break;
        case kLayerId:

          new_end = std::remove_if(strokePool.begin(), strokePool.end(),
                                   [op, value](const strokeGeom & stroke)
          { return stroke.testLayerId(op, value) == false; }   );
          break;
        case kRepeatId:

          new_end = std::remove_if(strokePool.begin(), strokePool.end(),
                                   [op, value](const strokeGeom & stroke)
          { return stroke.testRepeatId(op, value) == false; }   );
          break;
        case kMapRed:

          if (useFilterMap) {
            new_end = std::remove_if(strokePool.begin(), strokePool.end(),
                                     [op, value](const strokeGeom & stroke)
            { return stroke.testMapRedId(op, value) == false; }   );
          }
          break;
        case kMapGreen:

          if (useFilterMap) {
            new_end = std::remove_if(strokePool.begin(), strokePool.end(),
                                     [op, value](const strokeGeom & stroke)
            { return stroke.testMapGreenId(op, value) == false; }   );
          }
          break;
        case kMapBlue:

          if (useFilterMap) {
            new_end = std::remove_if(strokePool.begin(), strokePool.end(),
                                     [op, value](const strokeGeom & stroke)
            { return stroke.testMapBlueId(op, value) == false; }   );
          }
          break;
        default:
          break;
      }




      strokePool.erase(new_end, strokePool.end());


    }
  }
  return MS::kSuccess;
}


MStatus painting::compute( const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  MString method("painting::compute");
  if (plug != aOutput  ) { return ( MS::kUnknownParameter ); }

  // dummies to trigger compute on change
  float brushIdTexture = data.inputValue(aBrushIdTexture).asFloat();
  float paintIdTexture = data.inputValue(aPaintIdTexture).asFloat();
  MFloatVector strokeSortTexture = data.inputValue( aStrokeSortTexture ).asFloatVector();
  MFloatVector strokeFilterTexture = data.inputValue(
                                       aStrokeFilterTexture ).asFloatVector();

  int startFrom = data.inputValue(aStartFrom).asInt();

  MDataHandle mh = data.inputValue(aInMatrix, &st); mser;
  MMatrix wm = mh.asMatrix();
  MMatrix planeMatrix = data.inputValue(painting::aPlaneMatrix).asMatrix();
  MMatrix inversePlaneMatrix = planeMatrix.inverse();
  MVector planeNormal = (MVector::zAxis * planeMatrix).normal();

  double ptpThresh = data.inputValue(aMaxPointToPointDistance).asDouble();
  if (ptpThresh < 3.0) {
    ptpThresh = 3.0;
  }




  MArrayDataHandle hBrushes = data.inputArrayValue(aBrushes, &st ); msert;
  std::map<short, Brush> brushes = Brush::factory(
                                     hBrushes,
                                     painting::aBrushWidth,
                                     painting::aBrushRetention,
                                     painting::aBrushTip,
                                     painting::aBrushShape
                                   );

  MArrayDataHandle hPaints = data.inputArrayValue(aPaints, &st ); msert;
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

  // unsigned nStrokes = strokePool.size();
  // MFloatArray uVals(nStrokes);
  // MFloatArray vVals(nStrokes);

  iter = strokePool.begin();
  for (unsigned i = 0; iter != strokePool.end(); iter++, i++) {
    iter->setUV(inversePlaneMatrix);
  }

  overrideBrushIds( data, strokePool);
  overridePaintIds( data,  strokePool);

  filterStrokes(data,  strokePool);


  if (strokePool.size()) {
    sortStrokes(data, strokePool);

    int last = strokePool.size() - 1;
    if (startFrom < 0) {
      startFrom = 0;
    }
    if (startFrom > last) {
      startFrom = last;
    }
    for ( citer = strokePool.begin() + startFrom; citer != strokePool.end(); citer++) {
      pGeom->addStroke(*citer);
    }
    pGeom->setPreStops(ptpThresh);
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



MStatus painting::populateStrokePool(MDataBlock &data,
                                     std::vector<strokeGeom> &strokePool)
{
  MStatus st;
  MArrayDataHandle hStrokeCurves = data.inputValue(aStrokeCurves, &st); msert;
  unsigned nCurves = hStrokeCurves.elementCount();
  // int gid = 0;
  for (unsigned i = 0; i < nCurves; i++, hStrokeCurves.next()) {
    int index = hStrokeCurves.elementIndex(&st);
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
      strokePool.back().setParentId(index);
      // strokePool.back().setGlobalId(gid);

      // gid++;
    }
  }
  return MS::kSuccess;
}



bool painting::hasTexture(const MObject &attribute)
{
  MString name;
  MStatus st = getTextureName(attribute, name);
  if (st.error()) {return false; }
  return true;
}

MStatus painting::getTextureName(const MObject &attribute,
                                 MString &name) const {
  MStatus st;
  MPlugArray plugArray;
  MPlug plug(thisMObject(), attribute);
  bool hasConnection = plug.connectedTo(plugArray, 1, 0, &st); msert;
  if (! hasConnection) { return MS::kUnknownParameter; }
  name = plugArray[0].name(&st);
  return MS::kSuccess;
}


MStatus painting::sampleUVTexture(const MObject &attribute,   MFloatArray &uVals,
                                  MFloatArray &vVals, MFloatVectorArray &result) const {

  MStatus st;
  MString plugName;
  st = getTextureName(attribute, plugName);
  if (st.error()) {return MS::kFailure; }
  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();
  MFloatVectorArray transparencies;
  // MFloatVectorArray colors;

  int n = uVals.length();

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat,
        0, &uVals, &vVals, 0, 0, 0, 0, 0, result, transparencies); msert;
  return MS::kSuccess;
}


MStatus painting::sampleUVTexture(const MObject &attribute,   MFloatArray &uVals,
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
        0, &uVals, &vVals, 0, 0, 0, 0, 0, colors, transparencies); msert;

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

  bool displayLift;
  MPlug(thisObj, aDisplayLift).getValue(displayLift);

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
        stroke.getPoints(points, displayLift, stackHeight);
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
        stroke.getPoints(starts, displayLift, stackHeight);
        MFloatVectorArray ends;
        stroke.getZAxes(ends, displayLift);

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
        stroke.getPoints(starts, displayLift, stackHeight);

        MFloatVectorArray xAxes;
        stroke.getXAxes(xAxes, displayLift);

        MFloatVectorArray yAxes;
        stroke.getYAxes(yAxes, displayLift);

        MFloatVectorArray zAxes;
        stroke.getZAxes(zAxes, displayLift);

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


void painting::drawWireframeStops(
  const paintingGeom &geom, M3dView &view,
  const MDagPath &path,
  M3dView:: DisplayStatus status )
{
  setWireDrawColor(view, status);

  MObject thisObj = thisMObject();

  bool displayStops;
  MPlug(thisObj, aDisplayStops).getValue(displayStops);
  if (! displayStops) {
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



  short tmp;
  MPlug(thisObj, aDisplayTargets).getValue(tmp);
  TargetDisplay targetDisplayStyle = TargetDisplay(tmp);
  if (targetDisplayStyle == painting::kTargetsPoint
      || targetDisplayStyle == painting::kTargetsNone) {
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
        stroke.getStopPoints(points, stackHeight);
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
        stroke.getStopPoints(starts, stackHeight);
        MFloatVectorArray ends;
        stroke.getStopZAxes(ends);

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
        stroke.getStopPoints(starts, stackHeight);

        MFloatVectorArray xAxes;
        stroke.getStopXAxes(xAxes);

        MFloatVectorArray yAxes;
        stroke.getStopYAxes(yAxes);

        MFloatVectorArray zAxes;
        stroke.getStopZAxes(zAxes);

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

void painting::drawWireframeArrows(
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
    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;
      MMatrixArray mats;
      stroke.getDirectionMatrices(mats, displayLift, stackHeight);



      unsigned len = mats.length();
      if (! len) {
        continue;
      }

      MFloatPoint hdir = head  * float(stroke.direction());
      MFloatPoint ldir = left  * float(stroke.direction());
      MFloatPoint rdir = right * float(stroke.direction());






      for (unsigned i = 0; i < len; ++i)
      {
        float floats[4][4];
        mats[i].get(floats);
        MFloatMatrix fmat(floats);

        MFloatPoint c(fmat[3][0], fmat[3][1], fmat[3][2]);
        MFloatPoint h = hdir * fmat;
        MFloatPoint l = ldir * fmat;
        MFloatPoint r = rdir * fmat;


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
    const Brush &brush = geom.brushFromId(cluster.brushId());
    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;

      MFloatPointArray lefts;
      MFloatPointArray rights;

      stroke.getBorders(lefts, rights, brush, displayLift, stackHeight);

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
  drawWireframeArrows(geom, view, path, status);

  drawWireframeApproach(geom, view, path, status);
  drawWireframeClusterPath(geom, view, path, status);
  drawWireframeStops(geom, view, path, status);

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
    // double brushWidth = geom.brushFromId(cluster.brushId()).width;
    const Brush &brush = geom.brushFromId(cluster.brushId());
    MColor color = geom.paintFromId(cluster.paintId()).color;
    glColor4f(color.r , color.g , color.b, color.a);


    for (auto stroke : cluster.strokes())
    {
      stackHeight += stackGap;

      MFloatPointArray lefts;
      MFloatPointArray rights;

      stroke.getBorders(lefts, rights, brush, displayLift, stackHeight);

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



void painting::drawIds(const paintingGeom &geom, M3dView &view,
                       const MDagPath &path,
                       M3dView:: DisplayStatus status )
{

  setWireDrawColor(view, status);
  MObject thisObj = thisMObject();

  bool displayIds, displayParentIds, displayLayerIds, displayBrushIds, displayPaintIds,
       displayRepeatIds, displayLift;
  MPlug(thisObj, aDisplayLift).getValue(displayLift);
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
      if (displayIds) { text = text + "Id:" + stroke.id() + "\n" ;}
      if (displayParentIds) { text = text + "Pr:" + stroke.parentId() + "\n" ;}
      if (displayBrushIds) { text = text + "Br:" + stroke.brushId() + "\n" ;}
      if (displayPaintIds) { text = text + "Pt:" + stroke.paintId() + "\n" ;}
      if (displayLayerIds) { text = text + "Ly:" + stroke.layerId() + "\n" ;}
      if (displayRepeatIds) { text = text + "Rp:" + stroke.repeatId() + "\n" ;}


      MFloatPoint head;
      stroke.getHead(head, displayLift, stackHeight);
      MPoint textPos =  MPoint( head);

      view.drawText( text , textPos,  M3dView::kRight);
    }
  }

  glPopAttrib();



















  //   if (doDisplayIds)
  //   {
  //     glPushAttrib(GL_CURRENT_BIT);

  //     view.setDrawColor(wireColor );
  //     unsigned i = 0;
  //     for (int j = 0; j < numStrokes; ++j)
  //     {
  //       int k = i + 1;
  //       MString txt  = "";
  //       txt += j;
  //       MPoint textPos =  MPoint( targets[k][3][0], targets[k][3][1], targets[k][3][2]);
  //       view.drawText( txt , textPos,  M3dView::kRight);
  //       i +=  counts[j];
  //     }
  //     glPopAttrib();

  //   }


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


    // drawShaded(geom, view, path, status);
    // if (status == M3dView::kActive || status == M3dView::kLead
    //     || status == M3dView::kHilite || wfos ) {
    //   drawWireframe(geom, view, path, status);
    // }


  }
  // always draw any IDs that are enabled
  drawIds(geom, view, path, status);



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
  //   st = targetsPlug.getValue(dTargets); mser;
  //   fnXA.setObject(dTargets);
  //   MMatrixArray targets = fnXA.array(&st); mser;
  //   unsigned pl = targets.length();
  //   if (! pl ) {
  //     return MBoundingBox();
  //   }

  //   double stackGap;
  //   MPlug(thisObj, aStackGap).getValue(stackGap);

  //   MPlug countsPlug(thisObj, aOutCounts);
  //   MObject dCounts;
  //   st = countsPlug.getValue(dCounts); mser;
  //   fnI.setObject(dCounts);
  //   MIntArray counts = fnI.array(&st); mser;
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



