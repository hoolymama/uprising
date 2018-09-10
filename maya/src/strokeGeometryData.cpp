
#include "strokeGeometryData.h"
#include <jMayaIds.h>

const MTypeId strokeGeometryData::id( k_strokeGeometryData );
const MString strokeGeometryData::typeName( "strokeGeometryData" );

strokeGeometryData::strokeGeometryData() : fGeometry( NULL )
{
	// fGeometry = new strokeCurveGeom;
	fGeometry = new  std::vector<strokeGeom>;
}
strokeGeometryData::~strokeGeometryData()
{
	if ( NULL != fGeometry ) {
		delete fGeometry;
		fGeometry = NULL;
	}
}

void strokeGeometryData::copy ( const MPxData &other )
{
	*fGeometry = *(((const strokeGeometryData &)other).fGeometry);
}

MTypeId strokeGeometryData::typeId() const
{
	return strokeGeometryData::id;
}

MString strokeGeometryData::name() const
{
	return strokeGeometryData::typeName;
}
void *strokeGeometryData::creator()
{
	return new strokeGeometryData;
}
