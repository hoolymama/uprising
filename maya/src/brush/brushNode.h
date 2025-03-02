/**
 * @file brushNode.h
 * @brief Defines the Maya node for representing brushes in the scene.
 *
 * This file implements a Maya locator node for brushes:
 * - Represents physical brushes in the Maya scene
 * - Provides attributes for brush properties
 * - Supports different brush modes (painting, dipping, wiping)
 * - Integrates with Maya's dependency graph
 */

#ifndef _brushNODE_H
#define _brushNODE_H

#include <maya/MPxLocatorNode.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGMessage.h>
#include <maya/MDGModifier.h>

#include <maya/MPxNode.h>

#include "brushData.h"

#include "brush.h"

/**
 * @class brushNode
 * @brief Maya locator node for representing brushes in the scene.
 *
 * brushNode provides a visual and functional representation of brushes:
 * - Implements a locator for visual representation
 * - Manages brush properties through attributes
 * - Outputs different brush configurations for painting, dipping, and wiping
 * - Integrates with Maya's viewport and dependency graph
 */
class brushNode : public MPxLocatorNode
{
public:
  /**
   * @brief Default constructor.
   */
  brushNode();
  
  /**
   * @brief Virtual destructor.
   */
  virtual ~brushNode();
  
  /**
   * @brief Creates a new instance of the node.
   * @return Pointer to new brushNode instance
   */
  static void *creator();
  
  /**
   * @brief Initializes the node attributes.
   * @return MStatus Success or failure status
   */
  static MStatus initialize();
  
  /**
   * @brief Post-constructor setup.
   */
  virtual void postConstructor();

  /**
   * @brief Indicates if the node is bounded.
   * @return True as the brush has a bounding box
   */
  virtual bool isBounded() const;
  
  /**
   * @brief Gets the bounding box of the brush.
   * @return Bounding box for the brush
   */
  virtual MBoundingBox boundingBox() const;

  /**
   * @brief Computes the brush outputs.
   * @param plug The plug to compute
   * @param data The data block containing the node's data
   * @return MStatus Success or failure status
   */
  virtual MStatus compute(const MPlug &plug, MDataBlock &data);
 
  /** @brief Unique Maya type ID for the node */
  static MTypeId id;
  /** @brief Draw classification for Viewport 2.0 */
  static MString drawDbClassification;
  /** @brief Draw registrant ID for Viewport 2.0 */
  static MString drawRegistrantId;

  /** @brief Brush width */
  static MObject aWidth;
  /** @brief Brush tip position */
  static MObject aTip;
  /** @brief Bristle height */
  static MObject aBristleHeight;
  /** @brief Parameter for painting mode */
  static MObject aPaintingParam;
  /** @brief Parameter for dipping mode */
  static MObject aDipParam;
  /** @brief Parameter for wiping mode */
  static MObject aWipeParam;
  /** @brief Brush shape */
  static MObject aShape;
  /** @brief Tool center point scale */
  static MObject aTcpScale;
  /** @brief Base transformation matrix */
  static MObject aBaseMatrix;
  
private:
  /**
   * @brief Gets a brush from an attribute.
   * @param attribute Attribute containing brush data
   * @param brush Output brush object
   * @return MStatus Success or failure status
   */
  MStatus getBrush(MObject &attribute, Brush &brush);

  /**
   * @brief Outputs brush data to an attribute.
   * @param data Data block to write to
   * @param attribute Attribute to write to
   * @param brush Brush data to output
   * @return MStatus Success or failure status
   */
  MStatus outputData(MDataBlock &data, MObject &attribute, const Brush &brush);

  /** @brief Whether the brush is in service */
  static MObject aInService;
  /** @brief Physical ID of the brush */
  static MObject aPhysicalId;
  /** @brief Paint retention capability */
  static MObject aRetention;
  /** @brief Transition height parameter */
  static MObject aTransHeightParam;
  /** @brief Contact power */
  static MObject aContactPower;
  /** @brief Forward bias parameters */
  static MObject aForwardBias;
  /** @brief Gravity bias parameters */
  static MObject aGravityBias;

  /** @brief Initial wait time */
  static MObject aInitialWait;
  /** @brief Initial water amount */
  static MObject aInitialWater;
  /** @brief Initial dip count */
  static MObject aInitialDips;
  /** @brief Retardant amount */
  static MObject aRetardant;

  /** @brief Wipe bar position */
  static MObject aWipeBarPosition;

  /** @brief Line length */
  static MObject aLineLength;
  /** @brief Line thickness */
  static MObject aLineThickness;

  /** @brief DMX control */
  static MObject aDmx;

  /** @brief Output brush for painting */
  static MObject aOutPaintBrush;
  /** @brief Output brush for dipping */
  static MObject aOutDipBrush;
  /** @brief Output brush for wiping */
  static MObject aOutWipeBrush;

  /** @brief Brush model name */
  static MObject aModel;
};

/**
 * @namespace brushNodeCallback
 * @brief Namespace for brush node callbacks.
 */
namespace brushNodeCallback
{
	/** @brief Callback ID */
	static MCallbackId id;

	/**
	 * @brief Makes default connections for a new brush node.
	 * @param node Node to connect
	 * @param clientData Client data for callback
	 */
	static void makeDefaultConnections(MObject & node, void* clientData)
	{
		MPlug wmPlugmulti(node, brushNode::worldMatrix);
		MPlug wm(wmPlugmulti.elementByLogicalIndex(0));
		MPlug mt(node, brushNode::aBaseMatrix);

		MDGModifier mod;	
		mod.connect(wm, mt);
		MStatus stat = mod.doIt();
	}
}

#endif
