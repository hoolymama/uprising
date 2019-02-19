
#include <maya/MIOStream.h>
// #include <math.h>
// #include <algorithm>
// #include <map>

#include <maya/MFnPluginData.h>
// #include <maya/MPoint.h>

#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>



#include <maya/MFnUnitAttribute.h>

#include <maya/MFnNumericAttribute.h>

#include <maya/MFnMatrixAttribute.h>



#include "cImgUtils.h"


#include <jMayaIds.h>
// #include "mayaMath.h"
#include "errorMacros.h"
// #include "texUtils.h"

#include "skGraph.h"

#include "skTest.h"


MObject skTest::aImage;

MObject skTest::aMinGate;
MObject skTest::aMaxGate;

MObject skTest::aMedian;
// MObject skTest::aInvert;
MObject skTest::aMaxIterations;
MObject skTest::aMinBranchTwigLength;
MObject skTest::aMinLooseTwigLength;
MObject skTest::aSpanPixels;
MObject skTest::aMaxWidthPixels;
MObject skTest::aProjectionMatrix;


MObject skTest::aMult;
MObject skTest::aOffset;


MObject skTest::aOutput;



MTypeId skTest::id( k_skTest );

skTest::skTest() {}

skTest::~skTest() {}

void *skTest::creator() {
  return new skTest();
}


/// Post constructor
void
skTest::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}



// const double epsilon = 0.0001;


MStatus skTest::initialize()
{
  MStatus st;
  MString method("skTest::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnMatrixAttribute mAttr;

  aImage = tAttr.create("image", "img", cImgData::id ) ;
  tAttr.setStorable(false);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  st = addAttribute( aImage ); mser;

  aMinGate = nAttr.create( "minGate", "mng", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(1);
  st = addAttribute(aMinGate);

  aMaxGate = nAttr.create( "maxGate", "mxg", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(1);
  st = addAttribute(aMaxGate);

  aMedian = nAttr.create( "median", "med", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(1);
  st = addAttribute(aMedian);

  // aInvert = nAttr.create( "invert", "inv", MFnNumericData::kBoolean);
  // nAttr.setHidden(false);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setDefault(false);
  // st = addAttribute(aInvert);

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



  aMult = nAttr.create( "aStampMult", "sml", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0f);
  st = addAttribute(aMult); mser;

  aOffset = nAttr.create( "aStampOffset", "sof", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0.0f);
  st = addAttribute(aOffset); mser;



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
  attributeAffects( aMinGate, aOutput);
  attributeAffects( aMaxGate, aOutput);
  attributeAffects( aMedian, aOutput);
  attributeAffects( aMaxIterations, aOutput);
  attributeAffects( aMinBranchTwigLength, aOutput);
  attributeAffects( aMinLooseTwigLength, aOutput);
  attributeAffects( aSpanPixels, aOutput);
  attributeAffects( aMaxWidthPixels, aOutput);
  attributeAffects( aProjectionMatrix, aOutput);

  attributeAffects( aMult, aOutput);
  attributeAffects( aOffset, aOutput);

  return ( MS::kSuccess );

}


MStatus skTest::compute(const MPlug &plug, MDataBlock &data )
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

  generate(data, geom);

  hOutput.set(newData);
  data.setClean(plug);

  return MS:: kSuccess;
}


MStatus skTest::generate(MDataBlock &data, std::vector<skChain> *geom)
{
  MStatus st;


  CImg<unsigned char>  *pImage = cImgUtils::getImage(data, aImage );
  int minGate = data.inputValue(aMinGate).asInt();
  int maxGate = data.inputValue(aMaxGate).asInt();

  // bool invert = data.inputValue(aInvert).asBool();
  int median = data.inputValue(aMedian).asInt();
  int maxIterations = data.inputValue(aMaxIterations).asInt();
  MFloatMatrix projection = data.inputValue(aProjectionMatrix).asFloatMatrix();
  int minBranchLength =  data.inputValue(aMinBranchTwigLength).asInt();
  int minLooseTwigLength =  data.inputValue(aMinLooseTwigLength).asInt();
  int step =  data.inputValue(aSpanPixels).asInt();
  int maxWidthPixels =  data.inputValue(aMaxWidthPixels).asInt();

  float mult = data.inputValue(aMult).asFloat();
  float offset = data.inputValue(aOffset).asFloat();


  if (maxWidthPixels < 1)
  {
    maxWidthPixels = 1;
  }

  CImg<unsigned char> image = pImage->get_norm().normalize(0, 255);

  cimg_forXY(image, x, y)
  {
    unsigned char val = image(x, y) ;
    image(x, y) = (val > minGate && val < maxGate) ? 1 : 0;
  }

  int w = image.width();
  int h = image.height();


  if (median) { image.blur_median(median); }

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
    // now we have the Medial Axis Transform (mat) and (image)

    skGraph g(mat); // build
    g.prune(minBranchLength);
    g.removeLooseTwigs(minLooseTwigLength);
    if (! g.numNodes())
    {
      break;
    }
    // limit brush size
    g.clampRadius(maxWidthPixels);


    g.detachBranches();
    g.getChains(projection, *geom, step);

    // then paint black over the image
    g.draw(image, mult, offset);
    if (image.sum() < minLooseTwigLength)
    {
      break;
    }
  }

  return MS::kSuccess;
}


