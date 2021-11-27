#include <maya/MFnMatrixData.h>
#include <maya/MPlug.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>

#include "nodeUtils.h"

MMatrix NodeUtils::firstWorldMatrix(  const MObject & shapeNode)
{	
	MFnDagNode fnDag(shapeNode); 
	MObject dParent = fnDag.parent( 0 );

	MFnDependencyNode fnParent( dParent );
	MObject dWorldMatAttr = fnParent.attribute( "worldMatrix" );
	MPlug worldMatrixPlg = MPlug( dParent, dWorldMatAttr ).elementByLogicalIndex( 0 );

	MObject dWorldMat;
	worldMatrixPlg.getValue( dWorldMat );
	return MFnMatrixData( dWorldMat ).matrix();
}


MMatrix NodeUtils::fieldWorldMatrix(  const MObject & fieldNode)
{	
	MFnDependencyNode fnD(fieldNode); 
	MObject dWorldMatAttr = fnD.attribute( "worldMatrix" );
	MPlug worldMatrixPlg = MPlug( fieldNode, dWorldMatAttr ).elementByLogicalIndex( 0 );
	MObject dWorldMat;
	worldMatrixPlg.getValue( dWorldMat );
	return MFnMatrixData( dWorldMat ).matrix();
}