
#ifndef _collectStrokes_H
#define _collectStrokes_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "cImgData.h"
#include "stroke.h"


class collectStrokes : public MPxNode
{
public:
  collectStrokes();
  virtual ~collectStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void    postConstructor();
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId id;

private:

  MStatus  collect(MDataBlock &data, std::vector<Stroke> *geom);

  void getUVs(std::vector<Stroke> &strokePool, MFloatArray &uVals,
              MFloatArray &vVals) const;

  void assignUVs(MDataBlock &data, std::vector<Stroke> *geom) const;
  void filterStrokes(MDataBlock &data,  std::vector<Stroke> *geom) const;
  void sortStrokes(MDataBlock &data, std::vector<Stroke> *geom) const;
  bool setFilterMapColor(std::vector<Stroke> *geom) const;
  bool setSortMapColor(std::vector<Stroke> *geom) const;

  bool getMappedColors(std::vector<Stroke> *geom, MObject &attribute,
                       MFloatVectorArray &result) const;

  void cullStartEnd(MDataBlock &data,   std::vector<Stroke> *geom) const;



  bool overrideBrushIds(MDataBlock &data, std::vector<Stroke> *geom) const;


  bool overridePaintIds(MDataBlock &data, std::vector<Stroke> *geom) const;



  static MObject  aStrokes;

  // static MObject  aSeed;

  // static MObject  aStrokeCountFactor;

  static MObject  aProjection;

  static MObject  aStrokeSortKey;
  static MObject  aStrokeSortDirection;
  static MObject  aStrokeSortList;
  static MObject  aStrokeSortTexture;
  static MObject  aApplySort;

  static MObject  aStrokeFilterKey;
  static MObject  aStrokeFilterOperator;
  static MObject  aStrokeFilterOperand;
  static MObject  aStrokeFilterList;
  static MObject  aStrokeFilterTexture;
  static MObject  aApplyFilters;

  static MObject  aStartFrom;
  static MObject  aEndAt;


  static MObject  aBrushIdTexture;
  static MObject  aBrushIdRemapRamp;
  static MObject  aBrushIdRemapRange;
  static MObject  aPaintIdTexture;
  static MObject  aPaintIdRemapRamp;
  static MObject  aPaintIdRemapRange;



  static MObject  aOutput;
};



#endif