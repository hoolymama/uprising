/*
 *  testMulti.h
 *  animal
 *
 *  Created by Julian Mann on 01/08/2006.
 *  Copyright 2006 HoolyMama. All rights reserved.
 *
 */

#ifndef testMulti_H
#define testMulti_H


#include <maya/MPxNode.h>

class testMulti: public MPxNode {
public:
	virtual	void postConstructor();
	testMulti ();
	virtual ~testMulti ();
	virtual MStatus compute (const MPlug &plug, MDataBlock &data);
	static void *creator ();
	static MStatus initialize ();
	static MTypeId id;
private:

	static MObject aInput;
	static MObject aOutput;
};
#endif