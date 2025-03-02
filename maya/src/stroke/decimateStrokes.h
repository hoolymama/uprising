#ifndef _decimateStrokes_H
#define _decimateStrokes_H

#include <maya/MRampAttribute.h>
#include "strokeMutator.h"
#include "stroke.h"

/**
 * @brief A node for decimating strokes based on density maps.
 *
 * The decimateStrokes class extends strokeMutator to provide functionality for
 * selectively removing or thinning strokes based on density maps. It samples
 * density values from a texture map at stroke positions and uses these values
 * to determine which strokes or portions of strokes to remove. This allows for
 * creating variations in stroke density across a canvas.
 */
class decimateStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  decimateStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~decimateStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new decimateStrokes object.
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
   * @brief Decimates the input strokes based on density maps.
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
   * @brief Gets density values from a texture map at the specified points.
   * @param data The data block containing the node's data.
   * @param points The points to sample the texture at.
   * @param densities Output array to store the sampled density values.
   */
  void getDensities(
      MDataBlock &data,
      MFloatPointArray &points,
      MFloatArray &densities) const;

  /**
   * @brief Removes spans from strokes based on a span mask.
   * @param strokes Pointer to the strokes to modify.
   * @param spanMask Array indicating which spans to remove (0) or keep (1).
   */
  void removeSpans(
      std::vector<Stroke> *strokes,
      const MIntArray &spanMask) const;

  /**
   * @brief Applies dithering to density values to create a span mask.
   * @param smooth Smoothing factor for the dithering.
   * @param densities Array of density values to dither.
   * @param spanMask Output array to store the resulting span mask.
   */
  void dither(int smooth, const MFloatArray &densities, MIntArray &spanMask) const;

  /** @brief Density texture attribute for determining which strokes to remove. */
  static MObject aDensity;
  
  /** @brief Seed attribute for random number generation. */
  static MObject aSeed;
  
  /** @brief Smooth attribute for controlling the smoothness of the decimation. */
  static MObject aSmooth;
};

#endif