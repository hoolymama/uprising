
#include "paintingData.h"
#include <jMayaIds.h>

#include <maya/MDataBlock.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnPluginData.h>

#include "strokeCurveData.h"


#include "errorMacros.h"

const MTypeId paintingData::id( k_paintingData );
const MString paintingData::typeName( "paintingData" );

paintingData::paintingData() : m_pGeometry( 0 )
{
}
paintingData::~paintingData()
{

}

paintingGeom	*paintingData::geometry() const {
	return m_pGeometry;
}

// clean up
void	paintingData::clear() {
	if (m_pGeometry) {
		delete m_pGeometry; m_pGeometry = 0;
	}
}

// create new tree
MStatus paintingData::create(MDataBlock &data, MObject &strokeCurvesAtt,
                             const std::map<short, Brush> &brushes,
                             const std::map<short, Paint> &paints)
{

	MStatus st = MS::kSuccess;
	clear();
	m_pGeometry = new paintingGeom();

	if (! m_pGeometry) {return MS::kFailure;}

	m_pGeometry->setBrushes(brushes);
	m_pGeometry->setPaints(paints);


	MArrayDataHandle hStrokeCurves = data.inputValue(strokeCurvesAtt, &st); ert;
	unsigned nCurves = hStrokeCurves.elementCount();
	cerr << "nCurves: " << nCurves << endl;
	for (unsigned i = 0; i < nCurves; i++, hStrokeCurves.next()) {
		short index = short(hStrokeCurves.elementIndex(&st));

		MDataHandle hStrokeCurve = hStrokeCurves.inputValue(&st );
		MObject dStrokeCurve = hStrokeCurve.data();
		MFnPluginData fnStrokeCurves( dStrokeCurve , &st); ert;
		strokeCurveData *scData = (strokeCurveData *)fnStrokeCurves.data();
		strokeCurveGeom *geom = scData->fGeometry;

		// strokeCurveGeom *pScGeom = 0;
		// st = getData(hStrokeCurves, pScGeom);



		// cerr << "POINTER: " << geom << endl;
		// cerr << *geom << endl;

		m_pGeometry->addStrokeCurve(*geom);

		// if (geom) {
		//   cerr << "num strokes" << geom->m_strokes.size() << endl;
		//   cerr << "m_forceDip" << geom->m_forceDip << endl;
		//   cerr << "m_brushId" << geom->m_brushId << endl;
		//   cerr << "m_paintId" << geom->m_paintId << endl;
		// }

	}

	// NOTE: there is a paintingGeom which is either
	// null, has zero size, or has size
	// Any calling function should check its state
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




// void paintingData::copy ( const MPxData &other )
// {
// 	*m_pGeometry = *(((const paintingData &)other).m_pGeometry);
// }

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
