
#include <maya/MIOStream.h>

#include <maya/MFnPluginData.h>

#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include "cImgUtils.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "skGraph/skGraph.h"
#include "skChainNode.h"

#include <maya/MArrayDataBuilder.h>

MObject skChainNode::aImage;

MObject skChainNode::aMaxIterations;
MObject skChainNode::aMinBranchTwigLength;
MObject skChainNode::aMinLooseTwigLength;

MObject skChainNode::aSpan;
MObject skChainNode::aMaxWidth;
MObject skChainNode::aMaxStampWidth;
MObject skChainNode::aProjectionMatrix;
MObject skChainNode::aRadiusMult;
MObject skChainNode::aRadiusOffset;

MObject skChainNode::aMaxChainsPerOutput;
MObject skChainNode::aOutput;
MObject skChainNode::aOutputs;
MObject skChainNode::aOutputCount;

// MObject skChainNode::aOutput;

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

  aImage = tAttr.create("image", "img", cImgData::id);
  tAttr.setStorable(false);
  tAttr.setKeyable(false);

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

  aOutput = tAttr.create("output", "out", skChainData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  // attributeAffects(aImage, aOutput);
  // attributeAffects(aMaxIterations, aOutput);
  // attributeAffects(aMinBranchTwigLength, aOutput);
  // attributeAffects(aMinLooseTwigLength, aOutput);
  // attributeAffects(aSpan, aOutput);
  // attributeAffects(aMaxWidth, aOutput);
  // attributeAffects(aProjectionMatrix, aOutput);
  // attributeAffects(aRadiusMult, aOutput);
  // attributeAffects(aRadiusOffset, aOutput);
  // attributeAffects(aMaxStampWidth, aOutput);
  // attributeAffects(aMaxChainsPerOutput, aOutput);

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

  attributeAffects(aImage, aOutputCount);
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

  return (MS::kSuccess);
}

MStatus skChainNode::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  if (!((plug == aOutputs) || (plug == aOutputCount)))
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
  // cerr << "maxChains: " << maxChains << endl;
  if (maxChains < 1)
  {

    MDataHandle hOutput = bOutputs.addElement(0);
    // cerr << "Here 1:" << endl;
    MObject dOut = fnOut.create(kdid, &st);
    // cerr << "Here 2:" << endl;
    skChainData *newData = (skChainData *)fnOut.data(&st);
    mser;
    // cerr << "Here 3:" << endl;
    std::vector<skChain> *geom = newData->fGeometry;
    geom->clear();
    // cerr << "Here 4:" << endl;
    st = generate(data, geom);
    // cerr << "Here 5:" << endl;
    if (st.error())
    {
      return (MS::kUnknownParameter);
    }
    hOutput.set(newData);
    // cerr << "Here 6:" << endl;
    nextPlugIndex++;
  }
  else
  {
    std::vector<skChain> *allGeom = new std::vector<skChain>;
    st = generate(data, allGeom);
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
    }
  }
  // cerr << "nextPlugIndex: " << nextPlugIndex << endl;
  MDataHandle hOutputCount = data.outputValue(aOutputCount);
  hOutputCount.set(nextPlugIndex);
  // cerr << "Here 7:" << endl;
  hOutputs.setAllClean();
  hOutputCount.setClean();
  // cerr << "Here 8:" << endl;
  return MS::kSuccess;
}

MStatus skChainNode::generate(MDataBlock &data, std::vector<skChain> *geom)
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
  int maxWidthPixels = int(data.inputValue(aMaxWidth).asFloat() * cmToPixels);
  if (maxWidthPixels < 1)
  {
    maxWidthPixels = 1;
  }

  int maxStampWidthPixels = int(data.inputValue(aMaxStampWidth).asFloat() * cmToPixels);
  if (maxStampWidthPixels < 1)
  {
    maxStampWidthPixels = 1;
  }

  float radiusOffsetPixels = int(data.inputValue(aRadiusOffset).asFloat() * cmToPixels);
  float radiusMult = data.inputValue(aRadiusMult).asFloat();
  int maxIterations = data.inputValue(aMaxIterations).asInt();

  if (maxWidthPixels < 1)
  {
    maxWidthPixels = 1;
  }

  CImg<unsigned char> image = pImage->get_norm().normalize(0, 1);

  // if (median) { image.blur_median(median); }

  for (int i = 0; i < maxIterations; ++i)
  {
    // make a skeleton image from the BW image
    CImg<float> mat = image.get_distance(0);
    const CImgList<float> grad = mat.get_gradient("xyz");
    CImg<float> flux = image.get_flux(grad, 1, 1);
    CImg<bool> skel = image.get_skeleton(flux, mat, true, 0);
    for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
      {
        if (!skel(x, y))
        {
          mat(x, y) = 0.0;
        }
      }
    }
    skGraph g(mat); // build
    // now we have the Medial Axis Transform (mat) and (image)

    g.prune(minBranchLengthPixels);
    g.removeLooseTwigs(minLooseTwigLengthPixels);

    if (!g.numNodes())
    {
      break;
    }
    // limit brush size
    g.clampRadius(maxWidthPixels);

    g.draw(image, maxStampWidthPixels);

    g.adjustRadius(radiusMult, radiusOffsetPixels);

    g.detachBranches();
    g.getChains(projection, *geom, stepPixels);

    // then paint black over the image
    if (image.sum() < minLooseTwigLengthPixels)
    {
      break;
    }
  }

  return MS::kSuccess;
}
