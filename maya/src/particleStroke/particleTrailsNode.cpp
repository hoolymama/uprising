


#include <maya/MIOStream.h>

#include <vector>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>


#include <maya/MDoubleArray.h> 
#include <maya/MFnDoubleArrayData.h>
#include <maya/MVectorArray.h> 
#include <maya/MFnVectorArrayData.h>



#include <maya/MAnimControl.h>
#include <maya/MFnPluginData.h>


#include <jMayaIds.h>

#include "particleTrailsNode.h"
#include "particleTrailsData.h"

#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);

MObject particleTrailsNode::aCurrentTime;
MObject particleTrailsNode::aStartTime;
MObject particleTrailsNode::aParticleIds; 
MObject particleTrailsNode::aPositions; 
MObject particleTrailsNode::aColors; 
MObject particleTrailsNode::aWhites; 

MObject particleTrailsNode::aOutput; 

MTypeId particleTrailsNode::id(k_particleTrailsNode);

particleTrailsNode::particleTrailsNode():
    m_particleTrails() 
{
	m_lastTimeIEvaluated = MAnimControl::currentTime();
}

particleTrailsNode::~particleTrailsNode() {

}

void *particleTrailsNode::creator()
{
    return new particleTrailsNode();
}

const double epsilon = 0.0001;

MStatus particleTrailsNode::initialize()
{

    MStatus st;
    MFnTypedAttribute tAttr;
    MFnUnitAttribute uAttr;

	aCurrentTime = uAttr.create("currentTime", "ct", MFnUnitAttribute::kTime);
	uAttr.setStorable(true);
	addAttribute(aCurrentTime);

	aStartTime = uAttr.create("startTime", "st", MFnUnitAttribute::kTime);
	uAttr.setStorable(true);
	addAttribute(aStartTime);

	aParticleIds = tAttr.create("particleIds", "pid", MFnData::kDoubleArray);
	tAttr.setStorable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	addAttribute(aParticleIds);

	aPositions = tAttr.create("positions", "pos", MFnData::kVectorArray);
	tAttr.setStorable(false);
    tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	addAttribute(aPositions);

	aColors = tAttr.create("colors", "col", MFnData::kVectorArray);
	tAttr.setStorable(false);
    tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	addAttribute(aColors);

	aWhites = tAttr.create("whites", "wht", MFnData::kDoubleArray);
	tAttr.setStorable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	addAttribute(aWhites);


    aOutput = tAttr.create("output", "out", particleTrailsData::id);
    tAttr.setReadable(true);
    tAttr.setStorable(false);
    addAttribute(aOutput);

    st = attributeAffects(aCurrentTime, aOutput);
    st = attributeAffects(aStartTime, aOutput);
    return MS::kSuccess;
}

MStatus	particleTrailsNode::compute( const MPlug& plug, MDataBlock& data )
{
	if (plug != aOutput)
	{
		return (MS::kUnknownParameter);
	}
    MStatus st;

	MObject thisNode = thisMObject();

	MTime cT = timeValue(data, aCurrentTime);
	MTime sT = timeValue(data, aStartTime);
	MTime dT = cT - m_lastTimeIEvaluated;
	MTime oT = cT - sT;
	m_lastTimeIEvaluated = cT;

	double dt = dT.as(MTime::kSeconds);

    MDataHandle hOutput= data.outputValue(aOutput);
    MFnPluginData fnOutput;
    MTypeId kdid(particleTrailsData::id);
    MObject dOutput = fnOutput.create(kdid, &st); mser;
    particleTrailsData *newData = (particleTrailsData *)fnOutput.data(&st);
    mser;
    std::vector<particleTrail> *pOutTrails = newData->fGeometry;

    // Moving forwards OR still and on or after start time
    if (dt >= 0 && oT >=  MTime(0.0))
	{
        st = addToCache(data);mser;
	} else {
         m_particleTrails.clear();
    }

    pOutTrails->clear();
    std::vector<particleTrail>::const_iterator iter = m_particleTrails.begin();
    for (; iter != m_particleTrails.end(); iter++)
    {
        pOutTrails->push_back(*iter);
    }
    hOutput.set(newData);
    hOutput.setClean();

    return (MS::kSuccess);
}

MStatus particleTrailsNode::addToCache(
  MDataBlock &data
) {

	MStatus st;
	const MDoubleArray particleIds = MFnDoubleArrayData(data.inputValue(aParticleIds).data()).array();
	const MVectorArray positions = MFnVectorArrayData(data.inputValue(aPositions).data()).array();
	const MVectorArray colors = MFnVectorArrayData(data.inputValue(aColors).data()).array();
	const MDoubleArray whites = MFnDoubleArrayData(data.inputValue(aWhites).data()).array();

	unsigned len = particleIds.length();
	if (!(
		(positions.length() == len)
	 && (colors.length() == len)
	 && (whites.length() == len)
	)) {
		return MS::kFailure;
	}
 
	for (size_t i = 0; i < len; i++)
	{
		int particleId =  int(particleIds[i] + 0.1);
		if (! (particleId < m_particleTrails.size())) {
			m_particleTrails.resize(particleId+1);
		}
		m_particleTrails[particleId].append(
			MFloatPoint(positions[i]), 
			MColor(colors[i].x,colors[i].y,colors[i].z, whites[i])
		);
	}
	return MS::kSuccess;
}

void particleTrailsNode::postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
