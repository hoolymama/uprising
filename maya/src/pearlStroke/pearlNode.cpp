
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
#include "pearlNode.h"
#include "cImgData.h"
#include "cImgFloatData.h"
#include "nodeUtils.h"

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

  chainSegment(int i,
               const MVector &point,
               float radius,
               float stampRadius,
               const MFloatMatrix &xform,
               float cmToPixels)
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

  /// return true if done (if end point not on white).
  bool stampAndChain(
      CImg<unsigned char> *pOutImage,
      pearlChain &chain)
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
      chain.add(pearl(startPoint.x, startPoint.y, startRadius));
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

    chain.add(pearl(endPoint.x, endPoint.y, endRadius));
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

MObject pearlNode::aImage;

MObject pearlNode::aMaxIterations;
MObject pearlNode::aMinBranchTwigLength;
MObject pearlNode::aMinLooseTwigLength;

MObject pearlNode::aSpan;
MObject pearlNode::aMaxWidth;
MObject pearlNode::aMaxStampWidth;
MObject pearlNode::aOffsetWidth;

MObject pearlNode::aLongestChain;


// Display
MObject pearlNode::aPointSize;
MObject pearlNode::aColor1;
MObject pearlNode::aColor2;

MObject pearlNode::aDrawEdges;
MObject pearlNode::aDrawPoints;
MObject pearlNode::aDrawCircles;
MObject pearlNode::aRandomChainColor;

MObject pearlNode::aOutput;
MObject pearlNode::aOutputImage;

MTypeId pearlNode::id(k_pearlNode);
MString pearlNode::drawDbClassification("drawdb/geometry/pearlNode");
MString pearlNode::drawRegistrantId("pearlNodePlugin");

pearlNode::pearlNode() {}

pearlNode::~pearlNode() {}

void *pearlNode::creator()
{
  return new pearlNode();
}

void pearlNode::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus pearlNode::initialize()
{
  MStatus st;
  MString method("pearlNode::initialize");

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
  nAttr.setDefault(0.01);
  
  st = addAttribute(aMinBranchTwigLength);
  mser;

  aMinLooseTwigLength = nAttr.create("minLooseTwigLength", "mltl", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.01);
  st = addAttribute(aMinLooseTwigLength);
  mser;

  aSpan = nAttr.create("span", "spn", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.01);
  st = addAttribute(aSpan);
  mser;

  aMaxWidth = nAttr.create("maxWidth", "mwd", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.01);
  st = addAttribute(aMaxWidth);
  mser;

  aMaxStampWidth = nAttr.create("maxStampWidth", "msw",
                                MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.01);
  st = addAttribute(aMaxStampWidth);
  mser;

  aOffsetWidth = nAttr.create("offsetWidth", "ofw", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.0f);
  st = addAttribute(aOffsetWidth);
  mser;

  // Display
  aPointSize = nAttr.create("pointSize", "psz", MFnNumericData::kFloat);
  nAttr.setDefault(2.0f);
  nAttr.setKeyable(true);
  st = addAttribute(aPointSize);
  mser;


  // Display
  aLongestChain = nAttr.create("longestChain", "lch", MFnNumericData::kBoolean);
  nAttr.setDefault(false);
  nAttr.setKeyable(true);
  st = addAttribute(aLongestChain);
  mser;




  aColor1 = nAttr.createColor("drawColor1", "dc1");
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aColor1);
  mser;

  aColor2 = nAttr.createColor("drawColor2", "dc2");
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aColor2);
  mser;

  aDrawEdges = nAttr.create("drawEdges", "ded",
                            MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);

  addAttribute(aDrawEdges);

  aDrawPoints = nAttr.create("drawPoints", "dpt",
                             MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  addAttribute(aDrawPoints);

  aDrawCircles = nAttr.create("drawCircles", "dci",
                              MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setDefault(true);
  nAttr.setKeyable(true);
  addAttribute(aDrawCircles);

  aRandomChainColor = nAttr.create("randomChainColor", "rcc",
                                   MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setDefault(false);
  addAttribute(aRandomChainColor);

  aOutput = tAttr.create("output", "out", pearlChainData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  aOutputImage = tAttr.create("outputImage", "oim", cImgData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutputImage);

  attributeAffects(aImage, aOutput);
  attributeAffects(aMaxIterations, aOutput);
  attributeAffects(aMinBranchTwigLength, aOutput);
  attributeAffects(aMinLooseTwigLength, aOutput);
  attributeAffects(aSpan, aOutput);
  attributeAffects(aMaxWidth, aOutput);
  attributeAffects(aOffsetWidth, aOutput);
  attributeAffects(aMaxStampWidth, aOutput);
  attributeAffects(aLongestChain, aOutput);

  attributeAffects(aImage, aOutputImage);
  attributeAffects(aMaxIterations, aOutputImage);
  attributeAffects(aMinBranchTwigLength, aOutputImage);
  attributeAffects(aMinLooseTwigLength, aOutputImage);
  attributeAffects(aSpan, aOutputImage);
  attributeAffects(aMaxWidth, aOutputImage);
  attributeAffects(aOffsetWidth, aOutputImage);
  attributeAffects(aMaxStampWidth, aOutputImage);
  attributeAffects(aLongestChain, aOutputImage);

  return (MS::kSuccess);
}

MStatus pearlNode::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  if (!((plug == aOutput) || (plug == aOutputImage)))
  {
    return (MS::kUnknownParameter);
  }

  MMatrix wm = NodeUtils::firstWorldMatrix(thisMObject());

  MDataHandle hOutputImage = data.outputValue(aOutputImage);
  MFnPluginData fnOutImage;
  MTypeId kdidImage(cImgData::id);
  MObject dOutImage = fnOutImage.create(kdidImage, &st);
  mser;
  cImgData *newImageData = (cImgData *)fnOutImage.data(&st);
  mser;
  CImg<unsigned char> *pOutImage = newImageData->fImg;

  MFnPluginData fnOut;
  MTypeId kdid(pearlChainData::id);

  MDataHandle hOutput = data.outputValue(aOutput, &st);
  mser;

  MObject dOut = fnOut.create(kdid, &st);
  pearlChainData *newData = (pearlChainData *)fnOut.data(&st);
  mser;

  std::vector<pearlChain> *geom = newData->fGeometry;
  geom->clear();

  st = generate(data, geom, pOutImage);
  mser;

  hOutput.set(newData);
  pOutImage->normalize(0, 255);
  hOutputImage.set(newImageData);
  hOutput.setClean();
  hOutputImage.setClean();

  return MS::kSuccess;
}

/*
Generate all the chains.


*/
MStatus pearlNode::generate(
    MDataBlock &data,
    std::vector<pearlChain> *geom,
    CImg<unsigned char> *pInkImage) const
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

  pInkImage->assign(pImage->get_norm().normalize(0, 1));

  // st = generateFillerChains(data, geom, pInkImage);


  // int w = pInkImage->width();
  // int h = pInkImage->height();

  // float pixelsToCm = 1.0 / w;

  int minBranchLengthPixels = int(data.inputValue(aMinBranchTwigLength).asFloat() * w);
  minBranchLengthPixels = std::max(minBranchLengthPixels, 1);

  int minLooseTwigLengthPixels = int(data.inputValue(aMinLooseTwigLength).asFloat() * w);
  minLooseTwigLengthPixels = std::max(minLooseTwigLengthPixels, 1);

  int spanPixels = int(data.inputValue(aSpan).asFloat() * w);
  spanPixels = std::max(spanPixels, 1);

  float maxRadiusPixels = data.inputValue(aMaxWidth).asFloat() * w * 0.5f;
  maxRadiusPixels = std::max(maxRadiusPixels, 1.0f);

  float maxStampRadius = data.inputValue(aMaxStampWidth).asFloat() * 0.5;

  float radiusOffsetPixels = int(data.inputValue(aOffsetWidth).asFloat() * 0.5 * w);
  int maxIterations = data.inputValue(aMaxIterations).asInt();

  bool longestChain = data.inputValue(aLongestChain).asBool();

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

    skGraph g(mat); // build
    // now we have the Medial Axis Transform (mat) and (image)

    if (longestChain) {
      g.trimToLongestChain();
      g.prune(minBranchLengthPixels);
      g.removeLooseTwigs(minLooseTwigLengthPixels);
    } else {
      g.prune(minBranchLengthPixels);
      g.removeLooseTwigs(minLooseTwigLengthPixels);
      g.adjustRadius(radiusOffsetPixels, maxRadiusPixels);
      g.detachBranches();
    }

    if (!g.numNodes())
    {
      break;
    }

    int nChainsBefore = geom->size();
    int numAdded = g.getChains(*geom, spanPixels);

    if (numAdded == 0)
    {
      break;
    }

    // draw the chains
    MFloatMatrix transformation = imageTransform(w, h);

    std::vector<pearlChain>::const_iterator chainIter = geom->begin() + nChainsBefore;
    for (; chainIter != geom->end(); chainIter++)
    {
      const std::vector<pearl> &pearls = chainIter->points();
      std::vector<pearl>::const_iterator pointIter0 = pearls.begin();
      std::vector<pearl>::const_iterator pointIter1 = std::next(pointIter0);

      if (pearls.size() < 2)
      {
        continue;
      }
      for (int index = 0; pointIter1 != pearls.end(); pointIter0++, pointIter1++, index++)
      {

        chainSegment segment(
            index,
            pointIter0->point.x,
            pointIter0->point.y,
            pointIter0->radius,
            pointIter1->point.x,
            pointIter1->point.y,
            pointIter1->radius,
            maxStampRadius,
            transformation, w);

        segment.stampImage(pInkImage);
      }
    }
  }


  return MS::kSuccess;
}

// MStatus pearlNode::generateFillerChains(
//     MDataBlock &data,
//     std::vector<pearlChain> *geom,
//     CImg<unsigned char> *pInkImage) const
// {


//   return MS::kSuccess;
// }

MFloatMatrix pearlNode::imageTransform(int w, int h) const
{
  w *= 0.5;
  h *= 0.5;
  MFloatMatrix flip;
  flip.setToIdentity();
  flip[0][0] = w;
  flip[1][1] = -h;
  flip[3][0] = w;
  flip[3][1] = h;

  return flip;
}

void pearlNode::draw(M3dView &view,
                     const MDagPath &path,
                     M3dView::DisplayStyle style,
                     M3dView::DisplayStatus status)
{
}

bool pearlNode::isBounded() const
{
  return false;
}

MBoundingBox pearlNode::boundingBox() const
{
  return MBoundingBox();
}
