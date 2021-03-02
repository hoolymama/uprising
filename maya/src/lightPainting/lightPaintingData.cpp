
#include "lightPaintingData.h"
#include <jMayaIds.h>

#include <maya/MDataBlock.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnPluginData.h>

#include "strokeData.h"


#include "errorMacros.h"

const MTypeId lightPaintingData::id( k_lightPaintingData );
const MString lightPaintingData::typeName( "lightPaintingData" );

lightPaintingData::lightPaintingData() : m_pStrokes( 0 ) , m_pBrush( 0 ) {}
lightPaintingData::~lightPaintingData() {}

std::vector<Stroke>	*lightPaintingData::strokes() const {
	return m_pStrokes;
}
Brush	*lightPaintingData::brush() const {
	return m_pBrush;
}

// clean up
void	lightPaintingData::clear() {
	JPMDBG;
	if (m_pStrokes) {
		cerr << "YES m_pStrokes "<< m_pStrokes << endl;
		delete m_pStrokes; m_pStrokes = 0;
	}
	JPMDBG;
	if (m_pBrush) {
		cerr << "YES m_pBrush "<< m_pBrush << endl;
		delete m_pBrush; m_pBrush = 0;
	}
	JPMDBG;
}

MStatus lightPaintingData::create()
{
	JPMDBG;
	MStatus st = MS::kSuccess;
	clear();
	JPMDBG;
	m_pStrokes = new std::vector<Stroke>();
	JPMDBG;

	if (! m_pStrokes) {
		JPMDBG;
		clear();
		JPMDBG;
		return MS::kFailure;
	}
	JPMDBG;
	m_pBrush = new Brush();
	if (! m_pBrush) {
		JPMDBG;
		clear();
		JPMDBG;
		return MS::kFailure;
	}
	JPMDBG;

	return MS::kSuccess;
}

void lightPaintingData::copy(const MPxData &otherData)
{
	m_pStrokes = ((const lightPaintingData &)otherData).strokes();
	m_pBrush = ((const lightPaintingData &)otherData).brush();
	
}

lightPaintingData &lightPaintingData::operator=(const lightPaintingData &otherData ) {
	if (this != &otherData ) {
		m_pStrokes = otherData.strokes();
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
