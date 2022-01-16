
#ifndef _lightPaintingNODE_H
#define _lightPaintingNODE_H

#include <maya/MTypeId.h>
#include "paintingNodeBase.h"
#include "lightPaintingData.h"
#include "stroke.h"

class lightPainting : public paintingBase
{
public:
  lightPainting();

  virtual ~lightPainting();

  virtual bool isAbstractClass() const { return false; }

  static void *creator();

  static MStatus initialize();

  virtual void postConstructor();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;
  static MString drawDbClassification;
  static MString drawRegistrantId;

  static MObject aViewMatrix;
  static MObject aDisplayTargetColors;
  static MObject aOutput;

private:
  MStatus addStrokes(MDataBlock &data, std::vector<Stroke> *outStrokeGeom);

  static MObject aBrush;
  static MObject aColorGain;
  static MObject aWhiteGain;
  static MObject aWaitGain;

  lightPaintingData *m_pd;
};

#endif
