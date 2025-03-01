
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
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

const float PI = 3.14159265359;
MObject mapColorStrokes::aRGB;
MObject mapColorStrokes::aWhite;
MObject mapColorStrokes::aWait;

MObject mapColorStrokes::aRgbBlend;
MObject mapColorStrokes::aWhiteBlend;

MObject mapColorStrokes::aAngleWaitRemap;
MObject mapColorStrokes::aStartEndAngle;

MObject mapColorStrokes::aPivotSampleOnly;

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
  MRampAttribute rAttr;
  MFnUnitAttribute uAttr;

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

  aRgbBlend = nAttr.create("colorBlend", "cb", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0);
  st = addAttribute(aRgbBlend); mser;

  aWhiteBlend = nAttr.create("whiteBlend", "whtb", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(1.0);
  addAttribute(aWhiteBlend);

  aAngleWaitRemap = MRampAttribute::createCurveRamp("angleWaitRemap", "awr");
  st = addAttribute(aAngleWaitRemap);
  mser;

  aStartEndAngle = uAttr.create("startEndAngle", "sea",
                                MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  uAttr.setStorable(true);
  uAttr.setDefault(0.0);

  st = addAttribute(aStartEndAngle);
  mser;

  aPivotSampleOnly = nAttr.create("rgbwPivotSampleOnly", "pso", MFnNumericData::kBoolean);
  nAttr.setReadable(false);
  nAttr.setDefault(false);
  nAttr.setKeyable(true);
  addAttribute(aPivotSampleOnly);

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
  attributeAffects(aAngleWaitRemap, aOutput);
  attributeAffects(aStartEndAngle, aOutput);
  attributeAffects(aPivotSampleOnly, aOutput);

  attributeAffects(aRgbBlend, aOutput);
  attributeAffects(aWhiteBlend, aOutput);

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

  bool pivotSampleOnly = data.inputValue(aPivotSampleOnly).asBool();

  MFloatVector rgbDummy = data.inputValue(aRGB).asFloatVector();
  float whiteDummy = data.inputValue(aWhite).asFloat();
  float waitDummy = data.inputValue(aWait).asFloat();


  MFloatPointArray points;
  if (pivotSampleOnly)
  {
    getPivotPoints(strokes, points);
    MFloatVectorArray strokeColors;
    MFloatArray strokeWhites;
    getColors(data, points, strokeColors, strokeWhites);
    flattenStrokeColors(strokes, strokeColors, strokeWhites, colors, whites);

    // pivotSampleOnly is not valid for waits, so we resample the target points
    points.clear();
    getTargetPoints(strokes, points);
  }
  else
  {
    getTargetPoints(strokes, points);
    getColors(data, points, colors, whites);
  }

  bool doWaits = getWaits(data, points, waits);

  MRampAttribute rampAttr(thisMObject(), aAngleWaitRemap, &st);
  float startEndAngle = float(data.inputValue(aStartEndAngle).asAngle().asRadians());

  if (doWaits && waits.length() == points.length())
  {
    applyWaits(strokes, waits, rampAttr, startEndAngle);
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

  float rgbBlend = data.inputValue(aRgbBlend).asFloat();
  float whiteBlend = data.inputValue(aWhiteBlend).asFloat();

  applyColors(strokes, colors, whites, rgbBlend, whiteBlend);

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

MStatus mapColorStrokes::flattenStrokeColors(
    const std::vector<Stroke> *strokes,
    const MFloatVectorArray &strokeColors,
    const MFloatArray &strokeWhites,
    MFloatVectorArray &colors,
    MFloatArray &whites) const
{

  MStatus st;
  int numStrokes = strokes->size();
  if (!((numStrokes == strokeColors.length()) && (numStrokes == strokeWhites.length())))
  {
    return MS::kFailure;
  }
  std::vector<Stroke>::const_iterator iter = strokes->begin();
  unsigned strokeIndex = 0;
  for (; iter != strokes->end(); iter++, strokeIndex++)
  {
    unsigned count = iter->size();
    for (int i = 0; i < count; i++)
    {
      colors.append(strokeColors[strokeIndex]);
      whites.append(strokeWhites[strokeIndex]);
    }
  }
  return MS::kSuccess;
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
    if (wait == 0.0)
    {
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
    const MFloatArray &whites,
    float rgbBlend,
    float whiteBlend) const
{
  const float epsilon = 0.0000001;
  std::vector<Stroke>::iterator siter = strokes->begin();
  unsigned index = 0;

  // If both blend vals are zero, do nothing
  if ((rgbBlend - epsilon) < 0.0f && (whiteBlend - epsilon) < 0.0f)
  {
    return;
  }
  rgbBlend = fmin(1.0f, fmax(0.0f, rgbBlend));
  whiteBlend = fmin(1.0f, fmax(0.0f, whiteBlend));
  // If both blend vals are one, replace the existing color
  if ((rgbBlend + epsilon) > 1.0 && (whiteBlend + epsilon) > 1.0)
  {
    for (unsigned i = 0; siter != strokes->end(); siter++, i++)
    {

      Stroke::target_iterator titer = siter->targets_begin();
      for (; titer != siter->targets_end(); titer++, index++)
      {
        MColor color(colors[index].x, colors[index].y, colors[index].z, whites[index]);
        titer->setColor(color);
      }
    }
    return;
  }

  // Blend with the existing color
  float oneMinusRgbBlend = 1.0f - rgbBlend;
  float oneMinusWhiteBlend = 1.0f - whiteBlend;

  for (unsigned i = 0; siter != strokes->end(); siter++, i++)
  {
    Stroke::target_iterator titer = siter->targets_begin();
    for (; titer != siter->targets_end(); titer++, index++)
    {
      const MColor &existing = titer->color();
      MColor color(
          colors[index].x * rgbBlend + existing.r * oneMinusRgbBlend,
          colors[index].y * rgbBlend + existing.g * oneMinusRgbBlend,
          colors[index].z * rgbBlend + existing.b * oneMinusRgbBlend,
          whites[index] * whiteBlend + existing.a * oneMinusWhiteBlend);
      titer->setColor(color);
    }
  }
}


void mapColorStrokes::applyWaits(
    std::vector<Stroke> *strokes,
    const MFloatArray &waits,
    MRampAttribute &rampAttr,
    float startEndAngle) const
{
  std::vector<Stroke>::iterator siter = strokes->begin();
  unsigned index = 0;
  for (unsigned i = 0; siter != strokes->end(); siter++, i++)
  {
    Stroke::target_iterator titer = siter->targets_begin();

    float param;
    for (; titer != siter->targets_end(); titer++, index++)
    {
      if (titer == siter->targets_begin())
      {
        param = startEndAngle / PI;
      }
      else if (std::next(titer) == siter->targets_end())
      {
        param = startEndAngle / PI;
      }
      else
      {
        MFloatVector a = titer->position() - std::prev(titer)->position();
        MFloatVector b = std::next(titer)->position() - titer->position();
        param = a.angle(b) / PI;
      }
      float waitMult;
      rampAttr.getValueAtPosition(param, waitMult);
      titer->setWait(waits[index] * waitMult);
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
    // above a brightness threshold with no more than 3 non-bright targets
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
        if (count > 1) {
          strokes->push_back(Stroke(*iter, startPeg, count));
        }
      }
    }
  }
}