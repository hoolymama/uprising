#ifndef _diptych_H
#define _diptych_H

// #define cimg_display 0

#include <maya/MPxLocatorNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
#include <maya/MPointArray.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>

/**
 * @file diptych.h
 * @brief Defines the diptych class for creating and manipulating canvas-like structures in Maya.
 * 
 * The diptych class provides functionality for creating and manipulating canvas-like structures
 * in Maya, including board and painting representations with customizable properties such as
 * dimensions, displacement, and visualization options. It supports generating mesh geometry
 * with probe points for interaction with other systems.
 */

/**
 * @class diptych
 * @brief A locator node that represents a canvas-like structure with board and painting elements.
 * 
 * The diptych class extends MPxLocatorNode to create a specialized node that represents
 * a canvas-like structure with board and painting elements. It provides functionality for:
 * - Generating mesh geometry for visualization
 * - Creating probe points for interaction with other systems
 * - Calculating transformation matrices for board and painting elements
 * - Supporting displacement for creating non-flat surfaces
 * - Providing visualization options for different components
 */
class diptych : public MPxLocatorNode
{
public:
  /**
   * @brief Default constructor.
   */
  diptych();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~diptych();
  
  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new diptych object.
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
   * @brief Indicates whether the node has a boundable volume.
   * @return true if the node is bounded, false otherwise.
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
  
  /** @brief Classification string for the draw override. */
  static MString drawDbClassification;
  
  /** @brief Registrant ID for the draw override. */
  static MString drawRegistrantId;

private:
  /**
   * @brief Calculates the transformation matrix for the painting element.
   * @param data The data block containing the node's data.
   * @param xScale Scale factor for the x-axis.
   * @param yScale Scale factor for the y-axis.
   * @return MMatrix representing the painting's transformation.
   */
  MMatrix calculatePaintingMatrix(MDataBlock &data, double xScale, double yScale) const;

  /**
   * @brief Generates mesh geometry and probe points for the diptych.
   * @param data The data block containing the node's data.
   * @param boardMatrix Transformation matrix for the board.
   * @param paintingMatrix Transformation matrix for the painting.
   * @param outProbePoints Output array to store generated probe points.
   * @param outGeom Output object to store generated mesh geometry.
   * @return MStatus indicating success or failure.
   */
  MStatus generateMeshAndProbes(
      MDataBlock &data,
      const MMatrix &boardMatrix,
      const MMatrix &paintingMatrix,
      MPointArray & outProbePoints,
      MObject &outGeom
     ) const;

  /**
   * @brief Creates a patch of geometry with displacement.
   * @param numX Number of points in the X direction.
   * @param numY Number of points in the Y direction.
   * @param offset Offset for vertex indices.
   * @param matrix Transformation matrix for the patch.
   * @param displacement Array of displacement values.
   * @param points Output array to store generated points.
   * @param faceCounts Output array to store face counts.
   * @param connectivity Output array to store vertex connectivity.
   */
  void createPatch(
      int numX,
      int numY,
      int offset,
      const MMatrix &matrix,
      const MDoubleArray &displacement,
      MPointArray &points,
      MIntArray &faceCounts,
      MIntArray &connectivity) const;

  /**
   * @brief Creates a border around a patch of geometry.
   * @param numX Number of points in the X direction.
   * @param numY Number of points in the Y direction.
   * @param offset Offset for vertex indices.
   * @param matrix Transformation matrix for the border.
   * @param points Output array to store generated points.
   * @param faceCounts Output array to store face counts.
   * @param connectivity Output array to store vertex connectivity.
   */
  void createBorder(
      int numX,
      int numY,
      int offset,
      const MMatrix &matrix,
      MPointArray &points,
      MIntArray &faceCounts,
      MIntArray &connectivity) const;

  /**
   * @brief Calculates displacement values for the diptych surface.
   * @param data The data block containing the node's data.
   * @param numPoints Number of points to calculate displacement for.
   * @param rightDisplacement Output array for right side displacement values.
   * @param leftDisplacement Output array for left side displacement values.
   */
  void getDisplacement(
      MDataBlock &data,
      int numPoints,
      MDoubleArray &rightDisplacement,
      MDoubleArray &leftDisplacement) const;

  /**
   * @brief Adds probe points to the output array.
   * @param numX Number of points in the X direction.
   * @param numY Number of points in the Y direction.
   * @param matrix Transformation matrix for the probe points.
   * @param points Output array to store generated probe points.
   */
  void appendProbePoints(
      int numX,
      int numY,
      const MMatrix &matrix,
      MPointArray &points) const;

  /** @brief Board height attribute. */
  static MObject aBoardHeight;

  /** @brief Gap between board elements attribute. */
  static MObject aGap;

  /** @brief Apply crop flag attribute. */
  static MObject aApplyCrop;
  
  /** @brief Crop corner attribute. */
  static MObject aCropCorner;
  
  /** @brief Crop resolution attribute. */
  static MObject aCropResolution;
  
  /** @brief Image resolution attribute. */
  static MObject aImageResolution;

  /** @brief Pin offset attribute. */
  static MObject aPinOffset;

  /** @brief Pin 0 attribute. */
  static MObject aPin0;
  
  /** @brief Pin X attribute. */
  static MObject aPinX;
  
  /** @brief Pin Y attribute. */
  static MObject aPinY;
  
  /** @brief Pin painting attribute. */
  static MObject aPinPainting;

  /** @brief Displacement attribute. */
  static MObject aDisplacement;
  
  /** @brief Output mesh attribute. */
  static MObject aOutMesh;

public:
  /** @brief Output square matrix attribute. */
  static MObject aOutSquareMatrix;
  
  /** @brief Output board matrix attribute. */
  static MObject aOutBoardMatrix;
  
  /** @brief Output pin matrix attribute. */
  static MObject aOutPinMatrix;
  
  /** @brief Output painting matrix attribute. */
  static MObject aOutPaintingMatrix;

  /** @brief Output probe points attribute. */
  static MObject aOutProbePoints;

  /** @brief Number of probes attribute. */
  static MObject aNumProbes;
  
  /** @brief Probe border attribute. */
  static MObject aProbeBorder;

  /** @brief Board color attribute. */
  static MObject aBoardColor;
  
  /** @brief Square color attribute. */
  static MObject aSquareColor;
  
  /** @brief Pin color attribute. */
  static MObject aPinColor;
  
  /** @brief Probe color attribute. */
  static MObject aProbeColor;

  /** @brief Mirror flag attribute. */
  static MObject aMirror;
  
  /** @brief Painting locator size attribute. */
  static MObject aPaintingLocatorSize;

  /** @brief Display square matrix flag attribute. */
  static MObject aDisplaySquareMatrix;
  
  /** @brief Display board matrix flag attribute. */
  static MObject aDisplayBoardMatrix;
  
  /** @brief Display pin matrix flag attribute. */
  static MObject aDisplayPinMatrix;
  
  /** @brief Display painting matrix flag attribute. */
  static MObject aDisplayPaintingMatrix;
  
  /** @brief Display probes flag attribute. */
  static MObject aDisplayProbes;
  
  /** @brief Probe point size attribute. */
  static MObject aProbePointSize;
};

#endif
