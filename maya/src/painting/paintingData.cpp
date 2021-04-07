
#include "paintingData.h"
#include <jMayaIds.h>

#include <maya/MDataBlock.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnPluginData.h>

// #include "strokeData.h"


#include "errorMacros.h"

const MTypeId paintingData::id( k_paintingData );
const MString paintingData::typeName( "paintingData" );

paintingData::paintingData() : m_pGeometry( 0 ) {}
paintingData::~paintingData() {}

paintingGeom	*paintingData::geometry() const {
	return m_pGeometry;
}

// clean up
void	paintingData::clear() {
	if (m_pGeometry) {
		delete m_pGeometry; 
		m_pGeometry = 0;
	}
}

MStatus paintingData::create()
{

	MStatus st = MS::kSuccess;
	clear();
	m_pGeometry = new paintingGeom();

	if (! m_pGeometry) {
		return MS::kFailure;
	}
	return MS::kSuccess;
}

void paintingData::copy(const MPxData &otherData)
{
	m_pGeometry = ((const paintingData &)otherData).geometry();
}

paintingData &paintingData::operator=(const paintingData &otherData ) {
	if (this != &otherData ) {
		m_pGeometry = otherData.geometry();
	}
	return *this;
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
