#ifndef _pearlChainData
#define _pearlChainData

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "pearlChain.h"

class pearlChainData : public MPxData
{
public:

	pearlChainData();
	virtual ~pearlChainData();
	virtual void            copy ( const MPxData &other );
	virtual MTypeId         typeId() const;
	virtual MString         name() const;

	static void *creator();
public:

	static const MString typeName;
	static const MTypeId id;


	std::vector< pearlChain > *fGeometry;
};
#endif



