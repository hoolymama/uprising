#ifndef _scribbleStrokes_H
#define _scribbleStrokes_H
#include <vector>

#include <maya/MRampAttribute.h>

#include "strokeMutator.h"
#include "stroke.h"
#include "target.h"

/**
 * @brief A node for creating scribble effects on strokes by applying transformations.
 *
 * The scribbleStrokes class extends strokeMutator to provide functionality for
 * creating scribble-like effects on strokes. It applies various transformations
 * to the targets within strokes, including tilt, bank, and twist, based on
 * texture maps and other parameters. This allows for creating natural-looking
 * scribble effects with varying pressure and orientation.
 */
class scribbleStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  scribbleStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~scribbleStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new scribbleStrokes object.
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
   * @brief Applies scribble transformations to the input strokes.
   * @param plug The plug being computed.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to be modified.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus mutate(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *strokes) const;

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /**
   * @enum ColorPropagation
   * @brief Defines methods for propagating colors in scribble strokes.
   */
  enum ColorPropagation
  {
    kOverride,    /**< Override existing colors. */
    kInterpolate  /**< Interpolate between existing colors. */
  };

  /**
   * @brief Gets mapped vector values from a texture attribute.
   * @param attribute The texture attribute to sample.
   * @param data The data block containing the node's data.
   * @param points The points to sample the texture at.
   * @param result Output array to store the mapped vector values.
   */
  void getMappedValues(
      const MObject &attribute,
      MDataBlock &data,
      MFloatPointArray &points,
      MFloatVectorArray &result) const;

  /**
   * @brief Gets mapped float values from a texture attribute.
   * @param attribute The texture attribute to sample.
   * @param data The data block containing the node's data.
   * @param points The points to sample the texture at.
   * @param result Output array to store the mapped float values.
   */
  void getMappedValues(
      const MObject &attribute,
      MDataBlock &data,
      MFloatPointArray &points,
      MFloatArray &result) const;

  /**
   * @brief Gets mapped vector values from a texture attribute with scaling.
   * @param attribute The texture attribute to sample.
   * @param data The data block containing the node's data.
   * @param points The points to sample the texture at.
   * @param scale Scale factor to apply to the mapped values.
   * @param result Output array to store the mapped vector values.
   */
  void getMappedValues(
      const MObject &attribute,
      MDataBlock &data,
      MFloatPointArray &points,
      float scale,
      MFloatVectorArray &result) const;

  /**
   * @brief Gets mapped float values from a texture attribute with scaling.
   * @param attribute The texture attribute to sample.
   * @param data The data block containing the node's data.
   * @param points The points to sample the texture at.
   * @param scale Scale factor to apply to the mapped values.
   * @param result Output array to store the mapped float values.
   */
  void getMappedValues(
      const MObject &attribute,
      MDataBlock &data,
      MFloatPointArray &points,
      float scale,
      MFloatArray &result) const;

  /**
   * @brief Calculates scribble transformation matrices for a stroke.
   * @param stroke The stroke to calculate transformations for.
   * @param mapIndex Index for mapping values.
   * @param tilts Array of tilt angles.
   * @param banks Array of bank angles.
   * @param twists Array of twist angles.
   * @param order Rotation order for applying the transformations.
   * @param scribbleTransforms Output vector to store the calculated transformation matrices.
   */
  void calculateScribbleTransforms(
      const Stroke &stroke,
      int mapIndex,
      const MFloatArray &tilts,
      const MFloatArray &banks,
      const MFloatArray &twists,
      PaintingEnums::BrushRotateOrder order,
      std::vector<MFloatMatrix> &scribbleTransforms) const;

  /**
   * @brief Gets a follow matrix for a target.
   * @param target The target to get the follow matrix for.
   * @param mat Output parameter to store the follow matrix.
   */
  void getFollowMatrix(const Target &target, MFloatMatrix &mat) const;

  /**
   * @brief Applies a tilt transformation to a matrix.
   * @param angle The tilt angle in radians.
   * @param mat The matrix to apply the tilt to.
   */
  void applyTilt(float angle, MFloatMatrix &mat) const;
  
  /**
   * @brief Applies a bank transformation to a matrix.
   * @param angle The bank angle in radians.
   * @param mat The matrix to apply the bank to.
   */
  void applyBank(float angle, MFloatMatrix &mat) const;
  
  /**
   * @brief Applies a twist transformation to a matrix.
   * @param angle The twist angle in radians.
   * @param mat The matrix to apply the twist to.
   */
  void applyTwist(float angle, MFloatMatrix &mat) const;

  /** @brief Target rotation matrix attribute. */
  static MObject aTargetRotationMatrix;
  /** @brief Point density attribute. */
  static MObject aPointDensity;
  /** @brief Minimum points attribute. */
  static MObject aMinimumPoints;
  /** @brief Angle attribute. */
  static MObject aAngle;
  /** @brief Radius gain attribute. */
  static MObject aRadiusGain;
  /** @brief Radius offset attribute. */
  static MObject aRadiusOffset;

  /** @brief Tilt attribute. */
  static MObject aTilt;
  /** @brief Bank attribute. */
  static MObject aBank;
  /** @brief Twist attribute. */
  static MObject aTwist;

  /** @brief Project to plane flag attribute. */
  static MObject aProjectToPlane;
  /** @brief Projection plane matrix attribute. */
  static MObject aProjectionPlaneMatrix;
  
  /** @brief Tilt map attribute. */
  static MObject aTiltMap;
  /** @brief Bank map attribute. */
  static MObject aBankMap;
  /** @brief Twist map attribute. */
  static MObject aTwistMap;
  /** @brief Rotate order attribute. */
  static MObject aRotateOrder;

  /** @brief Color propagation attribute. */
  static MObject aColorPropagation;
  /** @brief Color override attribute. */
  static MObject aColorOverride;
  /** @brief White override attribute. */
  static MObject aWhiteOverride;
  /** @brief Weight override attribute. */
  static MObject aWeightOverride;
};

#endif