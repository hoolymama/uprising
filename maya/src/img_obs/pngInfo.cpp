
// #include "lodepng.h"
// #include <iostream>


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


#include "pngInfo.h"


using namespace cimg_library;


//	static
void *pngInfo::creator()
{
	return new pngInfo();
}

MSyntax pngInfo::newSyntax()
{
	MSyntax syn;
	syn.addFlag(kPaletteFlag, kPaletteFlagL, MSyntax::kBoolean  );
	syn.addFlag(kPaletteSizeFlag, kPaletteSizeFlagL, MSyntax::kBoolean  );

	syn.setObjectType(MSyntax::kStringObjects, 1, 1);


	return syn;

}


MStatus pngInfo::doIt( const MArgList &args )
{
	MStatus st;

	MArgDatabase  argData(syntax(), args);

	MStringArray argStrings;
	argData.getObjects(argStrings);
	MString filename = argStrings[0];

	if (argData.isFlagSet(kPaletteFlag)) {
		return handlePaletteFlag(filename);
	}

	if (argData.isFlagSet(kPaletteSizeFlag)) {
		return handlePaletteSizeFlag(filename);
	}

	return MS::kSuccess;
}



MStatus pngInfo::readImage(const MString &filename, lodepng::State &state, unsigned &w,
                           unsigned &h, std::vector<unsigned char> &image)
{

	std::vector<unsigned char> buffer;
	unsigned error;
	error = lodepng::load_file(buffer, filename.asChar() );
	if (error) {
		MGlobal::displayError("Can't read PNG file: " + filename);
		return MS::kFailure;
	}
	error = lodepng::decode(image, w, h, state, buffer);
	if (error) {
		MGlobal::displayError("Can't decode PNG file: " + filename);
		return MS::kFailure;
	}
	return MS::kSuccess;
}


MStatus pngInfo::handlePaletteSizeFlag(const MString &filename) {

	MStatus st;
	MIntArray result;
	lodepng::State state;
	unsigned w, h;
	std::vector<unsigned char> image;
	st = readImage(filename, state, w, h, image);  msert;

	LodePNGColorMode &color = state.info_png.color;
	if (color.colortype != LCT_PALETTE) {
		MGlobal::displayError("Image is not an indexed PNG with separate palette: " + filename);
		return MS::kFailure;
	}
	setResult(int(color.palettesize));
	return MS::kSuccess;
}




MStatus pngInfo::handlePaletteFlag(const MString &filename) {

	MStatus st;
	MIntArray result;
	lodepng::State state;
	unsigned w, h;
	std::vector<unsigned char> image;
	st = readImage(filename, state, w, h, image); msert;

	LodePNGColorMode &color = state.info_png.color;
	if (color.colortype != LCT_PALETTE) {
		MGlobal::displayError("Image is not an indexed PNG with separate palette: " + filename);
		return MS::kFailure;
	}

	unsigned char *palette = color.palette;
	size_t j = 0;
	for (int i = 0; i < color.palettesize; ++i)
	{
		result.append(int(palette[j])); j++;
		result.append(int(palette[j])); j++;
		result.append(int(palette[j])); j++;
		result.append(int(palette[j])); j++;
	}
	setResult(result);
	return MS::kSuccess;
}





// /*
// Display general info about the PNG.
// */
// void displayPNGInfo(const LodePNGInfo &info)
// {
// 	const LodePNGColorMode &color = info.color;

// 	cerr << "Compression method: " << info.compression_method << std::endl;
// 	cerr << "Filter method: " << info.filter_method << std::endl;
// 	cerr << "Interlace method: " << info.interlace_method << std::endl;
// 	cerr << "Color type: " << color.colortype << std::endl;
// 	cerr << "Bit depth: " << color.bitdepth << std::endl;

// 	cerr << "Bits per pixel: " << lodepng_get_bpp(&color) << std::endl;
// 	cerr << "Channels per pixel: " << lodepng_get_channels(&color) << std::endl;
// 	cerr << "Is greyscale type: " << lodepng_is_greyscale_type(&color) << std::endl;
// 	cerr << "Can have alpha: " << lodepng_can_have_alpha(&color) << std::endl;
// 	cerr << "Palette size: " << color.palettesize << std::endl;
// 	cerr << "Has color key: " << color.key_defined << std::endl;
// 	if (color.key_defined)
// 	{
// 		cerr << "Color key r: " << color.key_r << std::endl;
// 		cerr << "Color key g: " << color.key_g << std::endl;
// 		cerr << "Color key b: " << color.key_b << std::endl;
// 	}
// 	cerr << "Texts: " << info.text_num << std::endl;
// 	for (size_t i = 0; i < info.text_num; i++)
// 	{
// 		cerr << "Text: " << info.text_keys[i] << ": " << info.text_strings[i] << std::endl <<
// 		     std::endl;
// 	}
// 	unsigned char *palette = color.palette;

// 	size_t j = 0;
// 	for (int i = 0; i < color.palettesize; ++i)
// 	{
// 		cerr << i << ": " << int(palette[j]) << " "; j++;
// 		cerr << int(palette[j]) << " "; j++;
// 		cerr << int(palette[j]) << " "; j++;
// 		cerr << int(palette[j]); j++;
// 		cerr << endl;
// 	}






// 	cerr << "International texts: " << info.itext_num << std::endl;
// 	for (size_t i = 0; i < info.itext_num; i++)
// 	{
// 		cerr << "Text: "
// 		     << info.itext_keys[i] << ", "
// 		     << info.itext_langtags[i] << ", "
// 		     << info.itext_transkeys[i] << ": "
// 		     << info.itext_strings[i] << std::endl << std::endl;
// 	}
// 	cerr << "Time defined: " << info.time_defined << std::endl;
// 	if (info.time_defined)
// 	{
// 		const LodePNGTime &time = info.time;
// 		cerr << "year: " << time.year << std::endl;
// 		cerr << "month: " << time.month << std::endl;
// 		cerr << "day: " << time.day << std::endl;
// 		cerr << "hour: " << time.hour << std::endl;
// 		cerr << "minute: " << time.minute << std::endl;
// 		cerr << "second: " << time.second << std::endl;
// 	}
// 	cerr << "Physics defined: " << info.phys_defined << std::endl;
// 	if (info.phys_defined)
// 	{
// 		cerr << "physics X: " << info.phys_x << std::endl;
// 		cerr << "physics Y: " << info.phys_y << std::endl;
// 		cerr << "physics unit: " << info.phys_unit << std::endl;
// 	}
// }


// /*
// Display the names and sizes of all chunks in the PNG file.
// */
// void displayChunkNames(const std::vector<unsigned char> &buffer)
// {
// 	// Listing chunks is based on the original file, not the decoded png info.
// 	const unsigned char *chunk, *begin, *end, *next;
// 	end = &buffer.back() + 1;
// 	begin = chunk = &buffer.front() + 8;

// 	cerr << std::endl << "Chunks:" << std::endl;
// 	cerr << " type: length(s)";
// 	std::string last_type;
// 	while (chunk + 8 < end && chunk >= begin)
// 	{
// 		char type[5];
// 		lodepng_chunk_type(type, chunk);
// 		if (std::string(type).size() != 4)
// 		{
// 			cerr << "this is probably not a PNG" << std::endl;
// 			return;
// 		}

// 		if (last_type != type)
// 		{
// 			cerr << std::endl;
// 			cerr << " " << type << ": ";
// 		}
// 		last_type = type;

// 		cerr << lodepng_chunk_length(chunk) << ", ";

// 		next = lodepng_chunk_next_const(chunk);
// 		if (next <= chunk) { break; } // integer overflow
// 		chunk = next;
// 	}
// 	cerr << std::endl;
// }


// /*
// Show ASCII art preview of the image
// */
// void displayAsciiArt(const std::vector<unsigned char> &image, unsigned w, unsigned h)
// {
// 	if (w > 0 && h > 0)
// 	{
// 		cerr << std::endl << "ASCII Art Preview: " << std::endl;
// 		unsigned w2 = 48;
// 		if (w < w2) { w2 = w; }
// 		unsigned h2 = h * w2 / w;
// 		h2 = (h2 * 2) / 3; //compensate for non-square characters in terminal
// 		if (h2 > (w2 * 2)) { h2 = w2 * 2; } //avoid too large output

// 		cerr << '+';
// 		for (unsigned x = 0; x < w2; x++) { cerr << '-'; }
// 		cerr << '+' << std::endl;
// 		for (unsigned y = 0; y < h2; y++)
// 		{
// 			cerr << "|";
// 			for (unsigned x = 0; x < w2; x++)
// 			{
// 				unsigned x2 = x * w / w2;
// 				unsigned y2 = y * h / h2;
// 				int r = image[y2 * w * 4 + x2 * 4 + 0];
// 				int g = image[y2 * w * 4 + x2 * 4 + 1];
// 				int b = image[y2 * w * 4 + x2 * 4 + 2];
// 				int a = image[y2 * w * 4 + x2 * 4 + 3];
// 				int lightness = ((r + g + b) / 3) * a / 255;
// 				int min = (r < g && r < b) ? r : (g < b ? g : b);
// 				int max = (r > g && r > b) ? r : (g > b ? g : b);
// 				int saturation = max - min;
// 				int letter = 'i'; //i for grey, or r,y,g,c,b,m for colors
// 				if (saturation > 32)
// 				{
// 					int h = lightness >= (min + max) / 2;
// 					if (h) { letter = (min == r ? 'c' : (min == g ? 'm' : 'y')); }
// 					else { letter = (max == r ? 'r' : (max == g ? 'g' : 'b')); }
// 				}
// 				int symbol = ' ';
// 				if (lightness > 224) { symbol = '@'; }
// 				else if (lightness > 128) { symbol = letter - 32; }
// 				else if (lightness > 32) { symbol = letter; }
// 				else if (lightness > 16) { symbol = '.'; }
// 				cerr << (char)symbol;
// 			}
// 			cerr << "|";
// 			cerr << std::endl;
// 		}
// 		cerr << '+';
// 		for (unsigned x = 0; x < w2; x++) { cerr << '-'; }
// 		cerr << '+' << std::endl;
// 	}
// }


// /*
// Show the filtertypes of each scanline in this PNG image.
// */
// void displayFilterTypes(const std::vector<unsigned char> &buffer, bool ignore_checksums)
// {
// 	//Get color type and interlace type
// 	lodepng::State state;
// 	if (ignore_checksums)
// 	{
// 		state.decoder.ignore_crc = 1;
// 		state.decoder.zlibsettings.ignore_adler32 = 1;
// 	}
// 	unsigned w, h;
// 	unsigned error;
// 	error = lodepng_inspect(&w, &h, &state, &buffer[0], buffer.size());

// 	if (error)
// 	{
// 		cerr << "inspect error " << error << ": " << lodepng_error_text(error) << std::endl;
// 		return;
// 	}

// 	if (state.info_png.interlace_method == 1)
// 	{
// 		cerr << "showing filtertypes for interlaced PNG not supported by this example" <<
// 		     std::endl;
// 		return;
// 	}

// 	//Read literal data from all IDAT chunks
// 	const unsigned char *chunk, *begin, *end, *next;
// 	end = &buffer.back() + 1;
// 	begin = chunk = &buffer.front() + 8;

// 	std::vector<unsigned char> zdata;

// 	while (chunk + 8 < end && chunk >= begin)
// 	{
// 		char type[5];
// 		lodepng_chunk_type(type, chunk);
// 		if (std::string(type).size() != 4)
// 		{
// 			cerr << "this is probably not a PNG" << std::endl;
// 			return;
// 		}

// 		if (std::string(type) == "IDAT")
// 		{
// 			const unsigned char *cdata = lodepng_chunk_data_const(chunk);
// 			unsigned clength = lodepng_chunk_length(chunk);
// 			if (chunk + clength + 12 > end || clength > buffer.size()
// 			    || chunk + clength + 12 < begin) {
// 				cerr << "invalid chunk length" << std::endl;
// 				return;
// 			}

// 			for (unsigned i = 0; i < clength; i++)
// 			{
// 				zdata.push_back(cdata[i]);
// 			}
// 		}

// 		next = lodepng_chunk_next_const(chunk);
// 		if (next <= chunk) { break; } // integer overflow
// 		chunk = next;
// 	}

// 	//Decompress all IDAT data
// 	std::vector<unsigned char> data;
// 	error = lodepng::decompress(data, &zdata[0], zdata.size());

// 	if (error)
// 	{
// 		cerr << "decompress error " << error << ": " << lodepng_error_text(
// 		       error) << std::endl;
// 		return;
// 	}

// 	//A line is 1 filter byte + all pixels
// 	size_t linebytes = 1 + lodepng_get_raw_size(w, 1, &state.info_png.color);

// 	if (linebytes == 0)
// 	{
// 		cerr << "error: linebytes is 0" << std::endl;
// 		return;
// 	}

// 	cerr << "Filter types: ";
// 	for (size_t i = 0; i < data.size(); i += linebytes)
// 	{
// 		cerr << (int)(data[i]) << " ";
// 	}
// 	cerr << std::endl;

// }


// /*
// Main
// */


// void pngInfo::pngInfo(const MString &mayaFn)
// {
// 	cerr << "pngInfo: " << mayaFn << endl;
// 	std::string filename = mayaFn.asChar();
// 	bool ignore_checksums = true;

// 	std::vector<unsigned char> buffer;
// 	std::vector<unsigned char> image;
// 	std::vector<unsigned char> idximg;

// 	unsigned w, h;

// 	lodepng::load_file(buffer, filename); //load the image file with given filename


// 	lodepng::State state;
// 	if (ignore_checksums)
// 	{
// 		state.decoder.ignore_crc = 1;
// 		state.decoder.zlibsettings.ignore_adler32 = 1;
// 	}

// 	unsigned error = lodepng::decode(image, w, h, state, buffer);



// 	if (error)
// 	{
// 		cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
// 		return;
// 	}

// 	cerr << "Filesize: " << buffer.size() << " (" << buffer.size() / 1024 << "K)" <<
// 	     std::endl;
// 	cerr << "Width: " << w << std::endl;
// 	cerr << "Height: " << h << std::endl;
// 	cerr << "Num pixels: " << w *h << std::endl;

// 	if (w > 0 && h > 0)
// 	{
// 		cerr << "Top left pixel color:"
// 		     << " r: " << (int)image[0]
// 		     << " g: " << (int)image[1]
// 		     << " b: " << (int)image[2]
// 		     << " a: " << (int)image[3]
// 		     << std::endl;
// 	}


// 	displayPNGInfo(state.info_png);
// 	cerr << std::endl;
// 	displayChunkNames(buffer);
// 	cerr << std::endl;
// 	displayFilterTypes(buffer, ignore_checksums);
// 	cerr << std::endl;
// 	displayAsciiArt(image, w, h);


// 	lodepng::State idxstate;
// 	// idxstate.info_raw.colortype =  LCT_PALETTE;
// 	idxstate.decoder.color_convert = false;
// 	error = lodepng::decode(idximg, w, h, idxstate, buffer);
// 	cerr << "err:" << error << endl;
// 	if (! error) {
// 		for (int i = 0; i <  h ; i += 10)
// 		{
// 			for (int j = 0; j < w ;  j += 10)
// 			{
// 				int n =  (int)idximg[((w * i) + j)];
// 				if (n < 10) {
// 					cerr << " ";
// 				}
// 				cerr << n << " ";
// 			}
// 			cerr << endl;
// 		}
// 		cerr << endl;
// 	}






// }





// int main(int argc, char *argv[]) /*list the chunks*/
// {
// 	bool ignore_checksums = false;
// 	std::string filename = "";
// 	for (int i = 1; i < argc; i++)
// 	{
// 		if (std::string(argv[i]) == "--ignore_checksums") { ignore_checksums = true; }
// 		else { filename = argv[i]; }
// 	}
// 	if (filename == "")
// 	{
// 		cerr << "Please provide a filename to preview" << std::endl;
// 		return 0;
// 	}

// 	std::vector<unsigned char> buffer;
// 	std::vector<unsigned char> image;
// 	unsigned w, h;

// 	lodepng::load_file(buffer, filename); //load the image file with given filename

// 	lodepng::State state;
// 	if (ignore_checksums)
// 	{
// 		state.decoder.ignore_crc = 1;
// 		state.decoder.zlibsettings.ignore_adler32 = 1;
// 	}

// 	unsigned error = lodepng::decode(image, w, h, state, buffer);

// 	if (error)
// 	{
// 		cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
// 		return 0;
// 	}

// 	cerr << "Filesize: " << buffer.size() << " (" << buffer.size() / 1024 << "K)" <<
// 	          std::endl;
// 	cerr << "Width: " << w << std::endl;
// 	cerr << "Height: " << h << std::endl;
// 	cerr << "Num pixels: " << w *h << std::endl;

// 	if (w > 0 && h > 0)
// 	{
// 		cerr << "Top left pixel color:"
// 		          << " r: " << (int)image[0]
// 		          << " g: " << (int)image[1]
// 		          << " b: " << (int)image[2]
// 		          << " a: " << (int)image[3]
// 		          << std::endl;
// 	}


// 	displayPNGInfo(state.info_png);
// 	cerr << std::endl;
// 	displayChunkNames(buffer);
// 	cerr << std::endl;
// 	displayFilterTypes(buffer, ignore_checksums);
// 	cerr << std::endl;
// 	displayAsciiArt(image, w, h);
// }

