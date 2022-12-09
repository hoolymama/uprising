
#ifndef _funCurve_H
#define _funCurve_H

#include <maya/MPxNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
#include <maya/MStringArray.h>

#include <maya/MObject.h>
#include <maya/MPxNode.h>

class funCurve : public MPxNode
{
public:
  funCurve();
  virtual ~funCurve();
  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();

  MStatus compute(const MPlug &plug, MDataBlock &data);
  MStatus createCurve(const MPointArray &points, MObject &curveData, double minimumPointDistance=0.0);
  MString generateFunction(const MStringArray &context, const MString &expression, const MString &functionName);
  void generateContext(MDataBlock &data,  double g_p0, double g_p1, int samples, MStringArray &context);

    static MTypeId id;

  private:
    static MObject aParameterRange;
    static MObject aParameterRangeMin;
    static MObject aParameterRangeMax;
    static MObject aSamples;

    static MObject aSubSampleRange;
    static MObject aSubSampleRangeBegin;
    static MObject aSubSampleRangeEnd;

    static MObject aPreCommand;

    static MObject aCoefficients;
    static MObject aCoefficientName;
    static MObject aCoefficientValue;

    static MObject aExpression;
    
    static MObject aMinimumEPDistance;

    static MObject aOutput;
  };

#endif
