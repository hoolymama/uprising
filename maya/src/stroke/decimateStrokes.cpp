
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
#include "decimateStrokes.h"
#include "stroke.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

const float PI = 3.14159265359;

MObject decimateStrokes::aDensity;
MObject decimateStrokes::aSeed;  
MObject decimateStrokes::aSmooth;
   
MTypeId decimateStrokes::id(k_decimateStrokes);

decimateStrokes::decimateStrokes() {}

decimateStrokes::~decimateStrokes() {}

void *decimateStrokes::creator()
{
  return new decimateStrokes();
}

/// Post constructor
void decimateStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus decimateStrokes::initialize()
{
  MStatus st;
  MString method("decimateStrokes::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;

  aDensity = nAttr.create("density", "den", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aDensity);

  aSeed = nAttr.create("seed", "sd", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aSeed);

  aSmooth = nAttr.create("smooth", "sm", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  
  addAttribute(aSmooth);

  

  attributeAffects(aSmooth, aOutput);
  attributeAffects(aSeed, aOutput);
  attributeAffects(aDensity, aOutput);


  return (MS::kSuccess);
}

MStatus decimateStrokes::mutate(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{
  MStatus st;


  int seedVal = data.inputValue( aSeed).asInt()  ;
  srand48(seedVal);
  int smooth = data.inputValue( aSmooth).asInt()  ;
  
  
  float density =  data.inputValue( aDensity).asFloat()  ;

  MFloatArray densities;

  MFloatPointArray points;
  getSpanPoints(strokes, points);
  getDensities(data, points, densities);

  MIntArray spanMask;
  dither( smooth, densities, spanMask);


  removeSpans(strokes, spanMask);

  return MS::kSuccess;
}

void decimateStrokes::getDensities(
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatArray &densities) const
{

  MStatus st;
  MObject thisObj = thisMObject();

  unsigned len = points.length();
  bool isMapped = false;

  if (TexUtils::hasTexture(thisObj, decimateStrokes::aDensity))
  {

    st = TexUtils::sampleSolidTexture(
        thisObj,
        decimateStrokes::aDensity,
        1.0,
        points,
        densities);

    if (!st.error())
    {
      isMapped = true;
    }
  }
  if (!isMapped)
  {
    float density = data.inputValue(decimateStrokes::aDensity).asFloat();

    densities = MFloatArray(len, density);
  }
}


void decimateStrokes::dither( int smooth, const MFloatArray &densities, MIntArray &spanMask) const
{
  int count = densities.length();
  spanMask.setLength(count);
  if (smooth < 1)
	{
    for (int i = 0; i < count; i++)
    {
      spanMask[i] = int( drand48() < densities[i]);
    }
		return;
	}
 
  
  MFloatArray buffer = MFloatArray(count);
  for (int i = 0; i < count; i++)
  {
    buffer[i] = float( drand48() < densities[i]);
  }


  for (int i = 0; i < count; i++)
  {
		int fromStart = i;
		int fromEnd = (count - 1) - i;
    int n = std::min(smooth, std::min(fromStart, fromEnd));
	  if (n < 1)
		{
			continue;
		}
    float denom = (2 * n) + 1;
    
    float mean = 0.0;
    for (int j = i - n; j < i + n + 1; j++)
    {
      mean += buffer[j];
    }
    mean = mean / denom;
    spanMask[i] = int(mean > 0.5);
  }
}



void decimateStrokes::removeSpans(
    std::vector<Stroke> *strokes,
    const MIntArray &spanMask) const
{


  std::vector<Stroke> sourceStrokes(*strokes);
  strokes->clear();

  int maskId = 0;

  // cerr << "sourceStrokes.size()" << sourceStrokes.size() << endl;
  std::vector<Stroke>::iterator iter = sourceStrokes.begin();
  std::vector<Stroke>::iterator enditer = sourceStrokes.end();


  for (; iter != enditer; iter++)
  {

    unsigned origLength = iter->targets().size();
    unsigned startPeg = 0;
    unsigned endPeg = 0;
 
    bool prevDoSpan = false;

    const std::vector<Target> targets = iter->targets();
    std::vector<Target>::const_iterator targetIter = std::next(targets.begin());
    for (int localSpanId = 0; targetIter != targets.end(); targetIter++, maskId++, localSpanId++) 
    {
      bool doSpan = spanMask[maskId];
      if (doSpan) {
        if (prevDoSpan) {
          if ( targetIter == std::prev(targets.end())) {
            // last target and 
            endPeg = localSpanId +1;
            unsigned targetCount = (endPeg - startPeg) +1;
            strokes->push_back(Stroke(*iter, startPeg, targetCount));
          }
        } else {
          startPeg = localSpanId;
        }

        // int targetCount = (i - startPeg) + 1;
      } else { // cut this span
        if (prevDoSpan) { // ending a sequence of spans
          endPeg = localSpanId;
          unsigned targetCount = (endPeg - startPeg) +1;
          strokes->push_back(Stroke(*iter, startPeg, targetCount));
        } else {
          // do nothing
        }
      }
      prevDoSpan = doSpan;
    }
  }
}