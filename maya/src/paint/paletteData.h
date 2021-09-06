#ifndef _paletteData
#define _paletteData
#include <map>
#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "paint.h"

class paletteData : public MPxData
{
public:

	paletteData();
	virtual ~paletteData();
	virtual void            copy ( const MPxData &other );
	virtual MTypeId         typeId() const;
	virtual MString         name() const;

	static void *creator();
public:

	static const MString typeName;
	static const MTypeId id;


	std::map<int, Paint > *fGeometry;
};
#endif



