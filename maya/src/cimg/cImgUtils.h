
#ifndef cImgUtils_H
#define cImgUtils_H

#include <math.h>

#include <maya/MFloatMatrix.h>
#include <maya/MFloatPoint.h>



class cImgUtils {

public:

	static void toImageCoords(
	  const MFloatPoint &point,
	  const MFloatMatrix &inverseProjection,
	  int w, int h,  float &x, float &y, bool clamp = true);




	static void toImageCoords(float u, float v, int w, int h,  float &x, float &y);




};


#endif


// struct {
// 	bool operator()(std::vector<unsigned char> a, std::vector<unsigned char> b) const
// 	{
// 		int spectrum = a.size();
// 		int last = spectrum - 1;
// 		for (int i = 0; i < last; ++i)
// 		{
// 			if (a[i] < b[i]) {
// 				return true;
// 			}
// 			else if (a[i] > b[i]) {
// 				return false;
// 			}
// 		}
// 		return (a[last] < b[last]);

// 	}
// } ColorCompare;


// struct {
// 	bool operator()(std::vector<unsigned char> a, std::vector<unsigned char> b) const
// 	{
// 		int spectrum = a.size();
// 		for (int i = 0; i < spectrum; ++i)
// 		{
// 			if (a[i] != b[i]) {
// 				return false;
// 			}
// 		}
// 		return true;
// 	}
// } ColorEquals;





// if (colorList.size() > 0) {
// 	cerr << "colorList[0][0]: " << int(colorList[0][0]) << endl;
// }
// std::sort(colorList.begin(), colorList.end(), ColorCompare); // 1 1 2 2 3 3 3 4 4 5 5 6 7


// for (auto &elem : idxMap) {
// 	for (int i = 0; i < spectrum; ++i)
// 	{
// 		cerr << int(elem.first[i]) << " ";
// 	}
// 	cerr << " : " << elem.second << endl;
// }



// auto last = std::unique(colorList.begin(), colorList.end(), ColorEquals);
// colorList.erase(last, colorList.end());



// cerr << "size: " << colorList.size() << endl;

// int i;
// for (auto color : colorList)
// {
// 	for (int c = 0; c < spectrum; c++)
// 	{
// 		cerr << int(color[c]) << " ";
// 	}
// 	cerr << endl;
// }

// result.assign(image);
