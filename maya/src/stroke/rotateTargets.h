/**
 * @file rotateTargets.h
 * @brief Defines the rotateTargets class for applying rotation transformations to stroke targets.
 * 
 * This file contains the rotateTargets class which extends strokeMutator to provide
 * functionality for applying rotation transformations to targets within strokes.
 * It allows for creating sinusoidal or random rotation effects along various axes.
 */

#ifndef _rotateTargets_H
#define _rotateTargets_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @class rotateTargets
 * @brief A node for applying rotation transformations to targets within strokes.
 * 
 * The rotateTargets class extends strokeMutator to create a specialized node that
 * applies rotation transformations to targets within strokes. It can create
 * sinusoidal rotation patterns with configurable frequency and amplitude,
 * and can apply these rotations along different axes.
 */
class rotateTargets : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  rotateTargets();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~rotateTargets();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new rotateTargets object.
   */
  static void *creator();
  
  /**
   * @brief Initialize attributes and connections for the node.
   * @return MStatus indicating success or failure.
   */
  static MStatus initialize();
  
  /**
   * @brief Post-constructor initialization.
   */
  virtual void postConstructor();
  
  /**
   * @brief Indicates whether this class is abstract.
   * @return False, as this is a concrete implementation.
   */
  virtual bool isAbstractClass() const { return false; }
  
  /**
   * @brief Applies rotation transformations to the targets in the input strokes.
   * @param plug The plug being computed.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to be modified.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus mutate(
    const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;
  
  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /** @brief Canvas matrix attribute for defining the rotation space. */
  static MObject aCanvasMatrix;

  /** @brief Sine frequency attribute for controlling the frequency of sinusoidal rotations. */
  static MObject aSineFrequency;
  
  /** @brief Sine amplitude attribute for controlling the amplitude of sinusoidal rotations. */
  static MObject aSineAmplitude;
  
  /** @brief Random sine offset flag attribute for adding randomness to the sine pattern. */
  static MObject aRandomSineOffset;
  
  /** @brief Seed attribute for controlling random number generation. */
  static MObject aSeed;
  
  /** @brief Axis attribute for specifying which axis to rotate around. */
  static MObject aAxis;
  
  /**
   * @enum RotationAxis
   * @brief Defines axes for applying rotations.
   */
  enum RotationAxis
  {
    kCanvas, /**< Rotate around the canvas normal axis. */
    kTilt,   /**< Rotate around the tilt axis. */
    kTwist,  /**< Rotate around the twist axis. */
    kBank,   /**< Rotate around the bank axis. */
  };
};

#endif