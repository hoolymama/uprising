
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
      MDataBlock &data,
      std::vector<Stroke> *geom) ;

  virtual void filterStrokes(MDataBlock &data, std::vector<Stroke> *geom) const;
  virtual void sortStrokes(MDataBlock &data, std::vector<Stroke> *geom) const;
  virtual bool setFilterMapColor(std::vector<Stroke> *geom) const;
  virtual bool setSortMapColor(std::vector<Stroke> *geom) const;

  void getPivotPoints(
    const std::vector<Stroke> *geom,
    MFloatPointArray &result) const;

  void getTargetPoints(
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
};

#endif