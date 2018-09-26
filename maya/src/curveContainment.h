#ifndef curveContainment_H
#define curveContainment_H


#include <vector>

#include <maya/MPxNode.h>
#include "pointOnCurveInfo.h"

class curveContainment: public MPxNode {
public:
	virtual	void		postConstructor();
	curveContainment (); virtual ~curveContainment ();
	virtual MStatus compute (const MPlug &plug, MDataBlock &data);
	static void *creator (); static MStatus initialize (); static MTypeId id;
private:


	enum ContainmentType {kFirst, kLongest};


	MStatus getLengthsForCurve(ContainmentType ctype, MObject &dCurve,
	                           MFnMesh &meshFn, MMeshIsectAccelParams &ap, double sampleDist, int maxIterations,
	                           double tolerance, double &startLength, double &endLength);

	void getLongestChainIterators(
	  const std::vector<pointOnCurveInfo> &curvePoints,
	  std::vector<pointOnCurveInfo>::const_iterator &longestChainStart,
	  std::vector<pointOnCurveInfo>::const_iterator &longestChainEnd);

	void getFirstChainIterators(
	  const std::vector<pointOnCurveInfo> &curvePoints,
	  std::vector<pointOnCurveInfo>::const_iterator &chainStart,
	  std::vector<pointOnCurveInfo>::const_iterator &chainEnd);

	pointOnCurveInfo halfWayPoint(
	  const MFnNurbsCurve &curveFn,
	  const MFnMesh &meshFn,
	  const MMeshIsectAccelParams &ap,
	  double dist );


	static MObject aCurve;
	static MObject aMesh;
	static MObject aContainmentType ;
	static MObject aSampleDistance;
	static MObject aMaxIterations ;

	static MObject aTolerance ;
	static MObject aOutLength;

};
#endif
