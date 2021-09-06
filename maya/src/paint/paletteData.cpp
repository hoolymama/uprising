
#include "paletteData.h"
#include <jMayaIds.h>

const MTypeId paletteData::id( k_paletteData );
const MString paletteData::typeName( "paletteData" );

paletteData::paletteData() : fGeometry( NULL )
{
	fGeometry = new  std::map<int, Paint>;
}
paletteData::~paletteData()
{
	if ( NULL != fGeometry ) {
		delete fGeometry;
		fGeometry = NULL;
	}
}

void paletteData::copy ( const MPxData &other )
{
	*fGeometry = *(((const paletteData &)other).fGeometry);
}

MTypeId paletteData::typeId() const
{
	return paletteData::id;
}

MString paletteData::name() const
{
	return paletteData::typeName;
}
void *paletteData::creator()
{
	return new paletteData;
}
