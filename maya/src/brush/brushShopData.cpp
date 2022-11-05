

#include "brushShopData.h"
#include <jMayaIds.h>

#include <maya/MDataBlock.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnPluginData.h>

#include "errorMacros.h"

const MTypeId brushShopData::id(k_brushShopData);
const MString brushShopData::typeName("brushShopData");

brushShopData::brushShopData() : fGeometry(0)
{
	fGeometry = new BrushShop();
}
brushShopData::~brushShopData()
{
	if (0 != fGeometry)
	{
		delete fGeometry;
		fGeometry = 0;
	}
}

void brushShopData::copy(const MPxData &other)
{
	*fGeometry = *(((const brushShopData &)other).fGeometry);
}

brushShopData &brushShopData::operator=(const brushShopData &otherData)
{
	if (this != &otherData)
	{
		*fGeometry = *(otherData.fGeometry);
	}
	return *this;
}

MTypeId brushShopData::typeId() const
{
	return brushShopData::id;
}

MString brushShopData::name() const
{
	return brushShopData::typeName;
}
void *brushShopData::creator()
{
	return new brushShopData;
}
