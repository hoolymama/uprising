
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>

#include <maya/MFnPluginData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>

#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnGenericAttribute.h>
#include <maya/MRampAttribute.h>

#include <maya/MArrayDataHandle.h>
#include <maya/MAngle.h>

#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>

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

#include <maya/MFnNurbsCurveData.h>

#include "strokeData.h"
#include "collectStrokes.h"
#include "stroke.h"
#include "cImgUtils.h"

#include "filterDefinition.h"
#include "sortDefinition.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"
#include "texUtils.h"


const double rad_to_deg = (180 / 3.1415927);
const double deg_to_rad = (3.1415927 / 180);


MObject collectStrokes::aStrokes;

// MObject collectStrokes::aSeed;
// MObject collectStrokes::aStrokeCountFactor;

MObject collectStrokes::aProjection;

MObject collectStrokes::aStrokeSortKey;
MObject collectStrokes::aStrokeSortDirection;
MObject collectStrokes::aStrokeSortList;
MObject collectStrokes::aStrokeSortTexture;
MObject collectStrokes::aApplySort;

MObject collectStrokes::aStrokeFilterKey;
MObject collectStrokes::aStrokeFilterOperator;
MObject collectStrokes::aStrokeFilterOperand;
MObject collectStrokes::aStrokeFilterList;
MObject collectStrokes::aStrokeFilterTexture;
MObject collectStrokes::aApplyFilters;

MObject collectStrokes::aStartFrom;
MObject collectStrokes::aEndAt;



MObject collectStrokes::aBrushIdTexture;
MObject collectStrokes::aBrushIdRemapRamp;
MObject collectStrokes::aBrushIdRemapRange;
MObject collectStrokes::aPaintIdTexture;
MObject collectStrokes::aPaintIdRemapRamp;
MObject collectStrokes::aPaintIdRemapRange;


MObject collectStrokes::aOutput;


MTypeId collectStrokes::id( k_collectStrokes );

collectStrokes::collectStrokes() {}

collectStrokes::~collectStrokes() {}

void *collectStrokes::creator() {
  return new collectStrokes();
}


/// Post constructor
void
collectStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}



const double epsilon = 0.0001;


MStatus collectStrokes::initialize()
{
  MStatus st;
  MString method("collectStrokes::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
  MFnEnumAttribute eAttr;
  MFnMatrixAttribute mAttr;
  MFnCompoundAttribute cAttr;

  aStrokes = tAttr.create( "strokes", "stks", strokeData::id );
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aStrokes);



  MMatrix identity;
  identity.setToIdentity();

  aProjection = mAttr.create( "projection", "prj",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setKeyable(true);
  mAttr.setDefault(identity);
  addAttribute(aProjection);

  aStrokeSortKey = eAttr.create("strokeSortKey", "stsk", Stroke::kBrushId);
  eAttr.addField("Id", Stroke::kStrokeId);

  eAttr.addField("Brush Id", Stroke::kBrushId);
  eAttr.addField("Paint Id", Stroke::kPaintId);
  eAttr.addField("Repeat Id", Stroke::kRepeatId);
  eAttr.addField("Layer Id", Stroke::kLayerId);
  eAttr.addField("Parent Id", Stroke::kParentId);
  eAttr.addField("Target Count", Stroke::kTargetCount);
  eAttr.addField("Custom Brush Id", Stroke::kCustomBrushId);

  eAttr.addField("Map Red", Stroke::kMapRed);
  eAttr.addField("Map Green", Stroke::kMapGreen);
  eAttr.addField("Map Blue", Stroke::kMapBlue);
  aStrokeSortDirection = eAttr.create("strokeSortDirection", "stsd",
                                      Stroke::kSortAscending);
  eAttr.addField("Ascending",  Stroke::kSortAscending);
  eAttr.addField("Descending", Stroke::kSortDescending);

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

  aStrokeFilterKey = eAttr.create("strokeFilterKey", "stfk", Stroke::kBrushId);
  eAttr.addField("Id", Stroke::kStrokeId);
  eAttr.addField("Brush Id", Stroke::kBrushId);
  eAttr.addField("Paint Id", Stroke::kPaintId);
  eAttr.addField("Repeat Id", Stroke::kRepeatId);
  eAttr.addField("Layer Id", Stroke::kLayerId);
  eAttr.addField("Parent Id", Stroke::kParentId);
  eAttr.addField("Target Count", Stroke::kTargetCount);
  eAttr.addField("Custom Brush Id", Stroke::kCustomBrushId);

  eAttr.addField("Map Red", Stroke::kMapRed);
  eAttr.addField("Map Green", Stroke::kMapGreen);
  eAttr.addField("Map Blue", Stroke::kMapBlue);

  eAttr.setHidden( false );
  eAttr.setKeyable( true );

  aStrokeFilterOperator = eAttr.create("strokeFilterOperator", "stfop",
                                       Stroke::kGreaterThan);
  eAttr.addField(">", Stroke::kGreaterThan);
  eAttr.addField("<", Stroke::kLessThan);
  eAttr.addField("==", Stroke::kEqualTo);
  eAttr.addField("!=", Stroke::kNotEqualTo);
  eAttr.addField("nop", Stroke::kNoOp);
  eAttr.setHidden( false );
  eAttr.setKeyable( true );

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

  aApplyFilters = nAttr.create( "applyFilters", "apfl", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aApplyFilters );

  aStartFrom = nAttr.create("startFrom", "stfm", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  st = addAttribute(aStartFrom);

  aEndAt = nAttr.create("endAt", "edat", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(-1);
  st = addAttribute(aEndAt);





  aBrushIdTexture = nAttr.createColor( "brushIdTexture", "bidt");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aBrushIdTexture);

  aBrushIdRemapRamp  = MRampAttribute::createCurveRamp("brushIdRemapRamp", "brrp");
  st = addAttribute( aBrushIdRemapRamp ); mser;

  aBrushIdRemapRange = nAttr.create("brushIdRemapRange", "brrg", MFnNumericData::k2Int);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aBrushIdRemapRange);



  aPaintIdTexture = nAttr.createColor( "paintIdTexture", "pidt");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aPaintIdTexture);

  aPaintIdRemapRamp  = MRampAttribute::createCurveRamp("paintIdRemapRamp", "prrp");
  st = addAttribute( aPaintIdRemapRamp ); mser;

  aPaintIdRemapRange = nAttr.create("paintIdRemapRange", "prrg", MFnNumericData::k2Int);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aPaintIdRemapRange);





  aOutput = tAttr.create("output", "out", strokeData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);


  st = attributeAffects(aStrokes, aOutput);


  st = attributeAffects(aProjection, aOutput);

  st = attributeAffects(aStrokeSortKey, aOutput);
  st = attributeAffects(aStrokeSortDirection, aOutput);
  st = attributeAffects(aStrokeSortList, aOutput);
  st = attributeAffects(aStrokeSortTexture, aOutput);

  st = attributeAffects(aStrokeFilterKey, aOutput);
  st = attributeAffects(aStrokeFilterOperator, aOutput);
  st = attributeAffects(aStrokeFilterOperand, aOutput);
  st = attributeAffects(aStrokeFilterList, aOutput);
  st = attributeAffects(aStrokeFilterTexture, aOutput);

  st = attributeAffects(aApplyFilters, aOutput);
  st = attributeAffects(aApplySort, aOutput);

  st = attributeAffects(aStrokeFilterTexture, aOutput);

  st = attributeAffects(aStartFrom, aOutput);
  st = attributeAffects(aEndAt, aOutput);



  st = attributeAffects(aBrushIdTexture, aOutput);
  st = attributeAffects(aBrushIdRemapRamp, aOutput);
  st = attributeAffects(aBrushIdRemapRange, aOutput);
  st = attributeAffects(aPaintIdTexture, aOutput);
  st = attributeAffects(aPaintIdRemapRamp, aOutput);
  st = attributeAffects(aPaintIdRemapRange, aOutput);




  return ( MS::kSuccess );

}


MStatus collectStrokes::compute(const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  if (plug != aOutput) {
    return (MS::kUnknownParameter );
  }

  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(strokeData:: id);

  MObject dOut = fnOut.create(kdid, & st);
  strokeData *newData = (strokeData * )fnOut.data(&st); mser;
  std::vector < Stroke > *geom = newData->fGeometry;
  geom->clear();

  collect(data, geom);

  assignUVs(data, geom);
  overrideBrushIds( data, geom);
  overridePaintIds( data, geom);

  filterStrokes(data,  geom);
  sortStrokes(data,  geom);
  cullStartEnd(data, geom);

  hOutput.set(newData);
  data.setClean(plug);

  return MS:: kSuccess;
}


MStatus collectStrokes::collect(MDataBlock &data, std::vector<Stroke> *geom)
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
    const std::vector<Stroke> *strokeGeom = sData->fGeometry;

    std::vector<Stroke>::const_iterator citer;
    for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++) {
      geom->push_back(*citer);
      geom->back().setParentId(index);
    }
  }
  return MS::kSuccess;
}



void collectStrokes::assignUVs(MDataBlock &data, std::vector<Stroke> *geom) const
{
  MMatrix inverseProjMat = data.inputValue(
                             collectStrokes::aProjection).asMatrix().inverse();
  std::vector<Stroke>::iterator iter;
  iter = geom->begin();
  for (iter = geom->begin(); iter != geom->end(); iter++) {
    iter->setUV(inverseProjMat);
  }
}

void collectStrokes::getUVs(std::vector<Stroke> &strokes, MFloatArray &uVals,
                            MFloatArray &vVals) const
{
  int len =  strokes.size();
  uVals.setLength(len);
  vVals.setLength(len);

  std::vector<Stroke>::iterator iter = strokes.begin();
  for (unsigned i = 0; iter != strokes.end(); iter++, i++) {
    float &u  = uVals[i];
    float &v  = vVals[i];
    iter->getUV(u, v);
  }
}

bool collectStrokes::getMappedColors(std::vector<Stroke> *geom, MObject &attribute,
                                     MFloatVectorArray &result) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  if (! TexUtils::hasTexture(thisObj, attribute))
  {
    return false;
  }

  MFloatArray uVals;
  MFloatArray vVals;
  getUVs(*geom, uVals, vVals);

  st = TexUtils::sampleUVTexture(thisObj, attribute,  uVals, vVals,
                                 result);
  if (st.error()) {
    return false;
  }
  return true;
}





bool collectStrokes::setFilterMapColor(std::vector<Stroke> *geom) const
{
  MFloatVectorArray result;
  if (! getMappedColors(geom, collectStrokes::aStrokeFilterTexture, result))
  {
    return false;
  }
  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++) {
    iter->setFilterColor(result[i]);
  }
  return true;
}

bool collectStrokes::setSortMapColor(std::vector<Stroke> *geom) const
{

  MFloatVectorArray result;
  if (! getMappedColors(geom, collectStrokes::aStrokeSortTexture, result))
  {
    return false;
  }

  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++) {
    iter->setSortColor(result[i]);
  }
  return true;
}






void collectStrokes::filterStrokes(MDataBlock &data,  std::vector<Stroke> *geom) const

{
  MStatus st;

  bool applyFilters =  data.inputValue(aApplyFilters).asBool();
  if (! applyFilters) {
    return ;
  }

  MArrayDataHandle hFilterMulti = data.inputArrayValue(aStrokeFilterList, &st );
  if ( st.error()) {
    return;
  }

  FilterDefinition filterDefinition(
    hFilterMulti,
    aStrokeFilterKey,
    aStrokeFilterOperator,
    aStrokeFilterOperand
  );

  if (! filterDefinition.hasFilters())
  {
    return;
  }


  /* Set the mapped colors so they may be used for filtering */
  bool useFilterMap = filterDefinition.usesMap();
  if (useFilterMap)
  {
    useFilterMap = setFilterMapColor(geom);
  }


  for (auto filteriter =  filterDefinition.begin(); filteriter != filterDefinition.end() ;
       filteriter++)

  {
    Stroke::FilterOperator op = std::get<1>(*filteriter);
    if (op == Stroke::kNoOp) {
      continue;
    }

    int value = std::get<2>(*filteriter);
    Stroke::SortFilterKey key = std::get<0>(*filteriter);

    std::vector<Stroke>::iterator new_end = geom->end();

    switch  (key)
    {
      case Stroke::kStrokeId:
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke & stroke)
        { return stroke.testStrokeId(op, value) == false; }   );
        break;

      case Stroke::kBrushId:
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke & stroke)
        { return stroke.testBrushId(op, value) == false; }   );
        break;
      case Stroke::kPaintId:
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke & stroke)
        { return stroke.testPaintId(op, value) == false; }   );
        break;
      case Stroke::kRepeatId:
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke & stroke)
        { return stroke.testRepeatId(op, value) == false; }   );
        break;
      case Stroke::kTargetCount:
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke & stroke)
        { return stroke.testTargetCount(op, value) == false; }   );
        break;
      case Stroke::kLayerId:
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke & stroke)
        { return stroke.testLayerId(op, value) == false; }   );
        break;
      case Stroke::kParentId:
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke & stroke)
        { return stroke.testParentId(op, value) == false; }   );
        break;

      case Stroke::kCustomBrushId:
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke & stroke)
        { return stroke.testCustomBrushId(op, value) == false; }   );
        break;


      case Stroke::kMapRed:
        if (useFilterMap) {
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          { return stroke.testMapRedId(op, value) == false; }   );
        }
        break;
      case Stroke::kMapGreen:
        if (useFilterMap) {
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          { return stroke.testMapGreenId(op, value) == false; }   );
        }
        break;
      case Stroke::kMapBlue:
        if (useFilterMap) {
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          { return stroke.testMapBlueId(op, value) == false; }   );
        }
        break;
      default:
        break;
    }

    geom->erase(new_end, geom->end());
  }

  return  ;
}

void collectStrokes::cullStartEnd(MDataBlock &data,   std::vector<Stroke> *geom) const
{

  int startFrom = data.inputValue(aStartFrom).asInt();


  int endAt = data.inputValue(aEndAt).asInt();
  int last = geom->size() - 1;
  if (endAt > -1 && endAt < geom->size()) {
    last = endAt;
  }

  if (startFrom < 0) {
    startFrom = 0;
  }
  if (startFrom > last) {
    startFrom = last;
  }


  int diff = geom->size() - (last + 1);

  if (startFrom > 0) {
    std::vector<Stroke>::iterator start_iter =  geom->begin() + startFrom;
    geom->erase(geom->begin(), geom->begin() + startFrom);
  }

  if (diff > 0) {
    std::vector<Stroke>::iterator end_iter =  geom->end() - diff;
    geom->erase(end_iter, geom->end());
  }

}


void collectStrokes::sortStrokes(MDataBlock &data,   std::vector<Stroke> *geom) const

{
  MStatus st;


  bool applySort =  data.inputValue(aApplySort ).asBool();
  if (! applySort) {
    return;
  }


  MArrayDataHandle hSortMulti = data.inputArrayValue(aStrokeSortList, &st );
  if ( st.error()) {
    return;
  }


  SortDefinition sortDefinition(hSortMulti, collectStrokes::aStrokeSortKey ,
                                collectStrokes::aStrokeSortDirection );


  if (! sortDefinition.hasSort())
  {
    return;
  }


  /* Set the mapped colors so they may be used for filtering */
  bool useSortMap = sortDefinition.usesMap();
  if (useSortMap)
  {
    useSortMap = setSortMapColor(geom);
  }


  std::vector<Stroke>::iterator iter;
  for ( iter = geom->begin(); iter != geom->end(); iter++) {
    iter->clearSortStack();
  }



  for (auto sortiter =  sortDefinition.begin(); sortiter != sortDefinition.end() ;
       sortiter++)

  {

    bool ascending = (sortiter->second == Stroke::kSortAscending);
    switch (sortiter->first)
    {
      case Stroke::kStrokeId:
        for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendStrokeIdToSortStack(ascending);}
        break;
      case Stroke::kBrushId:
        for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendBrushIdToSortStack(ascending);}
        break;
      case Stroke::kPaintId:
        for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendPaintIdToSortStack(ascending);}
        break;
      case Stroke::kLayerId:
        for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendLayerIdToSortStack(ascending);}
        break;
      case Stroke::kParentId:
        for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendParentIdToSortStack(ascending);}
        break;
      case Stroke::kRepeatId:
        for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendRepeatIdToSortStack(ascending);}
        break;
      case Stroke::kTargetCount:
        for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendTargetCountToSortStack(ascending);}
        break;

      case Stroke::kCustomBrushId:
        for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendCustomBrushIdToSortStack(ascending);}
        break;

      case Stroke::kMapRed:
        if (useSortMap) {
          for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendMapRedIdToSortStack(ascending);}
        }
        break;
      case Stroke::kMapGreen:
        if (useSortMap) {
          for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendMapGreenIdToSortStack(ascending);}
        }
        break;
      case Stroke::kMapBlue:
        if (useSortMap) {
          for ( iter = geom->begin(); iter != geom->end(); iter++) {iter->appendMapBlueIdToSortStack(ascending);}
        }
        break;
      default:
        break;
    }
  }

  std::sort(geom->begin(), geom->end());
}



bool collectStrokes::overrideBrushIds(MDataBlock &data, std::vector<Stroke> *geom) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  if (! TexUtils::hasTexture(thisObj, collectStrokes::aBrushIdTexture))
  {
    return false;
  }

  MFloatArray uVals;
  MFloatArray vVals;
  getUVs(*geom, uVals, vVals);

  const int2 &range = data.inputValue( aBrushIdRemapRange ).asInt2();
  int low = range[0];
  int high = range[1];

  MIntArray result;
  st = TexUtils::sampleUVTexture(thisObj, collectStrokes::aBrushIdTexture,  uVals, vVals,
                                 collectStrokes::aBrushIdRemapRamp,  low,  high,  result);

  if (st.error()) {
    return false;
  }

  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++) {
    iter->setBrushId(result[i]);
  }
  return true;
}

bool collectStrokes::overridePaintIds(MDataBlock &data, std::vector<Stroke> *geom) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  if (! TexUtils::hasTexture(thisObj, collectStrokes::aPaintIdTexture))
  {
    return false;
  }

  MFloatArray uVals;
  MFloatArray vVals;
  getUVs(*geom, uVals, vVals);

  const int2 &range = data.inputValue( aPaintIdRemapRange ).asInt2();
  int low = range[0];
  int high = range[1];

  MIntArray result;
  st = TexUtils::sampleUVTexture(thisObj, collectStrokes::aPaintIdTexture,  uVals, vVals,
                                 collectStrokes::aPaintIdRemapRamp,  low,  high,  result);

  if (st.error()) {
    return false;
  }

  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++) {
    iter->setPaintId(result[i]);
  }
  return true;
}



