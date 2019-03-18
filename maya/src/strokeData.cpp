
#include "strokeData.h"
#include <jMayaIds.h>

const MTypeId strokeData::id( k_strokeData );
const MString strokeData::typeName( "strokeData" );

strokeData::strokeData() : fGeometry( NULL )
{
	fGeometry = new  std::vector<Stroke>;
}
strokeData::~strokeData()
{
	if ( NULL != fGeometry ) {
		delete fGeometry;
		fGeometry = NULL;
	}
}

void strokeData::copy ( const MPxData &other )
{
	*fGeometry = *(((const strokeData &)other).fGeometry);
}

MTypeId strokeData::typeId() const
{
	return strokeData::id;
}

MString strokeData::name() const
{
	return strokeData::typeName;
}
void *strokeData::creator()
{
	return new strokeData;
}
