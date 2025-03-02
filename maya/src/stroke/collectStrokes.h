/**
 * @file collectStrokes.h
 * @brief Defines the collectStrokes class for combining multiple stroke inputs.
 * 
 * This file contains the collectStrokes class which extends strokeNodeBase to provide
 * functionality for collecting and combining strokes from multiple inputs. It allows
 * for creating complex stroke compositions by merging strokes from different sources.
 */

#ifndef _collectStrokes_H
#define _collectStrokes_H

#include <maya/MVector.h>
 
#include <maya/MPxNode.h>

#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
 
#include "stroke.h"

#include "strokeNodeBase.h"

/**
 * @class collectStrokes
 * @brief A node for collecting and combining strokes from multiple inputs.
 * 
 * The collectStrokes class extends strokeNodeBase to create a specialized node that
 * collects and combines strokes from multiple inputs. It provides a way to merge
 * strokes from different sources into a single output, which can be useful for
 * creating complex stroke compositions or for organizing stroke processing pipelines.
 */
class collectStrokes : public strokeNodeBase
{
public:
  /**
   * @brief Default constructor.
   */
  collectStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~collectStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new collectStrokes object.
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
   * @brief Generates stroke geometry by collecting strokes from inputs.
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @param pStrokes Pointer to a vector to store the collected strokes.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus generateStrokeGeometry(
    const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /** @brief Input strokes attribute (multi). */
  static MObject aStrokes;
};

#endif