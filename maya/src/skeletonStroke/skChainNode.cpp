
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

#include "cImgUtils.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "skGraph.h"
#include "skChainNode.h"

#include <maya/MArrayDataBuilder.h>

MObject skChainNode::aImage;

MObject skChainNode::aMaxIterations;
MObject skChainNode::aMinBranchTwigLength;
MObject skChainNode::aMinLooseTwigLength;

MObject skChainNode::aSeedPoints;
MObject skChainNode::aFields;

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

  aFields = msgAttr.create("fields", "flds");
  msgAttr.setArray(true);
  st = addAttribute(aFields);

  aSeedPoints = tAttr.create("seedPoints", "spts", MFnData::kVectorArray);
  tAttr.setStorable(false);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aSeedPoints);

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
  attributeAffects(aSeedPoints, aOutputs);
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

  pOutImage->normalize(0,255);

  hOutputImage.set(newImageData);

  MDataHandle hOutputCount = data.outputValue(aOutputCount);
  hOutputCount.set(nextPlugIndex);

  hOutputs.setAllClean();
  hOutputCount.setClean();
  hOutputImage.setClean();

  return MS::kSuccess;
}

MStatus skChainNode::generate(MDataBlock &data, std::vector<skChain> *geom, CImg<unsigned char> *pOutImage)
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
  st = generateFlow(data, geom, pOutImage);
  return MS::kSuccess;

  MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();
  float scale = projection[0][0] * 2.0f; // just use the width.

  float cmToPixels = float(w) / scale;

  int minBranchLengthPixels = int(data.inputValue(aMinBranchTwigLength).asFloat() * cmToPixels);
  if (minBranchLengthPixels < 1)
  {
    minBranchLengthPixels = 1;
  }
  int minLooseTwigLengthPixels = int(data.inputValue(aMinLooseTwigLength).asFloat() * cmToPixels);
  if (minLooseTwigLengthPixels < 1)
  {
    minLooseTwigLengthPixels = 1;
  }
  int stepPixels = int(data.inputValue(aSpan).asFloat() * cmToPixels);
  if (stepPixels < 1)
  {
    stepPixels = 1;
  }
  float maxRadiusPixels = data.inputValue(aMaxWidth).asFloat() * cmToPixels * 0.5f;
  maxRadiusPixels = std::max(maxRadiusPixels, 1.0f);

  float maxStampRadiusPixels = data.inputValue(aMaxStampWidth).asFloat() * cmToPixels * 0.5f;
  maxStampRadiusPixels = std::max(maxStampRadiusPixels, 1.0f);

  float radiusOffsetPixels = int(data.inputValue(aRadiusOffset).asFloat() * cmToPixels);
  float radiusMult = data.inputValue(aRadiusMult).asFloat();
  int maxIterations = data.inputValue(aMaxIterations).asInt();

  CImg<unsigned char> image = pImage->get_norm().normalize(0, 1);

  for (int i = 0; i < maxIterations; ++i)
  {
    // make a skeleton image from the BW image
    CImg<float> mat = image.get_distance(0);
    const CImgList<float> grad = mat.get_gradient("xyz");
    CImg<float> flux = image.get_flux(grad, 1, 1);
    CImg<bool> skel = image.get_skeleton(flux, mat, true, 0);

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

    g.draw(image, maxStampRadiusPixels);

    g.adjustRadius(radiusMult, radiusOffsetPixels);

    g.detachBranches();

    //////////////////
    g.getChains(projection, *geom, stepPixels);

    // then paint black over the image
    if (image.sum() < minLooseTwigLengthPixels)
    {
      break;
    }
  }

  return MS::kSuccess;
}

MStatus skChainNode::generateFlow(
    MDataBlock &data,
    std::vector<skChain> *geom,
    CImg<unsigned char> *pOutImage)
{
  cerr << "generateFlow" << endl;
  int w = pOutImage->width();
  int h = pOutImage->height();

  MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();

  // float scale = projection[0][0]; // just use the width.
  float pixelWidth = projection[0][0] * 2.0 / float(w);

  int circleRadiusPixels = 5;
  float circleRadius = pixelWidth * circleRadiusPixels;

  unsigned char color[] = {0};


  // 0 to 1 
  MFloatMatrix norm;
  norm.setToIdentity();
  norm[0][0] = w*0.5;
  norm[1][1] = -h*0.5;
  norm[3][0] = w *0.5;
  norm[3][1] = h *0.5;


  // MFloatMatrix transformation = norm * projection.inverse();


  MVectorArray seedPoints = MFnVectorArrayData(data.inputValue(aSeedPoints).data()).array();
  unsigned len = seedPoints.length();

  for (size_t i = 0; i < len; i++)
  {

    MFloatVector seedPointImage = MFloatPoint(seedPoints[i]) *  projection.inverse()  * norm ;

    cerr << "seedPointImage:" << seedPointImage << " -- seedPoints[i]:" << seedPoints[i] << endl;
    skChain chain;
    skPoint(seedPoints[i].x, seedPoints[i].y, circleRadius);
    chain.add(skPoint(seedPoints[i].x, seedPoints[i].y, circleRadius));
    chain.add(skPoint(seedPoints[i].x-0.5, seedPoints[i].y, circleRadius));
    

    // result.assign(m_width, m_height, 1, 1, 0);
    unsigned char color[] = {0};
    pOutImage->draw_circle(seedPointImage.x, seedPointImage.y, circleRadiusPixels, color);

    geom->push_back(chain);

  }

  return MS::kSuccess;
}