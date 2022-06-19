
#include <vector>

#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include "strokeData.h"
#include "strokeNodeBase.h"
#include "stroke.h"
#include "filterDefinition.h"
#include "sortDefinition.h"

#include <jMayaIds.h>
#include "texUtils.h"

const double rad_to_deg = (180 / 3.1415927);
const double deg_to_rad = (3.1415927 / 180);

MObject strokeNodeBase::aStrokeSortKey;
MObject strokeNodeBase::aStrokeSortDirection;
MObject strokeNodeBase::aStrokeSortList;
MObject strokeNodeBase::aStrokeSortTexture;
MObject strokeNodeBase::aApplySort;

MObject strokeNodeBase::aStrokeFilterKey;
MObject strokeNodeBase::aStrokeFilterOperator;
MObject strokeNodeBase::aStrokeFilterOperand;
MObject strokeNodeBase::aStrokeFilterList;
MObject strokeNodeBase::aStrokeFilterTexture;
MObject strokeNodeBase::aApplyFilters;

MObject strokeNodeBase::aStartFrom;
MObject strokeNodeBase::aEndAt;

MObject strokeNodeBase::aOutput;

MTypeId strokeNodeBase::id(k_strokeNodeBase);

strokeNodeBase::strokeNodeBase() {}

strokeNodeBase::~strokeNodeBase() {}

void *strokeNodeBase::creator()
{
  return new strokeNodeBase();
}

const double epsilon = 0.0001;

MStatus strokeNodeBase::initialize()
{
  MStatus st;
  MString method("strokeNodeBase::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;

  MFnEnumAttribute eAttr;

  MFnCompoundAttribute cAttr;

  aStrokeSortKey = eAttr.create("strokeSortKey", "stsk", Stroke::kBrushId);
  eAttr.addField("Stroke Id", Stroke::kStrokeId);
  eAttr.addField("Brush Id", Stroke::kBrushId);
  eAttr.addField("Paint Id", Stroke::kPaintId);
  eAttr.addField("Repeat Id", Stroke::kRepeatId);
  eAttr.addField("Layer Id", Stroke::kLayerId);
  eAttr.addField("Parent Id", Stroke::kParentId);
  eAttr.addField("Target Count", Stroke::kTargetCount);
  eAttr.addField("Map Red", Stroke::kMapRed);
  eAttr.addField("Map Green", Stroke::kMapGreen);
  eAttr.addField("Map Blue", Stroke::kMapBlue);

  aStrokeSortDirection = eAttr.create("strokeSortDirection", "stsd",
                                      Stroke::kSortAscending);
  eAttr.addField("Ascending", Stroke::kSortAscending);
  eAttr.addField("Descending", Stroke::kSortDescending);

  aStrokeSortList = cAttr.create("strokeSortList", "stsl");
  cAttr.addChild(aStrokeSortKey);
  cAttr.addChild(aStrokeSortDirection);
  cAttr.setArray(true);
  st = addAttribute(aStrokeSortList);
  mser;

  aStrokeSortTexture = nAttr.createColor("strokeSortTexture", "stst");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aStrokeSortTexture);

  aApplySort = nAttr.create("applySort", "apst", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aApplySort);

  aStrokeFilterKey = eAttr.create("strokeFilterKey", "stfk", Stroke::kBrushId);
  eAttr.addField("Stroke Id", Stroke::kStrokeId);
  eAttr.addField("Brush Id", Stroke::kBrushId);
  eAttr.addField("Paint Id", Stroke::kPaintId);
  eAttr.addField("Repeat Id", Stroke::kRepeatId);
  eAttr.addField("Layer Id", Stroke::kLayerId);
  eAttr.addField("Parent Id", Stroke::kParentId);
  eAttr.addField("Target Count", Stroke::kTargetCount);
  eAttr.addField("Map Red", Stroke::kMapRed);
  eAttr.addField("Map Green", Stroke::kMapGreen);
  eAttr.addField("Map Blue", Stroke::kMapBlue);

  eAttr.setHidden(false);
  eAttr.setKeyable(true);

  aStrokeFilterOperator = eAttr.create("strokeFilterOperator", "stfop",
                                       Stroke::kGreaterThan);
  eAttr.addField(">", Stroke::kGreaterThan);
  eAttr.addField("<", Stroke::kLessThan);
  eAttr.addField("==", Stroke::kEqualTo);
  eAttr.addField("!=", Stroke::kNotEqualTo);
  eAttr.addField("nop", Stroke::kNoOp);
  eAttr.setHidden(false);
  eAttr.setKeyable(true);

  aStrokeFilterOperand = nAttr.create("strokeFilterOperand", "stfod",
                                      MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);

  aStrokeFilterList = cAttr.create("strokeFilterList", "stfl");
  cAttr.addChild(aStrokeFilterKey);
  cAttr.addChild(aStrokeFilterOperator);
  cAttr.addChild(aStrokeFilterOperand);
  cAttr.setArray(true);
  st = addAttribute(aStrokeFilterList);
  mser;

  aStrokeFilterTexture = nAttr.createColor("strokeFilterTexture", "stft");
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setConnectable(true);
  addAttribute(aStrokeFilterTexture);

  aApplyFilters = nAttr.create("applyFilters", "apfl", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(true);
  addAttribute(aApplyFilters);

  aStartFrom = nAttr.create("startFrom", "stfm", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  st = addAttribute(aStartFrom);

  aEndAt = nAttr.create("endAt", "edat", MFnNumericData::kInt);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(-1);
  st = addAttribute(aEndAt);

  aOutput = tAttr.create("output", "out", strokeData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  st = attributeAffects(aApplySort, aOutput);
  st = attributeAffects(aStrokeSortKey, aOutput);
  st = attributeAffects(aStrokeSortDirection, aOutput);
  st = attributeAffects(aStrokeSortList, aOutput);
  st = attributeAffects(aStrokeSortTexture, aOutput);
  st = attributeAffects(aApplyFilters, aOutput);
  st = attributeAffects(aStrokeFilterKey, aOutput);
  st = attributeAffects(aStrokeFilterOperator, aOutput);
  st = attributeAffects(aStrokeFilterOperand, aOutput);
  st = attributeAffects(aStrokeFilterList, aOutput);
  st = attributeAffects(aStrokeFilterTexture, aOutput);
  st = attributeAffects(aStartFrom, aOutput);
  st = attributeAffects(aEndAt, aOutput);

  return (MS::kSuccess);
}

MStatus strokeNodeBase::generateStrokeGeometry(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *geom)
{
  setStrokeIds(geom);
  filterStrokes(data, geom);
  sortStrokes(data, geom);
  cullStartEnd(data, geom);

  return MS::kSuccess;
}

MStatus strokeNodeBase::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  if (plug != aOutput)
  {
    return (MS::kUnknownParameter);
  }

  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(strokeData::id);

  MObject dOut = fnOut.create(kdid, &st);
  strokeData *newData = (strokeData *)fnOut.data(&st);
  mser;

  std::vector<Stroke> *geom = newData->fGeometry;

  st = generateStrokeGeometry(plug, data, geom);

  hOutput.set(newData);
  hOutput.setClean();
  return MS::kSuccess;
}

void strokeNodeBase::setStrokeIds(std::vector<Stroke> *geom) const
{
  unsigned i = 0;
  std::vector<Stroke>::iterator iter = geom->begin();
  for (; iter != geom->end(); iter++, i++)
  {
    iter->setStrokeId(i);
  }
}

void strokeNodeBase::getPivotPoints(const std::vector<Stroke> *geom, MFloatPointArray &result) const
{
  for (std::vector<Stroke>::const_iterator iter = geom->begin(); iter != geom->end(); iter++)
  {
    result.append(iter->pivot().position());
  }
}

void strokeNodeBase::getFirstTargetPoints(
    const std::vector<Stroke> *geom,
    MFloatPointArray &result) const
{
  for (std::vector<Stroke>::const_iterator iter = geom->begin(); iter != geom->end(); iter++)
  {
    result.append(iter->targets()[0].position());
  }
}

void strokeNodeBase::getTargetPoints(
    const std::vector<Stroke> *geom,
    MFloatPointArray &result) const
{
  for (std::vector<Stroke>::const_iterator iter = geom->begin(); iter != geom->end(); iter++)
  {
    const std::vector<Target> targets = iter->targets();
    for (std::vector<Target>::const_iterator targetIter = targets.begin(); targetIter != targets.end(); targetIter++)
    {
      const MFloatMatrix &mat = targetIter->matrix();
      result.append(MFloatPoint(mat[3][0], mat[3][1], mat[3][2]));
    }
  }
}

void strokeNodeBase::getSpanPoints(
    const std::vector<Stroke> *geom,
    MFloatPointArray &result) const
{
  for (std::vector<Stroke>::const_iterator iter = geom->begin(); iter != geom->end(); iter++)
  {
    const std::vector<Target> targets = iter->targets();
    MFloatPoint lastPos = targets[0].position();

    std::vector<Target>::const_iterator targetIter = std::next(targets.begin());
    for (; targetIter != targets.end(); targetIter++)
    {
      MFloatPoint thisPos = targetIter->position();
      result.append((thisPos + lastPos) / 2.0f);
      lastPos = thisPos;
    }
  }
}

bool strokeNodeBase::setFilterMapColor(std::vector<Stroke> *geom) const
{
  MStatus st;
  MObject thisObj = thisMObject();
  if (!TexUtils::hasTexture(thisObj, strokeNodeBase::aStrokeFilterTexture))
  {
    return false;
  }

  MFloatPointArray points;
  getPivotPoints(geom, points);

  MFloatVectorArray result;
  st = TexUtils::sampleSolidTexture(thisObj, strokeNodeBase::aStrokeFilterTexture, 1.0, points, result);
  if (st.error())
  {
    return false;
  }

  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++)
  {
    iter->setFilterColor(result[i]);
  }
  return true;
}

bool strokeNodeBase::setSortMapColor(std::vector<Stroke> *geom) const
{
  MStatus st;
  MObject thisObj = thisMObject();
  if (!TexUtils::hasTexture(thisObj, strokeNodeBase::aStrokeSortTexture))
  {
    return false;
  }

  MFloatPointArray points;
  getPivotPoints(geom, points);

  MFloatVectorArray result;
  st = TexUtils::sampleSolidTexture(
      thisObj, strokeNodeBase::aStrokeSortTexture, 1.0, points, result);
  if (st.error())
  {
    return false;
  }

  std::vector<Stroke>::iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++, i++)
  {
    iter->setSortColor(result[i]);
  }
  return true;
}

void strokeNodeBase::filterStrokes(MDataBlock &data, std::vector<Stroke> *geom) const

{
  MStatus st;

  bool applyFilters = data.inputValue(aApplyFilters).asBool();
  if (!applyFilters)
  {
    return;
  }

  MArrayDataHandle hFilterMulti = data.inputArrayValue(aStrokeFilterList, &st);
  if (st.error())
  {
    return;
  }

  FilterDefinition filterDefinition(
      hFilterMulti,
      aStrokeFilterKey,
      aStrokeFilterOperator,
      aStrokeFilterOperand);

  if (!filterDefinition.hasFilters())
  {
    return;
  }

  /* Set the mapped colors so they may be used for filtering */
  bool useFilterMap = filterDefinition.usesMap();
  if (useFilterMap)
  {
    useFilterMap = setFilterMapColor(geom);
  }

  for (auto filteriter = filterDefinition.begin(); filteriter != filterDefinition.end();
       filteriter++)

  {
    Stroke::FilterOperator op = std::get<1>(*filteriter);
    if (op == Stroke::kNoOp)
    {
      continue;
    }

    int value = std::get<2>(*filteriter);
    Stroke::SortFilterKey key = std::get<0>(*filteriter);

    std::vector<Stroke>::iterator new_end = geom->end();

    switch (key)
    {

    case Stroke::kStrokeId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke)
                               { return stroke.testStrokeId(op, value) == false; });
      break;
    case Stroke::kBrushId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke)
                               { return stroke.testBrushId(op, value) == false; });
      break;
    case Stroke::kPaintId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke)
                               { return stroke.testPaintId(op, value) == false; });
      break;
    case Stroke::kRepeatId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke)
                               { return stroke.testRepeatId(op, value) == false; });
      break;
    case Stroke::kTargetCount:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke)
                               { return stroke.testTargetCount(op, value) == false; });
      break;
    case Stroke::kLayerId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke)
                               { return stroke.testLayerId(op, value) == false; });
      break;
    case Stroke::kParentId:
      new_end = std::remove_if(geom->begin(), geom->end(),
                               [op, value](const Stroke &stroke)
                               { return stroke.testParentId(op, value) == false; });
      break;

    case Stroke::kMapRed:
      if (useFilterMap)
      {
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke &stroke)
                                 { return stroke.testMapRedId(op, value) == false; });
      }
      break;
    case Stroke::kMapGreen:
      if (useFilterMap)
      {
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke &stroke)
                                 { return stroke.testMapGreenId(op, value) == false; });
      }
      break;
    case Stroke::kMapBlue:
      if (useFilterMap)
      {
        new_end = std::remove_if(geom->begin(), geom->end(),
                                 [op, value](const Stroke &stroke)
                                 { return stroke.testMapBlueId(op, value) == false; });
      }

      break;
    default:
      break;
    }

    geom->erase(new_end, geom->end());
  }

  return;
}

void strokeNodeBase::cullStartEnd(MDataBlock &data, std::vector<Stroke> *geom) const
{

  int startFrom = data.inputValue(aStartFrom).asInt();
  int endAt = data.inputValue(aEndAt).asInt();

  if (endAt <= -1)
  {
    endAt = geom->size();
  }
  startFrom = std::max(std::min(startFrom, endAt), 0);

  if (startFrom > 0 || endAt < geom->size())
  {
    std::vector<Stroke>::iterator new_end = geom->end();

    // This is probably inefficient - but at least it doesn't crash.
    new_end = std::remove_if(geom->begin(), geom->end(),
                             [startFrom, endAt](const Stroke &stroke)
                             {
                               int sid = stroke.strokeId();
                               return (sid < startFrom || sid >= endAt);
                             });
    geom->erase(new_end, geom->end());
  }
}

void strokeNodeBase::sortStrokes(MDataBlock &data, std::vector<Stroke> *geom) const

{
  MStatus st;

  bool applySort = data.inputValue(aApplySort).asBool();
  if (!applySort)
  {
    return;
  }

  MArrayDataHandle hSortMulti = data.inputArrayValue(aStrokeSortList, &st);
  if (st.error())
  {
    return;
  }

  SortDefinition sortDefinition(hSortMulti, strokeNodeBase::aStrokeSortKey,
                                strokeNodeBase::aStrokeSortDirection);

  if (!sortDefinition.hasSort())
  {
    return;
  }

  /* Set the mapped colors so they may be used for filtering */
  bool useSortMap = sortDefinition.usesMap();
  if (useSortMap)
  {
    useSortMap = setSortMapColor(geom);
  }

  std::vector<Stroke>::iterator iter;
  for (iter = geom->begin(); iter != geom->end(); iter++)
  {
    iter->clearSortStack();
  }

  for (auto sortiter = sortDefinition.begin(); sortiter != sortDefinition.end();
       sortiter++)

  {

    bool ascending = (sortiter->second == Stroke::kSortAscending);
    switch (sortiter->first)
    {
    case Stroke::kStrokeId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendStrokeIdToSortStack(ascending);
      }
      break;

    case Stroke::kBrushId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendBrushIdToSortStack(ascending);
      }
      break;
    case Stroke::kPaintId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendPaintIdToSortStack(ascending);
      }
      break;
    case Stroke::kLayerId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendLayerIdToSortStack(ascending);
      }
      break;
    case Stroke::kParentId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendParentIdToSortStack(ascending);
      }
      break;

    case Stroke::kRepeatId:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendRepeatIdToSortStack(ascending);
      }
      break;
    case Stroke::kTargetCount:
      for (iter = geom->begin(); iter != geom->end(); iter++)
      {
        iter->appendTargetCountToSortStack(ascending);
      }
      break;

    case Stroke::kMapRed:
      if (useSortMap)
      {
        for (iter = geom->begin(); iter != geom->end(); iter++)
        {
          iter->appendMapRedIdToSortStack(ascending);
        }
      }
      break;
    case Stroke::kMapGreen:
      if (useSortMap)
      {
        for (iter = geom->begin(); iter != geom->end(); iter++)
        {
          iter->appendMapGreenIdToSortStack(ascending);
        }
      }
      break;
    case Stroke::kMapBlue:
      if (useSortMap)
      {
        for (iter = geom->begin(); iter != geom->end(); iter++)
        {
          iter->appendMapBlueIdToSortStack(ascending);
        }
      }
      break;
    default:
      break;
    }
  }

  std::sort(geom->begin(), geom->end());
}
