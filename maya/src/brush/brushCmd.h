/**
 * @file brushCmd.h
 * @brief Defines the Maya command for brush operations.
 *
 * This file implements a Maya command for brush operations:
 * - Provides command-line access to brush functionality
 * - Supports querying brush triangles for visualization
 * - Allows retrieving brush tool center point
 * - Integrates with Maya's command system
 */

#ifndef	__brushCmd_H__
#define	__brushCmd_H__

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxCommand.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnDependencyNode.h>
#include "brushNode.h"

/** @brief Flag for querying brush triangles */
#define kTrianglesFlag	 "-tri"
#define kTrianglesFlagL "-triangles"

/** @brief Flag for querying brush tool center point */
#define kTcpFlag	 "-tcp"
#define kTcpFlagL "-toolCenterPoint"

/**
 * @class brushCmd
 * @brief Maya command for brush operations.
 *
 * brushCmd provides command-line access to brush functionality:
 * - Extends MPxCommand for Maya's command system
 * - Supports querying brush triangles for visualization
 * - Allows retrieving brush tool center point
 * - Provides syntax for command arguments
 */
class brushCmd : public MPxCommand
/** @dia:pos -52.2,6.6 */
{
public:
	/**
	 * @brief Default constructor.
	 */
	brushCmd() {}
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~brushCmd() {}

	/**
	 * @brief Executes the command.
	 * @param args Command arguments
	 * @return MStatus Success or failure status
	 */
	MStatus doIt(const MArgList &args);

	/**
	 * @brief Creates a new instance of the command.
	 * @return Pointer to new brushCmd instance
	 */
	static void *creator();

	/**
	 * @brief Creates command syntax.
	 * @return MSyntax object defining command syntax
	 */
	static MSyntax newSyntax();

private:
	/**
	 * @brief Handles the triangles flag.
	 * @param geom Brush geometry to get triangles from
	 * @return MStatus Success or failure status
	 */
	MStatus handleTrianglesFlag(const Brush &geom);
	
	/**
	 * @brief Handles the tool center point flag.
	 * @param geom Brush geometry to get TCP from
	 * @return MStatus Success or failure status
	 */
	MStatus handleTcpFlag(const Brush &geom);
};

#endif	//	!____

