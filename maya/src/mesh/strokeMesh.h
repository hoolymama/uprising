

#include <maya/MPointArray.h>
#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>

class strokeMesh : public MPxNode
{
public:
	strokeMesh();
	virtual ~strokeMesh();

	virtual MStatus compute(const MPlug &, MDataBlock &);
	virtual void postConstructor();

	static void *creator();
	static MStatus initialize();
	static MTypeId id;

	void calcDefaultProfile(
		int numProfilePoints,
		double profileScale,
		MPointArray &profileVertices) const;

	void extrude(const std::vector<Stroke> *pStrokes, const MPointArray &profileVertices, MObject &geom);

	int createTube(
		int numTargets,
		int numProfilePoints,
		int vertexIndex,
		MIntArray &faceCounts,
		MIntArray &connectivity);

private:
	static MObject aStrokes; // array of chain points
	static MObject aNumProfilePoints;
	static MObject aProfileScale;
	static MObject aCurveSet;
	static MObject aProfileCurves;

	// static MObject aUWidth;
	// static MObject aUOffset;

	//  static MObject aVLength;
	//  static MObject aVOffset;

	static MObject aOutMesh;
};
