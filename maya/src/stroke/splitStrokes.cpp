
#include <maya/MIOStream.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include "splitStrokes.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "strokeUtils.h"

// const double rad_to_deg = (180 / 3.1415927);
// const double deg_to_rad = (3.1415927 / 180);


MTypeId splitStrokes::id(k_splitStrokes);

splitStrokes::splitStrokes() {}

splitStrokes::~splitStrokes() {}


MObject splitStrokes::aStrokeIds;
MObject splitStrokes::aSplitCounts;
MObject splitStrokes::aSplitParams;


void *splitStrokes::creator()
{
  return new splitStrokes();
}

/// Post constructor
void splitStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus splitStrokes::initialize()
{
  MStatus st;
  MString method("splitStrokes::initialize");

	inheritAttributesFrom("strokeMutator");
  MFnTypedAttribute tAttr;

	aStrokeIds = tAttr.create("strokeIds", "sids", MFnData::kIntArray);
	tAttr.setStorable(true);
  tAttr.setWritable(true);
	st = addAttribute(aStrokeIds);mser;

	aSplitCounts = tAttr.create("splitCounts", "scts", MFnData::kIntArray);
	tAttr.setStorable(true);
  tAttr.setWritable(true);
	st = addAttribute(aSplitCounts);mser;

	aSplitParams = tAttr.create("splitParams", "spms", MFnData::kDoubleArray);
	tAttr.setStorable(true);
  tAttr.setWritable(true);
	st = addAttribute(aSplitParams);mser;

  attributeAffects(aStrokeIds,aOutput);
  attributeAffects(aSplitCounts,aOutput);
  attributeAffects(aSplitParams,aOutput);

  return (MS::kSuccess);
}


MStatus splitStrokes::mutate(
  const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const 
{

  const MIntArray strokeIds = MFnIntArrayData(data.inputValue(aStrokeIds).data()).array();
  const MIntArray splitCounts = MFnIntArrayData(data.inputValue(aSplitCounts).data()).array();
  const MDoubleArray splitParams = MFnDoubleArrayData(data.inputValue(aSplitParams).data()).array();

  // Convert the arrays to an easier to manage data structure
  std::vector< std::pair <int, MDoubleArray> > splits;
  int paramIndex = 0;
  for (int i = 0; i < strokeIds.length(); i++) {
      splits.push_back(std::make_pair(strokeIds[i], MDoubleArray()));
      for (int j = 0; j < splitCounts[i]; j++, paramIndex++) {
          splits[i].second.append(splitParams[paramIndex]);
      }
  }



  std::vector<Stroke> sourceStrokes(*strokes);
  strokes->clear();

  std::vector<Stroke>::const_iterator iter = sourceStrokes.begin();
  std::vector<Stroke>::const_iterator enditer = sourceStrokes.end();

  int peg = 0;
  int strokeIndex = 0;
  for (; iter != enditer; iter++, strokeIndex++)
  {
    // keep the peg in sync
    if (splits[peg].first > strokeIndex) {
          strokes->push_back(*iter);
      // we don't split this stroke. just copy it over.
    } else if (splits[peg].first == strokeIndex) { 
      // peg == strokeIndex
      // we split this stroke. and append the splits to the strokes vector.
      splitStroke( splits[peg].second, &(*iter), strokes );
      peg++;
    } else {
      // SHOULD NOT HAPPEN
    }
  }
  return MS::kSuccess;
}

void splitStrokes::splitStroke(
  const MDoubleArray &splitParams,
  const Stroke *stroke,
  std::vector<Stroke> *strokes) const
{
  // Split the stroke
  MObject curveObject;
  StrokeUtils::createCurve(stroke, curveObject);



}