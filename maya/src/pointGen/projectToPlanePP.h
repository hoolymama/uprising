#include <maya/MPxNode.h>

class projectToPlanePP : public MPxNode
{
public:
	virtual void postConstructor();
	projectToPlanePP();
	virtual ~projectToPlanePP();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	static void *creator();
	static MStatus initialize();
	static MTypeId id;

private:
	static MObject aInput;
	static MObject aPlaneNormal;
	static MObject aOutput;
};
