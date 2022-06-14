
#ifndef stepRamp_H
#define stepRamp_H

#include <maya/MPxNode.h>

class stepRamp: public MPxNode {

public:

	virtual	void		postConstructor();
	stepRamp ();
	virtual ~stepRamp ();

	virtual MStatus compute (const MPlug &plug, MDataBlock &data);
	static void *creator ();
	static MStatus initialize ();
	static MTypeId id;



private:

	static MObject aNumSteps;
	static MObject aRange;
	static MObject aRamp;
	static MObject aOutput;

};

#endif

