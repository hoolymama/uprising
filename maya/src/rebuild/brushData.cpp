
#include "brushData.h"
#include <jMayaIds.h>

#include <maya/MDataBlock.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnPluginData.h>


#include "errorMacros.h"

const MTypeId brushData::id( k_brushData );
const MString brushData::typeName( "brushData" );

brushData::brushData() : m_pGeometry( 0 ) {}
brushData::~brushData() {}

brush	*brushData::geometry() const {
	return m_pGeometry;
}

// clean up
/*void	brushData::clear() {
	if (m_pGeometry) {
		delete m_pGeometry; m_pGeometry = 0;
	}
}*/
/*
MStatus brushData::create()
{

	MStatus st = MS::kSuccess;
	clear();
	m_pGeometry = new paintingGeom();

	if (! m_pGeometry) {
		return MS::kFailure;
	}
	return MS::kSuccess;
}*/

// void brushData::copy(const MPxData &otherData)
// {
// 	m_pGeometry = ((const brushData &)otherData).geometry();
// }

// brushData &brushData::operator=(const brushData &otherData ) {
// 	if (this != &otherData ) {
// 		m_pGeometry = otherData.geometry();
// 	}
// 	return *this;
// }

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
