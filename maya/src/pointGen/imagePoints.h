/**
 * @file imagePoints.h
 * @brief Defines the imagePoints class for generating points based on image data.
 * 
 * This file contains the imagePoints class which extends Maya's MPxLocatorNode to provide
 * functionality for generating and visualizing points based on image data. It processes
 * density and radius images to create points with appropriate properties, and provides
 * methods for relaxing, culling, and sorting these points.
 */

#ifndef _imagePoints_H
#define _imagePoints_H

#define cimg_display 0

#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include <maya/MObject.h>
#include <maya/MPxNode.h>

#include "dotData.h"
#include "CImg.h"

using namespace cimg_library;

/**
 * @class imagePoints
 * @brief A node for generating and visualizing points based on image data.
 * 
 * The imagePoints class extends MPxLocatorNode to create a specialized node that
 * generates and visualizes points based on image data. It processes density and
 * radius images to create points with appropriate properties, and provides methods
 * for relaxing, culling, and sorting these points. The node also supports gradient
 * calculation and various display options.
 */
class imagePoints : public MPxLocatorNode
{
public:
  /**
   * @brief Default constructor.
   */
  imagePoints();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~imagePoints();
  
  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new imagePoints object.
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
   * @brief Compute method for calculating the node's outputs.
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @return MStatus indicating success or failure.
   */
  MStatus compute(const MPlug &plug, MDataBlock &data);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;
  /** @brief Draw classification for Viewport 2.0. */
  static MString drawDbClassification;
  /** @brief Draw registrant ID for Viewport 2.0. */
  static MString drawRegistrantId;

private:
  /**
   * @brief Creates dots based on image data.
   * @param data The data block containing the node's data.
   * @param dots Vector to store the created dots.
   */
  void makeDots(MDataBlock &data, std::vector<dotData> &dots);
  
  /**
   * @brief Relaxes the positions of dots to reduce clustering.
   * @param data The data block containing the node's data.
   * @param dots Vector of dots to relax.
   */
  void relaxDots(MDataBlock &data, std::vector<dotData> &dots);
  
  /**
   * @brief Removes dots that don't meet certain criteria.
   * @param data The data block containing the node's data.
   * @param dots Vector of dots to cull.
   */
  void cullDots(MDataBlock &data, std::vector<dotData> &dots);
  
  /**
   * @brief Sorts dots based on a direction vector.
   * @param data The data block containing the node's data.
   * @param dots Vector of dots to sort.
   * @param worldMatrix The world transformation matrix.
   */
  void sortDots(MDataBlock &data, std::vector<dotData> &dots, const MMatrix &worldMatrix);
  
  /**
   * @brief Calculates gradient vectors for dots.
   * @param data The data block containing the node's data.
   * @param dots Vector of dots to calculate gradients for.
   */
  void makeGradients(MDataBlock &data, std::vector<dotData> &dots);
  
  /**
   * @brief Gets an image from a Maya attribute.
   * @param data The data block containing the node's data.
   * @param attribute The attribute containing the image data.
   * @return Pointer to the CImg image.
   */
  CImg<unsigned char> *getImage(MDataBlock &data, MObject &attribute);

  /** @brief Density image attribute. */
  static MObject aDensityImage;
  /** @brief Radius image attribute. */
  static MObject aRadiusImage;
  /** @brief Mask image attribute. */
  static MObject aMask;

  /** @brief Density ramp attribute. */
  static MObject aDensityRamp;
  /** @brief Minimum density range attribute. */
  static MObject aDensityRangeMin;
  /** @brief Maximum density range attribute. */
  static MObject aDensityRangeMax;
  /** @brief Density range attribute. */
  static MObject aDensityRange;

  /** @brief Radius ramp attribute. */
  static MObject aRadiusRamp;
  /** @brief Minimum radius range attribute. */
  static MObject aRadiusRangeMin;
  /** @brief Maximum radius range attribute. */
  static MObject aRadiusRangeMax;
  /** @brief Radius range attribute. */
  static MObject aRadiusRange;

  /** @brief Number of neighbors for relaxation attribute. */
  static MObject aNeighbors;
  /** @brief Number of relaxation iterations attribute. */
  static MObject aIterations;
  /** @brief Force magnitude for relaxation attribute. */
  static MObject aMagnitude;
  /** @brief Random seed attribute. */
  static MObject aSeed;

  /** @brief Sort direction vector attribute. */
  static MObject aSortVector;

  /** @brief Gradient image attribute. */
  static MObject aGradientImage;
  /** @brief Flag to enable gradient calculation attribute. */
  static MObject aDoGradient;
  /** @brief Flag to normalize gradients attribute. */
  static MObject aNormalizeGradient;

  /** @brief Output U coordinates attribute. */
  static MObject aOutU;
  /** @brief Output V coordinates attribute. */
  static MObject aOutV;
  /** @brief Output world space points attribute. */
  static MObject aOutPointsWorld;
  /** @brief Output world space gradients attribute. */
  static MObject aOutGradientsWorld;

public:
  /** @brief Output points attribute. */
  static MObject aOutPoints;
  /** @brief Output gradients attribute. */
  static MObject aOutGradients;

  /** @brief Output radius attribute. */
  static MObject aOutRadius;
  /** @brief Line thickness for display attribute. */
  static MObject aLineThickness;
  /** @brief Point size for display attribute. */
  static MObject aPointSize;
  /** @brief Circle display size attribute. */
  static MObject aCircleDisplaySize;
  /** @brief Color for display attribute. */
  static MObject aColor;
  /** @brief Flag to display points attribute. */
  static MObject aDisplayPoints;
  /** @brief Flag to display circles attribute. */
  static MObject aDisplayCircles;
  /** @brief Flag to display order attribute. */
  static MObject aDisplayOrder;

  /** @brief Gradient color for display attribute. */
  static MObject aGradientColor;
  /** @brief Flag to display gradients attribute. */
  static MObject aDisplayGradient;
  /** @brief Gradient line length for display attribute. */
  static MObject aGradientLineLength;
};

#endif
