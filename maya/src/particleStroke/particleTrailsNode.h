/**
 * @file particleTrailsNode.h
 * @brief Defines the particleTrailsNode class for generating particle trails.
 * 
 * This file contains the particleTrailsNode class which provides functionality for
 * generating and managing particle trails in Maya. It tracks particle positions and
 * colors over time to create trail effects.
 */

#ifndef _particleTrailsNode_H
#define _particleTrailsNode_H

#include <vector>

#include <maya/MPxNode.h>

#include <maya/MDataBlock.h>
#include <maya/MTime.h>
#include <maya/MDataBlock.h>
#include <maya/MDataBlock.h>
#include "particleTrail.h"

/**
 * @class particleTrailsNode
 * @brief A node for generating and managing particle trails.
 * 
 * The particleTrailsNode class extends MPxNode to create a specialized node that
 * generates and manages particle trails in Maya. It tracks particle positions and
 * colors over time, maintaining a cache of trail data that can be used for
 * visualization or further processing.
 */
class particleTrailsNode : public MPxNode
{

public:
    /**
     * @brief Default constructor.
     */
    particleTrailsNode();

    /**
     * @brief Virtual destructor.
     */
    virtual ~particleTrailsNode();
    
    /**
     * @brief Creator function for Maya's plugin system.
     * @return Pointer to a new particleTrailsNode object.
     */
    static void *creator();
    
    /**
     * @brief Initialize attributes and connections for the node.
     * @return MStatus indicating success or failure.
     */
    static MStatus initialize();
    
    /**
     * @brief Post-constructor setup for the node.
     */
    virtual void postConstructor();
    
    /** @brief Unique type ID for this node type. */
    static MTypeId id;
    
    /**
     * @brief Compute method for calculating the node's outputs.
     * @param plug The plug to compute.
     * @param data The data block containing the node's data.
     * @return MStatus indicating success or failure.
     */
    virtual MStatus compute(const MPlug &plug, MDataBlock &data);

private:
    /** @brief Current time attribute. */
    static MObject aCurrentTime;
    
    /** @brief Start time attribute. */
    static MObject aStartTime;
    
    /** @brief Particle IDs attribute. */
    static MObject aParticleIds;
    
    /** @brief Positions attribute. */
    static MObject aPositions;
    
    /** @brief Colors attribute. */
    static MObject aColors;
    
    /** @brief Whites attribute. */
    static MObject aWhites;

    /** @brief Output attribute. */
    static MObject aOutput;  

    /** @brief Last time the node was evaluated. */
    MTime m_lastTimeIEvaluated;
    
    /** @brief Cache of particle trails. */
    std::vector<particleTrail> m_particleTrails;
    
    /**
     * @brief Gets a time value from the data block.
     * @param data The data block to get the time from.
     * @param attribute The time attribute to get.
     * @return The time value.
     */
    MTime timeValue(MDataBlock &data, MObject &attribute);

    /**
     * @brief Adds current particle data to the cache.
     * @param data The data block containing the particle data.
     * @return MStatus indicating success or failure.
     */
    MStatus addToCache(MDataBlock &data);
};

/**
 * @brief Gets a time value from the data block.
 * @param data The data block to get the time from.
 * @param attribute The time attribute to get.
 * @return The time value.
 */
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
