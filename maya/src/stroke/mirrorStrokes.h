#ifndef _mirrorStrokes_H
#define _mirrorStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief A node for mirroring strokes across a plane.
 *
 * The mirrorStrokes class extends strokeMutator to provide functionality for
 * mirroring strokes across a specified plane. It allows for creating symmetrical
 * stroke patterns by reflecting existing strokes, with options to translate
 * the strokes before or after mirroring.
 */
class mirrorStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  mirrorStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~mirrorStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new mirrorStrokes object.
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
   * @brief Mirrors the input strokes across the specified plane.
   * @param plug The plug being computed.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to be modified.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus mutate(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /** @brief Translation vector attribute for positioning strokes before or after mirroring. */
  static MObject aTranslate;
  
  /** @brief Mirror plane attribute defining the plane to mirror across. */
  static MObject aMirror;

  // static MObject aPoint; ///> The point to aim towards.
};

#endif
