/**
 * @file bulgeStrokes.h
 * @brief Defines the bulgeStrokes class for creating bulge effects on strokes.
 * 
 * This file contains the bulgeStrokes class which extends strokeMutator to provide
 * functionality for creating bulge effects on strokes. It allows for modifying the
 * radius or weight of targets within strokes based on ramp values, creating
 * variations in stroke thickness.
 */

#ifndef _bulgeStrokes_H
#define _bulgeStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @class bulgeStrokes
 * @brief A node for creating bulge effects on strokes.
 * 
 * The bulgeStrokes class extends strokeMutator to create a specialized node that
 * applies bulge effects to strokes. It modifies the radius or weight of targets
 * within strokes based on ramp values, allowing for creating variations in stroke
 * thickness along the length of the stroke.
 */
class bulgeStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  bulgeStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~bulgeStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new bulgeStrokes object.
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
   * @brief Applies bulge effects to the input strokes.
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
  /** @brief Multiplier attribute for controlling the intensity of the bulge effect. */
  static MObject aMult;
  
  /** @brief Ramp attribute for defining the bulge profile along the stroke. */
  static MObject aRamp;
};

#endif