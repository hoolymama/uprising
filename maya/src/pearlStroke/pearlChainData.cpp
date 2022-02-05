
#include "pearlChainData.h"
#include <jMayaIds.h>

const MTypeId pearlChainData::id( k_pearlChainData );
const MString pearlChainData::typeName( "pearlChainData" );

pearlChainData::pearlChainData() : fGeometry( NULL )
{
	fGeometry = new  std::vector<pearlChain>;
}
pearlChainData::~pearlChainData()
{
	if ( NULL != fGeometry ) {
		delete fGeometry;
		fGeometry = NULL;
	}
}

void pearlChainData::copy ( const MPxData &other )
{
	*fGeometry = *(((const pearlChainData &)other).fGeometry);
}

MTypeId pearlChainData::typeId() const
{
	return pearlChainData::id;
}

MString pearlChainData::name() const
{
	return pearlChainData::typeName;
}
void *pearlChainData::creator()
{
	return new pearlChainData;
}
