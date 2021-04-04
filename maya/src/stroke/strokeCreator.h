
#ifndef _strokeCreator_H
#define _strokeCreator_H

#include <strokeNodeBase.h>
#include <maya/MPxNode.h>
#include "stroke.h"

/**
 * @brief A node with attributes common to al nodes that generate strokes. 
 *
 * It inherits from strokeNodeBase and adds point density and initial rot matrix.
 */
class strokeCreator : public strokeNodeBase
{
public:
  enum BrushRotateOrder
  {
    kTwistTiltBank,
    kTiltBankTwist,
    kBankTwistTilt,
    kTiltTwistBank,
    kTwistBankTilt,
    kBankTiltTwist,
  };

  strokeCreator();
  virtual ~strokeCreator();
  virtual bool isAbstractClass() const { return true; }

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  virtual MStatus generateStrokeGeometry(
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

  static MTypeId id;

protected:
  void applyRotations(MDataBlock &data, std::vector<Stroke> *pOutStrokes) const;

  static MObject aPointDensity; ///< The minimum number of targets per centimeter.
  static MObject aBrushTiltStart;
  static MObject aBrushTiltEnd;
  static MObject aBrushTilt;
  static MObject aBrushTwistStart;
  static MObject aBrushTwistEnd;
  static MObject aBrushTwist;
  static MObject aBrushBankStart;
  static MObject aBrushBankEnd;
  static MObject aBrushBank;
  static MObject aBrushRotateOrder;
};

#endif