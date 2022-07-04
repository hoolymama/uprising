#ifndef _brushShopData
#define _brushShopData

#include <map>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

#include "brush.h"
#include "brushRack.h"



class brushShopData : public MPxData
{
public:

	brushShopData();
	virtual ~brushShopData();

	void	 clear();

	// Brush	*geometry() const ;

	virtual MTypeId         typeId() const;
	virtual MString         name() const;
	void copy ( const MPxData &other );
	brushShopData &operator=(const brushShopData &otherData ) ;

	static void *creator();

public:

	static const MString typeName;
	static const MTypeId id;

	std::map<Brush::Shape, BrushRack> *fGeometry;
};
#endif


