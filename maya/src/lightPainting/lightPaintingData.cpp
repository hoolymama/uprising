
#include "lightPaintingData.h"
#include <jMayaIds.h>

#include <maya/MDataBlock.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnPluginData.h>

#include "strokeData.h"

#include "errorMacros.h"

const MTypeId lightPaintingData::id(k_lightPaintingData);
const MString lightPaintingData::typeName("lightPaintingData");

lightPaintingData::lightPaintingData() : m_pStrokes(0), m_pBrushes(0) {}
lightPaintingData::~lightPaintingData() {}

std::vector<Stroke> *lightPaintingData::strokes() const
{
	return m_pStrokes;
}
// Brush *lightPaintingData::brush() const
// {
// 	return m_pBrush;
// }

std::map<int, Brush> *lightPaintingData::brushes() const
{
	return m_pBrushes;
}

// clean up
void lightPaintingData::clear()
{

	if (m_pStrokes)
	{
		// cerr << "YES m_pStrokes " << m_pStrokes << endl;
		delete m_pStrokes;
		m_pStrokes = 0;
	}

	// if (m_pBrush)
	// {
	// 	// cerr << "YES m_pBrush " << m_pBrush << endl;
	// 	delete m_pBrush;
	// 	m_pBrush = 0;
	// }

		if (m_pBrushes)
	{
		// cerr << "YES m_pBrush " << m_pBrush << endl;
		delete m_pBrushes;
		m_pBrushes = 0;
	}
}

MStatus lightPaintingData::create()
{

	MStatus st = MS::kSuccess;
	clear();

	m_pStrokes = new std::vector<Stroke>();

	if (!m_pStrokes)
	{
		clear();
		return MS::kFailure;
	}

	// m_pBrush = new Brush();
	// if (!m_pBrush)
	// {
	// 	clear();
	// 	return MS::kFailure;
	// }


	m_pBrushes = new std::map<int, Brush>();
	if (!m_pBrushes)
	{
		clear();
		return MS::kFailure;
	}

	return MS::kSuccess;
}

void lightPaintingData::copy(const MPxData &otherData)
{
	m_pStrokes = ((const lightPaintingData &)otherData).strokes();
	// m_pBrush = ((const lightPaintingData &)otherData).brush();
	m_pBrushes = ((const lightPaintingData &)otherData).brushes();
	
}

lightPaintingData &lightPaintingData::operator=(const lightPaintingData &otherData)
{
	if (this != &otherData)
	{
		m_pStrokes = otherData.strokes();
		// m_pBrush = otherData.brush();
		m_pBrushes = otherData.brushes();
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
