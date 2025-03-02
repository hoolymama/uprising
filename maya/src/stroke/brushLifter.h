#ifndef _brushLifter_H
#define _brushLifter_H

#include "strokeMutator.h"
#include "stroke.h"
#include "brushRack.h"
#include "brushShop.h"

/**
 * @brief A node for applying brush properties and transformations to strokes.
 *
 * The brushLifter class extends strokeMutator to provide functionality for
 * applying brush properties and transformations to strokes. It can reassign
 * brush IDs, apply lift transformations, and apply bias to strokes based on
 * brush properties. This allows for creating variations in stroke appearance
 * by changing the brush properties and orientation.
 */
class brushLifter : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  brushLifter();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~brushLifter();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new brushLifter object.
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
   * @brief Applies brush properties and transformations to the input strokes.
   * @param plug The plug being computed.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to be modified.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus mutate(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

  /**
   * @enum ApplyBias
   * @brief Defines options for applying bias to strokes.
   */
  enum ApplyBias
  {
    kOff,             /**< Do not apply bias. */
    kBeforeRotation,  /**< Apply bias before rotation. */
    kAfterRotation    /**< Apply bias after rotation. */
  };

private:
  /** @brief Flag to enable brush ID reassignment. */
  static MObject aReassignBrushIds;
  
  /** @brief Flag to enable lift transformation. */
  static MObject aApplyLift;
  
  /** @brief Option for applying bias. */
  static MObject aApplyBias;

  /** @brief Brush shop attribute for accessing brush definitions. */
  static MObject aBrushShop;

  /**
   * @brief Assigns brushes to strokes based on their brush IDs.
   * @param brushShop The brush shop containing brush definitions.
   * @param strokes Pointer to the strokes to assign brushes to.
   */
  void assignBrushes(BrushShop &brushShop, std::vector<Stroke> *strokes) const;

  /**
   * @brief Gets the brush shop from the node's attributes.
   * @param data The data block containing the node's data.
   * @param brushShop Output parameter to store the brush shop.
   * @return MStatus indicating success or failure.
   */
  MStatus getBrushShop(MDataBlock &data, BrushShop &brushShop) const;
  
  /**
   * @brief Collects brushes from the node's attributes.
   * @param data The data block containing the node's data.
   * @param brushes Output map to store the collected brushes.
   * @return MStatus indicating success or failure.
   */
  MStatus collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const;
  
  /**
   * @brief Gets tangents from a curve object.
   * @param curveObject The curve object to get tangents from.
   * @param tangents Output array to store the tangents.
   */
  void getTangents(const MObject &curveObject, MVectorArray &tangents) const;
  
  /**
   * @brief Sets weights for a stroke based on brush properties.
   * @param brush The brush to use for weight calculation.
   * @param curveObject The curve object representing the stroke path.
   * @param stroke Pointer to the stroke to set weights for.
   */
  void setWeights(const Brush &brush, const MObject &curveObject, Stroke *stroke) const;
  
  /**
   * @brief Applies bias to a stroke based on brush properties.
   * @param brush The brush to use for bias calculation.
   * @param curveObject The curve object representing the stroke path.
   * @param stroke Pointer to the stroke to apply bias to.
   * @param tangents Array of tangent vectors along the stroke.
   */
  void applyBias(const Brush &brush, const MObject &curveObject, Stroke *stroke, MVectorArray &tangents) const;

  /**
   * @brief Applies rotation to a stroke based on brush properties.
   * @param brush The brush to use for rotation calculation.
   * @param curveObject The curve object representing the stroke path.
   * @param tangents Array of tangent vectors along the stroke.
   * @param stroke Pointer to the stroke to apply rotation to.
   */
  void applyRotation(const Brush &brush, const MObject &curveObject, const MVectorArray &tangents, Stroke *stroke) const;
  
  /**
   * @brief Applies lift transformation to a stroke based on brush properties.
   * @param brush The brush to use for lift calculation.
   * @param stroke Pointer to the stroke to apply lift to.
   */
  void applyLift(const Brush &brush, Stroke *stroke) const;
};

#endif