/**
 * @file tumbler.h
 * @brief Defines the tumbler class for simulating rolling particles with orientation.
 * 
 * This file contains the tumbler class which extends Maya's MPxNode to provide
 * functionality for simulating rolling particles with orientation. It calculates
 * torques based on sampling force gradients near particles and updates their
 * orientation accordingly. Particles can also have goal orientations that they
 * strive to match.
 * 
 * Created by Julian Mann on 02/12/2006.
 * Copyright 2006 hooly|mama. All rights reserved.
 */

/*
 *  tumbler.h
 *  jtools
 *
 *  Created by Julian Mann on 02/12/2006.
 *  Copyright 2006 hooly|mama. All rights reserved.
 *
 */
// tumbler - simulation of rolling particles
// torques are calculated for the particle based on sampling the gradient of forces near the particle.
// The resulting rotation is sent back to the particle object.

// Particles can also have a goal orientation, and will strive to
// match that orientation by the shortest angular path according to goal weights
//
#ifndef _tumbler
#define _tumbler

#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MEulerRotation.h>
#include <maya/MTime.h>
#include "mayaMath.h"
#include <maya/MTypeId.h>

/** @brief Rotation order constant for XYZ rotation. */
#define ROTATE_ORDER_XYZ 0
/** @brief Rotation order constant for YZX rotation. */
#define ROTATE_ORDER_YZX 1
/** @brief Rotation order constant for ZXY rotation. */
#define ROTATE_ORDER_ZXY 2
/** @brief Rotation order constant for XZY rotation. */
#define ROTATE_ORDER_XZY 3
/** @brief Rotation order constant for YXZ rotation. */
#define ROTATE_ORDER_YXZ 4
/** @brief Rotation order constant for ZYX rotation. */
#define ROTATE_ORDER_ZYX 5

/** @brief Conversion factor from radians to degrees. */
#define ROT_TO_DEG 57.295779524

/**
 * @class tumbler
 * @brief A node for simulating rolling particles with orientation.
 * 
 * The tumbler class extends MPxNode to create a specialized node that
 * simulates rolling particles with orientation. It calculates torques based on
 * sampling force gradients near particles and updates their orientation accordingly.
 * Particles can also have goal orientations that they strive to match by the
 * shortest angular path according to goal weights.
 */
class tumbler : public MPxNode
{

public:
	/**
	 * @brief Default constructor.
	 */
	tumbler();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~tumbler();

	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new tumbler object.
	 */
	static void *creator();
	
	/**
	 * @brief Initialize attributes and connections for the node.
	 * @return MStatus indicating success or failure.
	 */
	static MStatus initialize();

	/**
	 * @brief Compute method for calculating the node's outputs.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	/** @brief Unique type ID for this node type. */
	static MTypeId id;

	// inputs
	/** @brief Position attribute. */
	static MObject aPosition;
	/** @brief Velocity attribute. */
	static MObject aVelocity;
	/** @brief Mass attribute. */
	static MObject aMass;
	/** @brief Impulse attribute. */
	static MObject aImpulse;
	/** @brief Local impulse attribute. */
	static MObject aLocalImpulse;

	/** @brief Front axis attribute. */
	static MObject aFrontAxis;
	/** @brief Up axis attribute. */
	static MObject aUpAxis;

	/** @brief Goal front direction attribute. */
	static MObject aGoalFront;
	/** @brief Goal up direction attribute. */
	static MObject aGoalUp;
	/** @brief Goal weight attribute. */
	static MObject aGoalWeight;
	/** @brief Per-point goal weight attribute. */
	static MObject aGoalWeightPP;

	/** @brief Current rotation attribute. */
	static MObject aPhi;
	/** @brief Angular velocity attribute. */
	static MObject aOmega;

	/** @brief Torque conservation attribute. */
	static MObject aTorqueConserve;
	/** @brief Rotation order attribute. */
	static MObject aRotateOrder;
	/** @brief Output unit attribute. */
	static MObject aOutputUnit;

	/** @brief Output rotation attribute. */
	static MObject aOutPhi;
	/** @brief Output points attribute. */
	static MObject aOutPoints;
	/** @brief Output angular velocity attribute. */
	static MObject aOutOmega;
	/** @brief Output front direction attribute. */
	static MObject aOutFront;
	/** @brief Output up direction attribute. */
	static MObject aOutUp;
	/** @brief Output rotation as Euler angles attribute. */
	static MObject aOutRotation;

	/** @brief Current time attribute. */
	static MObject aCurrentTime;

	/** @brief Output rotation data attribute. */
	static MObject aOutRotationData;

private:
	/** @brief Last time the node was evaluated. */
	MTime m_lastTimeIEvaluated;

	/**
	 * @brief Gets the forces applied to the particles.
	 * @param block The data block containing the node's data.
	 * @param positions Array of particle positions.
	 * @param velocities Array of particle velocities.
	 * @param densities Array of particle densities.
	 * @param dT Time delta since last evaluation.
	 * @param appliedForce Output array to store the calculated forces.
	 * @return MStatus indicating success or failure.
	 */
	MStatus getAppliedForces(
		MDataBlock &block,
		const MVectorArray &positions,
		const MVectorArray &velocities,
		const MDoubleArray &densities,
		const MTime &dT,
		MVectorArray &appliedForce);

	/**
	 * @brief Performs a simulation step.
	 * @param dt Time delta for the simulation step.
	 * @param data The data block containing the node's data.
	 * @param phi Array of current rotations.
	 * @param omega Array of current angular velocities.
	 * @param goalFront Array of goal front directions.
	 * @param goalUp Array of goal up directions.
	 * @param goalWeightPP Array of per-point goal weights.
	 * @param impulse Array of impulses.
	 * @param points Array of particle positions.
	 * @param velocities Array of particle velocities.
	 * @param outPhi Output array for updated rotations.
	 * @param outOmega Output array for updated angular velocities.
	 * @param outFront Output array for updated front directions.
	 * @param outUp Output array for updated up directions.
	 * @param outRotation Output array for updated rotations as Euler angles.
	 * @return MStatus indicating success or failure.
	 */
	MStatus simStep(
		double dt,
		MDataBlock &data,
		const MVectorArray &phi,
		const MVectorArray &omega,
		const MVectorArray &goalFront,
		const MVectorArray &goalUp,
		const MDoubleArray &goalWeightPP,
		const MVectorArray &impulse,
		const MVectorArray &points,
		const MVectorArray &velocities,
		MVectorArray &outPhi,
		MVectorArray &outOmega,
		MVectorArray &outFront,
		MVectorArray &outUp,
		MVectorArray &outRotation);
};

#endif
