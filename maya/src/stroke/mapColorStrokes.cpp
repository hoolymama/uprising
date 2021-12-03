
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>

#include <maya/MMeshIntersector.h>

#include "strokeData.h"
#include "mapColorStrokes.h"
#include "stroke.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

MObject mapColorStrokes::aRGB;
MObject mapColorStrokes::aWhite;
MObject mapColorStrokes::aWait;


MObject mapColorStrokes::aMesh;
MObject mapColorStrokes::aPoint;
MObject mapColorStrokes::aBias;
MObject mapColorStrokes::aDoOcclusion;
MObject mapColorStrokes::aRemoveBlackSpans;
MObject mapColorStrokes::aBlackSpanThreshold;

MTypeId mapColorStrokes::id(k_mapColorStrokes);

mapColorStrokes::mapColorStrokes() {}

mapColorStrokes::~mapColorStrokes() {}

void *mapColorStrokes::creator()
{
  return new mapColorStrokes();
}

/// Post constructor
void mapColorStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus mapColorStrokes::initialize()
{
  MStatus st;
  MString method("mapColorStrokes::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;

  aRGB = nAttr.createColor("rgb", "rgb");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aRGB);

  MObject aR = nAttr.child(0);
  MObject aG = nAttr.child(1);
  MObject aB = nAttr.child(2);

  aWhite = nAttr.create("white", "wht", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aWhite);

  aWait = nAttr.create("wait", "wai", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aWait);


  aPoint = nAttr.createPoint("occlusionPoint", "opt");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aPoint);

  aMesh = tAttr.create("occlusionMesh", "omsh", MFnData::kMesh);
  tAttr.setReadable(false);
  addAttribute(aMesh);

  aBias = nAttr.create("occlusionBias", "obs", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aBias);

  aDoOcclusion = nAttr.create("doOcclusion", "occ", MFnNumericData::kBoolean);
  nAttr.setReadable(false);
  nAttr.setDefault(false);
  nAttr.setKeyable(true);
  addAttribute(aDoOcclusion);

  aRemoveBlackSpans = nAttr.create("removeBlackSpans", "rbs", MFnNumericData::kBoolean);
  nAttr.setReadable(false);
  nAttr.setDefault(false);
  nAttr.setKeyable(true);
  addAttribute(aRemoveBlackSpans);

  aBlackSpanThreshold = nAttr.create("blackSpanThreshold", "bst", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.001f);
  addAttribute(aBlackSpanThreshold);

  attributeAffects(aPoint, aOutput);
  attributeAffects(aMesh, aOutput);
  attributeAffects(aBias, aOutput);
  attributeAffects(aDoOcclusion, aOutput);
  attributeAffects(aRemoveBlackSpans, aOutput);
  attributeAffects(aBlackSpanThreshold, aOutput);

  attributeAffects(aRGB, aOutput);
  attributeAffects(aR, aOutput);
  attributeAffects(aG, aOutput);
  attributeAffects(aB, aOutput);
  attributeAffects(aWhite, aOutput);

  attributeAffects(aWait, aOutput); 
  return (MS::kSuccess);
}

MStatus mapColorStrokes::mutate(
 const MPlug &plug, 
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{
  MStatus st;

  MFloatVectorArray colors;
  MFloatArray whites;
  MFloatArray waits;
  

  MFloatPointArray points;
  getTargetPoints(strokes, points);

  getColors(data, points, colors, whites);
  bool doWaits = getWaits(data, points, waits);

  if (doWaits && waits.length() == points.length()) {
    applyWaits(strokes, waits);
  }


  if ((points.length() != colors.length()) || (points.length() != whites.length()))
  {
    return MS::kUnknownParameter;
  }

  bool doOcclusion = data.inputValue(aDoOcclusion).asBool();
  if (doOcclusion)
  {
    st = occludeColors(data, points, colors, whites);
  }

  applyColors(strokes, colors, whites);

  if (data.inputValue(aRemoveBlackSpans).asBool())
  {
    float thresh = data.inputValue(aBlackSpanThreshold).asFloat();

    removeBlackSpans(strokes, thresh);
  }


  return MS::kSuccess;
}

void mapColorStrokes::getColors(
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatVectorArray &colors,
    MFloatArray &whites) const
{

  MStatus st;
  MObject thisObj = thisMObject();

  unsigned len = points.length();
  bool isColorMapped = false;
  bool isWhiteMapped = false;

  if (TexUtils::hasTexture(thisObj, mapColorStrokes::aRGB))
  {

    st = TexUtils::sampleSolidTexture(thisObj, mapColorStrokes::aRGB, 1.0, points, colors);

    if (!st.error())
    {
      isColorMapped = true;
    }
  }
  if (!isColorMapped)
  {
    MFloatVector color = data.inputValue(aRGB).asFloatVector();
    colors = MFloatVectorArray(len, color);
  }

  if (TexUtils::hasTexture(thisObj, mapColorStrokes::aWhite))
  {

    st = TexUtils::sampleSolidTexture(
        thisObj,
        mapColorStrokes::aWhite,
        1.0,
        points,
        whites);

    if (!st.error())
    {
      isWhiteMapped = true;
    }
  }
  if (!isWhiteMapped)
  {
    float white = data.inputValue(aWhite).asFloat();
    whites = MFloatArray(len, white);
  }
}




bool mapColorStrokes::getWaits(
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatArray &waits) const
{

  MStatus st;
  MObject thisObj = thisMObject();

  unsigned len = points.length();
  bool isMapped = false;

  if (TexUtils::hasTexture(thisObj, mapColorStrokes::aWait))
  {

    st = TexUtils::sampleSolidTexture(
        thisObj,
        mapColorStrokes::aWait,
        1.0,
        points,
        waits);

    if (!st.error())
    {
      isMapped = true;
    }
  }
  if (!isMapped)
  {
    float wait = data.inputValue(aWait).asFloat();
    if (wait == 0.0) {
      return false;
    }
    waits = MFloatArray(len, wait);
  }
  return true;
}



MStatus mapColorStrokes::occludeColors(
    MDataBlock &data,
    const MFloatPointArray &points,
    MFloatVectorArray &colors,
    MFloatArray &whites) const
{

  MStatus st;
  unsigned len = points.length();
  float3 &fpoint = data.inputValue(aPoint).asFloat3();
  MFloatPoint camPoint(fpoint[0], fpoint[1], fpoint[2]);
  const float &bias = data.inputValue(aBias).asFloat();

  MDataHandle hMesh = data.inputValue(aMesh, &st);
  msert;
  MObject inMesh = hMesh.asMeshTransformed();
  MFnMesh mFnIn(inMesh, &st);
  msert;

  MMeshIsectAccelParams ap = mFnIn.autoUniformGridParams();
  MFloatPoint fpSource;
  MFloatVector fvRayDir;
  float hitRayParam;
  MFloatPoint fHitPoint;

  for (unsigned i = 0; i < len; i++)
  {
    const MFloatVector &fvRayDir = MFloatVector(camPoint - points[i]).normal();

    const MFloatPoint &fpSource = points[i] + (fvRayDir * bias);

    bool hit = mFnIn.anyIntersection(
        fpSource, fvRayDir, NULL, NULL, false,
        MSpace::kWorld, 99999, false, &ap, fHitPoint,
        &hitRayParam, NULL, NULL, NULL, NULL, 0.000001f, &st);
    mser;
    if (hit)
    {
      colors[i] = MFloatVector(0, 0, 0);
      whites[i] = 0.0f;
    }
  }

  return MS::kSuccess;
}

void mapColorStrokes::applyColors(
    std::vector<Stroke> *strokes,
    const MFloatVectorArray &colors,
    const MFloatArray &whites) const
{
  std::vector<Stroke>::iterator siter = strokes->begin();
  unsigned index = 0;
  for (unsigned i = 0; siter != strokes->end(); siter++, i++)
  {

    Stroke::target_iterator titer = siter->targets_begin();
    for (; titer != siter->targets_end(); titer++, index++)
    {
      MColor color(colors[index].x, colors[index].y, colors[index].z, whites[index]);
      titer->setColor(color);
    }
  }
}


void mapColorStrokes::applyWaits(
    std::vector<Stroke> *strokes,
    const MFloatArray &waits) const
{
  std::vector<Stroke>::iterator siter = strokes->begin();
  unsigned index = 0;
  for (unsigned i = 0; siter != strokes->end(); siter++, i++)
  {
    Stroke::target_iterator titer = siter->targets_begin();
    for (; titer != siter->targets_end(); titer++, index++)
    {
      titer->setWait(waits[index]);
    }
  }
}


void mapColorStrokes::removeBlackSpans(
    std::vector<Stroke> *strokes,
    float thresh) const
{

  thresh = fmax(fmin(thresh, 1.0f), 0.0f);
  std::vector<Stroke> sourceStrokes(*strokes);

  strokes->clear();

  // cerr << "sourceStrokes.size()" << sourceStrokes.size() << endl;
  std::vector<Stroke>::iterator iter = sourceStrokes.begin();
  std::vector<Stroke>::iterator enditer = sourceStrokes.end();

  for (; iter != enditer; iter++)
  {

    // get the ids of targets that are bright enough
    MIntArray brightIds;
    std::vector<Target>::const_iterator titer = iter->targets().begin();
    std::vector<Target>::const_iterator tenditer = iter->targets().end();
    for (unsigned j = 0; titer != tenditer; titer++, j++)
    {
      if (titer->luminance() > thresh)
      {
        brightIds.append(j);
      }
    }

    unsigned origLength = iter->targets().size();
    unsigned startPeg = 0;
    unsigned endPeg = 0;
    unsigned leng = brightIds.length();
    bool gap = true;

    // loop through the bright IDs and calculate a begin and end for each
    // contiguous chain of targets. A contiguous chain is defined as targets
    // above a bightness threshold with no more than 3 non-bright targets
    // separating them from another chain.
    for (unsigned i = 0; i < leng; i++)
    {
      unsigned this_id = brightIds[i];

      if (gap)
      {
        startPeg = (this_id > 0) ? this_id - 1 : this_id;
        gap = false;
      }
      endPeg = this_id + 1;

      if (i < (leng - 1) && (brightIds[i + 1] - brightIds[i]) > 3)
      {
        gap = true;
      }

      if (gap || i == leng - 1)
      {
        if (this_id < origLength - 1)
        {
          endPeg += 1;
        }
        unsigned count = endPeg - startPeg;
        strokes->push_back(Stroke(*iter, startPeg, count));
      }
    }
  }
}