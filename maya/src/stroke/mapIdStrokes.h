#ifndef _mapIdStrokes_H
#define _mapIdStrokes_H

#include <map>
#include <maya/MRampAttribute.h>
#include "strokeMutator.h"
#include "brushShop.h"
#include "stroke.h"

/**
 * @brief A node for mapping IDs to strokes based on texture maps or other criteria.
 *
 * The mapIdStrokes class extends strokeMutator to provide functionality for
 * assigning various IDs (brush, paint, brush model, etc.) to strokes based on
 * texture maps or other criteria. This allows for creating variations in stroke
 * appearance by changing the brush or paint used for different strokes based on
 * their position or other properties.
 */
class mapIdStrokes : public strokeMutator
{
public:
  /**
   * @brief Default constructor.
   */
  mapIdStrokes();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~mapIdStrokes();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new mapIdStrokes object.
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
   * @brief Maps IDs to the input strokes based on texture maps or other criteria.
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
   * @brief Gets IDs from a texture map at the specified points.
   * @param data The data block containing the node's data.
   * @param points The points to sample the texture at.
   * @param quantizeLevels The number of quantization levels for the IDs.
   * @param resultIds Output array to store the resulting IDs.
   */
  void getIds(
      MDataBlock &data,
      MFloatPointArray &points,
      int quantizeLevels,
      MIntArray &resultIds) const;

  /**
   * @brief Applies paint IDs to the strokes.
   * @param strokes Pointer to the strokes to apply the paint IDs to.
   * @param ids Array of paint IDs to apply.
   */
  void applyPaintIds(
      std::vector<Stroke> *strokes,
      const MIntArray &ids) const;

  /**
   * @brief Assigns paint IDs to the strokes based on the node's attributes.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to assign paint IDs to.
   * @return MStatus indicating success or failure.
   */
  MStatus assignPaintIds(MDataBlock &data, std::vector<Stroke> *strokes) const;
  
  /**
   * @brief Assigns brush IDs to the strokes based on the node's attributes.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to assign brush IDs to.
   * @return MStatus indicating success or failure.
   */
  MStatus assignBrushIds(MDataBlock &data, std::vector<Stroke> *strokes) const;
  
  /**
   * @brief Assigns brush model IDs to the strokes based on the node's attributes.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to assign brush model IDs to.
   * @return MStatus indicating success or failure.
   */
  MStatus assignBrushModelIds(MDataBlock &data, std::vector<Stroke> *strokes) const;
 
  /**
   * @brief Gets the brush shop from the node's attributes.
   * @param data The data block containing the node's data.
   * @param brushShop Output parameter to store the brush shop.
   * @return MStatus indicating success or failure.
   */
  MStatus getBrushShop(MDataBlock &data, BrushShop &brushShop) const;
  
  /**
   * @brief Assigns width band layer IDs to the strokes based on the node's attributes.
   * @param data The data block containing the node's data.
   * @param strokes Pointer to the strokes to assign width band layer IDs to.
   * @return MStatus indicating success or failure.
   */
  MStatus assignWidthBandLayerIds(MDataBlock &data, std::vector<Stroke> *strokes) const;

  /** @brief Sample parameter attribute for texture sampling. */
  static MObject aSampleParam;

  /** @brief Flag to enable brush ID assignment. */
  static MObject aDoBrushId;
  
  /** @brief Brush ID map attribute for determining brush IDs. */
  static MObject aBrushIdMap;
  
  /** @brief Brush ID map quantize level attribute for discretizing brush IDs. */
  static MObject aBrushIdMapQuantizeLevel;
  
  /** @brief Brush ID offset attribute for shifting brush IDs. */
  static MObject aBrushIdOffset;

  /** @brief Flag to enable paint ID assignment. */
  static MObject aDoPaintId;
  
  /** @brief Paint ID map attribute for determining paint IDs. */
  static MObject aPaintIdMap;
  
  /** @brief Paint ID map quantize level attribute for discretizing paint IDs. */
  static MObject aPaintIdMapQuantizeLevel;
  
  /** @brief Paint ID offset attribute for shifting paint IDs. */
  static MObject aPaintIdOffset;
  
  /** @brief Palette attribute for paint colors. */
  static MObject aPalette;

  /** @brief Flag to enable brush model ID assignment. */
  static MObject aDoBrushModelId;
  
  /** @brief Brush shop attribute for accessing brush models. */
  static MObject aBrushShop;
  
  /** @brief Seed attribute for random number generation. */
  static MObject aSeed;

  /** @brief Flag to enable width band layer ID assignment. */
  static MObject aDoWidthBandLayerIds;
  
  /** @brief Width band level attribute for determining layer IDs. */
  static MObject aWidthBandLevel;
};

#endif