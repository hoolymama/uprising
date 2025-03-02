#ifndef _funCurve_H
#define _funCurve_H

#include <maya/MPxNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>
#include <maya/MStringArray.h>

#include <maya/MObject.h>
#include <maya/MPxNode.h>

/**
 * @file funCurve.h
 * @brief Defines the funCurve class for generating parametric curves in Maya.
 * 
 * This file contains the funCurve class which provides functionality for generating
 * parametric curves in Maya based on mathematical expressions. It allows users to
 * define custom functions and generate curves with specified parameter ranges and sampling.
 */

/**
 * @class funCurve
 * @brief A node for generating parametric curves based on mathematical expressions.
 * 
 * The funCurve class extends MPxNode to create a specialized node that generates
 * parametric curves based on mathematical expressions. It provides functionality for:
 * - Defining custom mathematical functions
 * - Generating curves with specified parameter ranges
 * - Controlling sampling density
 * - Setting coefficient values for expressions
 */
class funCurve : public MPxNode
{
public:
  /**
   * @brief Default constructor.
   */
  funCurve();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~funCurve();
  
  /**
   * @brief Creator function for Maya's plugin system.
   * @return Pointer to a new funCurve object.
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
  MStatus compute(const MPlug &plug, MDataBlock &data);
  
  /**
   * @brief Creates a curve from a set of points.
   * @param points Array of points to create the curve from.
   * @param curveData Output object to store the created curve.
   * @param minimumPointDistance Minimum distance between points (optional).
   * @return MStatus indicating success or failure.
   */
  MStatus createCurve(const MPointArray &points, MObject &curveData, double minimumPointDistance=0.0);
  
  /**
   * @brief Generates a function string from an expression.
   * @param context Array of context strings.
   * @param expression The mathematical expression to use.
   * @param functionName The name to give the generated function.
   * @return MString containing the generated function.
   */
  MString generateFunction(const MStringArray &context, const MString &expression, const MString &functionName);
  
  /**
   * @brief Generates context for function evaluation.
   * @param data The data block containing the node's data.
   * @param g_p0 Start parameter value.
   * @param g_p1 End parameter value.
   * @param samples Number of samples.
   * @param context Output array to store the generated context.
   */
  void generateContext(MDataBlock &data, double g_p0, double g_p1, int samples, MStringArray &context);

  /** @brief Unique type ID for this node type. */
  static MTypeId id;

private:
  /** @brief Parameter range attribute. */
  static MObject aParameterRange;
  
  /** @brief Minimum parameter range value attribute. */
  static MObject aParameterRangeMin;
  
  /** @brief Maximum parameter range value attribute. */
  static MObject aParameterRangeMax;
  
  /** @brief Number of samples attribute. */
  static MObject aSamples;

  /** @brief Sub-sample range attribute. */
  static MObject aSubSampleRange;
  
  /** @brief Sub-sample range begin attribute. */
  static MObject aSubSampleRangeBegin;
  
  /** @brief Sub-sample range end attribute. */
  static MObject aSubSampleRangeEnd;

  /** @brief Pre-command attribute. */
  static MObject aPreCommand;

  /** @brief Coefficients compound attribute. */
  static MObject aCoefficients;
  
  /** @brief Coefficient name attribute. */
  static MObject aCoefficientName;
  
  /** @brief Coefficient value attribute. */
  static MObject aCoefficientValue;

  /** @brief Expression attribute. */
  static MObject aExpression;
  
  /** @brief Minimum edit point distance attribute. */
  static MObject aMinimumEPDistance;

  /** @brief Output curve attribute. */
  static MObject aOutput;
};

#endif
