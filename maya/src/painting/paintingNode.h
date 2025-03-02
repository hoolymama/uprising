/**
 * @file paintingNode.h
 * @brief Defines the painting class for representing a painting in Maya.
 * 
 * This file contains the painting class which extends the paintingBase class
 * to represent a painting in Maya. It provides functionality for managing strokes,
 * palettes, and visualization options for paintings.
 */

#ifndef _paintingNODE_H
#define _paintingNODE_H

#include "paintingData.h"
#include "paintingNodeBase.h"

/**
 * @class painting
 * @brief A node that represents a painting in Maya.
 * 
 * The painting class extends the paintingBase class to create a specialized node
 * that represents a painting in Maya. It provides functionality for:
 * - Managing strokes and clusters
 * - Handling palette data
 * - Controlling visualization options
 * - Supporting approach targets and paths
 */
class painting : public paintingBase
{
public:
  /**
   * @brief Default constructor.
   */
  painting();
  
  /**
   * @brief Indicates whether this class is abstract.
   * @return false, as this class is concrete.
   */
  virtual bool isAbstractClass() const { return false; }
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~painting();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new painting object.
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
   * @brief Compute method for calculating the node's outputs.
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;
  
  /** @brief Classification string for the draw override. */
  static MString drawDbClassification;
  
  /** @brief Registrant ID for the draw override. */
  static MString drawRegistrantId;

  /** @brief Canvas matrix attribute. */
  static MObject aCanvasMatrix;

  /** @brief Display approach targets flag attribute. */
  static MObject aDisplayApproachTargets;
  
  /** @brief Display cluster path flag attribute. */
  static MObject aDisplayClusterPath;
  
  /** @brief Display pivots flag attribute. */
  static MObject aDisplayPivots;

  /** @brief Display paint IDs flag attribute. */
  static MObject aDisplayPaintIds;
  
  /** @brief Display repeat IDs flag attribute. */
  static MObject aDisplayRepeatIds;
  
  /** @brief Display brush model IDs flag attribute. */
  static MObject aDisplayBrushModelIds;

  /** @brief Display contact width flag attribute. */
  static MObject aDisplayContactWidth;
  
  /** @brief Cluster path color attribute. */
  static MObject aClusterPathColor;

  /** @brief Stack gap attribute. */
  static MObject aStackGap;

  /** @brief Output painting data attribute. */
  static MObject aOutput;
  
private:
  /**
   * @brief Gets the palette data from the data block.
   * @param data The data block containing the node's data.
   * @param palette Output map to store the palette data.
   * @return MStatus indicating success or failure.
   */
  MStatus getPalette(MDataBlock &data, std::map<int, Paint> &palette) const;
 
  /**
   * @brief Adds strokes to the painting geometry.
   * @param data The data block containing the node's data.
   * @param pGeom Pointer to the painting geometry to add strokes to.
   * @return MStatus indicating success or failure.
   */
  MStatus addStrokes(MDataBlock &data, paintingGeom *pGeom);

  /** @brief Approach distance start attribute. */
  static MObject aApproachDistanceStart;
  
  /** @brief Approach distance mid attribute. */
  static MObject aApproachDistanceMid;
  
  /** @brief Approach distance end attribute. */
  static MObject aApproachDistanceEnd;
  
  /** @brief Approach distance compound attribute. */
  static MObject aApproachDistance;

  /** @brief Maximum point-to-point distance attribute (in cm). */
  static MObject aMaxPointToPointDistance;

  /** @brief Palette attribute. */
  static MObject aPalette;

  /** @brief Pointer to the painting data. */
  paintingData *m_pd;
};
 
#endif
