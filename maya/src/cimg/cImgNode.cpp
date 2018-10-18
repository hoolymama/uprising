#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgNode.h"
#include "jMayaIds.h"
#include "textureSampling.h"

MTypeId     cImgNode::id( k_cImgNode );



MObject 	cImgNode::aImageFilename;

MObject 	cImgNode::aSamplePoints;




MObject 	cImgNode::aOutput;
MObject 	cImgNode::aOutPoints;
MObject 	cImgNode::aOutDirections;
// MObject 	cImgNode::aNormalizeOutput;



cImgNode::cImgNode() {}

cImgNode::~cImgNode() {}

void *cImgNode::creator()
{
	return new cImgNode();
}

MStatus cImgNode::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;


	aImageFilename = tAttr.create("imageFilename", "im", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setUsedAsFilename(true);
	addAttribute( aImageFilename );

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	aOutPoints = tAttr.create("outPoints", "opts", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutPoints );

	aOutDirections = tAttr.create("outDirections", "odrs", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutDirections );

	st = attributeAffects(aImageFilename, aOutput ); mser;
	st = attributeAffects(aImageFilename, aOutPoints ); mser;
	st = attributeAffects(aImageFilename, aOutDirections ); mser;


	return MS::kSuccess;
}





MStatus cImgNode::compute( const MPlug &plug, MDataBlock &data ) {

	if ( plug != aOutput) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;

	// get the image up front, then if something goes wrong
	// we just set output and make it clean

	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;

	cImgData *newData = (cImgData *)fnOut.data(&st); mser;

	CImg<unsigned char> *image = newData->fImg;

	MString imageFilename = data.inputValue(aImageFilename).asString();


	image->assign(imageFilename.asChar());
	CImg<unsigned char> small_image = image->get_resize(32, 32, -100, -100, 5 );


	int w = image->width();
	int h = image->height();
	int channels = image->spectrum();

	CImgList<float> gradList = small_image.get_gradient("xy", 0);

	gradList(0).resize(w, h, -100, -100, 5);
	gradList(1).resize(w, h, -100, -100, 5);

	int wg = gradList(0).width();
	int hg = gradList(0).height();


	MVectorArray points;
	MVectorArray directions;


	if (gradList.size() == 2 && wg > 0 && hg > 0) {


		for (int y = 0; y < h; y += 2)
		{
			;
			for (int x = 0; x < w ; x += 2)
			{

				float gx = gradList(0).atXY(x, y);
				float gy = gradList(1).atXY(x, y);

				points.append(
				  MVector(
				    ((x + 0.5) / w),
				    (1.0 - (y + 0.5) / h),
				    0));


				/* gy is normally */
				directions.append(
				  MVector(
				    gy ,
				    gx,
				    0).normal());

			}
		}
	}



	MDataHandle hOutPoints = data.outputValue(aOutPoints);
	MFnVectorArrayData fnOutPoints;
	MObject dOutPoints = fnOutPoints.create(points);
	hOutPoints.set(dOutPoints);
	st = data.setClean( aOutPoints );

	MDataHandle hOutDirections = data.outputValue(aOutDirections);
	MFnVectorArrayData fnOutDirections;
	MObject dOutDirections = fnOutDirections.create(directions);
	hOutDirections.set(dOutDirections);
	st = data.setClean( aOutDirections );


	hOutput.set(newData);
	data.setClean( plug );
	return MS::kSuccess;
}
