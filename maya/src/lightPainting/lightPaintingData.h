#ifndef _lightPaintingData
#define _lightPaintingData

#include <map>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

#include "brush.h"
#include "stroke.h"
 
class lightPaintingData : public MPxData
{
public:

	lightPaintingData();
	virtual ~lightPaintingData();

	void	 clear();

	MStatus create() ;

	// MStatus  create(
	//   MDataBlock &data,
	//   MObject &strokeCurvesAtt);

	virtual void copy ( const MPxData &other );
	lightPaintingData &operator=(const lightPaintingData &otherData ) ;
	
	std::vector<Stroke>	*geometry() const ;
	Brush *brush() const ;

	

	virtual MTypeId         typeId() const;
	virtual MString         name() const;

	static void *creator();

public:

	static const MString typeName;
	static const MTypeId id;

	std::vector<Stroke> *m_pGeometry;
	Brush *m_pBrush;
	
};
#endif

