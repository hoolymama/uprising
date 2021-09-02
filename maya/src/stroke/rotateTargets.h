
#ifndef _rotateTargets_H
#define _rotateTargets_H

#include "strokeMutator.h"
#include "stroke.h"

class rotateTargets : public strokeMutator
{
public:
  rotateTargets();
  virtual ~rotateTargets();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(
    const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;
  static MTypeId id;

private:

  static MObject aCanvasMatrix;

  static MObject aSineFrequency;
  static MObject aSineAmplitude;
  static MObject aRandomSineOffset;
  static MObject aSeed;
  static MObject aAxis;
  

  enum RotationAxis
	{
		kCanvas,
		kTilt,
		kTwist,
		kBank,
	};

 
};

#endif