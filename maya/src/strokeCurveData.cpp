
#include "strokeCurveData.h"
#include <jMayaIds.h>

const MTypeId strokeCurveData::id( k_strokeCurveData );
const MString strokeCurveData::typeName( "strokeCurveData" );

strokeCurveData::strokeCurveData() : fGeometry( NULL )
{
	// fGeometry = new strokeCurveGeom;
	fGeometry = new  std::vector<strokeGeom>;
}
strokeCurveData::~strokeCurveData()
{
	if ( NULL != fGeometry ) {
		delete fGeometry;
		fGeometry = NULL;
	}
}

void strokeCurveData::copy ( const MPxData &other )
{
	*fGeometry = *(((const strokeCurveData &)other).fGeometry);
}

MTypeId strokeCurveData::typeId() const
{
	return strokeCurveData::id;
}

MString strokeCurveData::name() const
{
	return strokeCurveData::typeName;
}
void *strokeCurveData::creator()
{
	return new strokeCurveData;
}
