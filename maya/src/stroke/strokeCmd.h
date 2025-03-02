/**
 * @file strokeCmd.h
 * @brief Defines the strokeCmd class for executing stroke-related commands in Maya.
 * 
 * This file contains the strokeCmd class which extends Maya's MPxCommand to provide
 * a command interface for working with strokes. It includes functionality for querying
 * and manipulating stroke properties such as coil values, weights, and pivot positions.
 */

#ifndef strokeCmd_H__
#define strokeCmd_H__

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include "stroke.h"

/** @brief Flag for specifying a stroke index. */
#define kStrokeIndexFlag "-si"
/** @brief Long version of the stroke index flag. */
#define kStrokeIndexFlagL "-strokeIndex"

/** @brief Flag for querying or setting coil values. */
#define kCoilFlag "-cl"
/** @brief Long version of the coil flag. */
#define kCoilFlagL "-coil"

/** @brief Flag for querying or setting weight values. */
#define kWeightFlag "-w"
/** @brief Long version of the weight flag. */
#define kWeightFlagL "-weight"

/** @brief Flag for querying the maximum coil value. */
#define kMaxCoilFlag "-mcl"
/** @brief Long version of the maximum coil flag. */
#define kMaxCoilFlagL "-maxCoil"

/** @brief Flag for querying the stroke count. */
#define kStrokeCountFlag "-sc"
/** @brief Long version of the stroke count flag. */
#define kStrokeCountFlagL "-strokeCount"

/** @brief Flag for querying or setting pivot positions. */
#define kPivotPositionFlag "-pvp"
/** @brief Long version of the pivot position flag. */
#define kPivotPositionFlagL "-pivotPosition"

/**
 * @class strokeCmd
 * @brief A command for working with strokes in Maya.
 * 
 * The strokeCmd class extends MPxCommand to create a command interface for
 * working with strokes in Maya. It provides functionality for querying and
 * manipulating stroke properties such as coil values, weights, and pivot positions.
 */
class strokeCmd : public MPxCommand
{
public:
	/**
	 * @brief Default constructor.
	 */
	strokeCmd() {}
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~strokeCmd() {}

	/**
	 * @brief Executes the command.
	 * @param args The command arguments.
	 * @return MStatus indicating success or failure.
	 */
	MStatus doIt(const MArgList &args);

	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new strokeCmd object.
	 */
	static void *creator();

	/**
	 * @brief Creates the syntax for the command.
	 * @return MSyntax object defining the command syntax.
	 */
	static MSyntax newSyntax();

	/**
	 * @brief Handles the stroke count flag.
	 * @param strokes Vector of strokes to query.
	 * @return MStatus indicating success or failure.
	 */
	MStatus handleStrokeCountFlag(const std::vector<Stroke> &strokes);
	
	/**
	 * @brief Handles the coil flag for a specific stroke.
	 * @param strokes Vector of strokes to query or modify.
	 * @param strokeId ID of the stroke to operate on.
	 * @return MStatus indicating success or failure.
	 */
	MStatus handleCoilFlag(const std::vector<Stroke> &strokes, int strokeId);
	
	/**
	 * @brief Handles the maximum coil flag.
	 * @param strokes Vector of strokes to query.
	 * @return MStatus indicating success or failure.
	 */
	MStatus handleMaxCoilFlag(const std::vector<Stroke> &strokes);
	
	/**
	 * @brief Handles the weight flag for a specific stroke.
	 * @param strokes Vector of strokes to query or modify.
	 * @param strokeId ID of the stroke to operate on.
	 * @return MStatus indicating success or failure.
	 */
	MStatus handleWeightFlag(const std::vector<Stroke> &strokes, int strokeId);
	
	/**
	 * @brief Handles the pivot position flag for a specific stroke.
	 * @param strokes Vector of strokes to query or modify.
	 * @param strokeId ID of the stroke to operate on.
	 * @return MStatus indicating success or failure.
	 */
	MStatus handlePivotPositionFlag(const std::vector<Stroke> &strokes, int strokeId);
	
	/**
	 * @brief Gets the stroke ID from command arguments.
	 * @param strokes Vector of strokes to validate against.
	 * @param argData Command argument database.
	 * @param status Output parameter for operation status.
	 * @return The stroke ID if valid, or -1 if invalid.
	 */
	int getStrokeId(const std::vector<Stroke> &strokes, MArgDatabase &argData,
						MStatus *status);
};

#endif
