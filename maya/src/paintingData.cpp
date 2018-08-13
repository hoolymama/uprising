
#include "paintingData.h"
#include <jMayaIds.h>

const MTypeId paintingData::id( k_paintingData );
const MString paintingData::typeName( "paintingData" );

paintingData::paintingData() : fGeometry( NULL )
{
	fGeometry = new paintingGeom;
}
paintingData::~paintingData()
{
	if ( NULL != fGeometry ) {
		delete fGeometry;
		fGeometry = NULL;
	}
}

void paintingData::copy ( const MPxData &other )
{
	*fGeometry = *(((const paintingData &)other).fGeometry);
}

MTypeId paintingData::typeId() const
{
	return paintingData::id;
}

MString paintingData::name() const
{
	return paintingData::typeName;
}
void *paintingData::creator()
{
	return new paintingData;
}
