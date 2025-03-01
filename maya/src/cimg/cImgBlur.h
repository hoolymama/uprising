/**
 * @file cImgBlur.h
 * @brief Maya node for applying Gaussian or box blur to images
 * 
 * This class implements a Maya node that provides configurable image blurring capabilities.
 * It supports both Gaussian and box blur with adjustable blur radius and boundary conditions.
 * The node inherits from cImgProcess for common image processing functionality.
 */

#ifndef _cImgBlur
#define _cImgBlur

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

/**
 * @class cImgBlur
 * @brief Image blur processing node
 * 
 * This node provides configurable image blurring capabilities with the following features:
 * - Gaussian or box blur modes
 * - Adjustable blur radius/amount
 * - Configurable boundary conditions (Dirichlet or Neumann)
 * - Real-time preview in Maya viewport
 */
class cImgBlur : public cImgProcess
{
public:
	/**
	 * @brief Default constructor
	 */
	cImgBlur();

	/**
	 * @brief Virtual destructor
	 */
	virtual ~cImgBlur();

	/**
	 * @brief Indicates this is a concrete class that can be instantiated
	 * @return false since this class can be instantiated
	 */
	virtual bool isAbstractClass() const { return false; }

	/**
	 * @brief Creator method for Maya plugin system
	 * @return Pointer to a new instance of cImgBlur
	 */
	static void *creator();

	/**
	 * @brief Initialize method for Maya plugin system
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	static MTypeId id;               ///< Maya type ID for this node

	/**
	 * @brief Process the input image and apply blur
	 * @param data The data block containing the node's data
	 * @param image The input image to process
	 * @param result The output image where the result will be stored
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                        CImg<unsigned char> &result);

	// Input Attributes
	static MObject aAmount;          ///< Blur radius/amount (float)
	static MObject aBoundary;        ///< Boundary condition type (enum)
	static MObject aGaussian;        ///< Whether to use Gaussian blur (bool)

private:
	/**
	 * @enum Boundary
	 * @brief Defines boundary conditions for blur operation
	 * 
	 * kBoundaryDirichlet: Zero boundary conditions (edges fade to black)
	 * kBoundaryNeumann: Neumann boundary conditions (edges extend their values)
	 */
	enum Boundary { 
		kBoundaryDirichlet,         ///< Zero boundary conditions
		kBoundaryNeumann            ///< Neumann boundary conditions
	};
};

#endif
