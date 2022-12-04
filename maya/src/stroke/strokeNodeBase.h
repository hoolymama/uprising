
#ifndef _strokeNodeBase_H
#define _strokeNodeBase_H

#include <maya/MPxNode.h>

#include "stroke.h"

class strokeNodeBase : public MPxNode
{
public:
  strokeNodeBase();
  virtual ~strokeNodeBase();
  virtual bool isAbstractClass() const { return true; }

  static void *creator();
  static MStatus initialize();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;

protected:
  virtual MStatus generateStrokeGeometry(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *geom);

  void getPivotPoints(
      const std::vector<Stroke> *geom,
      MFloatPointArray &result) const;

  void getStrokeParamPoints(
    const std::vector<Stroke> *geom, 
    float strokeParam, 
    MFloatPointArray &result) const;

  void getFirstTargetPoints(
      const std::vector<Stroke> *geom,
      MFloatPointArray &result) const;

  void getTargetPoints(
      const std::vector<Stroke> *geom,
      MFloatPointArray &result) const;

void getTargetColors(
    const std::vector<Stroke> *geom,
    MColorArray &result) const;


void getTangents(
    const std::vector<Stroke> *geom,
    MFloatVectorArray &result) const;


  void getSpanPoints(
      const std::vector<Stroke> *geom,
      MFloatPointArray &result) const;

  void cullStartEnd(MDataBlock &data, std::vector<Stroke> *geom) const;

  static MObject aStrokeSortKey;
  static MObject aStrokeSortDirection;
  static MObject aStrokeSortList;
  static MObject aStrokeSortTexture;
  static MObject aApplySort;

  static MObject aStrokeFilterKey;
  static MObject aStrokeFilterOperator;
  static MObject aStrokeFilterOperand;
  static MObject aStrokeFilterList;
  static MObject aStrokeFilterTexture;
  static MObject aApplyFilters;

  static MObject aStartFrom;
  static MObject aEndAt;
  

  static MObject aOutput;

private:
  void setStrokeIds(std::vector<Stroke> *geom) const;
  void filterStrokes(MDataBlock &data, std::vector<Stroke> *geom) const;
  void sortStrokes(MDataBlock &data, std::vector<Stroke> *geom) const;
  bool setFilterMapColor(std::vector<Stroke> *geom) const;
  bool setSortMapColor(std::vector<Stroke> *geom) const;
};

#endif