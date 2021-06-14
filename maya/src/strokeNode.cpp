
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
#include "strokeNode.h"
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

MObject strokeNode::aMinimumPoints;

MObject strokeNode::aPointDensity;

MObject strokeNode::aRepeats;
MObject strokeNode::aRepeatOffset;
MObject strokeNode::aRepeatMirror;
MObject strokeNode::aRepeatOscillate;
MObject strokeNode::aRepeatFan;

MObject strokeNode::aRepeatSeed;
MObject strokeNode::aRepeatProbability;

MObject strokeNode::aRepeatRandomAngleOffset;
MObject strokeNode::aRepeatRandomTangentOffset;
MObject strokeNode::aRepeatRandomNormalOffset;

MObject strokeNode::aSeed;

MObject strokeNode::aStrokeDirection;

MObject strokeNode::aPaintFlowFactor;

MObject strokeNode::aEntryLength;
MObject strokeNode::aExitLength;
MObject strokeNode::aTransitionBlendMethod;
MObject strokeNode::aLocalContact;

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

MObject strokeNode::aBrushIdTexture;
MObject strokeNode::aBrushIdRemapRamp;
MObject strokeNode::aBrushIdRemapRange;
MObject strokeNode::aPaintIdTexture;
MObject strokeNode::aPaintIdRemapRamp;
MObject strokeNode::aPaintIdRemapRange;
MObject strokeNode::aPaintIdRemapIndices;

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

MObject strokeNode::aStartFrom;
MObject strokeNode::aEndAt;

MObject strokeNode::aTransformPivot;

MObject strokeNode::aRotationTexture;
MObject strokeNode::aRotationScale;
MObject strokeNode::aTranslationTexture;
MObject strokeNode::aTranslationSampleDistance;

MObject strokeNode::aTranslationScale;
MObject strokeNode::aTranslationDirection;

MObject strokeNode::aOutput;
MObject strokeNode::aOutCoil;
MTypeId strokeNode::id(k_strokeNode);

strokeNode::strokeNode() {}

strokeNode::~strokeNode() {}

void *strokeNode::creator()
{
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
  // MFnGenericAttribute gAttr;

  MMatrix identity;
  identity.setToIdentity();

  aProjection = mAttr.create("projection", "prj", MFnMatrixAttribute::kDouble);
  mAttr.setStorable(false);
  mAttr.setHidden(true);
  mAttr.setDefault(identity);
  addAttribute(aProjection);

  aStrokeCountFactor = nAttr.create("strokeCountFactor",
                                    "stcf", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  nAttr.setDefault(1.0);
  st = addAttribute(aStrokeCountFactor);
  mser;

  aPaintFlowFactor = nAttr.create("paintFlowFactor",
                                  "pffc", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.001);
  nAttr.setDefault(1.0);
  st = addAttribute(aPaintFlowFactor);
  mser;

  aMinimumPoints = nAttr.create("minimumPoints", "mnpts", MFnNumericData::kInt);
  mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aMinimumPoints);
  mser;

  aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kDouble);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(1.0);
  st = addAttribute(aPointDensity);
  mser;

  aStrokeDirection = eAttr.create("reverseDirection", "revd", Stroke::kForwards);
  eAttr.addField("forwards", Stroke::kForwards);
  eAttr.addField("backwards", Stroke::kBackwards);
  eAttr.addField("startUppermost", Stroke::kStartUppermost);
  eAttr.addField("endUppermost", Stroke::kEndUppermost);
  eAttr.addField("radialIn", Stroke::kRadialIn);
  eAttr.addField("radialOut", Stroke::kRadialOut);

  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute(aStrokeDirection);
  mser;

  aEntryLength = nAttr.create("entryLength", "enl", MFnNumericData::kDouble);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(1.0);
  st = addAttribute(aEntryLength);
  mser;

  aExitLength = nAttr.create("exitLength", "exl", MFnNumericData::kDouble);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(1.0);
  st = addAttribute(aExitLength);
  mser;

  aLocalContact = nAttr.create("localContact", "lcon", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aLocalContact);

  aTransitionBlendMethod = eAttr.create("transitionBlendMethod", "tbm",
                                        Stroke::kTransitionMin);
  eAttr.addField("minimum", Stroke::kTransitionMin);
  eAttr.addField("maximum", Stroke::kTransitionMax);
  eAttr.addField("blend", Stroke::kTransitionBlend);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute(aTransitionBlendMethod);
  mser;

  aRepeats = nAttr.create("repeats", "rpts", MFnNumericData::kShort);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aRepeats);
  mser;

  aRepeatOffset = nAttr.create("repeatOffset", "rpof", MFnNumericData::kDouble);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  addAttribute(aRepeatOffset);

  aRepeatMirror = nAttr.create("repeatMirror", "rpmr", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aRepeatMirror);

  aRepeatOscillate = nAttr.create("repeatOscillate", "rpoc", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aRepeatOscillate);

  aRepeatFan = uAttr.create("repeatFan", "rpfn", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  uAttr.setStorable(true);
  addAttribute(aRepeatFan);

  aRepeatSeed = nAttr.create("repeatSeed", "rpsd", MFnNumericData::kInt);
  mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aRepeatSeed);
  mser;

  aRepeatProbability = nAttr.create("repeatProbability", "rppb", MFnNumericData::kDouble);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  addAttribute(aRepeatProbability);

  aRepeatRandomAngleOffset = uAttr.create("repeatRandomAngleOffset", "rprao", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  st = addAttribute(aRepeatRandomAngleOffset);
  mser;

  aRepeatRandomTangentOffset = nAttr.create("repeatRandomTangentOffset", "rprto", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aRepeatRandomTangentOffset);

  aRepeatRandomNormalOffset = nAttr.create("repeatRandomNormalOffset", "rpno", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aRepeatRandomNormalOffset);

  aSeed = nAttr.create("seed", "sd", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  st = addAttribute(aSeed);
  mser;

  aPivotFraction = nAttr.create("pivotFraction", "pfrc", MFnNumericData::kDouble);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setMin(0.0);
  nAttr.setMax(1.0);
  nAttr.setDefault(0.5);
  addAttribute(aPivotFraction);

  aRepeatPivot = nAttr.create("repeatPivot", "rpiv", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aRepeatPivot);

  aBrushId = nAttr.create("brushId", "brid", MFnNumericData::kInt);
  mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aBrushId);
  mser;

  aPaintId = nAttr.create("paintId", "ptid", MFnNumericData::kInt);
  mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aPaintId);
  mser;

  aLayerId = nAttr.create("layerId", "lyid", MFnNumericData::kInt);
  mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aLayerId);
  mser;

  aActive = nAttr.create("active", "act", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aActive);
  mser;

  aBrushFollowStroke = nAttr.create("followStroke", "fst", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aBrushFollowStroke);
  mser;

  aBrushTiltRamp = MRampAttribute::createCurveRamp("brushTiltRamp", "brtl");
  st = addAttribute(aBrushTiltRamp);
  mser;
  aBrushBankRamp = MRampAttribute::createCurveRamp("brushBankRamp", "brbk");
  st = addAttribute(aBrushBankRamp);
  mser;
  aBrushTwistRamp = MRampAttribute::createCurveRamp("brushTwistRamp", "brtw");
  st = addAttribute(aBrushTwistRamp);
  mser;

  aBrushTiltRangeMin = uAttr.create("brushTiltRangeMin", "btlrn",
                                    MFnUnitAttribute::kAngle);
  uAttr.setDefault(MAngle(-90, MAngle::kDegrees));
  aBrushTiltRangeMax = uAttr.create("brushTiltRangeMax", "btlrx",
                                    MFnUnitAttribute::kAngle);
  uAttr.setDefault(MAngle(90, MAngle::kDegrees));
  aBrushTiltRange = nAttr.create("brushTiltRange", "btlr", aBrushTiltRangeMin,
                                 aBrushTiltRangeMax);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  st = addAttribute(aBrushTiltRange);
  mser;

  aBrushBankRangeMin = uAttr.create("brushBankRangeMin", "bbnrn",
                                    MFnUnitAttribute::kAngle);
  uAttr.setDefault(MAngle(-90, MAngle::kDegrees));
  aBrushBankRangeMax = uAttr.create("brushBankRangeMax", "bbnrx",
                                    MFnUnitAttribute::kAngle);
  uAttr.setDefault(MAngle(90, MAngle::kDegrees));
  aBrushBankRange = nAttr.create("brushBankRange", "bbnr", aBrushBankRangeMin,
                                 aBrushBankRangeMax);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  st = addAttribute(aBrushBankRange);
  mser;

  aBrushTwistRangeMin = uAttr.create("brushTwistRangeMin", "btwrn",
                                     MFnUnitAttribute::kAngle);
  uAttr.setDefault(MAngle(-180, MAngle::kDegrees));
  aBrushTwistRangeMax = uAttr.create("brushTwistRangeMax", "btwrx",
                                     MFnUnitAttribute::kAngle);
  uAttr.setDefault(MAngle(180, MAngle::kDegrees));
  aBrushTwistRange = nAttr.create("brushTwistRange", "btwr", aBrushTwistRangeMin,
                                  aBrushTwistRangeMax);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  st = addAttribute(aBrushTwistRange);
  mser;

  ////////////

  aBrushIdTexture = nAttr.createColor("brushIdTexture", "bidt");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aBrushIdTexture);

  // aBrushIdTexture = tAttr.create("brushIdImage", "bidi", cImgData::id ) ;
  // tAttr.setStorable(false);
  // st = addAttribute( aBrushIdTexture ); mser;

  aBrushIdRemapRamp = MRampAttribute::createCurveRamp("brushIdRemapRamp", "brrp");
  st = addAttribute(aBrushIdRemapRamp);
  mser;

  aBrushIdRemapRange = nAttr.create("brushIdRemapRange", "brrg", MFnNumericData::k2Int);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aBrushIdRemapRange);

  aPaintIdTexture = nAttr.createColor("paintIdTexture", "pidt");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aPaintIdTexture);

  // aPaintIdTexture = tAttr.create("paintIdImage", "pidi", cImgData::id ) ;
  // tAttr.setStorable(false);
  // st = addAttribute( aPaintIdTexture ); mser;

  aPaintIdRemapRamp = MRampAttribute::createCurveRamp("paintIdRemapRamp", "prrp");
  st = addAttribute(aPaintIdRemapRamp);
  mser;

  aPaintIdRemapRange = nAttr.create("paintIdRemapRange", "prrg", MFnNumericData::k2Int);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aPaintIdRemapRange);

  aPaintIdRemapIndices = tAttr.create("paintIdRemapIndices", "piri", MFnData::kString);
  tAttr.setStorable(true);
  addAttribute(aPaintIdRemapIndices);

  ////////////

  aStrokeSortKey = eAttr.create("strokeSortKey", "stsk", Stroke::kBrushId);
  eAttr.addField("Id", Stroke::kStrokeId);
  eAttr.addField("Brush Id", Stroke::kBrushId);
  eAttr.addField("Paint Id", Stroke::kPaintId);
  eAttr.addField("Repeat Id", Stroke::kRepeatId);
  eAttr.addField("Layer Id", Stroke::kLayerId);
  eAttr.addField("Target Count", Stroke::kTargetCount);
  eAttr.addField("Custom Brush Id", Stroke::kCustomBrushId);
  eAttr.addField("Map Red", Stroke::kMapRed);
  eAttr.addField("Map Green", Stroke::kMapGreen);
  eAttr.addField("Map Blue", Stroke::kMapBlue);

  aStrokeSortDirection = eAttr.create("strokeSortDirection", "stsd",
                                      Stroke::kSortAscending);
  eAttr.addField("Ascending", Stroke::kSortAscending);
  eAttr.addField("Descending", Stroke::kSortDescending);

  aStrokeSortList = cAttr.create("strokeSortList", "stsl");
  cAttr.addChild(aStrokeSortKey);
  cAttr.addChild(aStrokeSortDirection);
  cAttr.setArray(true);
  st = addAttribute(aStrokeSortList);
  mser;

  aStrokeSortTexture = nAttr.createColor("strokeSortTexture", "stst");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aStrokeSortTexture);

  aApplySort = nAttr.create("applySort", "apst", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aApplySort);

  aStrokeFilterKey = eAttr.create("strokeFilterKey", "stfk", Stroke::kBrushId);
  eAttr.addField("Id", Stroke::kStrokeId);
  eAttr.addField("Brush Id", Stroke::kBrushId);
  eAttr.addField("Paint Id", Stroke::kPaintId);
  eAttr.addField("Repeat Id", Stroke::kRepeatId);
  eAttr.addField("Layer Id", Stroke::kLayerId);
  eAttr.addField("Target Count", Stroke::kTargetCount);
  eAttr.addField("Custom Brush Id", Stroke::kCustomBrushId);
  eAttr.addField("Map Red", Stroke::kMapRed);
  eAttr.addField("Map Green", Stroke::kMapGreen);
  eAttr.addField("Map Blue", Stroke::kMapBlue);

  eAttr.setHidden(false);
  eAttr.setKeyable(true);

  aStrokeFilterOperator = eAttr.create("strokeFilterOperator", "stfop",
                                       Stroke::kGreaterThan);
  eAttr.addField(">", Stroke::kGreaterThan);
  eAttr.addField("<", Stroke::kLessThan);
  eAttr.addField("==", Stroke::kEqualTo);
  eAttr.addField("!=", Stroke::kNotEqualTo);
  eAttr.addField("nop", Stroke::kNoOp);
  eAttr.setHidden(false);
  eAttr.setKeyable(true);

  aStrokeFilterOperand = nAttr.create("strokeFilterOperand", "stfod",
                                      MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);

  aStrokeFilterList = cAttr.create("strokeFilterList", "stfl");
  cAttr.addChild(aStrokeFilterKey);
  cAttr.addChild(aStrokeFilterOperator);
  cAttr.addChild(aStrokeFilterOperand);
  cAttr.setArray(true);
  st = addAttribute(aStrokeFilterList);
  mser;

  aStrokeFilterTexture = nAttr.createColor("strokeFilterTexture", "stft");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aStrokeFilterTexture);

  aApplyFilters = nAttr.create("applyFilters", "apfl", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aApplyFilters);

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

  aTransformPivot = nAttr.create("transformPivot", "tfpv", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aTransformPivot);

  aRotationTexture = nAttr.createColor("rotationTexture", "rtx");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aRotationTexture);

  aTranslationTexture = nAttr.createColor("translationTexture", "trx");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aTranslationTexture);

  aTranslationSampleDistance = nAttr.create("translationSampleDistance",
                                            "tsd", MFnNumericData::kDouble);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(0.01);
  addAttribute(aTranslationSampleDistance);

  aRotationScale = nAttr.create("rotationScale",
                                "rsc", MFnNumericData::kDouble);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(1.0);
  addAttribute(aRotationScale);
  aTranslationScale = nAttr.create("translationScale",
                                   "tsc", MFnNumericData::kDouble);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(1.0);
  addAttribute(aTranslationScale);

  aTranslationDirection = eAttr.create("translationDirection", "tdir", Stroke::kConstant);
  eAttr.addField("constant", Stroke::kConstant);
  eAttr.addField("alternateRepeat", Stroke::kAlternateRepeat);
  eAttr.addField("alternateAll", Stroke::kAlternateAll);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute(aTranslationDirection);
  ;
  mser;

  aOutCoil = uAttr.create("outCoil", "ocl", MFnUnitAttribute::kAngle);
  uAttr.setReadable(true);
  uAttr.setWritable(false);
  uAttr.setStorable(false);
  st = addAttribute(aOutCoil);

  aOutput = tAttr.create("output", "out", strokeData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  st = attributeAffects(aPointDensity, aOutput);
  st = attributeAffects(aMinimumPoints, aOutput);
  st = attributeAffects(aStrokeDirection, aOutput);
  st = attributeAffects(aPaintFlowFactor, aOutput);
  st = attributeAffects(aRepeats, aOutput);
  st = attributeAffects(aRepeatOffset, aOutput);
  st = attributeAffects(aRepeatMirror, aOutput);
  st = attributeAffects(aRepeatOscillate, aOutput);
  st = attributeAffects(aRepeatSeed, aOutput);
  st = attributeAffects(aRepeatProbability, aOutput);
  st = attributeAffects(aRepeatRandomAngleOffset, aOutput);
  st = attributeAffects(aRepeatRandomTangentOffset, aOutput);
  st = attributeAffects(aRepeatRandomNormalOffset, aOutput);
  st = attributeAffects(aRepeatFan, aOutput);
  st = attributeAffects(aSeed, aOutput);
  st = attributeAffects(aActive, aOutput);
  st = attributeAffects(aStrokeCountFactor, aOutput);
  st = attributeAffects(aPivotFraction, aOutput);
  st = attributeAffects(aRepeatPivot, aOutput);
  st = attributeAffects(aEntryLength, aOutput);
  st = attributeAffects(aExitLength, aOutput);
  st = attributeAffects(aLocalContact, aOutput);
  st = attributeAffects(aTransitionBlendMethod, aOutput);
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
  st = attributeAffects(aBrushIdTexture, aOutput);
  st = attributeAffects(aBrushIdRemapRamp, aOutput);
  st = attributeAffects(aBrushIdRemapRange, aOutput);
  st = attributeAffects(aPaintIdTexture, aOutput);
  st = attributeAffects(aPaintIdRemapRamp, aOutput);
  st = attributeAffects(aPaintIdRemapRange, aOutput);
  st = attributeAffects(aPaintIdRemapIndices, aOutput);
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
  st = attributeAffects(aEndAt, aOutput);
  st = attributeAffects(aTransformPivot, aOutput);
  st = attributeAffects(aRotationTexture, aOutput);
  st = attributeAffects(aTranslationTexture, aOutput);
  st = attributeAffects(aTranslationSampleDistance, aOutput);
  st = attributeAffects(aTranslationScale, aOutput);
  st = attributeAffects(aTranslationDirection, aOutput);
  st = attributeAffects(aRotationScale, aOutput);

  attributeAffects(aPointDensity, aOutCoil);
  attributeAffects(aMinimumPoints, aOutCoil);
  attributeAffects(aStrokeDirection, aOutCoil);
  attributeAffects(aPaintFlowFactor, aOutCoil);
  attributeAffects(aRepeats, aOutCoil);
  attributeAffects(aRepeatOffset, aOutCoil);
  attributeAffects(aRepeatMirror, aOutCoil);
  attributeAffects(aRepeatOscillate, aOutCoil);
  attributeAffects(aRepeatSeed, aOutCoil);
  attributeAffects(aRepeatProbability, aOutCoil);
  attributeAffects(aRepeatRandomAngleOffset, aOutCoil);
  attributeAffects(aRepeatRandomTangentOffset, aOutCoil);
  attributeAffects(aRepeatRandomNormalOffset, aOutCoil);
  attributeAffects(aRepeatFan, aOutCoil);
  attributeAffects(aSeed, aOutCoil);
  attributeAffects(aActive, aOutCoil);
  attributeAffects(aStrokeCountFactor, aOutCoil);
  attributeAffects(aPivotFraction, aOutCoil);
  attributeAffects(aRepeatPivot, aOutCoil);
  attributeAffects(aEntryLength, aOutCoil);
  attributeAffects(aExitLength, aOutCoil);
  attributeAffects(aLocalContact, aOutCoil);
  attributeAffects(aTransitionBlendMethod, aOutCoil);
  attributeAffects(aBrushTiltRamp, aOutCoil);
  attributeAffects(aBrushBankRamp, aOutCoil);
  attributeAffects(aBrushTwistRamp, aOutCoil);
  attributeAffects(aBrushTiltRange, aOutCoil);
  attributeAffects(aBrushBankRange, aOutCoil);
  attributeAffects(aBrushTwistRange, aOutCoil);
  attributeAffects(aBrushFollowStroke, aOutCoil);
  attributeAffects(aBrushId, aOutCoil);
  attributeAffects(aPaintId, aOutCoil);
  attributeAffects(aLayerId, aOutCoil);
  attributeAffects(aProjection, aOutCoil);
  attributeAffects(aBrushIdTexture, aOutCoil);
  attributeAffects(aBrushIdRemapRamp, aOutCoil);
  attributeAffects(aBrushIdRemapRange, aOutCoil);
  attributeAffects(aPaintIdTexture, aOutCoil);
  attributeAffects(aPaintIdRemapRamp, aOutCoil);
  attributeAffects(aPaintIdRemapRange, aOutCoil);
  attributeAffects(aPaintIdRemapIndices, aOutCoil);
  attributeAffects(aStrokeSortKey, aOutCoil);
  attributeAffects(aStrokeSortDirection, aOutCoil);
  attributeAffects(aStrokeSortList, aOutCoil);
  attributeAffects(aStrokeSortTexture, aOutCoil);
  attributeAffects(aStrokeFilterKey, aOutCoil);
  attributeAffects(aStrokeFilterOperator, aOutCoil);
  attributeAffects(aStrokeFilterOperand, aOutCoil);
  attributeAffects(aStrokeFilterList, aOutCoil);
  attributeAffects(aStrokeFilterTexture, aOutCoil);
  attributeAffects(aStartFrom, aOutCoil);
  attributeAffects(aEndAt, aOutCoil);
  attributeAffects(aTransformPivot, aOutCoil);
  attributeAffects(aRotationTexture, aOutCoil);
  attributeAffects(aTranslationTexture, aOutCoil);
  attributeAffects(aTranslationSampleDistance, aOutCoil);
  attributeAffects(aTranslationScale, aOutCoil);
  attributeAffects(aTranslationDirection, aOutCoil);
  attributeAffects(aRotationScale, aOutCoil);

  return (MS::kSuccess);
}

MStatus strokeNode::generateStrokeGeometry(MDataBlock &data,
                                           std::vector<Stroke> *geom,
                                           double &coilRadians) const
{
  return MS::kSuccess;
}

MStatus strokeNode::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  if (!((plug == aOutput) || (plug == aOutCoil)))
  {
    return (MS::kUnknownParameter);
  }

  double coilRadians = 0.0;

  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(strokeData::id);

  MObject dOut = fnOut.create(kdid, &st);
  strokeData *newData = (strokeData *)fnOut.data(&st);
  mser;

  std::vector<Stroke> *geom = newData->fGeometry;
  st = generateStrokeGeometry(data, geom, coilRadians);

  bool transformPivot = data.inputValue(aTransformPivot).asBool();

  assignUVs(data, geom);
  transformStrokes(data, geom);

  if (transformPivot)
  {
    assignUVs(data, geom);
  }
  overrideBrushIds(data, geom);
  overridePaintIds(data, geom);
  filterStrokes(data, geom);
  sortStrokes(data, geom);
  cullStartEnd(data, geom);

  MDataHandle hCoil = data.outputValue(aOutCoil);
  hCoil.set(MAngle(coilRadians));
  hCoil.setClean();

  hOutput.set(newData);
  hOutput.setClean();
  // data.setClean(plug);

  return MS::kSuccess;
}

void strokeNode::assignUVs(MDataBlock &data, std::vector<Stroke> *geom) const
{
  MMatrix inverseProjMat = data.inputValue(strokeNode::aProjection).asMatrix().inverse();
  std::vector<Stroke>::iterator iter;
  iter = geom->begin();
  for (iter = geom->begin(); iter != geom->end(); iter++)
  {
    iter->setUV(inverseProjMat);
  }
}

void strokeNode::getUVs(std::vector<Stroke> &strokes, MFloatArray &uVals,
                        MFloatArray &vVals) const
{
  int len = strokes.size();
  uVals.setLength(len);
  vVals.setLength(len);

  std::vector<Stroke>::iterator iter = strokes.begin();
  for (unsigned i = 0; iter != strokes.end(); iter++, i++)
  {
    float &u = uVals[i];
    float &v = vVals[i];
    iter->getUV(u, v);
  }
}

bool strokeNode::overrideBrushIds(MDataBlock &data, std::vector<Stroke> *geom) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  if (!TexUtils::hasTexture(thisObj, strokeNode::aBrushIdTexture))
  {
    return false;
  }

  MFloatArray uVals;
  MFloatArray vVals;
  getUVs(*geom, uVals, vVals);

  const int2 &range = data.inputValue(aBrushIdRemapRange).asInt2();
  int low = range[0];
  int high = range[1];

  MIntArray result;
  st = TexUtils::sampleUVTexture(thisObj, strokeNode::aBrushIdTexture, uVals, vVals,
                                 strokeNode::aBrushIdRemapRamp, low, high, result);

  if (st.error())
  {
    return false;
  }

  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++)
  {
    iter->setBrushId(result[i]);
  }
  return true;
}

bool strokeNode::overridePaintIds(MDataBlock &data, std::vector<Stroke> *geom) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  if (!TexUtils::hasTexture(thisObj, strokeNode::aPaintIdTexture))
  {
    return false;
  }

  MFloatArray uVals;
  MFloatArray vVals;
  getUVs(*geom, uVals, vVals);

  MString remapIndicesString = data.inputValue(aPaintIdRemapIndices).asString();
  MStringArray remapIndicesSA;
  remapIndicesString.split(' ', remapIndicesSA);
  unsigned remapIndicesLength = remapIndicesSA.length();

  int low, high;
  MIntArray result;
  if (remapIndicesLength)
  {
    low = 0;
    high = remapIndicesLength - 1;
  }
  else
  {
    const int2 &range = data.inputValue(aPaintIdRemapRange).asInt2();
    low = range[0];
    high = range[1];
  }
  st = TexUtils::sampleUVTexture(thisObj, strokeNode::aPaintIdTexture, uVals, vVals,
                                 strokeNode::aPaintIdRemapRamp, low, high, result);
  if (st.error())
  {
    return false;
  }

  if (remapIndicesLength)
  {
    MIntArray remapIndices;
    for (int i = 0; i < remapIndicesLength; i++)
    {
      remapIndices.append(remapIndicesSA[i].asInt());
    }

    std::vector<Stroke>::iterator iter = geom->begin();
    for (unsigned i = 0; iter != geom->end(); iter++, i++)
    {
      iter->setPaintId(remapIndices[result[i]]);
    }
  }
  else
  {
    std::vector<Stroke>::iterator iter = geom->begin();
    for (unsigned i = 0; iter != geom->end(); iter++, i++)
    {
      iter->setPaintId(result[i]);
    }
  }
  return true;
}

bool strokeNode::rotateStrokes(
    MDataBlock &data,
    MFloatArray &uVals,
    MFloatArray &vVals,
    std::vector<Stroke> *geom) const
{
  double scale = data.inputValue(aRotationScale).asDouble();
  scale = scale * 90 * deg_to_rad;
  MFloatArray result;
  MStatus st = TexUtils::sampleUVTexture(
      thisMObject(),
      strokeNode::aRotationTexture,
      uVals,
      vVals,
      -scale,
      scale,
      result);

  if (st.error())
  {
    return false;
  }
  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++)
  {
    iter->rotate(result[i]);
  }
  return true;
}

bool strokeNode::translateStrokes(
    MDataBlock &data,
    MFloatArray &uVals,
    MFloatArray &vVals,
    std::vector<Stroke> *geom) const
{
  double sampleDist = data.inputValue(aTranslationSampleDistance).asDouble();
  double scale = data.inputValue(aTranslationScale).asDouble();
  bool transformPivot = data.inputValue(aTransformPivot).asBool();

  Stroke::TranslationDirection directionPolicy = Stroke::TranslationDirection(data.inputValue(aTranslationDirection).asShort());

  //  scale = data.inputValue(aTranslationScale).asDouble();

  MFloatVectorArray result;
  MStatus st = TexUtils::sampleUVGradient(
      thisMObject(),
      strokeNode::aTranslationTexture,
      sampleDist,
      scale,
      uVals,
      vVals,
      result,
      sampleDist);
  if (st.error())
  {
    return false;
  }
  std::vector<Stroke>::iterator iter = geom->begin();
  if (directionPolicy == Stroke::kConstant)
  {
    for (unsigned i = 0; iter != geom->end(); iter++, i++)
    {
      iter->translate(result[i], transformPivot);
    }
  }
  else if (directionPolicy == Stroke::kAlternateRepeat)
  {
    for (unsigned i = 0; iter != geom->end(); iter++, i++)
    {
      if (iter->repeatId() % 2 == 1)
      {
        iter->translate(-result[i], transformPivot);
      }
      else
      {
        iter->translate(result[i], transformPivot);
      }
    }
  }
  else
  {
    for (unsigned i = 0; iter != geom->end(); iter++, i++)
    {
      if (i % 2 == 1)
      {
        iter->translate(-result[i], transformPivot);
      }
      else
      {
        iter->translate(result[i], transformPivot);
      }
    }
  }
  return true;
}

void strokeNode::transformStrokes(MDataBlock &data, std::vector<Stroke> *geom) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  bool doRotation = TexUtils::hasTexture(thisObj, strokeNode::aRotationTexture);
  bool doTranslation = TexUtils::hasTexture(thisObj, strokeNode::aTranslationTexture);

  if (!(doRotation || doTranslation))
  {
    return;
  }

  MFloatArray uVals;
  MFloatArray vVals;
  getUVs(*geom, uVals, vVals);

  if (doRotation)
  {
    rotateStrokes(data, uVals, vVals, geom);
  }
  if (doTranslation)
  {
    translateStrokes(data, uVals, vVals, geom);
  }
}

bool strokeNode::getMappedColors(std::vector<Stroke> *geom, MObject &attribute,
                                 MFloatVectorArray &result) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  if (!TexUtils::hasTexture(thisObj, attribute))
  {
    return false;
  }

  MFloatArray uVals;
  MFloatArray vVals;
  getUVs(*geom, uVals, vVals);

  st = TexUtils::sampleUVTexture(thisObj, attribute, uVals, vVals,
                                 result);
  if (st.error())
  {
    return false;
  }
  return true;
}

bool strokeNode::setFilterMapColor(std::vector<Stroke> *geom) const
{
  MFloatVectorArray result;
  if (!getMappedColors(geom, strokeNode::aStrokeFilterTexture, result))
  {
    return false;
  }
  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++)
  {
    iter->setFilterColor(result[i]);
  }
  return true;
}

bool strokeNode::setSortMapColor(std::vector<Stroke> *geom) const
{

  MFloatVectorArray result;
  if (!getMappedColors(geom, strokeNode::aStrokeSortTexture, result))
  {
    return false;
  }

  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++)
  {
    iter->setSortColor(result[i]);
  }
  return true;
}

void strokeNode::filterStrokes(MDataBlock &data, std::vector<Stroke> *geom) const

{
  MStatus st;

  bool applyFilters = data.inputValue(aApplyFilters).asBool();
  if (!applyFilters)
  {
    return;
  }

  MArrayDataHandle hFilterMulti = data.inputArrayValue(aStrokeFilterList, &st);
  if (st.error())
  {
    return;
  }

  FilterDefinition filterDefinition(
      hFilterMulti,
      aStrokeFilterKey,
      aStrokeFilterOperator,
      aStrokeFilterOperand);

  if (!filterDefinition.hasFilters())
  {
    return;
  }

  /* Set the mapped colors so they may be used for filtering */
  bool useFilterMap = filterDefinition.usesMap();
  if (useFilterMap)
  {
    useFilterMap = setFilterMapColor(geom);
  }

  for (auto filteriter = filterDefinition.begin(); filteriter != filterDefinition.end();
       filteriter++)

  {
    Stroke::FilterOperator op = std::get<1>(*filteriter);
    if (op == Stroke::kNoOp)
    {
      continue;
    }

    int value = std::get<2>(*filteriter);
    Stroke::SortFilterKey key = std::get<0>(*filteriter);

    std::vector<Stroke>::iterator new_end = geom->end();

    switch (key)
    {
    case Stroke::kStrokeId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke) { return stroke.testStrokeId(op, value) == false; });
      break;

    case Stroke::kBrushId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke) { return stroke.testBrushId(op, value) == false; });
      break;
    case Stroke::kPaintId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke) { return stroke.testPaintId(op, value) == false; });
      break;
    case Stroke::kLayerId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke) { return stroke.testLayerId(op, value) == false; });
      break;
    case Stroke::kRepeatId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke) { return stroke.testRepeatId(op, value) == false; });
      break;
    case Stroke::kTargetCount:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke) { return stroke.testTargetCount(op, value) == false; });
      break;

    case Stroke::kCustomBrushId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke) { return stroke.testCustomBrushId(op, value) == false; });
      break;

    case Stroke::kMapRed:
      if (useFilterMap)
      {
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke &stroke) { return stroke.testMapRedId(op, value) == false; });
      }
      break;
    case Stroke::kMapGreen:
      if (useFilterMap)
      {
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke &stroke) { return stroke.testMapGreenId(op, value) == false; });
      }
      break;
    case Stroke::kMapBlue:
      if (useFilterMap)
      {
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke &stroke) { return stroke.testMapBlueId(op, value) == false; });
      }

      break;
    default:
      break;
    }

    geom->erase(new_end, geom->end());
  }

  return;
}

void strokeNode::cullStartEnd(MDataBlock &data, std::vector<Stroke> *geom) const
{

  int startFrom = data.inputValue(aStartFrom).asInt();

  int endAt = data.inputValue(aEndAt).asInt();
  int last = geom->size() - 1;
  if (endAt > -1 && endAt < geom->size())
  {
    last = endAt;
  }

  if (startFrom < 0)
  {
    startFrom = 0;
  }
  if (startFrom > last)
  {
    startFrom = last;
  }

  int diff = geom->size() - (last + 1);

  if (startFrom > 0)
  {
    std::vector<Stroke>::iterator start_iter = geom->begin() + startFrom;
    geom->erase(geom->begin(), geom->begin() + startFrom);
  }

  if (diff > 0)
  {
    std::vector<Stroke>::iterator end_iter = geom->end() - diff;
    geom->erase(end_iter, geom->end());
  }
}

void strokeNode::sortStrokes(MDataBlock &data, std::vector<Stroke> *geom) const

{
  MStatus st;

  bool applySort = data.inputValue(aApplySort).asBool();
  if (!applySort)
  {
    return;
  }

  MArrayDataHandle hSortMulti = data.inputArrayValue(aStrokeSortList, &st);
  if (st.error())
  {
    return;
  }

  SortDefinition sortDefinition(hSortMulti, strokeNode::aStrokeSortKey,
                                strokeNode::aStrokeSortDirection);

  if (!sortDefinition.hasSort())
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
  for (iter = geom->begin(); iter != geom->end(); iter++)
  {
    iter->clearSortStack();
  }

  for (auto sortiter = sortDefinition.begin(); sortiter != sortDefinition.end();
       sortiter++)

  {

    bool ascending = (sortiter->second == Stroke::kSortAscending);
    switch (sortiter->first)
    {
    case Stroke::kStrokeId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendStrokeIdToSortStack(ascending);
      }
      break;
    case Stroke::kBrushId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendBrushIdToSortStack(ascending);
      }
      break;
    case Stroke::kPaintId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendPaintIdToSortStack(ascending);
      }
      break;
    case Stroke::kLayerId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendLayerIdToSortStack(ascending);
      }
      break;
    case Stroke::kRepeatId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendRepeatIdToSortStack(ascending);
      }
      break;
    case Stroke::kTargetCount:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendTargetCountToSortStack(ascending);
      }
      break;
    case Stroke::kCustomBrushId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendCustomBrushIdToSortStack(ascending);
      }
      break;

    case Stroke::kMapRed:
      if (useSortMap)
      {
        for (iter = geom->begin(); iter != geom->end(); iter++)
        {
          iter->appendMapRedIdToSortStack(ascending);
        }
      }
      break;
    case Stroke::kMapGreen:
      if (useSortMap)
      {
        for (iter = geom->begin(); iter != geom->end(); iter++)
        {
          iter->appendMapGreenIdToSortStack(ascending);
        }
      }
      break;
    case Stroke::kMapBlue:
      if (useSortMap)
      {
        for (iter = geom->begin(); iter != geom->end(); iter++)
        {
          iter->appendMapBlueIdToSortStack(ascending);
        }
      }
      break;
    default:
      break;
    }
  }

  std::sort(geom->begin(), geom->end());
}
