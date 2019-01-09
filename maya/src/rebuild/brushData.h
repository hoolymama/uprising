#ifndef _brushData
#define _brushData

#include <map>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
// #include "paintingGeom.h"

#include "brush.h"
// #include "paint.h"



class brushData : public MPxData
{
public:

	brushData();
	virtual ~brushData();

	void	 clear();

	brush	*geometry() const ;

	virtual MTypeId         typeId() const;
	virtual MString         name() const;

	static void *creator();

public:

	static const MString typeName;
	static const MTypeId id;

	brush *m_pGeometry;
};
#endif

