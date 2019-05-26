
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


MObject skChainNode::aImage;


MObject skChainNode::aMaxIterations;
MObject skChainNode::aMinBranchTwigLength;
MObject skChainNode::aMinLooseTwigLength;
MObject skChainNode::aSpanPixels;
MObject skChainNode::aMaxWidthPixels;
MObject skChainNode::aProjectionMatrix;

MObject skChainNode::aRadiusMult;
MObject skChainNode::aRadiusOffset;

MObject skChainNode::aMaxStampWidthPixels;

MObject skChainNode::aOutput;



MTypeId skChainNode::id( k_skChainNode );

skChainNode::skChainNode() {}

skChainNode::~skChainNode() {}

void *skChainNode::creator() {
  return new skChainNode();
}

void
skChainNode::postConstructor()
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

  aImage = tAttr.create("image", "img", cImgData::id ) ;
  tAttr.setStorable(false);
  tAttr.setKeyable(false);

  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  st = addAttribute( aImage ); mser;

  aMaxIterations = nAttr.create( "maxIterations", "mxi", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(5);
  st = addAttribute(aMaxIterations);

  aMinBranchTwigLength = nAttr.create("minBranchTwigLength", "mbtl", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault( 1 );
  st = addAttribute( aMinBranchTwigLength ); mser

  aMinLooseTwigLength = nAttr.create( "minLooseTwigLength", "mltl", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(1);
  nAttr.setDefault(1);
  st = addAttribute(aMinLooseTwigLength); mser;

  aSpanPixels = nAttr.create( "spanPixels", "spx", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(1);
  nAttr.setDefault(10);
  st = addAttribute(aSpanPixels); mser;

  aMaxWidthPixels = nAttr.create( "maxWidthPixels", "mwpx", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(1);
  nAttr.setDefault(10);
  st = addAttribute(aMaxWidthPixels); mser;

  aRadiusMult = nAttr.create( "radiusMult", "rml", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0f);
  st = addAttribute(aRadiusMult); mser;

  aRadiusOffset = nAttr.create( "radiusOffset", "rof", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.0f);
  st = addAttribute(aRadiusOffset); mser;

  aMaxStampWidthPixels = nAttr.create( "maxStampWidthPixels", "mswpx",
                                       MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(1);
  nAttr.setDefault(10);
  st = addAttribute(aMaxStampWidthPixels); mser;



  MFloatMatrix identity;
  identity.setToIdentity();
  aProjectionMatrix = mAttr.create( "projectionMatrix", "pmat",
                                    MFnMatrixAttribute::kFloat );
  mAttr.setStorable( false );
  mAttr.setHidden( false );
  mAttr.setKeyable(true);
  mAttr.setDefault(identity);
  addAttribute(aProjectionMatrix);

  aOutput = tAttr.create("output", "out", skChainData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);


  attributeAffects( aImage, aOutput);

  attributeAffects( aMaxIterations, aOutput);
  attributeAffects( aMinBranchTwigLength, aOutput);
  attributeAffects( aMinLooseTwigLength, aOutput);
  attributeAffects( aSpanPixels, aOutput);
  attributeAffects( aMaxWidthPixels, aOutput);
  attributeAffects( aProjectionMatrix, aOutput);

  attributeAffects( aRadiusMult, aOutput);
  attributeAffects( aRadiusOffset, aOutput);
  attributeAffects( aMaxStampWidthPixels, aOutput);


  return ( MS::kSuccess );

}


MStatus skChainNode::compute(const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  if (plug != aOutput) {
    return (MS::kUnknownParameter );
  }

  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(skChainData::id);
  MObject dOut = fnOut.create(kdid, & st);
  skChainData *newData = (skChainData * )fnOut.data(&st); mser;
  std::vector < skChain > *geom = newData->fGeometry;
  geom->clear();

  st = generate(data, geom);
  if (st.error()) {
    return (MS::kUnknownParameter );
  }

  hOutput.set(newData);
  data.setClean(plug);

  return MS:: kSuccess;
}


MStatus skChainNode::generate(MDataBlock &data, std::vector<skChain> *geom)
{
  MStatus st;

  // JPMDBG;
  CImg<unsigned char>  *pImage = cImgUtils::getImage(data, aImage );
  // JPMDBG;
  if (! pImage)
  {
    return MS::kUnknownParameter;
  }
  // JPMDBG;
  int w = pImage->width();
  int h = pImage->height();
  // JPMDBG;
  if (! (w && h)) {
    return MS::kUnknownParameter;
  }
  // JPMDBG;
  int maxIterations = data.inputValue(aMaxIterations).asInt();
  MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();
  int minBranchLength =  data.inputValue(aMinBranchTwigLength).asInt();
  int minLooseTwigLength =  data.inputValue(aMinLooseTwigLength).asInt();
  int step =  data.inputValue(aSpanPixels).asInt();
  int maxWidthPixels =  data.inputValue(aMaxWidthPixels).asInt();

  float radiusMult = data.inputValue(aRadiusMult).asFloat();
  float radiusOffset = data.inputValue(aRadiusOffset).asFloat();
  int maxStampWidthPixels =  data.inputValue(aMaxStampWidthPixels).asInt();

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
        if (! skel(x, y)) {
          mat(x, y) = 0.0;
        }
      }
    }
    skGraph g(mat); // build
    // now we have the Medial Axis Transform (mat) and (image)

    g.prune(minBranchLength);
    g.removeLooseTwigs(minLooseTwigLength);
    if (! g.numNodes())
    {
      break;
    }
    // limit brush size
    g.clampRadius(maxWidthPixels);

    g.draw(image, maxStampWidthPixels);

    g.adjustRadius(radiusMult , radiusOffset);


    g.detachBranches();
    g.getChains(projection, *geom, step);

    // then paint black over the image
    if (image.sum() < minLooseTwigLength)
    {
      break;
    }
  }

  return MS::kSuccess;
}


