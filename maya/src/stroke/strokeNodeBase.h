/**
 * @file strokeNodeBase.h
 * @brief Defines the strokeNodeBase class, the base class for all stroke-related nodes in Maya.
 * 
 * This file contains the strokeNodeBase class which extends Maya's MPxNode to provide
 * base functionality for all stroke-related nodes. It includes methods for generating
 * stroke geometry, extracting various points from strokes, and filtering/sorting strokes.
 */

#ifndef _strokeNodeBase_H
#define _strokeNodeBase_H

#include <maya/MPxNode.h>

#include "stroke.h"

/**
 * @class strokeNodeBase
 * @brief Base class for all stroke-related nodes in Maya.
 * 
 * The strokeNodeBase class extends MPxNode to create a base class for all
 * stroke-related nodes. It provides common functionality for generating stroke
 * geometry, extracting various points from strokes, and filtering/sorting strokes.
 * This class is abstract and meant to be subclassed by specific stroke node types.
 */
class strokeNodeBase : public MPxNode
{
public:
  /**
   * @brief Default constructor.
   */
  strokeNodeBase();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~strokeNodeBase();
  
  /**
   * @brief Indicates whether this class is abstract.
   * @return True, as this is an abstract base class.
   */
  virtual bool isAbstractClass() const { return true; }

  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new strokeNodeBase object.
   */
  static void *creator();
  
  /**
   * @brief Initialize attributes and connections for the node.
   * @return MStatus indicating success or failure.
   */
  static MStatus initialize();

  /**
   * @brief Compute method for calculating the node's outputs.
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

protected:
  /**
   * @brief Generates stroke geometry based on the node's attributes.
   * 
   * This is the main method that derived classes should override to implement
   * their specific stroke generation logic.
   * 
   * @param plug The plug to compute.
   * @param data The data block containing the node's data.
   * @param geom Pointer to a vector to store the generated strokes.
   * @return MStatus indicating success or failure.
   */
  virtual MStatus generateStrokeGeometry(
      const MPlug &plug,
      MDataBlock &data,
      std::vector<Stroke> *geom);

  /**
   * @brief Extracts pivot points from strokes.
   * @param geom Pointer to a vector of strokes.
   * @param result Output array to store the pivot points.
   */
  void getPivotPoints(
      const std::vector<Stroke> *geom,
      MFloatPointArray &result) const;

  /**
   * @brief Extracts points at a specific stroke parameter from strokes.
   * @param geom Pointer to a vector of strokes.
   * @param strokeParam Parameter value along the stroke (0.0 to 1.0).
   * @param result Output array to store the points.
   */
  void getStrokeParamPoints(
    const std::vector<Stroke> *geom, 
    float strokeParam, 
    MFloatPointArray &result) const;

  /**
   * @brief Extracts the first target point from each stroke.
   * @param geom Pointer to a vector of strokes.
   * @param result Output array to store the first target points.
   */
  void getFirstTargetPoints(
      const std::vector<Stroke> *geom,
      MFloatPointArray &result) const;

  /**
   * @brief Extracts all target points from strokes.
   * @param geom Pointer to a vector of strokes.
   * @param result Output array to store all target points.
   */
  void getTargetPoints(
      const std::vector<Stroke> *geom,
      MFloatPointArray &result) const;

  /**
   * @brief Extracts all target colors from strokes.
   * @param geom Pointer to a vector of strokes.
   * @param result Output array to store all target colors.
   */
  void getTargetColors(
    const std::vector<Stroke> *geom,
    MColorArray &result) const;

  /**
   * @brief Extracts all tangent vectors from strokes.
   * @param geom Pointer to a vector of strokes.
   * @param result Output array to store all tangent vectors.
   */
  void getTangents(
    const std::vector<Stroke> *geom,
    MFloatVectorArray &result) const;

  /**
   * @brief Extracts span points from strokes.
   * @param geom Pointer to a vector of strokes.
   * @param result Output array to store the span points.
   */
  void getSpanPoints(
      const std::vector<Stroke> *geom,
      MFloatPointArray &result) const;

  /**
   * @brief Culls the start and end of strokes based on attributes.
   * @param data The data block containing the node's data.
   * @param geom Pointer to a vector of strokes to modify.
   */
  void cullStartEnd(MDataBlock &data, std::vector<Stroke> *geom) const;

  /** @brief Stroke sort key attribute. */
  static MObject aStrokeSortKey;
  /** @brief Stroke sort direction attribute. */
  static MObject aStrokeSortDirection;
  /** @brief Stroke sort list attribute. */
  static MObject aStrokeSortList;
  /** @brief Stroke sort texture attribute. */
  static MObject aStrokeSortTexture;
  /** @brief Apply sort flag attribute. */
  static MObject aApplySort;

  /** @brief Stroke filter key attribute. */
  static MObject aStrokeFilterKey;
  /** @brief Stroke filter operator attribute. */
  static MObject aStrokeFilterOperator;
  /** @brief Stroke filter operand attribute. */
  static MObject aStrokeFilterOperand;
  /** @brief Stroke filter list attribute. */
  static MObject aStrokeFilterList;
  /** @brief Stroke filter texture attribute. */
  static MObject aStrokeFilterTexture;
  /** @brief Apply filters flag attribute. */
  static MObject aApplyFilters;

  /** @brief Start from attribute. */
  static MObject aStartFrom;
  /** @brief End at attribute. */
  static MObject aEndAt;
  
  /** @brief Output attribute. */
  static MObject aOutput;

private:
  /**
   * @brief Sets unique IDs for strokes.
   * @param geom Pointer to a vector of strokes to modify.
   */
  void setStrokeIds(std::vector<Stroke> *geom) const;
  
  /**
   * @brief Filters strokes based on attributes.
   * @param data The data block containing the node's data.
   * @param geom Pointer to a vector of strokes to filter.
   */
  void filterStrokes(MDataBlock &data, std::vector<Stroke> *geom) const;
  
  /**
   * @brief Sorts strokes based on attributes.
   * @param data The data block containing the node's data.
   * @param geom Pointer to a vector of strokes to sort.
   */
  void sortStrokes(MDataBlock &data, std::vector<Stroke> *geom) const;
  
  /**
   * @brief Sets filter map colors for strokes.
   * @param geom Pointer to a vector of strokes to modify.
   * @return True if successful, false otherwise.
   */
  bool setFilterMapColor(std::vector<Stroke> *geom) const;
  
  /**
   * @brief Sets sort map colors for strokes.
   * @param geom Pointer to a vector of strokes to modify.
   * @return True if successful, false otherwise.
   */
  bool setSortMapColor(std::vector<Stroke> *geom) const;
};

#endif