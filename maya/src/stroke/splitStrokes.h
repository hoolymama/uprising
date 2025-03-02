#ifndef _splitStrokes_H
#define _splitStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief A node for splitting strokes into multiple segments at specified parameters.
 * 
 * The splitStrokes class extends strokeMutator to provide functionality for splitting
 * strokes into multiple segments at specified parameter values. This allows for
 * creating discontinuities in strokes or breaking them into smaller, more manageable
 * pieces while preserving their overall appearance.
 */
class splitStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  splitStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~splitStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new splitStrokes object.
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
   * @brief Splits the input strokes at specified parameters.
   * @param plug The plug being computed.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to be modified.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus mutate(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /** @brief Stroke IDs attribute for specifying which strokes to split. */
  static MObject aStrokeIds;
  
  /** @brief Split counts attribute for specifying how many splits to make per stroke. */
  static MObject aSplitCounts;
  
  /** @brief Split parameters attribute for specifying where to split each stroke. */
  static MObject aSplitParams;
  
  /**
   * @brief Splits a single stroke at the specified parameters.
   * @param splitParams Array of parameter values where the stroke should be split.
   * @param stroke Pointer to the stroke to split.
   * @param strokes Pointer to the vector where the resulting split strokes will be stored.
   */
  void splitStroke(
    const MDoubleArray &splitParams,
    const Stroke *stroke,
    std::vector<Stroke> *strokes) const;

  // static MObject aPoint; ///> The point to aim towards. 
};

#endif