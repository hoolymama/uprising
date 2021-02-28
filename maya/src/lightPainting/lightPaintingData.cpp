
#include "lightPaintingData.h"
#include <jMayaIds.h>

#include <maya/MDataBlock.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnPluginData.h>

// #include "strokeData.h"


#include "errorMacros.h"

const MTypeId lightPaintingData::id( k_lightPaintingData );
const MString lightPaintingData::typeName( "lightPaintingData" );

lightPaintingData::lightPaintingData() : m_pGeometry( 0 ) {}
lightPaintingData::~lightPaintingData() {}

std::vector<Stroke>	*lightPaintingData::geometry() const {
	return m_pGeometry;
}
Brush	*lightPaintingData::brush() const {
	return m_pBrush;
}

// clean up
void	lightPaintingData::clear() {
	if (m_pGeometry) {
		delete m_pGeometry; m_pGeometry = 0;
	}
	if (m_pBrush) {
		delete m_pBrush; m_pBrush = 0;
	}
}

MStatus lightPaintingData::create()
{

	MStatus st = MS::kSuccess;
	clear();

	m_pGeometry = new std::vector<Stroke>();
	

	if (! m_pGeometry) {
		return MS::kFailure;
	}
	m_pBrush = new Brush();
	if (! m_pBrush) {
		clear();
		return MS::kFailure;
	}

	return MS::kSuccess;
}

void lightPaintingData::copy(const MPxData &otherData)
{
	m_pGeometry = ((const lightPaintingData &)otherData).geometry();
	m_pBrush = ((const lightPaintingData &)otherData).brush();
	
}

lightPaintingData &lightPaintingData::operator=(const lightPaintingData &otherData ) {
	if (this != &otherData ) {
		m_pGeometry = otherData.geometry();
		m_pBrush = otherData.brush();	
	}
	return *this;
}

MTypeId lightPaintingData::typeId() const
{
	return lightPaintingData::id;
}

MString lightPaintingData::name() const
{
	return lightPaintingData::typeName;
}
void *lightPaintingData::creator()
{
	return new lightPaintingData;
}
