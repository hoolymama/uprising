/**
 * @file cImgNoise.h
 * @brief Maya node for generating procedural noise.
 *
 * This file implements a Maya node for noise generation:
 * - Generates procedural noise using Simplex noise
 * - Supports multiple octaves for fractal noise
 * - Configurable frequency, amplitude, and persistence
 * - Optional blur for smoothing
 */

#ifndef _cImgNoise
#define _cImgNoise

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgData.h"

/**
 * @class cImgNoise
 * @brief Maya node for generating procedural noise.
 *
 * cImgNoise provides noise generation functionality:
 * - Simplex noise-based generation
 * - Multi-octave fractal noise support
 * - Configurable noise parameters
 * - Optional blur post-processing
 */
class cImgNoise : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgNoise();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgNoise();

	/**
	 * @brief Computes the noise output.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgNoise instance
	 */
	static void* creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	/** @brief Unique Maya type ID for the node */
	static MTypeId id;

private:
	/** @brief Output resolution */
	static MObject aResolution;

	/** @brief Base frequency of noise */
	static MObject aFrequency;
	/** @brief Base amplitude of noise */
	static MObject aAmplitude;
	/** @brief Frequency multiplier between octaves */
	static MObject aLacunarity;
	/** @brief Amplitude multiplier between octaves */
	static MObject aPersistence;
	/** @brief Number of octaves to generate */
	static MObject aOctaves;

	/** @brief Amount of blur to apply */
	static MObject aBlur;
	/** @brief Output noise image */
	static MObject aOutput;
};

#endif
