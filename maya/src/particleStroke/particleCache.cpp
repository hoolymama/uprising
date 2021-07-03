

 
 
#include <vector>

#include <maya/MObject.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MGlobal.h>


#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <maya/MDataHandle.h>

#include "particleCache.h"

particleCache::particleCache( )
	: m_particleTrails()
{}

void particleCache::clear() {
	m_particleTrails.clear();
}

const std::vector<particleTrail>& particleCache::particlesTrails() const
{
	return m_particleTrails;
}



MStatus particleCache::add(
  double dt ,
  const  MObject &node,
  MDataBlock &data
) {

	MStatus st;
	MFnDependencyNode dn(node, &st); msert;

	MObject att = dn.attribute	( "particleId", &st); msert;
	MDoubleArray particleIds = MFnDoubleArrayData(data.inputValue(att).data()).array();
	unsigned len = particleIds.length();
 
	MVectorArray pos = 		getVectorArray("position", node, data, len, &st); mser;
	MVectorArray rgb = 		getVectorArray("rgbPP", node, data, len, &st); mser;
	
	for (size_t i = 0; i < len; i++)
	{
		int particleId =  int(particleIds[i] + 0.1);
		if (! (particleId < m_particleTrails.size())) {
			m_particleTrails.resize(particleId+1);
		}
		m_particleTrails[particleId].append(MFloatPoint(pos[i]), MColor(rgb[i].x,rgb[i].y,rgb[i].z));
	}
	return MS::kSuccess;
}


particleCache::~particleCache() {
	clear();
}

 