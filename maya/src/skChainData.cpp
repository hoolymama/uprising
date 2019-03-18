
#include "skChainData.h"
#include <jMayaIds.h>

const MTypeId skChainData::id( k_skChainData );
const MString skChainData::typeName( "skChainData" );

skChainData::skChainData() : fGeometry( NULL )
{
	fGeometry = new  std::vector<skChain>;
}
skChainData::~skChainData()
{
	if ( NULL != fGeometry ) {
		delete fGeometry;
		fGeometry = NULL;
	}
}

void skChainData::copy ( const MPxData &other )
{
	*fGeometry = *(((const skChainData &)other).fGeometry);
}

MTypeId skChainData::typeId() const
{
	return skChainData::id;
}

MString skChainData::name() const
{
	return skChainData::typeName;
}
void *skChainData::creator()
{
	return new skChainData;
}
