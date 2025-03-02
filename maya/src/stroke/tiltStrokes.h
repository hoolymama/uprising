/**
 * @file tiltStrokes.h
 * @brief Defines the tiltStrokes class for applying tilt transformations to strokes.
 * 
 * This file contains the tiltStrokes class which extends strokeMutator to provide
 * functionality for applying tilt transformations to strokes based on gradient textures.
 * It allows for modifying the orientation of targets within strokes to create
 * tilt effects based on image data.
 */

#ifndef _tiltStrokes_H
#define _tiltStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @class tiltStrokes
 * @brief A node for applying tilt transformations to strokes based on gradient textures.
 * 
 * The tiltStrokes class extends strokeMutator to create a specialized node that
 * applies tilt transformations to strokes based on gradient textures. It samples
 * a gradient texture at target positions and uses the gradient values to determine
 * the tilt angle and direction for each target.
 */
class tiltStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  tiltStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~tiltStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new tiltStrokes object.
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
   * @brief Applies tilt transformations to the input strokes.
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
  /** @brief Canvas matrix attribute for defining the sampling space. */
  static MObject aCanvasMatrix;
  
  /** @brief Gradient texture attribute for determining tilt values. */
  static MObject aGradientTexture;
  
  /** @brief Sample distance attribute for gradient sampling. */
  static MObject aSampleDistance;
  
  /** @brief Strength attribute for controlling the intensity of the tilt effect. */
  static MObject aStrength;
};

#endif