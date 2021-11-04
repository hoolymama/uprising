
#ifndef _particleTrailsNode_H
#define _particleTrailsNode_H

#include <vector>

#include <maya/MPxNode.h>

#include <maya/MDataBlock.h>
#include <maya/MTime.h>
#include <maya/MDataBlock.h>
#include <maya/MDataBlock.h>
#include "particleTrail.h"


class particleTrailsNode : public MPxNode
{

public:
    particleTrailsNode();

    virtual ~particleTrailsNode();
    static void *creator();
    static MStatus initialize();
    virtual void postConstructor();
    static MTypeId id;
    virtual MStatus compute(const MPlug &plug, MDataBlock &data);

private:

    static MObject aCurrentTime;
    static MObject aStartTime;
    
    static MObject aParticleIds;
    static MObject aPositions;
    static MObject aColors;
    static MObject aWhites;

    static MObject aOutput;  

    MTime m_lastTimeIEvaluated;
    std::vector<particleTrail> m_particleTrails;
    MTime timeValue(MDataBlock &data, MObject &attribute);

    MStatus addToCache(MDataBlock &data);


};

inline MTime particleTrailsNode::timeValue(MDataBlock &data, MObject &attribute)
{
    MStatus status;
    MDataHandle hValue = data.inputValue(attribute, &status);

    MTime value(0.0);
    if (status == MS::kSuccess)
        value = hValue.asTime();
    return (value);
}

#endif
