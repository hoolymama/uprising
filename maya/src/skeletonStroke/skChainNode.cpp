
#include <maya/MIOStream.h>

#include <maya/MFnPluginData.h>

#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnVectorArrayData.h>

#include <maya/MPlugArray.h>
#include <maya/MObjectArray.h>
#include <maya/MFnField.h>
#include <maya/MAngle.h>

#include "cImgUtils.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "skGraph.h"
#include "skChainNode.h"
#include "cImgData.h"
#include "cImgFloatData.h"

#include <maya/MArrayDataBuilder.h>

struct chainSegment
{
  int index;
  float toPixels;
  MFloatMatrix transformation;

  MVector startPoint;
  MFloatVector startCoord;
  float startRadius;
  float startStampRadius;

  MVector endPoint;
  MFloatVector endCoord;
  float endRadius;
  float endStampRadius;

  chainSegment(int i,
               float point0x,
               float point0y,
               float radius0,
               float point1x,
               float point1y,
               float radius1,
               float maxStampRadius,
               const MFloatMatrix &xform,
               float cmToPixels)
      : index(i),
        toPixels(cmToPixels),
        startPoint(MVector(point0x, point0y, 0.0)),
        startRadius(radius0),
        endPoint(MVector(point1x, point1y, 0.0)),
        endRadius(radius1),
        transformation(xform)
  {
    startCoord = MFloatPoint(startPoint) * transformation;
    endCoord = MFloatPoint(endPoint) * transformation;

    startStampRadius = std::min(maxStampRadius, startRadius);
    endStampRadius = std::min(maxStampRadius, endRadius);
  }

  chainSegment(int i, const MVector &point, float radius, float stampRadius, const MFloatMatrix &xform, float cmToPixels)
      : index(i),
        toPixels(cmToPixels),
        startPoint(point),
        startRadius(radius),
        startStampRadius(stampRadius),
        transformation(xform)
  {
    startPoint.z = 0.0;
    startCoord = MFloatPoint(startPoint) * transformation;
  }

  void applyForce(const MVector &force)
  {
    endPoint = startPoint + force;
    endCoord = MFloatPoint(endPoint) * transformation;
    endRadius = startRadius;
    endStampRadius = startStampRadius;
  }

  /// return true if done (if end point not on white).
  bool stampAndChain(
      CImg<unsigned char> *pOutImage,
      skChain &chain)
  {
    unsigned char color[] = {0};
    int w = pOutImage->width();
    int h = pOutImage->height();

    bool isFirstPoint = index == 0;
    bool done = isOnBlack(w, h, pOutImage, 1);
    // done means we are on black. We can still draw the segment and create the chain if the chain
    // is in progress, but we will return done so that this is the last segment in the chain.
    // However, if this is the first point, then we bail out before drawing anything. We don't start
    // on black.

    bool startIsOnBlack = isFirstPoint && isOnBlack(w, h, pOutImage, 0);

    if (startIsOnBlack && done)
    {
      return true;
    }

    if (isFirstPoint) // do first circle
    {
      chain.add(skPoint(startPoint.x, startPoint.y, startRadius));
      pOutImage->draw_circle(
          startCoord.x,
          startCoord.y,
          startStampRadius * toPixels,
          color);
    }

    // calculate the stamp rectangle

    if (!startPoint.isEquivalent(endPoint))
    {
      MVector side = (MVector::zAxis ^ (endPoint - startPoint)).normal();
      MFloatVector startOffset = side * startStampRadius;
      MFloatVector endOffset = side * endStampRadius;

      MFloatVector pA = MFloatPoint(startPoint + startOffset) * transformation;
      MFloatVector pB = MFloatPoint(startPoint - startOffset) * transformation;
      MFloatVector pC = MFloatPoint(endPoint - endOffset) * transformation;
      MFloatVector pD = MFloatPoint(endPoint + endOffset) * transformation;

      CImg<int> points(4, 2);
      int thePoints[] = {int(pA.x), int(pA.y), int(pB.x), int(pB.y), int(pC.x), int(pC.y), int(pD.x), int(pD.y)};
      int *iterator = thePoints;

      cimg_forX(points, i)
      {
        points(i, 0) = *(iterator++);
        points(i, 1) = *(iterator++);
      }

      pOutImage->draw_polygon(points, color, 1.0f);
    }

    chain.add(skPoint(endPoint.x, endPoint.y, endRadius));
    pOutImage->draw_circle(
        endCoord.x,
        endCoord.y,
        endStampRadius * toPixels,
        color);

    return done;
  }

  /// We use this to stamp an image based on an existing chain
  void stampImage(CImg<unsigned char> *pOutImage)
  {
    unsigned char color[] = {0};
    int w = pOutImage->width();
    int h = pOutImage->height();

    bool isFirstPoint = index == 0;
    // bool done = isOnBlack(w,h,pOutImage, 1);

    // bool startIsOnBlack = isFirstPoint && isOnBlack(w,h,pOutImage, 0);

    if (isFirstPoint) // do first circle
    {
      pOutImage->draw_circle(
          startCoord.x,
          startCoord.y,
          startStampRadius * toPixels,
          color);
    }

    // calculate the stamp polygon
    // This is an isosceles trapezoid aligned along the segment
    // where each end is the width of the circle diameter at the point.

    if (!startPoint.isEquivalent(endPoint))
    {
      MVector side = (MVector::zAxis ^ (endPoint - startPoint)).normal();
      MFloatVector startOffset = side * startStampRadius;
      MFloatVector endOffset = side * endStampRadius;

      MFloatVector pA = MFloatPoint(startPoint + startOffset) * transformation;
      MFloatVector pB = MFloatPoint(startPoint - startOffset) * transformation;
      MFloatVector pC = MFloatPoint(endPoint - endOffset) * transformation;
      MFloatVector pD = MFloatPoint(endPoint + endOffset) * transformation;

      CImg<int> points(4, 2);
      int thePoints[] = {int(pA.x), int(pA.y), int(pB.x), int(pB.y), int(pC.x), int(pC.y), int(pD.x), int(pD.y)};
      int *iterator = thePoints;

      cimg_forX(points, i)
      {
        points(i, 0) = *(iterator++);
        points(i, 1) = *(iterator++);
      }

      pOutImage->draw_polygon(points, color, 1.0f);
    }

    pOutImage->draw_circle(
        endCoord.x,
        endCoord.y,
        endStampRadius * toPixels,
        color);
  }

  bool isOnBlack(int w, int h, CImg<unsigned char> *pOutImage, int param = 1)
  {
    MFloatVector c = param == 1 ? endCoord : startCoord;
    return (
        c.x < 0 ||
        c.x >= w ||
        c.y < 0 ||
        c.y >= h ||
        (*pOutImage)(c.x, c.y) == 0);
  }
};

MObject skChainNode::aImage;

MObject skChainNode::aMaxIterations;
MObject skChainNode::aMinBranchTwigLength;
MObject skChainNode::aMinLooseTwigLength;

MObject skChainNode::aSeedPoints;
MObject skChainNode::aFields;
MObject skChainNode::aFlowImage;
MObject skChainNode::aSeedChainMaxSteps;
MObject skChainNode::aSeedChainSpan;
MObject skChainNode::aSeedChainStampWidth;
MObject skChainNode::aSeedChainWidth;

MObject skChainNode::aForceThreshold;
MObject skChainNode::aFlowRotation;

MObject skChainNode::aDoSeedChains;
MObject skChainNode::aDoFillerChains;

MObject skChainNode::aSpan;
MObject skChainNode::aMaxWidth;
MObject skChainNode::aMinWidth;
MObject skChainNode::aMaxStampWidth;
MObject skChainNode::aProjectionMatrix;
MObject skChainNode::aLongestChain;
MObject skChainNode::aOffsetWidth;

MObject skChainNode::aExtendLeavesAmount;
MObject skChainNode::aExtendLeavesAccuracy;

MObject skChainNode::aZebraImage;
MObject skChainNode::aZebraDilate;

MObject skChainNode::aTrigger;

MObject skChainNode::aMaxChainsPerOutput;
MObject skChainNode::aOutputs;
MObject skChainNode::aOutputImage;
MObject skChainNode::aOutputCount;
MTypeId skChainNode::id(k_skChainNode);

skChainNode::skChainNode() {}

skChainNode::~skChainNode() {}

void *skChainNode::creator()
{
  return new skChainNode();
}

void skChainNode::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus skChainNode::initialize()
{
  MStatus st;
  MString method("skChainNode::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnMatrixAttribute mAttr;
  MFnMessageAttribute msgAttr;
  MFnUnitAttribute uAttr;

  aImage = tAttr.create("image", "img", cImgData::id);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setReadable(false);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  st = addAttribute(aImage);
  mser;

  aSeedPoints = tAttr.create("seedPoints", "spts", MFnData::kVectorArray);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aSeedPoints);

  aFields = msgAttr.create("fields", "flds");
  msgAttr.setArray(true);
  st = addAttribute(aFields);
  aFlowImage = tAttr.create("flowImage", "fli", cImgFloatData::id);
  mser;
  tAttr.setStorable(false);
  tAttr.setKeyable(false);

  addAttribute(aFlowImage);

  aSeedChainSpan = nAttr.create("seedChainSpan", "sspn", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aSeedChainSpan);
  mser;

  aSeedChainWidth = nAttr.create("seedChainWidth", "scw",
                                 MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aSeedChainWidth);
  mser;

  aSeedChainStampWidth = nAttr.create("seedChainStampWidth", "ssw",
                                      MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aSeedChainStampWidth);
  mser;

  aSeedChainMaxSteps = nAttr.create("seedChainMaxSteps", "scms", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(5);
  st = addAttribute(aSeedChainMaxSteps);
  mser;

  aForceThreshold = nAttr.create("forceThreshold", "fth",
                                 MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aForceThreshold);
  mser;

  aFlowRotation = uAttr.create("flowRotation", "frot",
                               MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  uAttr.setStorable(true);
  st = addAttribute(aFlowRotation);
  mser;

  aDoSeedChains = nAttr.create("doSeedChains", "dsc",
                               MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDoSeedChains);

  aDoFillerChains = nAttr.create("doFillerChains", "dfc",
                                 MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  addAttribute(aDoFillerChains);

  aMaxIterations = nAttr.create("maxIterations", "mxi", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(5);
  st = addAttribute(aMaxIterations);
  mser;

  aMinBranchTwigLength = nAttr.create("minBranchTwigLength", "mbtl", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aMinBranchTwigLength);
  mser;

  aMinLooseTwigLength = nAttr.create("minLooseTwigLength", "mltl", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aMinLooseTwigLength);
  mser;

  aSpan = nAttr.create("span", "spn", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aSpan);
  mser;

  aMaxWidth = nAttr.create("maxWidth", "mwd", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aMaxWidth);
  mser;

  aMinWidth = nAttr.create("mainWidth", "mnwd", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aMinWidth);
  mser;

  aMaxStampWidth = nAttr.create("maxStampWidth", "msw",
                                MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aMaxStampWidth);
  mser;

  aLongestChain = nAttr.create("longestChain", "lch", MFnNumericData::kBoolean);
  nAttr.setDefault(false);
  nAttr.setKeyable(true);
  st = addAttribute(aLongestChain);
  mser;

  // aRadiusMult = nAttr.create("radiusMult", "rml", MFnNumericData::kFloat);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setKeyable(true);
  // nAttr.setDefault(1.0f);
  // st = addAttribute(aRadiusMult);
  // mser;

  aOffsetWidth = nAttr.create("offsetWidth", "ofw", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.0f);
  st = addAttribute(aOffsetWidth);
  mser;

  MFloatMatrix identity;
  identity.setToIdentity();
  aProjectionMatrix = mAttr.create("projectionMatrix", "pmat",
                                   MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  mAttr.setDefault(identity);
  addAttribute(aProjectionMatrix);

  aMaxChainsPerOutput = nAttr.create("maxChainsPerOutput", "mco", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(0);
  nAttr.setMin(0);
  st = addAttribute(aMaxChainsPerOutput);
  mser;

  aExtendLeavesAmount = nAttr.create("extendLeavesAmount", "elam", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(0.0f);
  st = addAttribute(aExtendLeavesAmount);

  aExtendLeavesAccuracy = nAttr.create("extendLeavesAccuracy", "elac", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(4);
  addAttribute(aExtendLeavesAccuracy);



  aZebraImage = tAttr.create("zebraImage", "zimg", cImgData::id);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setReadable(false);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  st = addAttribute(aZebraImage);
  mser;

  aZebraDilate = nAttr.create("zebraDilate", "zdil", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(0);
  st = addAttribute(aZebraDilate);

  aTrigger = nAttr.create("trigger", "trg", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setStorable(false);
  nAttr.setReadable(false);
  nAttr.setKeyable(true);
  nAttr.setArray(true);
  nAttr.setIndexMatters(false);
  addAttribute(aTrigger);

  aOutputs = tAttr.create("outputs", "outs", skChainData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setUsesArrayDataBuilder(true);
  addAttribute(aOutputs);

  aOutputCount = nAttr.create("outputCount", "opc", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(false);
  nAttr.setReadable(true);
  nAttr.setWritable(false);
  st = addAttribute(aOutputCount);

  aOutputImage = tAttr.create("outputImage", "oim", cImgData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutputImage);

  attributeAffects(aImage, aOutputs);
  attributeAffects(aZebraImage, aOutputs);
  attributeAffects(aZebraDilate, aOutputs);
  attributeAffects(aMaxIterations, aOutputs);
  attributeAffects(aMinBranchTwigLength, aOutputs);
  attributeAffects(aMinLooseTwigLength, aOutputs);
  attributeAffects(aSpan, aOutputs);
  attributeAffects(aMaxWidth, aOutputs);
  attributeAffects(aMinWidth, aOutputs);
  attributeAffects(aProjectionMatrix, aOutputs);
  attributeAffects(aOffsetWidth, aOutputs);
  attributeAffects(aMaxStampWidth, aOutputs);
  attributeAffects(aMaxChainsPerOutput, aOutputs);
  attributeAffects(aSeedPoints, aOutputs);
  attributeAffects(aFields, aOutputs);
  attributeAffects(aFlowImage, aOutputs);
  attributeAffects(aSeedChainMaxSteps, aOutputs);
  attributeAffects(aSeedChainSpan, aOutputs);
  attributeAffects(aSeedChainWidth, aOutputs);
  attributeAffects(aSeedChainStampWidth, aOutputs);
  attributeAffects(aForceThreshold, aOutputs);
  attributeAffects(aFlowRotation, aOutputs);
  attributeAffects(aDoSeedChains, aOutputs);
  attributeAffects(aDoFillerChains, aOutputs);
  attributeAffects(aTrigger, aOutputs);
  attributeAffects(aLongestChain, aOutputs);
  attributeAffects(aExtendLeavesAmount, aOutputs);
  attributeAffects(aExtendLeavesAccuracy, aOutputs);

  attributeAffects(aImage, aOutputCount);
  attributeAffects(aZebraImage, aOutputCount);
  attributeAffects(aZebraDilate, aOutputCount);
  attributeAffects(aSeedPoints, aOutputCount);
  attributeAffects(aMaxIterations, aOutputCount);
  attributeAffects(aMinBranchTwigLength, aOutputCount);
  attributeAffects(aMinLooseTwigLength, aOutputCount);
  attributeAffects(aSpan, aOutputCount);
  attributeAffects(aMaxWidth, aOutputCount);
  attributeAffects(aMinWidth, aOutputCount);
  attributeAffects(aProjectionMatrix, aOutputCount);
  attributeAffects(aOffsetWidth, aOutputCount);
  attributeAffects(aMaxStampWidth, aOutputCount);
  attributeAffects(aMaxChainsPerOutput, aOutputCount);
  attributeAffects(aSeedChainMaxSteps, aOutputCount);
  attributeAffects(aSeedPoints, aOutputCount);
  attributeAffects(aFields, aOutputCount);
  attributeAffects(aFlowImage, aOutputs);
  attributeAffects(aSeedChainMaxSteps, aOutputCount);
  attributeAffects(aSeedChainSpan, aOutputCount);
  attributeAffects(aSeedChainWidth, aOutputCount);
  attributeAffects(aSeedChainStampWidth, aOutputCount);
  attributeAffects(aForceThreshold, aOutputCount);
  attributeAffects(aFlowRotation, aOutputCount);
  attributeAffects(aDoSeedChains, aOutputCount);
  attributeAffects(aDoFillerChains, aOutputCount);
  attributeAffects(aTrigger, aOutputCount);
  attributeAffects(aLongestChain, aOutputCount);
  attributeAffects(aExtendLeavesAmount, aOutputCount);
  attributeAffects(aExtendLeavesAccuracy, aOutputCount);

  attributeAffects(aImage, aOutputImage);
  attributeAffects(aZebraImage, aOutputImage);
  attributeAffects(aZebraDilate, aOutputImage);
  attributeAffects(aSeedPoints, aOutputImage);
  attributeAffects(aMaxIterations, aOutputImage);
  attributeAffects(aMinBranchTwigLength, aOutputImage);
  attributeAffects(aMinLooseTwigLength, aOutputImage);
  attributeAffects(aSpan, aOutputImage);
  attributeAffects(aMaxWidth, aOutputImage);
  attributeAffects(aMinWidth, aOutputImage);
  attributeAffects(aProjectionMatrix, aOutputImage);
  attributeAffects(aOffsetWidth, aOutputImage);
  attributeAffects(aMaxStampWidth, aOutputImage);
  attributeAffects(aMaxChainsPerOutput, aOutputImage);
  attributeAffects(aSeedChainMaxSteps, aOutputImage);
  attributeAffects(aSeedPoints, aOutputImage);
  attributeAffects(aFields, aOutputImage);
  attributeAffects(aFlowImage, aOutputs);
  attributeAffects(aSeedChainMaxSteps, aOutputImage);
  attributeAffects(aSeedChainSpan, aOutputImage);
  attributeAffects(aSeedChainWidth, aOutputImage);
  attributeAffects(aSeedChainStampWidth, aOutputImage);
  attributeAffects(aForceThreshold, aOutputImage);
  attributeAffects(aFlowRotation, aOutputImage);
  attributeAffects(aDoSeedChains, aOutputImage);
  attributeAffects(aDoFillerChains, aOutputImage);
  attributeAffects(aTrigger, aOutputImage);
  attributeAffects(aLongestChain, aOutputImage);
  attributeAffects(aExtendLeavesAmount, aOutputImage);
  attributeAffects(aExtendLeavesAccuracy, aOutputImage);

  return (MS::kSuccess);
}

MStatus skChainNode::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  if (!((plug == aOutputs) || (plug == aOutputCount) || (plug == aOutputImage)))
  {
    return (MS::kUnknownParameter);
  }

  // Pull on the trigger plugs
  //////////////////////////////////////////////////
  MArrayDataHandle hTrigger = data.inputArrayValue(aTrigger);
  unsigned nt = hTrigger.elementCount();
  for (unsigned j = 0; j < nt; j++, hTrigger.next())
  {
    MDataHandle hTriggerEl = hTrigger.inputValue();
    hTrigger.inputValue().asFloat();
  }

  MDataHandle hOutputImage = data.outputValue(aOutputImage);
  MFnPluginData fnOutImage;
  MTypeId kdidImage(cImgData::id);
  MObject dOutImage = fnOutImage.create(kdidImage, &st);
  mser;
  cImgData *newImageData = (cImgData *)fnOutImage.data(&st);
  mser;
  CImg<unsigned char> *pOutImage = newImageData->fImg;

  int maxChains = data.inputValue(aMaxChainsPerOutput).asInt();
  MFnPluginData fnOut;
  MTypeId kdid(skChainData::id);

  MArrayDataHandle hOutputs = data.outputArrayValue(aOutputs, &st);
  mser;
  MArrayDataBuilder bOutputs = hOutputs.builder();
  int nextPlugIndex = 0;

  if (maxChains < 1)
  {
    // This is the version where we put all the chains on the first output plug.
    MDataHandle hOutput = bOutputs.addElement(0);
    MObject dOut = fnOut.create(kdid, &st);
    skChainData *newData = (skChainData *)fnOut.data(&st);
    mser;

    std::vector<skChain> *geom = newData->fGeometry;
    geom->clear();

    st = generate(data, geom, pOutImage);
    mser;

    hOutput.set(newData);

    nextPlugIndex++;
  }
  else
  {
    // This is the version where we split the chains into chunks and add outputs.
    std::vector<skChain> *allGeom = new std::vector<skChain>;
    st = generate(data, allGeom, pOutImage);
    int geomLength = allGeom->size();

    for (size_t i = 0; i < allGeom->size(); i += maxChains, nextPlugIndex++)
    {
      auto last = std::min(allGeom->size(), i + maxChains);
      auto start_itr = allGeom->begin() + i;
      auto end_itr = allGeom->begin() + last;

      MDataHandle hOutput = bOutputs.addElement(nextPlugIndex);
      MObject dOut = fnOut.create(kdid, &st);
      mser;
      skChainData *newData = (skChainData *)fnOut.data(&st);
      std::vector<skChain> *geom = newData->fGeometry;
      geom->resize(last - i);

      std::copy(start_itr, end_itr, geom->begin());

      hOutput.set(newData);
      // hOutput.setClean();
    }
    delete allGeom;
  }

  // pOutImage->normalize(0, 255);
  hOutputImage.set(newImageData);

  MDataHandle hOutputCount = data.outputValue(aOutputCount);

  hOutputs.set(bOutputs);
  hOutputs.setAllClean();

  hOutputCount.set(nextPlugIndex);
  hOutputCount.setClean();
  hOutputImage.setClean();

  return MS::kSuccess;
}

/*
Generate all the chains.


*/
MStatus skChainNode::generate(
    MDataBlock &data,
    std::vector<skChain> *geom,
    CImg<unsigned char> *pInkImage) const
{
  MStatus st;

  int dilate = data.inputValue(aZebraDilate).asInt();
  CImg<unsigned char> *pImage = cImgUtils::getImage(data, aImage);

  if (!pImage)
  {
    return MS::kUnknownParameter;
  }

  int w = pImage->width();
  int h = pImage->height();
  if (!(w && h))
  {
    return MS::kUnknownParameter;
  }
  
  pInkImage->assign(pImage->get_norm().normalize(0, 1));

  bool doZebra = false;
  CImg<unsigned char> *pZebraImage = cImgUtils::getImage(data, aZebraImage);
  if (pZebraImage)
  {
     int zw = pZebraImage->width();
     int zh = pZebraImage->height();
    if (zw && zh)
    {
      doZebra = true;
      CImg<unsigned char> zImageA = pZebraImage->get_norm().normalize(0, 1);
 
      if (zw != w || zh != h)
      {
        zImageA.resize(w, h);
      }
      CImg<unsigned char> zImageB(zImageA) ;
      zImageB ^= 1; // invert


      // Now we have both images.

      if (dilate < 0)
      {
        zImageA.erode(-dilate);
        zImageB.erode(-dilate);
      }
      else if (dilate > 0)
      {
        zImageA.dilate(dilate);
        zImageB.dilate(dilate);
      }

      zImageA &=(*pInkImage);
      zImageB &=(*pInkImage);
      st = generateFillerChains(data, geom, &zImageA);
      st = generateFillerChains(data, geom, &zImageB);

      pInkImage->assign(zImageA|zImageB);
    }
  } 

  if (!doZebra)
  {
    st = generateFillerChains(data, geom, pInkImage);
  }

  pInkImage->normalize(0, 255);
  // pInkImage->assign(pImage->get_norm().normalize(0, 1));

  // if (data.inputValue(aDoSeedChains).asBool())
  // {
  //   st = generateSeedChains(data, geom, pInkImage);
  // }

  // if (data.inputValue(aDoFillerChains).asBool())
  // {
    // st = generateFillerChains(data, geom, pInkImage);
  // }

  return MS::kSuccess;
}

MStatus skChainNode::generateFillerChains(
    MDataBlock &data,
    std::vector<skChain> *geom,
    CImg<unsigned char> *pInkImage) const
{

  int w = pInkImage->width();
  int h = pInkImage->height();

  float cmToPixels = getCmToPixels(data, pInkImage);
  float pixelsToCm = 1.0 / cmToPixels;

  int minBranchLengthPixels = int(data.inputValue(aMinBranchTwigLength).asFloat() * cmToPixels);
  minBranchLengthPixels = std::max(minBranchLengthPixels, 1);

  int minLooseTwigLengthPixels = int(data.inputValue(aMinLooseTwigLength).asFloat() * cmToPixels);
  minLooseTwigLengthPixels = std::max(minLooseTwigLengthPixels, 1);

  int spanPixels = int(data.inputValue(aSpan).asFloat() * cmToPixels);
  spanPixels = std::max(spanPixels, 1);

  float maxRadiusPixels = data.inputValue(aMaxWidth).asFloat() * cmToPixels * 0.5f;
  maxRadiusPixels = std::max(maxRadiusPixels, 1.0f);

  float minRadiusPixels = data.inputValue(aMinWidth).asFloat() * cmToPixels * 0.5f;
  minRadiusPixels = std::max(minRadiusPixels, 0.0f);

  float maxStampRadius = data.inputValue(aMaxStampWidth).asFloat() * 0.5;
  float maxStampRadiusPixels = data.inputValue(aMaxStampWidth).asFloat() * cmToPixels * 0.5f;
  maxStampRadiusPixels = std::max(maxStampRadiusPixels, 1.0f);

  float radiusOffsetPixels = int(data.inputValue(aOffsetWidth).asFloat() * 0.5 * cmToPixels);
  // float radiusMult = data.inputValue(aRadiusMult).asFloat();
  int maxIterations = data.inputValue(aMaxIterations).asInt();

  bool longestChain = data.inputValue(aLongestChain).asBool();

  float extendLeavesAmount = data.inputValue(aExtendLeavesAmount).asFloat();
  int extendLeavesAccuracy = data.inputValue(aExtendLeavesAccuracy).asInt();

  int lastNumNewChains = 999999;
  for (int i = 0; i < maxIterations; ++i)
  {
    // make a skeleton image from the BW image
    CImg<float> mat = pInkImage->get_distance(0);
    const CImgList<float> grad = mat.get_gradient("xyz");
    CImg<float> flux = pInkImage->get_flux(grad, 1, 1);
    CImg<bool> skel = pInkImage->get_skeleton(flux, mat, true, 0);

    cimg_forXY(skel, x, y)
    {
      if (!skel(x, y))
      {
        mat(x, y) = 0.0;
      }
    }

    //////////////////
    // now we have the Medial Axis Transform (mat) and image to stamp pInkImage
    skGraph g(mat); // build

    if (longestChain)
    {
      g.trimToLongestChain();
      g.prune(minBranchLengthPixels);
      g.removeLooseTwigs(minLooseTwigLengthPixels);
      g.adjustRadius(radiusOffsetPixels, maxRadiusPixels, minRadiusPixels);
      g.extendLeaves(extendLeavesAmount, extendLeavesAccuracy);
      // Single chain is left, so no need to detach branches
    }
    else
    {
      g.prune(minBranchLengthPixels);
      g.removeLooseTwigs(minLooseTwigLengthPixels);
      g.adjustRadius(radiusOffsetPixels, maxRadiusPixels, minRadiusPixels);
      g.detachBranches();
      g.extendLeaves(extendLeavesAmount, extendLeavesAccuracy);
    }

    if (!g.numNodes())
    {
      break;
    }

    //////////////////
    MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();

    int nChainsBefore = geom->size();
    int numAdded = g.getChains(projection, *geom, spanPixels);

    if (numAdded == 0)
    {
      break;
    }

    // draw the chains
    MFloatMatrix transformation = getTransformProjToImage(data, w, h);

    std::vector<skChain>::const_iterator chainIter = geom->begin() + nChainsBefore;
    for (; chainIter != geom->end(); chainIter++)
    {
      const std::vector<skPoint> &skpoints = chainIter->points();
      std::vector<skPoint>::const_iterator pointIter0 = skpoints.begin();
      std::vector<skPoint>::const_iterator pointIter1 = std::next(pointIter0);

      if (skpoints.size() < 2)
      {
        continue;
      }
      for (int index = 0; pointIter1 != skpoints.end(); pointIter0++, pointIter1++, index++)
      {

        chainSegment segment(
            index,
            pointIter0->x,
            pointIter0->y,
            pointIter0->radius,
            pointIter1->x,
            pointIter1->y,
            pointIter1->radius,
            maxStampRadius,
            transformation,
            cmToPixels);

        segment.stampImage(pInkImage);
      }
    }
  }

  return MS::kSuccess;
}

float skChainNode::getCmToPixels(MDataBlock &data, const CImg<unsigned char> *pImage) const
{
  MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();
  return float(pImage->width()) / (projection[0][0] * 2.0);
}

MFloatMatrix skChainNode::getTransformProjToImage(MDataBlock &data, int w, int h) const
{
  MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();

  MFloatMatrix norm;
  norm.setToIdentity();
  norm[0][0] = w * 0.5;
  norm[1][1] = -h * 0.5;
  norm[3][0] = w * 0.5;
  norm[3][1] = h * 0.5;

  return projection.inverse() * norm;
}

int skChainNode::getFields(MObjectArray &result) const
{
  MPlugArray plugArray;
  MPlug fieldArrayPlug(thisMObject(), aFields);
  int elementCount = fieldArrayPlug.numElements();
  for (size_t i = 0; i < elementCount; i++)
  {
    MPlug fieldPlug = fieldArrayPlug.elementByPhysicalIndex(i);
    bool hasConnections = fieldPlug.connectedTo(plugArray, 1, 0);
    if (hasConnections)
    {
      result.append(plugArray[i].node());
    }
  }
  return result.length();
}

MStatus skChainNode::generateSeedChains(
    MDataBlock &data,
    std::vector<skChain> *geom,
    CImg<unsigned char> *pInkImage) const
{

  MObjectArray fields;
  int numFields = getFields(fields);

  int w = pInkImage->width();
  int h = pInkImage->height();

  float cmToPixels = getCmToPixels(data, pInkImage);
  float pixelsToCm = 1.0 / cmToPixels;

  float circleStampRadius = data.inputValue(aSeedChainStampWidth).asFloat() * 0.5;
  float circleRadius = data.inputValue(aSeedChainWidth).asFloat() * 0.5;

  float span = data.inputValue(aSeedChainSpan).asFloat();
  int maxSteps = data.inputValue(aSeedChainMaxSteps).asInt();

  float circleStampRadiusPixels = circleStampRadius * cmToPixels;
  circleStampRadiusPixels = std::max(circleStampRadiusPixels, 1.0f);
  circleStampRadius = circleStampRadiusPixels * pixelsToCm;

  float forceThreshold = data.inputValue(aForceThreshold).asFloat();
  float angle = float(data.inputValue(aFlowRotation).asAngle().asRadians());

  MFloatMatrix transformation = getTransformProjToImage(data, w, h);

  CImg<float> *pFlowImage = cImgUtils::getFloatImage(data, aFlowImage);
  MFloatMatrix flowTransformation;
  MFloatMatrix flowTransformationInverse;
  if (pFlowImage)
  {
    MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();
    int fw = pFlowImage->width();
    int fh = pFlowImage->height();
    flowTransformation = getTransformProjToImage(data, fw, fh);

    MFloatMatrix rot;
    rot.setToIdentity();
    rot[0][0] = cos(angle);
    rot[0][1] = -sin(angle);
    rot[1][0] = sin(angle);
    rot[1][1] = cos(angle);

    flowTransformationInverse = flowTransformation.inverse() * rot;
  }

  MVectorArray seedPoints = MFnVectorArrayData(data.inputValue(aSeedPoints).data()).array();

  unsigned len = seedPoints.length();

  for (size_t i = 0; i < len; i++)
  {
    skChain chain;
    propagateAndStamp(
        seedPoints[i],
        fields,
        transformation,
        flowTransformation,
        flowTransformationInverse,
        circleRadius,
        circleStampRadius,
        span,
        cmToPixels,
        maxSteps,
        forceThreshold,
        angle,
        pInkImage,
        pFlowImage,
        chain);
    if (chain.size() > 1)
    {
      geom->push_back(chain);
    }
  }

  return MS::kSuccess;
}

void skChainNode::propagateAndStamp(
    const MVector &seedPoint,
    const MObjectArray &fields,
    const MFloatMatrix &transformation,
    const MFloatMatrix &flowTransformation,
    const MFloatMatrix &flowTransformationInverse,
    float circleRadius,
    float circleStampRadius,
    float span,
    float cmToPixels,
    int maxSteps,
    float forceThreshold,
    float angle,
    CImg<unsigned char> *pInkImage,
    CImg<float> *pFlowImage,
    skChain &chain) const
{

  float circleStampRadiusPixels = circleStampRadius * cmToPixels;

  MFnField fnField;
  unsigned char color[] = {0};

  MVector currPoint(seedPoint);

  for (size_t i = 0; i < maxSteps; i++)
  {

    // Initialize segment with first point
    chainSegment segment(
        i, currPoint,
        circleRadius,
        circleStampRadius,
        transformation,
        cmToPixels);

    MVector forceAccumulated;

    ////////////////////// Force from fields
    int numFields = fields.length();
    if (numFields > 0)
    {
      MVectorArray force;
      MVectorArray velocity(1, MVector::zero);
      MDoubleArray mass(1, 1.0);
      for (size_t f = 0; f < fields.length(); f++)
      {
        fnField.setObject(fields[f]);
        MVectorArray position(1, currPoint);
        MVectorArray result;
        fnField.getForceAtPoint(position, velocity, mass, force);
        forceAccumulated += force[0];
      }
    }
    //////////////////////

    // Now calculate force from the flowField
    //////////////////////
    if (pFlowImage)
    {
      int fw = pFlowImage->width();
      int fh = pFlowImage->height();
      MVector currPointImg = MFloatPoint(currPoint) * flowTransformation;
      int imageCoordX = int(currPointImg.x);
      int imageCoordY = int(currPointImg.y);
      if ((imageCoordX >= 0) &&
          (imageCoordY >= 0) &&
          (imageCoordX < fw) &&
          (imageCoordY < fh))
      {

        float x = (*pFlowImage)(imageCoordX, imageCoordY, 0, 0);
        float y = (*pFlowImage)(imageCoordX, imageCoordY, 0, 1);
        forceAccumulated += MVector((MFloatVector(x, y, 0.0)) * flowTransformationInverse);
      }
    }
    //////////////////////

    // If there was no force, then we are done.
    forceAccumulated.z = 0.0;
    if (forceAccumulated.length() < forceThreshold)
    {
      return;
    }

    forceAccumulated = forceAccumulated.normal() * span;

    segment.applyForce(forceAccumulated);
    bool done = segment.stampAndChain(pInkImage, chain);

    // stampAndChain returns true when
    if (done)
    {
      return;
    }

    currPoint = segment.endPoint;
    circleRadius = segment.endRadius;
    circleStampRadius = segment.endStampRadius;
  }
}