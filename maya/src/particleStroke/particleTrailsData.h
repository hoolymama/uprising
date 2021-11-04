#ifndef _particleTrailsData
#define _particleTrailsData
#include <vector>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "particleTrail.h"

class particleTrailsData : public MPxData
{
public:

	particleTrailsData();
	virtual ~particleTrailsData();
	virtual void            copy ( const MPxData &other );
	virtual MTypeId         typeId() const;
	virtual MString         name() const;

	static void *creator();
public:

	static const MString typeName;
	static const MTypeId id;


	std::vector< particleTrail > *fGeometry;
};
#endif



