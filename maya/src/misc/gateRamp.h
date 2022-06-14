
#ifndef gateRamp_H
#define gateRamp_H

#include <maya/MPxNode.h>

class gateRamp: public MPxNode {

public:

	virtual	void		postConstructor();
	gateRamp ();
	virtual ~gateRamp ();

	virtual MStatus compute (const MPlug &plug, MDataBlock &data);
	static void *creator ();
	static MStatus initialize ();
	static MTypeId id;



private:

	static MObject aNumGates;
	static MObject aRange;
	static MObject aPadding;
	static MObject aRamp;


	static MObject aOutputMin;
	static MObject aOutputMax;
	static MObject aOutput;

};

#endif