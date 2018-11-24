
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
#include "strokeNode.h"
#include "stroke.h"
#include "cImgUtils.h"

#include "filterDefinition.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);
// const double deg_to_rad = (3.1415927 / 180);


MObject strokeNode::aPointDensity;

MObject strokeNode::aRepeats;
MObject strokeNode::aRepeatOffset;
MObject strokeNode::aRepeatMirror;
MObject strokeNode::aRepeatOscillate;

MObject strokeNode::aSeed;

MObject strokeNode::aStrokeDirection;

MObject strokeNode::aEntryLength;
MObject strokeNode::aExitLength;


MObject strokeNode::aBrushId;
MObject strokeNode::aPaintId;
MObject strokeNode::aLayerId;

MObject strokeNode::aActive;
MObject strokeNode::aStrokeCountFactor;

MObject strokeNode::aPivotFraction;
MObject strokeNode::aRepeatPivot;


MObject strokeNode::aBrushTiltRamp;
MObject strokeNode::aBrushTiltRangeMin;
MObject strokeNode::aBrushTiltRangeMax;
MObject strokeNode::aBrushTiltRange;

MObject strokeNode::aBrushBankRamp;
MObject strokeNode::aBrushBankRangeMin;
MObject strokeNode::aBrushBankRangeMax;
MObject strokeNode::aBrushBankRange;

MObject strokeNode::aBrushTwistRamp;
MObject strokeNode::aBrushTwistRangeMin;
MObject strokeNode::aBrushTwistRangeMax;
MObject strokeNode::aBrushTwistRange;

MObject strokeNode::aBrushFollowStroke;

MObject strokeNode::aProjection;

MObject strokeNode::aBrushIdImage;
MObject strokeNode::aBrushIdRemapRamp;
MObject strokeNode::aBrushIdRemapRange;
MObject strokeNode::aPaintIdImage;
MObject strokeNode::aPaintIdRemapRamp;
MObject strokeNode::aPaintIdRemapRange;


MObject strokeNode::aStrokeSortKey;
MObject strokeNode::aStrokeSortDirection;
MObject strokeNode::aStrokeSortList;
MObject strokeNode::aStrokeSortTexture;
MObject strokeNode::aApplySort;

MObject strokeNode::aStrokeFilterKey;
MObject strokeNode::aStrokeFilterOperator;
MObject strokeNode::aStrokeFilterOperand;
MObject strokeNode::aStrokeFilterList;
MObject strokeNode::aStrokeFilterTexture;
MObject strokeNode::aApplyFilters;




MObject strokeNode::aOutput;


MTypeId strokeNode::id( k_strokeNode );

strokeNode::strokeNode() {}

strokeNode::~strokeNode() {}

void *strokeNode::creator() {
  return new strokeNode();
}

const double epsilon = 0.0001;


MStatus strokeNode::initialize()
{
  MStatus st;
  MString method("strokeNode::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
  MFnEnumAttribute eAttr;
  MFnMatrixAttribute mAttr;
  MFnCompoundAttribute cAttr;


  MMatrix identity;
  identity.setToIdentity();

  aProjection = mAttr.create( "projection", "prj",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);
  addAttribute(aProjection);



  aStrokeCountFactor = nAttr.create( "strokeCountFactor",
                                     "stcf", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  nAttr.setDefault(1.0);
  st = addAttribute(aStrokeCountFactor); mser;


  aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setStorable(true);
  nAttr.setDefault( 1.0 );
  st = addAttribute(aPointDensity); mser;

  aStrokeDirection = eAttr.create("reverseDirection", "revd", Stroke::kForwards);
  eAttr.addField("forwards", Stroke::kForwards);
  eAttr.addField("backwards", Stroke::kBackwards);
  eAttr.addField("startUppermost", Stroke::kStartUppermost);
  eAttr.addField("endUppermost", Stroke::kEndUppermost);

  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute(aStrokeDirection);
  mser;

  aEntryLength = nAttr.create("entryLength", "enl", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setStorable(true);
  nAttr.setDefault( 1.0 );
  st = addAttribute(aEntryLength); mser;


  aExitLength = nAttr.create("exitLength", "exl", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setStorable(true);
  nAttr.setDefault( 1.0 );
  st = addAttribute(aExitLength); mser;

  aRepeats = nAttr.create("repeats", "rpts", MFnNumericData::kShort);
  nAttr.setHidden( false );
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aRepeats); mser;

  aRepeatOffset = nAttr.create("repeatOffset", "rpof", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setStorable(true);
  addAttribute(aRepeatOffset);

  aRepeatMirror = nAttr.create( "repeatMirror", "rpmr", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aRepeatMirror);

  aRepeatOscillate = nAttr.create( "repeatOscillate", "rpoc", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aRepeatOscillate);



  aSeed = nAttr.create( "seed", "sd", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  st = addAttribute(aSeed); mser;

  aPivotFraction = nAttr.create("pivotFraction", "pfrc", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setMin(0.0);
  nAttr.setMax(1.0);
  nAttr.setDefault(0.5);
  addAttribute(aPivotFraction);

  aRepeatPivot = nAttr.create( "repeatPivot", "rpiv", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aRepeatPivot);

  aBrushId = nAttr.create("brushId", "brid", MFnNumericData::kInt); mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aBrushId); mser;

  aPaintId = nAttr.create("paintId", "ptid", MFnNumericData::kInt); mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aPaintId); mser;

  aLayerId = nAttr.create("layerId", "lyid", MFnNumericData::kInt); mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aLayerId); mser;

  aActive = nAttr.create( "active", "act", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aActive); mser;

  aBrushFollowStroke = nAttr.create( "followStroke", "fst", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aBrushFollowStroke); mser;


  aBrushTiltRamp  = MRampAttribute::createCurveRamp("brushTiltRamp", "brtl");
  st = addAttribute( aBrushTiltRamp ); mser;
  aBrushBankRamp  = MRampAttribute::createCurveRamp("brushBankRamp", "brbk");
  st = addAttribute( aBrushBankRamp ); mser;
  aBrushTwistRamp  = MRampAttribute::createCurveRamp("brushTwistRamp", "brtw");
  st = addAttribute( aBrushTwistRamp ); mser;


  aBrushTiltRangeMin = uAttr.create( "brushTiltRangeMin", "btlrn",
                                     MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(-90, MAngle::kDegrees));
  aBrushTiltRangeMax = uAttr.create( "brushTiltRangeMax", "btlrx",
                                     MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(90, MAngle::kDegrees));
  aBrushTiltRange = nAttr.create("brushTiltRange", "btlr", aBrushTiltRangeMin,
                                 aBrushTiltRangeMax);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aBrushTiltRange); mser;



  aBrushBankRangeMin = uAttr.create( "brushBankRangeMin", "bbnrn",
                                     MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(-90, MAngle::kDegrees));
  aBrushBankRangeMax = uAttr.create( "brushBankRangeMax", "bbnrx",
                                     MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(90, MAngle::kDegrees));
  aBrushBankRange = nAttr.create("brushBankRange", "bbnr", aBrushBankRangeMin,
                                 aBrushBankRangeMax);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aBrushBankRange); mser;



  aBrushTwistRangeMin = uAttr.create( "brushTwistRangeMin", "btwrn",
                                      MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(-180, MAngle::kDegrees));
  aBrushTwistRangeMax = uAttr.create( "brushTwistRangeMax", "btwrx",
                                      MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(180, MAngle::kDegrees));
  aBrushTwistRange = nAttr.create("brushTwistRange", "btwr", aBrushTwistRangeMin,
                                  aBrushTwistRangeMax);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aBrushTwistRange); mser;


  ////////////

  aBrushIdImage = tAttr.create("brushIdImage", "bidi", cImgData::id ) ;
  tAttr.setStorable(false);
  st = addAttribute( aBrushIdImage ); mser;

  aBrushIdRemapRamp  = MRampAttribute::createCurveRamp("brushIdRemapRamp", "brrp");
  st = addAttribute( aBrushIdRemapRamp ); mser;

  aBrushIdRemapRange = nAttr.create("brushIdRemapRange", "brrg", MFnNumericData::k2Int);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aBrushIdRemapRange);



  aPaintIdImage = tAttr.create("paintIdImage", "pidi", cImgData::id ) ;
  tAttr.setStorable(false);
  st = addAttribute( aPaintIdImage ); mser;

  aPaintIdRemapRamp  = MRampAttribute::createCurveRamp("paintIdRemapRamp", "prrp");
  st = addAttribute( aPaintIdRemapRamp ); mser;

  aPaintIdRemapRange = nAttr.create("paintIdRemapRange", "prrg", MFnNumericData::k2Int);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aPaintIdRemapRange);

  ////////////

  aStrokeSortKey = eAttr.create("strokeSortKey", "stsk", Stroke::kBrushId);
  eAttr.addField("Id", Stroke::kStrokeId);

  eAttr.addField("Brush Id", Stroke::kBrushId);
  eAttr.addField("Paint Id", Stroke::kPaintId);
  eAttr.addField("Repeat Id", Stroke::kRepeatId);
  eAttr.addField("Map Red", Stroke::kMapRed);
  eAttr.addField("Map Green", Stroke::kMapGreen);
  eAttr.addField("Map Blue", Stroke::kMapBlue);
  eAttr.addField("Layer Id", Stroke::kLayerId);



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
  eAttr.addField("Map Red", Stroke::kMapRed);
  eAttr.addField("Map Green", Stroke::kMapGreen);
  eAttr.addField("Map Blue", Stroke::kMapBlue);
  eAttr.addField("Layer Id", Stroke::kLayerId);

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

  //////////////





  aOutput = tAttr.create("output", "out", strokeData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);


  st = attributeAffects(aPointDensity, aOutput);


  st = attributeAffects(aStrokeDirection, aOutput);


  st = attributeAffects(aRepeats, aOutput);
  st = attributeAffects(aRepeatOffset, aOutput);
  st = attributeAffects(aRepeatMirror, aOutput);
  st = attributeAffects(aRepeatOscillate, aOutput);


  st = attributeAffects(aSeed, aOutput);

  st = attributeAffects(aActive, aOutput);
  st = attributeAffects(aStrokeCountFactor, aOutput);
  st = attributeAffects(aPivotFraction, aOutput);
  st = attributeAffects(aRepeatPivot, aOutput);

  st = attributeAffects(aEntryLength, aOutput);
  st = attributeAffects(aExitLength, aOutput);

  st = attributeAffects(aBrushTiltRamp, aOutput);
  st = attributeAffects(aBrushBankRamp, aOutput);
  st = attributeAffects(aBrushTwistRamp, aOutput);

  st = attributeAffects(aBrushTiltRange, aOutput);
  st = attributeAffects(aBrushBankRange, aOutput);
  st = attributeAffects(aBrushTwistRange, aOutput);

  st = attributeAffects(aBrushFollowStroke, aOutput);


  st = attributeAffects(aBrushId, aOutput);
  st = attributeAffects(aPaintId, aOutput);
  st = attributeAffects(aLayerId, aOutput);

  st = attributeAffects(aProjection, aOutput);

  st = attributeAffects(aBrushIdImage, aOutput);
  st = attributeAffects(aBrushIdRemapRamp, aOutput);
  st = attributeAffects(aBrushIdRemapRange, aOutput);
  st = attributeAffects(aPaintIdImage, aOutput);
  st = attributeAffects(aPaintIdRemapRamp, aOutput);
  st = attributeAffects(aPaintIdRemapRange, aOutput);


  st = attributeAffects(aStrokeSortKey, aOutput);
  st = attributeAffects(aStrokeSortDirection, aOutput);
  st = attributeAffects(aStrokeSortList, aOutput);
  st = attributeAffects(aStrokeSortTexture, aOutput);

  st = attributeAffects(aStrokeFilterKey, aOutput);
  st = attributeAffects(aStrokeFilterOperator, aOutput);
  st = attributeAffects(aStrokeFilterOperand, aOutput);
  st = attributeAffects(aStrokeFilterList, aOutput);
  st = attributeAffects(aStrokeFilterTexture, aOutput);



  return ( MS::kSuccess );

}

MStatus strokeNode::generateStrokeGeometry(MDataBlock &data,
    std::vector<Stroke> *geom) const
{
  return MS::kSuccess;
}

MStatus strokeNode::compute(const MPlug &plug, MDataBlock &data )
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
  st = generateStrokeGeometry(data, geom); mser;


  assignUVs(data, geom);
  overrideBrushIds( data, geom);
  overridePaintIds( data, geom);
  filterStrokes(data,  geom);

  // iter = geom->begin();
  // for (; iter != geom->end(); iter++) {
  //   iter->setUV(inversePlaneMatrix);
  // }



  // assign UVs
  // overrideBrushIds( data, strokePool);
  // overridePaintIds( data,  strokePool);

  hOutput.set(newData);
  data.setClean(plug);

  return MS:: kSuccess;
}


void strokeNode::assignUVs(MDataBlock &data, std::vector<Stroke> *geom) const
{
  MMatrix inverseProjMat = data.inputValue(strokeNode::aProjection).asMatrix().inverse();
  std::vector<Stroke>::iterator iter;
  iter = geom->begin();
  for (iter = geom->begin(); iter != geom->end(); iter++) {
    iter->setUV(inverseProjMat);
  }
}




void strokeNode::overrideBrushIds(MDataBlock &data, std::vector<Stroke> *geom) const
{

  MStatus st;

  MObject thisObj = thisMObject();
  MRampAttribute remapRamp( thisObj, aBrushIdRemapRamp ); mser;
  const int2 &range = data.inputValue( aBrushIdRemapRange ).asInt2();
  int low = range[0];
  int high = range[1];
  int span = high - low;

  // bool imageValid = false;
  MDataHandle hImageData = data.inputValue(aBrushIdImage, &st);
  if (st.error()) {return;}


  MObject dImageData = hImageData.data();
  MFnPluginData fnImageData( dImageData , &st);
  if (st.error()) {return;}

  cImgData *imageData = (cImgData *)fnImageData.data();
  const CImg<unsigned char> *image = imageData->fImg;
  int w = image->width();
  int h = image->height();
  if (!(w && h )) {return;}

  float normalizer = 1.0f / 255.0f;

  std::vector<Stroke>::iterator iter;
  for (iter = geom->begin(); iter !=  geom->end(); iter++)
  {
    float u, v;
    iter->getUV(u, v);
    float x, y;
    cImgUtils::toImageCoords(u, v, w, h, x, y);
    float value = float( image->atXY(int(x), int(y) , 0, 0)) * normalizer;

    float remappedVal;
    remapRamp.getValueAtPosition( value, remappedVal, &st );
    if (st.error()) {
      remappedVal =  value;
    }
    int ival = int((remappedVal * span) + low);
    ival = std::max(low, std::min(ival, high));
    iter->setBrushId(ival);
  }
}


void strokeNode::overridePaintIds(MDataBlock &data, std::vector<Stroke> *geom) const
{

  MStatus st;


  MObject thisObj = thisMObject();
  MRampAttribute remapRamp( thisObj, aPaintIdRemapRamp ); mser;
  const int2 &range = data.inputValue( aPaintIdRemapRange ).asInt2();
  int low = range[0];
  int high = range[1];
  int span = high - low;

  MDataHandle hImageData = data.inputValue(aPaintIdImage, &st);

  if (st.error()) {return;}

  MObject dImageData = hImageData.data();
  MFnPluginData fnImageData( dImageData , &st);

  if (st.error()) {return;}

  cImgData *imageData = (cImgData *)fnImageData.data();

  const CImg<float> image = imageData->fImg->get_normalize(0.0f, 1.0f);
  int w = image.width();
  int h = image.height();

  if (!(w && h )) {return;}

  std::vector<Stroke>::iterator iter;

  for (iter = geom->begin(); iter !=  geom->end(); iter++)
  {
    float u, v;
    iter->getUV(u, v);
    float x, y;
    cImgUtils::toImageCoords(u, v, w, h, x, y);
    float value = image.atXY(int(x), int(y) , 0, 0);

    float remappedVal;
    remapRamp.getValueAtPosition( value, remappedVal, &st );
    if (st.error()) {
      remappedVal =  value;
    }

    int ival = int((remappedVal * span) + low);
    ival = std::max(low, std::min(ival, high));
    iter->setPaintId(ival);
  }

}







void strokeNode::filterStrokes(MDataBlock &data,  std::vector<Stroke> *geom) const

{
  MStatus st;

  bool applyFilters =  data.inputValue(aApplyFilters ).asBool();
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
    aStrokeFilterOperand );


  /* Set the mapped colors so they may be used for filtering */
  if (filterDefinition.usesMap())
  {
    MFloatVectorArray filterMapColors;
    if  (hasTexture(Stroke::aStrokeFilterTexture)) {
      MFloatArray uVals;
      MFloatArray vVals;
      getUVs(strokePool, uVals, vVals);
      st = sampleUVTexture(Stroke::aStrokeFilterTexture, uVals, vVals, filterMapColors);
      if (! st.error()) {
        iter = geom->begin();
        for (unsigned i = 0; iter != geom->end(); iter++, i++) {
          iter->setFilterColor(filterMapColors[i]);
        }
      }
    }
  }

  std::vector<Stroke>::iterator iter;


  // for ( iter = geom->begin(); iter != geom->end(); iter++) {
  //   iter->clearSortStack();
  // }
  if (filterDefinition.hasFilters())
  {


    // std::vector< std::tuple <StrokeSortFilterKey, Stroke::StrokeFilterOperator, int> >::iterator
    // filteriter;

    // filterDefinition

    // for (filteriter = filterDefinition.begin(); filteriter != filterDefinition.end();
    //      ++filteriter)
    for (auto filteriter =  filterDefinition.begin(); filteriter !=  filterDefinition.end() ;
         filteriter++)

    {


      Stroke::StrokeFilterOperator op = std::get<1>(*filteriter);
      if (op == Stroke::kNoOp) {
        continue;
      }

      int value = std::get<2>(*filteriter);
      StrokeSortFilterKey key = std::get<0>(*filteriter);

      std::vector<Stroke>::iterator new_end = geom->end();

      switch  (key)
      {
        case kStrokeId:
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          { return stroke.testStrokeId(op, value) == false; }   );
          break;
        case kParentId:
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          { return stroke.testParentId(op, value) == false; }   );
          break;
        case kBrushId:
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          { return stroke.testBrushId(op, value) == false; }   );
          break;
        case kPaintId:
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          { return stroke.testPaintId(op, value) == false; }   );
          break;
        case kLayerId:
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          { return stroke.testLayerId(op, value) == false; }   );
          break;
        case kRepeatId:
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          { return stroke.testRepeatId(op, value) == false; }   );
          break;

        case kCustomBrushId:
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          {
            return stroke.testCustomBrushId(op, value) == false;
          }   );
          break;

        case kCustomPaintId:
          new_end = std::remove_if(geom->begin(), geom->end(),
                                   [op, value](const Stroke & stroke)
          {
            return stroke.testCustomPaintId(op, value) == false;
          }   );
          break;


        case kMapRed:
          if (useFilterMap) {
            new_end = std::remove_if(geom->begin(), geom->end(),
                                     [op, value](const Stroke & stroke)
            { return stroke.testMapRedId(op, value) == false; }   );
          }
          break;
        case kMapGreen:
          if (useFilterMap) {
            new_end = std::remove_if(geom->begin(), geom->end(),
                                     [op, value](const Stroke & stroke)
            { return stroke.testMapGreenId(op, value) == false; }   );
          }
          break;
        case kMapBlue:
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
  }
  return  ;
}








