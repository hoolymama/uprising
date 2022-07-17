

#ifndef StrokeUtils_H
#define StrokeUtils_H

#include <maya/MObject.h>
#include "stroke.h"



class StrokeUtils {

public:

	static MStatus createCurve(const Stroke *stroke, MObject &curveData) ;

};


#endif

