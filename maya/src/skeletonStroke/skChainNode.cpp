
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

#include "cImgUtils.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "skGraph.h"
#include "skChainNode.h"

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
MObject skChainNode::aSeedChainMaxSteps;
MObject skChainNode::aSeedChainSpan;
MObject skChainNode::aSeedChainStampWidth;
MObject skChainNode::aSeedChainWidth;

MObject skChainNode::aDoSeedChains;
MObject skChainNode::aDoFillerChains;

MObject skChainNode::aSpan;
MObject skChainNode::aMaxWidth;
MObject skChainNode::aMaxStampWidth;
MObject skChainNode::aProjectionMatrix;
MObject skChainNode::aRadiusMult;
MObject skChainNode::aRadiusOffset;

MObject skChainNode::aMaxChainsPerOutput;
MObject skChainNode::aOutputs;
MObject skChainNode::aOutputCount;

MObject skChainNode::aOutputImage;

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

  aImage = tAttr.create("image", "img", cImgData::id);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setReadable(false);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  st = addAttribute(aImage);
  mser;

  aSeedPoints = tAttr.create("seedPoints", "spts", MFnData::kVectorArray);
  tAttr.setStorable(false);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aSeedPoints);

  aFields = msgAttr.create("fields", "flds");
  msgAttr.setArray(true);
  st = addAttribute(aFields);

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
  nAttr.setDefault(5);
  st = addAttribute(aSeedChainMaxSteps);
  mser;

  aDoSeedChains = nAttr.create("doSeedChains", "dsc",
                               MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDoSeedChains);

  aDoFillerChains = nAttr.create("doFillerChains", "dfc",
                                 MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aDoFillerChains);

  aMaxIterations = nAttr.create("maxIterations", "mxi", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
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

  aMaxStampWidth = nAttr.create("maxStampWidth", "msw",
                                MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aMaxStampWidth);
  mser;

  aRadiusMult = nAttr.create("radiusMult", "rml", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0f);
  st = addAttribute(aRadiusMult);
  mser;

  aRadiusOffset = nAttr.create("radiusOffset", "rof", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.0f);
  st = addAttribute(aRadiusOffset);
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
  attributeAffects(aMaxIterations, aOutputs);
  attributeAffects(aMinBranchTwigLength, aOutputs);
  attributeAffects(aMinLooseTwigLength, aOutputs);
  attributeAffects(aSpan, aOutputs);
  attributeAffects(aMaxWidth, aOutputs);
  attributeAffects(aProjectionMatrix, aOutputs);
  attributeAffects(aRadiusMult, aOutputs);
  attributeAffects(aRadiusOffset, aOutputs);
  attributeAffects(aMaxStampWidth, aOutputs);
  attributeAffects(aMaxChainsPerOutput, aOutputs);
  attributeAffects(aSeedPoints, aOutputs);
  attributeAffects(aFields, aOutputs);
  attributeAffects(aSeedChainMaxSteps, aOutputs);
  attributeAffects(aSeedChainSpan, aOutputs);
  attributeAffects(aSeedChainWidth, aOutputs);
  attributeAffects(aSeedChainStampWidth, aOutputs);
  attributeAffects(aDoSeedChains, aOutputs);
  attributeAffects(aDoFillerChains, aOutputs);

  attributeAffects(aImage, aOutputCount);
  attributeAffects(aSeedPoints, aOutputCount);
  attributeAffects(aMaxIterations, aOutputCount);
  attributeAffects(aMinBranchTwigLength, aOutputCount);
  attributeAffects(aMinLooseTwigLength, aOutputCount);
  attributeAffects(aSpan, aOutputCount);
  attributeAffects(aMaxWidth, aOutputCount);
  attributeAffects(aProjectionMatrix, aOutputCount);
  attributeAffects(aRadiusMult, aOutputCount);
  attributeAffects(aRadiusOffset, aOutputCount);
  attributeAffects(aMaxStampWidth, aOutputCount);
  attributeAffects(aMaxChainsPerOutput, aOutputCount);
  attributeAffects(aSeedChainMaxSteps, aOutputCount);
  attributeAffects(aSeedPoints, aOutputCount);
  attributeAffects(aFields, aOutputCount);
  attributeAffects(aSeedChainMaxSteps, aOutputCount);
  attributeAffects(aSeedChainSpan, aOutputCount);
  attributeAffects(aSeedChainWidth, aOutputCount);
  attributeAffects(aSeedChainStampWidth, aOutputCount);
  attributeAffects(aDoSeedChains, aOutputCount);
  attributeAffects(aDoFillerChains, aOutputCount);

  attributeAffects(aImage, aOutputImage);
  attributeAffects(aSeedPoints, aOutputImage);
  attributeAffects(aMaxIterations, aOutputImage);
  attributeAffects(aMinBranchTwigLength, aOutputImage);
  attributeAffects(aMinLooseTwigLength, aOutputImage);
  attributeAffects(aSpan, aOutputImage);
  attributeAffects(aMaxWidth, aOutputImage);
  attributeAffects(aProjectionMatrix, aOutputImage);
  attributeAffects(aRadiusMult, aOutputImage);
  attributeAffects(aRadiusOffset, aOutputImage);
  attributeAffects(aMaxStampWidth, aOutputImage);
  attributeAffects(aMaxChainsPerOutput, aOutputImage);
  attributeAffects(aSeedChainMaxSteps, aOutputImage);
  attributeAffects(aSeedPoints, aOutputImage);
  attributeAffects(aFields, aOutputImage);
  attributeAffects(aSeedChainMaxSteps, aOutputImage);
  attributeAffects(aSeedChainSpan, aOutputImage);
  attributeAffects(aSeedChainWidth, aOutputImage);
  attributeAffects(aSeedChainStampWidth, aOutputImage);
  attributeAffects(aDoSeedChains, aOutputImage);
  attributeAffects(aDoFillerChains, aOutputImage);

  return (MS::kSuccess);
}

MStatus skChainNode::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  if (!((plug == aOutputs) || (plug == aOutputCount) || (plug == aOutputImage)))
  {
    return (MS::kUnknownParameter);
  }
  int maxChains = data.inputValue(aMaxChainsPerOutput).asInt();
  MFnPluginData fnOut;
  MTypeId kdid(skChainData::id);

  MArrayDataHandle hOutputs = data.outputArrayValue(aOutputs, &st);
  mser;
  MArrayDataBuilder bOutputs = hOutputs.builder();
  int nextPlugIndex = 0;

  MDataHandle hOutputImage = data.outputValue(aOutputImage);
  MFnPluginData fnOutImage;
  MTypeId kdidImage(cImgData::id);
  MObject dOutImage = fnOutImage.create(kdidImage, &st);
  mser;

  cImgData *newImageData = (cImgData *)fnOutImage.data(&st);
  mser;

  CImg<unsigned char> *pOutImage = newImageData->fImg;

  if (maxChains < 1)
  {
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
      hOutput.setClean();
    }
  }

  pOutImage->normalize(0, 255);

  hOutputImage.set(newImageData);

  MDataHandle hOutputCount = data.outputValue(aOutputCount);
  hOutputCount.set(nextPlugIndex);

  hOutputs.setAllClean();
  hOutputCount.setClean();
  hOutputImage.setClean();

  return MS::kSuccess;
}

MStatus skChainNode::generate(MDataBlock &data, std::vector<skChain> *geom, CImg<unsigned char> *pOutImage) const
{
  MStatus st;

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

  pOutImage->assign(pImage->get_norm().normalize(0, 1));

  if (data.inputValue(aDoSeedChains).asBool())
  {
    st = generateSeedChains(data, geom, pOutImage);
  }

  if (data.inputValue(aDoFillerChains).asBool())
  {
    st = generateFillerChains(data, geom, pOutImage);
  }

  return MS::kSuccess;
}

MStatus skChainNode::generateFillerChains(
    MDataBlock &data,
    std::vector<skChain> *geom,
    CImg<unsigned char> *pOutImage) const
{

  int w = pOutImage->width();
  int h = pOutImage->height();

  float cmToPixels = getCmToPixels(data, pOutImage);
  float pixelsToCm = 1.0 / cmToPixels;

  int minBranchLengthPixels = int(data.inputValue(aMinBranchTwigLength).asFloat() * cmToPixels);
  minBranchLengthPixels = std::max(minBranchLengthPixels, 1);

  int minLooseTwigLengthPixels = int(data.inputValue(aMinLooseTwigLength).asFloat() * cmToPixels);
  minLooseTwigLengthPixels = std::max(minLooseTwigLengthPixels, 1);

  int spanPixels = int(data.inputValue(aSpan).asFloat() * cmToPixels);
  spanPixels = std::max(spanPixels, 1);

  float maxRadiusPixels = data.inputValue(aMaxWidth).asFloat() * cmToPixels * 0.5f;
  maxRadiusPixels = std::max(maxRadiusPixels, 1.0f);

  float maxStampRadius = data.inputValue(aMaxStampWidth).asFloat() * 0.5;
  float maxStampRadiusPixels = data.inputValue(aMaxStampWidth).asFloat() * cmToPixels * 0.5f;
  maxStampRadiusPixels = std::max(maxStampRadiusPixels, 1.0f);

  float radiusOffsetPixels = int(data.inputValue(aRadiusOffset).asFloat() * cmToPixels);
  float radiusMult = data.inputValue(aRadiusMult).asFloat();
  int maxIterations = data.inputValue(aMaxIterations).asInt();

  int lastNumNewChains = 999999;
  for (int i = 0; i < maxIterations; ++i)
  {
    // make a skeleton image from the BW image
    CImg<float> mat = pOutImage->get_distance(0);
    const CImgList<float> grad = mat.get_gradient("xyz");
    CImg<float> flux = pOutImage->get_flux(grad, 1, 1);
    CImg<bool> skel = pOutImage->get_skeleton(flux, mat, true, 0);

    cimg_forXY(skel, x, y)
    {
      if (!skel(x, y))
      {
        mat(x, y) = 0.0;
      }
    }

    //////////////////
    skGraph g(mat); // build
    // now we have the Medial Axis Transform (mat) and (image)

    g.prune(minBranchLengthPixels);
    g.removeLooseTwigs(minLooseTwigLengthPixels);

    if (!g.numNodes())
    {
      break;
    }
    // limit brush size
    g.clampRadius(maxRadiusPixels);

    g.adjustRadius(radiusMult, radiusOffsetPixels);

    g.detachBranches();

    //////////////////
    MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();

    int nChainsBefore = geom->size();
    int numAdded = g.getChains(projection, *geom, spanPixels);

    if (numAdded == 0)
    {
      break;
    }
    // int nChainsAfter = geom->size();

    // draw the chains
    MFloatMatrix transformation = getTransformation(data, w, h);

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

        segment.stampImage(pOutImage);
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

MFloatMatrix skChainNode::getTransformation(MDataBlock &data, int w, int h) const
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

MStatus skChainNode::generateSeedChains(
    MDataBlock &data,
    std::vector<skChain> *geom,
    CImg<unsigned char> *pOutImage) const
{

  // Get the field MObjects
  MObjectArray fields;
  MPlugArray plugArray;
  MPlug fieldArrayPlug(thisMObject(), aFields);
  int elementCount = fieldArrayPlug.numElements();
  for (size_t i = 0; i < elementCount; i++)
  {
    MPlug fieldPlug = fieldArrayPlug.elementByPhysicalIndex(i);
    bool hasConnections = fieldPlug.connectedTo(plugArray, 1, 0);
    if (hasConnections)
    {
      fields.append(plugArray[i].node());
    }
  }

  int w = pOutImage->width();
  int h = pOutImage->height();

  float cmToPixels = getCmToPixels(data, pOutImage);
  float pixelsToCm = 1.0 / cmToPixels;

  float circleStampRadius = data.inputValue(aSeedChainStampWidth).asFloat() * 0.5;
  float circleRadius = data.inputValue(aSeedChainWidth).asFloat() * 0.5;

  float span = data.inputValue(aSeedChainSpan).asFloat();
  int maxSteps = data.inputValue(aSeedChainMaxSteps).asInt();

  float circleStampRadiusPixels = circleStampRadius * cmToPixels;
  circleStampRadiusPixels = std::max(circleStampRadiusPixels, 1.0f);
  circleStampRadius = circleStampRadiusPixels * pixelsToCm;

  MFloatMatrix transformation = getTransformation(data, w, h);

  MVectorArray seedPoints = MFnVectorArrayData(data.inputValue(aSeedPoints).data()).array();

  unsigned len = seedPoints.length();

  for (size_t i = 0; i < len; i++)
  {
    skChain chain;
    propagateAndStamp(
        seedPoints[i],
        fields,
        transformation,
        circleRadius,
        circleStampRadius,
        span,
        cmToPixels,
        maxSteps,
        pOutImage,
        chain);
    geom->push_back(chain);
  }

  return MS::kSuccess;
}

void skChainNode::propagateAndStamp(
    const MVector &seedPoint,
    const MObjectArray &fields,
    const MFloatMatrix &transformation,
    float circleRadius,
    float circleStampRadius,
    float span,
    float cmToPixels,
    int maxSteps,
    CImg<unsigned char> *pOutImage,
    skChain &chain) const
{

  // float circleRadiusPixels = circleRadius * cmToPixels;
  float circleStampRadiusPixels = circleStampRadius * cmToPixels;

  MFnField fnField;
  unsigned char color[] = {0};

  MVector currPoint(seedPoint);

  for (size_t i = 0; i < maxSteps; i++)
  {

    chainSegment segment(
        i, currPoint,
        circleRadius,
        circleStampRadius,
        transformation,
        cmToPixels);

    MVector forceAccumulated;
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
    forceAccumulated = forceAccumulated.normal() * span;

    segment.applyForce(forceAccumulated);
    bool done = segment.stampAndChain(pOutImage, chain);

    if (done)
    {
      break;
    }

    currPoint = segment.endPoint;
    circleRadius = segment.endRadius;
    circleStampRadius = segment.endStampRadius;
  }
}