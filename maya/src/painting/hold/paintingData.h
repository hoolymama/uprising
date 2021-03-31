#ifndef _paintingData
#define _paintingData

#include <map>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "paintingGeom.h"

#include "brush.h"
#include "paint.h"


class paintingData : public MPxData
{
public:

	paintingData();
	virtual ~paintingData();

	void	 clear();

	MStatus create() ;

	MStatus  create(
	  MDataBlock &data,
	  MObject &strokeCurvesAtt,
	  const std::map<short, Brush> &brushes,
	  const std::map<short, Paint> &paints);

	virtual void copy ( const MPxData &other );
	paintingData &operator=(const paintingData &otherData ) ;
	paintingGeom	*geometry() const ;

	virtual MTypeId         typeId() const;
	virtual MString         name() const;

	static void *creator();

public:

	static const MString typeName;
	static const MTypeId id;

	paintingGeom *m_pGeometry;
};
#endif

