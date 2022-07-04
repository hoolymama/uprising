
#ifndef _brushLifter_H
#define _brushLifter_H

#include "strokeMutator.h"
#include "stroke.h"
#include "brushRack.h"
#include "brushShop.h"

/**
 * @brief Rotate all target matrices so they aim their z (lance) towards a point.
 *
 */
class brushLifter : public strokeMutator
{
public:
  brushLifter();
  virtual ~brushLifter();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;

  static MTypeId id;

  enum ApplyBias
  {
    kOff,
    kBeforeRotation,
    kAfterRotation
  };

private:
  static MObject aReassignBrushIds;
  static MObject aApplyLift;
  static MObject aApplyBias;

  static MObject aBrushShop; ///> The list of available brushes as a shop.

  void assignBrushes(BrushShop &brushShop, std::vector<Stroke> *strokes) const;

  MStatus getBrushShop(MDataBlock &data, BrushShop &brushShop) const;
  MStatus collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const;
  MStatus createCurve(const Stroke *stroke, MObject &curveData) const;
  void getTangents(const MObject &curveObject, MVectorArray &tangents) const;
  void setWeights(const Brush &brush, const MObject &curveObject, Stroke *stroke) const;
  void applyBias(const Brush &brush, const MObject &curveObject, Stroke *stroke, MVectorArray &tangents) const;

  void applyRotation(const Brush &brush, const MObject &curveObject, const MVectorArray &tangents, Stroke *stroke) const;
  void applyLift(const Brush &brush, Stroke *stroke) const;
};

#endif