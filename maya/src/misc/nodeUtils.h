

#ifndef nodeUtils_H
#define nodeUtils_H

#include <maya/MObject.h>
#include <maya/MMatrix.h>



class NodeUtils {

public:

	static MMatrix firstWorldMatrix(  const MObject & shapeNode);
	static MMatrix fieldWorldMatrix(  const MObject & fieldNode);
};


#endif

