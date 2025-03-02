#ifndef _mapColorStrokes_H
#define _mapColorStrokes_H

#include <maya/MRampAttribute.h>
#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief A node for mapping colors to strokes based on texture maps.
 * 
 * The mapColorStrokes class extends strokeMutator to provide functionality for
 * mapping colors to strokes based on texture maps. It samples RGB and white
 * values from textures at stroke positions and applies these colors to the
 * targets within the strokes. It also supports occlusion testing against a mesh,
 * which can set occluded targets to black.
 */
class mapColorStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  mapColorStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~mapColorStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new mapColorStrokes object.
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
   * @brief Maps colors to the input strokes based on texture maps.
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
   * @brief Gets colors from texture maps at the specified points.
   * @param data The data block containing the node's data.
   * @param points The points to sample the textures at.
   * @param colors Output array to store the sampled RGB colors.
   * @param whites Output array to store the sampled white values.
   */
  void getColors(
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatVectorArray &colors,
    MFloatArray &whites
     ) const;

  /**
   * @brief Gets wait times from a texture map at the specified points.
   * @param data The data block containing the node's data.
   * @param points The points to sample the texture at.
   * @param waits Output array to store the sampled wait times.
   * @return True if wait times were successfully retrieved, false otherwise.
   */
  bool getWaits(
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatArray &waits) const;

  /**
   * @brief Performs occlusion testing against a mesh and modifies colors accordingly.
   * @param data The data block containing the node's data.
   * @param points The points to test for occlusion.
   * @param colors Output array of colors to modify based on occlusion.
   * @param whites Output array of white values to modify based on occlusion.
   * @return MStatus indicating success or failure.
   */
  MStatus occludeColors(
    MDataBlock &data,
    const MFloatPointArray &points,
    MFloatVectorArray &colors, 
    MFloatArray &whites
    ) const;

  /**
   * @brief Applies colors to the targets within strokes.
   * @param geom Pointer to the strokes to apply colors to.
   * @param colors Array of RGB colors to apply.
   * @param whites Array of white values to apply.
   * @param rgbBlend Blend factor for RGB colors (0.0 to 1.0).
   * @param whiteBlend Blend factor for white values (0.0 to 1.0).
   */
  void applyColors(   
    std::vector<Stroke> *geom,
    const MFloatVectorArray &colors,
    const MFloatArray &whites,
    float rgbBlend=1.0,
    float whiteBlend=1.0) const;
 
  /**
   * @brief Flattens stroke colors into arrays.
   * @param strokes Pointer to the strokes to flatten colors from.
   * @param strokeColors Output array to store the flattened RGB colors.
   * @param strokeWhites Output array to store the flattened white values.
   * @param colors Output array to store all flattened RGB colors.
   * @param whites Output array to store all flattened white values.
   * @return MStatus indicating success or failure.
   */
  MStatus flattenStrokeColors(
    const std::vector<Stroke> *strokes,
    const MFloatVectorArray &strokeColors, 
    const MFloatArray &strokeWhites, 
    MFloatVectorArray &colors, 
    MFloatArray &whites) const;

  /**
   * @brief Applies wait times to the targets within strokes.
   * @param strokes Pointer to the strokes to apply wait times to.
   * @param waits Array of wait times to apply.
   * @param rampAttr Ramp attribute for remapping wait times.
   * @param startEndAngle Angle for start and end wait time adjustment.
   */
  void applyWaits(
    std::vector<Stroke> *strokes,
    const MFloatArray &waits,
    MRampAttribute & rampAttr,
    float startEndAngle
    ) const;

  /**
   * @brief Removes black spans from strokes.
   * @param strokes Pointer to the strokes to process.
   * @param thresh Threshold for determining black colors.
   */
  void removeBlackSpans(std::vector<Stroke> *strokes, float thresh) const;

  /** @brief RGB texture attribute for mapping RGB colors. */
  static MObject aRGB;
  
  /** @brief White texture attribute for mapping white values. */
  static MObject aWhite;
  
  /** @brief Wait texture attribute for mapping wait times. */
  static MObject aWait;

  /** @brief RGB blend factor attribute. */
  static MObject aRgbBlend;
  
  /** @brief White blend factor attribute. */
  static MObject aWhiteBlend;

  /** @brief Angle wait remap attribute. */
  static MObject aAngleWaitRemap;
  
  /** @brief Start/end angle attribute. */
  static MObject aStartEndAngle;
  
  /** @brief Flag to sample only at pivot points. */
  static MObject aPivotSampleOnly;

  /** @brief Occlusion mesh attribute. */
  static MObject aMesh;
  
  /** @brief Occlusion test point attribute. */
  static MObject aPoint;
  
  /** @brief Occlusion ray test bias attribute. */
  static MObject aBias;
  
  /** @brief Flag to enable occlusion testing. */
  static MObject aDoOcclusion;
  
  /** @brief Flag to enable removal of black spans. */
  static MObject aRemoveBlackSpans;
  
  /** @brief Threshold for black span removal. */
  static MObject aBlackSpanThreshold;
};

#endif