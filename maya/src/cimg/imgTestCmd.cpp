

#include <maya/MString.h>
#include <maya/MGlobal.h>

#include <maya/MArgDatabase.h>
#include <maya/MSyntax.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MRenderUtil.h>

#include <errorMacros.h>


#include "CImg.h"


#include "imgTestCmd.h"


using namespace cimg_library;


//	static
void *imgTestCmd::creator()
{
	return new imgTestCmd();
}

MSyntax imgTestCmd::newSyntax()
{
	MSyntax syn;

	syn.addFlag(kFilenameFlag, kFilenameFlagL, MSyntax::kString );
	syn.addFlag(kPlugFlag, kPlugFlagL, MSyntax::kString );
	syn.addFlag(kResolutionFlag, kResolutionFlagL, MSyntax::kUnsigned );

	return syn;

}


void imgTestCmd::intensityProfile( MString &filename)
{

	CImg<unsigned char> image(filename.asChar());
	CImg<unsigned char>  visu(500, 400, 1, 3, 0);
	const unsigned char red[] = { 255, 0, 0 }, green[] = { 0, 255, 0 }, blue[] = { 0, 0, 255 };
	image.blur(2.5);
	CImgDisplay main_disp(image, "Click a point") ;
	CImgDisplay draw_disp(visu, "Intensity profile");


	while (!main_disp.is_closed() && !draw_disp.is_closed()) {
		main_disp.wait();
		if (main_disp.button() && main_disp.mouse_y() >= 0) {
			const int y = main_disp.mouse_y();
			visu.fill(0).draw_graph(image.get_crop(0, y, 0, 0, image.width() - 1, y, 0, 0), red, 1, 1,
			                        0, 255, 0);
			visu.draw_graph(image.get_crop(0, y, 0, 1, image.width() - 1, y, 0, 1), green, 1, 1, 0,
			                255, 0);
			visu.draw_graph(image.get_crop(0, y, 0, 2, image.width() - 1, y, 0, 2), blue, 1, 1, 0,
			                255, 0).display(draw_disp);
		}
	}
}

MStatus displayTexture(MString &plugName, unsigned int resolution)
{
	MStatus st;
	MFloatMatrix cameraMat;
	cameraMat.setToIdentity();
	MFloatVectorArray transparencies;
	MFloatVectorArray colors;

	int numPixels = resolution * resolution;
	cerr << "resolution: " << resolution << endl;
	cerr << "numPixels: " << numPixels << endl;

	MFloatArray uVals(numPixels);
	MFloatArray vVals(numPixels);
	// result.setLength(n);

	for ( int i = 0, y = (resolution - 1); y >= 0; y--) {
		float vVal =  (y + 0.5) / resolution;
		for (int x = 0; x < resolution; x++) {
			float uVal = (x + 0.5) / resolution;
			uVals.set( uVal, i );
			vVals.set( vVal, i );
			i++;
		}
	}

	st =  MRenderUtil::sampleShadingNetwork (plugName, numPixels, false, false, cameraMat,
	      0, &uVals, &vVals, 0, 0, 0, 0, 0, colors, transparencies);
	if (st.error()) {return st;}

	// cerr << "colors: " << colors << endl;

	unsigned char col;
	unsigned char *vals = new unsigned char[numPixels * 3];
	unsigned char *cPtr = vals;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < numPixels; j++)
		{
			col = (unsigned char)(colors[j][i] * 255);
			*cPtr = col; cPtr++;
		}
	}

	cerr << "about to make image: "  << endl;


	CImg<unsigned char> image(vals, resolution, resolution, 1, 3);


	cerr << "image dimensions: " << image.width() << "X" << image.height()  << endl;

	CImgDisplay main_disp(image) ;

	delete [] vals;

	while (!main_disp.is_closed() ) {
		main_disp.wait();
	}



	return MS::kSuccess;
}

MStatus imgTestCmd::doIt( const MArgList &args )
{
	MStatus st;


	MArgDatabase  argData(syntax(), args);

	if (argData.isFlagSet(kFilenameFlag)) {
		MString filename;
		st = argData.getFlagArgument (kFilenameFlag, 0, filename); msert;
		intensityProfile(filename);
	}
	else if (argData.isFlagSet(kPlugFlag)) {
		MString plugName;
		st = argData.getFlagArgument (kPlugFlag, 0, plugName); msert;

		int resolution = 128;
		if (argData.isFlagSet(kResolutionFlag)) {
			argData.getFlagArgument (kResolutionFlag, 0, resolution);
		}
		displayTexture(plugName, resolution);
	}
	return MS::kSuccess;
}

