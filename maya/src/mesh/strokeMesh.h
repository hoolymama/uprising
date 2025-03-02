/**
 * @file strokeMesh.h
 * @brief Defines the strokeMesh class for generating mesh geometry from stroke data.
 * 
 * This file contains the strokeMesh class which provides functionality for generating
 * mesh geometry from stroke data. It supports extruding profiles along stroke paths
 * to create tube-like or ribbon-like geometry for visualization of strokes.
 */

#include <maya/MPointArray.h>
#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>

/**
 * @class strokeMesh
 * @brief A node for generating mesh geometry from stroke data.
 * 
 * The strokeMesh class extends MPxNode to create a specialized node that generates
 * mesh geometry from stroke data. It provides functionality for:
 * - Extruding profile curves along stroke paths
 * - Creating tube-like geometry with customizable profile
 * - Creating ribbon-like geometry
 * - Supporting different profile shapes and scales
 */
class strokeMesh : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	strokeMesh();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~strokeMesh();

	/**
	 * @brief Compute method for calculating the node's outputs.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus compute(const MPlug &, MDataBlock &);
	
	/**
	 * @brief Post-constructor setup for the node.
	 */
	virtual void postConstructor();

	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new strokeMesh object.
	 */
	static void *creator();
	
	/**
	 * @brief Initialize attributes and connections for the node.
	 * @return MStatus indicating success or failure.
	 */
	static MStatus initialize();
	
	/** @brief Unique type ID for this node type. */
	static MTypeId id;

	/**
	 * @brief Calculates a default profile for extrusion.
	 * @param numProfilePoints Number of points in the profile.
	 * @param profileScale Scale factor for the profile.
	 * @param profileVertices Output array to store the profile vertices.
	 */
	void calcDefaultProfile(
		int numProfilePoints,
		double profileScale,
		MPointArray &profileVertices) const;

	/**
	 * @brief Extrudes a profile along stroke paths to create mesh geometry.
	 * @param pStrokes Pointer to a vector of strokes to extrude along.
	 * @param profileVertices Array of profile vertices to extrude.
	 * @param geom Output object to store the generated mesh geometry.
	 */
	void extrude(const std::vector<Stroke> *pStrokes, const MPointArray &profileVertices, MObject &geom);

	/**
	 * @brief Creates tube-like geometry by connecting profile points along a path.
	 * @param numTargets Number of target points along the path.
	 * @param numProfilePoints Number of points in the profile.
	 * @param vertexIndex Starting vertex index.
	 * @param faceCounts Output array to store face counts.
	 * @param connectivity Output array to store vertex connectivity.
	 * @return The next available vertex index.
	 */
	int createTube(
		int numTargets,
		int numProfilePoints,
		int vertexIndex,
		MIntArray &faceCounts,
		MIntArray &connectivity);

	/**
	 * @brief Creates ribbon-like geometry along a path.
	 * @param numTargets Number of target points along the path.
	 * @param vertexIndex Starting vertex index.
	 * @param faceCounts Output array to store face counts.
	 * @param connectivity Output array to store vertex connectivity.
	 * @return The next available vertex index.
	 */
	int createRibbon(
	int numTargets,
	int vertexIndex,
	MIntArray &faceCounts,
	MIntArray &connectivity);

private:
	/** @brief Array of strokes attribute. */
	static MObject aStrokes;
	
	/** @brief Number of profile points attribute. */
	static MObject aNumProfilePoints;
	
	/** @brief Profile scale attribute. */
	static MObject aProfileScale;
	
	/** @brief Curve set attribute. */
	static MObject aCurveSet;
	
	/** @brief Profile curves attribute. */
	static MObject aProfileCurves;

	// static MObject aUWidth;
	// static MObject aUOffset;

	//  static MObject aVLength;
	//  static MObject aVOffset;

	/** @brief Output mesh attribute. */
	static MObject aOutMesh;
};
