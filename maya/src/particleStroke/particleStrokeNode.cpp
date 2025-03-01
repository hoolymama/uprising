
#include <maya/MIOStream.h>

#include <vector>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnPluginData.h>

#include "particleStrokeNode.h"
#include "stroke.h"
#include "particleTrail.h"
#include "particleTrailsData.h"

#include <maya/MAnimControl.h>
 

#include <jMayaIds.h>
#include "errorMacros.h"
#include "mayaMath.h"

const double rad_to_deg = (180 / 3.1415927);

MObject particleStrokeNode::aTargetRotationMatrix; 
MObject particleStrokeNode::aTrails;

MTypeId particleStrokeNode::id(k_particleStrokeNode);

particleStrokeNode::particleStrokeNode(){}

particleStrokeNode::~particleStrokeNode(){}

void *particleStrokeNode::creator()
{
    return new particleStrokeNode();
}

const double epsilon = 0.0001;

MStatus particleStrokeNode::initialize()
{

    MStatus st;
    MFnTypedAttribute tAttr;
    MFnNumericAttribute nAttr;
  
    MFnMatrixAttribute mAttr;
 
    inheritAttributesFrom("strokeCreator");

    aTargetRotationMatrix= mAttr.create("targetRotationMatrix", "tmat", MFnMatrixAttribute::kFloat);
    mAttr.setStorable(false);
    mAttr.setHidden(false);
    mAttr.setKeyable(true);
    addAttribute(aTargetRotationMatrix);

    aTrails = tAttr.create("trails", "tls", particleTrailsData::id);
    tAttr.setStorable(false);
    tAttr.setKeyable(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kReset);
    addAttribute(aTrails);
    mser;

    st = attributeAffects(aTrails, aOutput);
    st = attributeAffects(aTargetRotationMatrix, aOutput);
    return MS::kSuccess;
}

MStatus particleStrokeNode::generateStrokeGeometry(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *pStrokes) 
{

    MStatus st;

    MFloatMatrix targetRotationMatrix = data.inputValue(aTargetRotationMatrix).asFloatMatrix();
    targetRotationMatrix = mayaMath::rotationOnly(targetRotationMatrix);

    MDataHandle hTrails = data.inputValue(aTrails);

    MObject dTrails = hTrails.data();
    MFnPluginData fnTrails(dTrails, &st);msert;

    particleTrailsData *ptData = (particleTrailsData *)fnTrails.data(&st);msert;

    const std::vector<particleTrail> *geom = ptData->fGeometry;

    
    float pointDensity = data.inputValue(aPointDensity).asFloat();

    std::vector<particleTrail>::const_iterator pTrail;
    for (pTrail = geom->begin(); pTrail != geom->end(); pTrail++)
    {
        if (pTrail->positions().length() < 2)
        {
            continue;
        }
        MFloatPointArray pts;
        MColorArray colors;

        strokeCreator::subsample(
                    pTrail->positions(),
                    pTrail->colors(),
                    pointDensity, 
                    pts,
                    colors);
        

        Stroke stroke(pts, targetRotationMatrix);
        Stroke::target_iterator target = stroke.targets_begin();
        for (int i=0; target != stroke.targets_end(); target++, i++)
        {
            target->setColor(colors[i]);
        }
        pStrokes->push_back(stroke);
    }

    strokeCreator::generateStrokeGeometry(plug,data,pStrokes);
    return (MS::kSuccess);
}

void particleStrokeNode::postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
