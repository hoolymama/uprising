
#ifndef _paintingNODE_H
#define _paintingNODE_H

#include "paintingData.h"
#include "paintingNodeBase.h"


class painting : public paintingBase
{
public:
  painting();
  virtual bool isAbstractClass() const { return false; }
  virtual ~painting();

  static void *creator();

  static MStatus initialize();

  virtual void postConstructor();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static MTypeId id;
  static MString drawDbClassification;
  static MString drawRegistrantId;


  static MObject aCanvasMatrix;

  static MObject aDisplayApproachTargets;
  static MObject aDisplayClusterPath;
  static MObject aDisplayPivots;

  static MObject aDisplayBrushIds;
  static MObject aDisplayPaintIds;
  static MObject aDisplayRepeatIds;

  static MObject aDisplayContactWidth;
  static MObject aClusterPathColor;

  static MObject aStackGap;

  static MObject aOutput;
private:


  MStatus  getPalette(MDataBlock &data, std::map<int, Paint> &palette) const;
  MStatus collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes);

  MStatus addStrokes(MDataBlock &data, paintingGeom *pGeom);

  static MObject aBrushes;

  static MObject aApproachDistanceStart;
  static MObject aApproachDistanceMid;
  static MObject aApproachDistanceEnd;
  static MObject aApproachDistance;

  static MObject aMaxPointToPointDistance; // cm

  static MObject aPalette;




  paintingData *m_pd;

};
 
#endif
