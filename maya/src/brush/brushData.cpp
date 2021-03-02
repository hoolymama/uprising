
#include "brushData.h"
#include <jMayaIds.h>

#include <maya/MDataBlock.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnPluginData.h>


#include "errorMacros.h"

const MTypeId brushData::id( k_brushData );
const MString brushData::typeName( "brushData" );

brushData::brushData() : fGeometry( 0 )
{
	fGeometry = new Brush();
}
brushData::~brushData()
{
	if ( 0 != fGeometry ) {
		delete fGeometry;
		fGeometry = 0;
	}
}


void brushData::copy ( const MPxData &other )
{
	*fGeometry = *(((const brushData &)other).fGeometry);
}

MTypeId brushData::typeId() const
{
	return brushData::id;
}

MString brushData::name() const
{
	return brushData::typeName;
}
void *brushData::creator()
{
	return new brushData;
}
