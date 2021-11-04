
#include "particleTrailsData.h"
#include <jMayaIds.h>

const MTypeId particleTrailsData::id( k_particleTrailsData );
const MString particleTrailsData::typeName( "particleTrailsData" );

particleTrailsData::particleTrailsData() : fGeometry( NULL )
{
	fGeometry = new  std::vector<particleTrail>;
}
particleTrailsData::~particleTrailsData()
{
	if ( NULL != fGeometry ) {
		delete fGeometry;
		fGeometry = NULL;
	}
}

void particleTrailsData::copy ( const MPxData &other )
{
	*fGeometry = *(((const particleTrailsData &)other).fGeometry);
}

MTypeId particleTrailsData::typeId() const
{
	return particleTrailsData::id;
}

MString particleTrailsData::name() const
{
	return particleTrailsData::typeName;
}
void *particleTrailsData::creator()
{
	return new particleTrailsData;
}
