
#ifndef _strokeNode_H
#define _strokeNode_H

#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MVectorArray.h>

#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include "cImgData.h"
#include "stroke.h"


class strokeNode : public MPxNode
{
public:
  strokeNode();
  virtual ~strokeNode();
  virtual bool    isAbstractClass() const {return true;}

  static void *creator();
  static MStatus initialize();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static  MTypeId id;

  static MObject aBrushTiltRamp;
  static MObject aBrushBankRamp;
  static MObject aBrushTwistRamp;
protected:

  virtual MStatus generateStrokeGeometry(MDataBlock &data,
                                         std::vector<Stroke> *geom) const;

  virtual void assignUVs(MDataBlock &data, std::vector<Stroke> *geom) const;
  virtual void overridePaintIds(MDataBlock &data, std::vector<Stroke> *geom) const;
  virtual void overrideBrushIds(MDataBlock &data, std::vector<Stroke> *geom) const;
  virtual void filterStrokes(MDataBlock &data,  std::vector<Stroke> *geom) const;
  static MObject aPointDensity;
  static MObject aRepeats;
  static MObject aRepeatOffset;
  static MObject aRepeatMirror;
  static MObject aRepeatOscillate;
  static MObject aSeed;

  static MObject aStrokeDirection;

  static MObject aEntryLength;
  static MObject aExitLength;


  static MObject aBrushId;
  static MObject aPaintId;
  static MObject aLayerId;
  static MObject aActive;
  static MObject aStrokeCountFactor;

  static MObject aPivotFraction;
  static MObject aRepeatPivot;


  static MObject aBrushTiltRangeMin;
  static MObject aBrushTiltRangeMax;
  static MObject aBrushTiltRange;


  static MObject aBrushBankRangeMin;
  static MObject aBrushBankRangeMax;
  static MObject aBrushBankRange;


  static MObject aBrushTwistRangeMin;
  static MObject aBrushTwistRangeMax;
  static MObject aBrushTwistRange;

  static MObject  aBrushFollowStroke;


  static MObject aProjection;

  static MObject  aBrushIdImage;
  static MObject  aBrushIdRemapRamp;
  static MObject  aBrushIdRemapRange;
  static MObject  aPaintIdImage;
  static MObject  aPaintIdRemapRamp;
  static MObject  aPaintIdRemapRange;



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



  static MObject aOutput;
};



#endif