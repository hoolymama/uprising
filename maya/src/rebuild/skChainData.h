#ifndef _skChainData
#define _skChainData

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "skChain.h"

class skChainData : public MPxData
{
public:

	skChainData();
	virtual ~skChainData();
	virtual void            copy ( const MPxData &other );
	virtual MTypeId         typeId() const;
	virtual MString         name() const;

	static void *creator();
public:

	static const MString typeName;
	static const MTypeId id;


	std::vector< skChain > *fGeometry;
};
#endif



