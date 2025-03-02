/**
 * @file particleStrokeNode.h
 * @brief Defines the particleStrokeNode class for generating strokes from particle trails.
 * 
 * This file contains the particleStrokeNode class which provides functionality for
 * generating strokes from particle trails in Maya. It extends the strokeCreator class
 * to create strokes based on particle movement and properties.
 */

#ifndef _particleStrokeNode_H
#define _particleStrokeNode_H

#include <maya/MObject.h>
#include <maya/MVectorArray.h>
#include <strokeCreator.h>
#include "stroke.h"
 
/**
 * @class particleStrokeNode
 * @brief A node for generating strokes from particle trails.
 * 
 * The particleStrokeNode class extends strokeCreator to create a specialized node that
 * generates strokes from particle trails in Maya. It converts particle trail data into
 * stroke geometry that can be used in the painting system.
 */
class particleStrokeNode : public strokeCreator
{

  /**
   * @brief Default constructor.
   */
  particleStrokeNode();
public:

  /**
   * @brief Virtual destructor.
   */
  virtual ~particleStrokeNode();
  
  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new particleStrokeNode object.
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
  
  /**
   * @brief Indicates whether this class is abstract.
   * @return False, as this class can be instantiated.
   */
  virtual bool isAbstractClass() const { return false; }
  
  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /**
   * @brief Generates stroke geometry from particle trail data.
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @param pStrokes Pointer to a vector to store the generated strokes.
   * @return MStatus indicating success or failure.
   */
  MStatus generateStrokeGeometry(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

  /** @brief Target rotation matrix attribute. */
  static MObject aTargetRotationMatrix;
  
  /** @brief Trails attribute. */
  static MObject aTrails;
};

#endif
