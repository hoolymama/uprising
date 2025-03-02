/**
 * @file pearlNode.h
 * @brief Defines the pearlNode class for generating and visualizing pearl chains.
 * 
 * This file contains the pearlNode class which provides functionality for generating
 * and visualizing pearl chains in Maya. It processes image data to create pearl chains
 * that can be used for stroke-like structures.
 */

#ifndef _pearlNode_H
#define _pearlNode_H
#define cimg_display 0

#include <maya/MPxLocatorNode.h>
#include <maya/MFloatPointArray.h>

#include "pearlChainData.h"
#include "pearlChain.h"
#include "CImg.h"


using namespace cimg_library;

/**
 * @class pearlNode
 * @brief A node for generating and visualizing pearl chains from image data.
 * 
 * The pearlNode class extends MPxLocatorNode to create a specialized node that
 * generates and visualizes pearl chains in Maya. It processes image data to create
 * pearl chains that follow features in the image, which can be used for stroke-like
 * structures or other visualization purposes.
 */
class pearlNode : public MPxLocatorNode
{
public:
  /**
   * @brief Default constructor.
   */
  pearlNode();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~pearlNode();

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new pearlNode object.
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

  /**
   * @brief Indicates whether this node is bounded.
   * @return True, as this node has a bounding box.
   */
  virtual bool isBounded() const;

  /**
   * @brief Returns the bounding box for the node.
   * @return MBoundingBox representing the node's bounds.
   */
  virtual MBoundingBox boundingBox() const;

  /**
   * @brief Legacy viewport draw method.
   * @param view The 3D view to draw in.
   * @param path The DAG path to this node.
   * @param style The display style to use.
   * @param status The display status to use.
   */
  virtual void draw(
    M3dView &view,
    const MDagPath &path,
    M3dView::DisplayStyle style,
    M3dView::DisplayStatus status);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;
  
  /** @brief Draw classification for Viewport 2.0. */
  static MString drawDbClassification;
  
  /** @brief Draw registrant ID for Viewport 2.0. */
  static MString drawRegistrantId;

  /** @brief Point size attribute. */
  static MObject aPointSize;
  
  /** @brief First color attribute. */
  static MObject aColor1;
  
  /** @brief Second color attribute. */
  static MObject aColor2;

  /** @brief Draw edges flag attribute. */
  static MObject aDrawEdges;
  
  /** @brief Draw points flag attribute. */
  static MObject aDrawPoints;
  
  /** @brief Draw circles flag attribute. */
  static MObject aDrawCircles;
  
  /** @brief Random chain color flag attribute. */
  static MObject aRandomChainColor;

  /** @brief Output attribute. */
  static MObject aOutput;

private:
  /**
   * @brief Generates pearl chains from image data.
   * @param data The data block containing the node's data.
   * @param geom Pointer to a vector to store the generated pearl chains.
   * @param pInkImage Pointer to the processed image.
   * @return MStatus indicating success or failure.
   */
  MStatus generate(
    MDataBlock &data, 
    std::vector<pearlChain> *geom, 
    CImg<unsigned char> *pInkImage) const;

  /**
   * @brief Calculates the transform matrix for the image.
   * @param w Width of the image.
   * @param h Height of the image.
   * @return MFloatMatrix representing the image transform.
   */
  MFloatMatrix imageTransform(int w, int h) const;

  /** @brief Image attribute. */
  static MObject aImage;

  /** @brief Maximum iterations attribute. */
  static MObject aMaxIterations;

  /** @brief Offset width attribute. */
  static MObject aOffsetWidth;
  
  /** @brief Maximum width attribute. */
  static MObject aMaxWidth;
  
  /** @brief Maximum stamp width attribute. */
  static MObject aMaxStampWidth;

  /** @brief Minimum branch twig length attribute. */
  static MObject aMinBranchTwigLength;

  /** @brief Minimum loose twig length attribute. */
  static MObject aMinLooseTwigLength;

  /** @brief Longest chain attribute. */
  static MObject aLongestChain;

  /** @brief Span attribute. */
  static MObject aSpan;

  /** @brief Output image attribute. */
  static MObject aOutputImage;
};

#endif