#ifndef _brushData
#define _brushData

#include <map>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

#include "brush.h"



class brushData : public MPxData
{
public:

	brushData();
	virtual ~brushData();

	void	 clear();

	// Brush	*geometry() const ;

	virtual MTypeId         typeId() const;
	virtual MString         name() const;
	void copy ( const MPxData &other );
	// brushData &operator=(const brushData &otherData ) ;

	static void *creator();

public:

	static const MString typeName;
	static const MTypeId id;

	Brush *fGeometry;
};
#endif


