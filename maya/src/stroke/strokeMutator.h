/**
 * @file strokeMutator.h
 * @brief Defines the strokeMutator class for modifying existing strokes in Maya.
 * 
 * This file contains the strokeMutator class which extends strokeNodeBase to provide
 * base functionality for nodes that modify existing strokes. It adds an input attribute
 * for receiving strokes and provides a framework for derived classes to implement
 * specific stroke mutation operations.
 */

#ifndef _strokeMutator_H
#define _strokeMutator_H

#include <strokeNodeBase.h>
#include <maya/MPxNode.h>
#include "stroke.h"

/**
 * @brief A node that takes input strokes, mutates them, and puts them on the
 * output. 
 *
 * It inherits from strokeNodeBase, which has no input strokes, and it adds the
 * input attribute (aStrokes). This class serves as a base class for all nodes
 * that modify existing strokes rather than creating new ones.
 */
class strokeMutator : public strokeNodeBase
{
public:
  /**
   * @brief Default constructor.
   */
  strokeMutator();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~strokeMutator();
  
  /**
   * @brief Indicates whether this class is abstract.
   * @return True, as this is an abstract base class.
   */
  virtual bool isAbstractClass() const { return true; }

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new strokeMutator object.
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
   * @brief Generates stroke geometry based on the node's attributes.
   * 
   * This implementation retrieves input strokes and passes them to the mutate method
   * for modification by derived classes.
   * 
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @param pStrokes Pointer to a vector to store the generated strokes.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus generateStrokeGeometry(
    const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *pStrokes) ;

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

protected:

  /**
   * @brief Derived classes should implement this and change the given strokes. 
   * 
   * This is the main method that derived classes should override to implement
   * their specific stroke mutation logic.
   * 
   * @param plug The plug being computed.
   * @param data The node's data block.
   * @param strokes A mutable vector of strokes to be modified.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus mutate(
    const   MPlug &plug, 
    MDataBlock &data,
    std::vector<Stroke> *strokes) const;

  /** @brief Input strokes attribute. */
  static MObject aStrokes;
  
  /** @brief A multi attribute that can be used to trigger the compute. */
  static MObject aTrigger;
};

#endif