#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>
#include <maya/MFnNurbsCurveData.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnMatrixData.h>

#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>

#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnStringData.h>

#include "funCurve.h"

#include <jMayaIds.h>
#include <attrUtils.h>

#include "mayaMath.h"

MObject funCurve::aParameterRange;
MObject funCurve::aParameterRangeMin;
MObject funCurve::aParameterRangeMax;

MObject funCurve::aSamples;

MObject funCurve::aSubSampleRange;
MObject funCurve::aSubSampleRangeBegin;
MObject funCurve::aSubSampleRangeEnd;

MObject funCurve::aCoefficients;
MObject funCurve::aCoefficientName;
MObject funCurve::aCoefficientValue;

MObject funCurve::aPreCommand;

MObject funCurve::aExpression;

MObject funCurve::aMinimumEPDistance;

MObject funCurve::aOutput;

MTypeId funCurve::id(k_funCurve);

funCurve::funCurve() {}

funCurve::~funCurve() {}

void *funCurve::creator()
{
	return new funCurve();
}

MStatus funCurve::initialize()
{
	MStatus st;
	MString method("funCurve::initialize");

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnCompoundAttribute cAttr;

	MFnMatrixAttribute mAttr;

	aParameterRangeMin = nAttr.create("parameterRangeMin", "prn", MFnNumericData::kDouble);
	nAttr.setDefault(0.0);
	aParameterRangeMax = nAttr.create("parameterRangeMax", "prx", MFnNumericData::kDouble);
	nAttr.setDefault(10.0);
	aParameterRange = nAttr.create("parameterRange", "pr", aParameterRangeMin, aParameterRangeMax);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	st = addAttribute(aParameterRange);
	mser;

	aSamples = nAttr.create("samples", "smp", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	nAttr.setDefault(100);
	st = addAttribute(aSamples);

	aSubSampleRangeBegin = nAttr.create("subSampleRangeBegin", "ssrn", MFnNumericData::kInt);
	nAttr.setDefault(0);
	nAttr.setMin(0);
	
	aSubSampleRangeEnd = nAttr.create("subSampleRangeEnd", "ssrx", MFnNumericData::kInt);
	nAttr.setDefault(-1);
	aSubSampleRange = nAttr.create("subSampleRange", "ssr", aSubSampleRangeBegin, aSubSampleRangeEnd);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	st = addAttribute(aSubSampleRange);
	mser;

	aCoefficientName = tAttr.create("coefficientName", "cfn", MFnData::kString);
	tAttr.setStorable(true);

	aCoefficientValue = nAttr.create("coefficientValue", "cfv", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	nAttr.setDefault(100);

	aCoefficients = cAttr.create("coefficients", "cf");
	cAttr.addChild(aCoefficientName);
	cAttr.addChild(aCoefficientValue);
	cAttr.setArray(true);
	cAttr.setIndexMatters(false);
	st = addAttribute(aCoefficients);

	MFnStringData stringData;

	aPreCommand = tAttr.create("preCommand", "pcmd", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setDefault(stringData.create("import math;\nimport sys;\nimport os"));
	st = addAttribute(aPreCommand);

	// Default expression creates a sphere
	MString defaultExpression("rad = math.sin(tn * math.pi) * 0.5 * radius\n");
	defaultExpression += "x = math.sin(2.0 * math.pi * t) * rad\n";
	defaultExpression += "y = math.cos(2.0 * math.pi * t) * rad\n";
	defaultExpression += "z = math.cos(tn * math.pi)  * 0.5 * radius\n";
	aExpression = tAttr.create("expression", "exp", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setDefault(stringData.create(defaultExpression));
	st = addAttribute(aExpression);


	aMinimumEPDistance = nAttr.create("minimumEPDistance", "mnep", MFnNumericData::kDouble);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.001);
	st = addAttribute(aMinimumEPDistance);



	aOutput = tAttr.create("output", "out", MFnData::kNurbsCurve);
	tAttr.setReadable(true);
	st = addAttribute(aOutput);

	st = attributeAffects(aParameterRange, aOutput);
	st = attributeAffects(aParameterRangeMin, aOutput);
	st = attributeAffects(aParameterRangeMax, aOutput);

	st = attributeAffects(aSamples, aOutput);
	st = attributeAffects(aSubSampleRange, aOutput);
	st = attributeAffects(aSubSampleRangeBegin, aOutput);
	st = attributeAffects(aSubSampleRangeEnd, aOutput);

	st = attributeAffects(aCoefficients, aOutput);
	st = attributeAffects(aCoefficientName, aOutput);
	st = attributeAffects(aCoefficientValue, aOutput);

	st = attributeAffects(aPreCommand, aOutput);
	st = attributeAffects(aExpression, aOutput);

	st = attributeAffects(aMinimumEPDistance, aOutput);
	


	return (MS::kSuccess);
}

MStatus funCurve::createCurve(const MPointArray &points, MObject &curveData, double minimumPointDistance)
{
	MStatus st;

	MFnNurbsCurveData dataCreator;
	curveData = dataCreator.create(&st);
	msert;
	MFnNurbsCurve curveFn;

	MPointArray curvePoints;
	curvePoints.clear();
	MPoint lastPoint;
	if (minimumPointDistance > 0.0)
	{
		curvePoints.append(points[0]);
		lastPoint=points[0];
		for (unsigned int i = 1; i < points.length(); i++)
		{
			if ((points[i] - lastPoint).length() > minimumPointDistance)
			{
				curvePoints.append(points[i]);
				lastPoint = points[i];
			}
		}
	}
	else
	{
		curvePoints = points;
	}
	if (curvePoints.length() < 2)
	{
		MGlobal::displayError("Not enough points to create curve");
		return MS::kFailure;
	}


	curveFn.createWithEditPoints(curvePoints, 3, MFnNurbsCurve::kOpen, false, false, false, curveData, &st);
	msert;

	return MS::kSuccess;
}

MStatus funCurve::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus st;
	if (plug != aOutput)
	{
		return (MS::kUnknownParameter);
	}
	
	MDataHandle hParameterRange = data.inputValue(aParameterRange);
	double g_p0 = hParameterRange.child(aParameterRangeMin).asDouble();
	double g_p1 = hParameterRange.child(aParameterRangeMax).asDouble();

 
	if (g_p0 > g_p1)
	{
		double tmp = g_p0;
		g_p0 = g_p1;
		g_p1 = tmp;
	}
	double g_pr = g_p1 - g_p0;

	///////////////////////////////////////////////
	int samples = data.inputValue(aSamples).asInt();
	samples = std::min( std::max(samples, 2), 1000000);

	MDataHandle hSubSampleRange = data.inputValue(aSubSampleRange);
	int subSampleRangeBegin = hSubSampleRange.child(aSubSampleRangeBegin).asInt();
	int subSampleRangeEnd = hSubSampleRange.child(aSubSampleRangeEnd).asInt();
	if (subSampleRangeEnd < 2 || subSampleRangeEnd > samples)
	{
		subSampleRangeEnd = samples;
	} 

	if (subSampleRangeBegin > subSampleRangeEnd)
	{
		int tmp = subSampleRangeBegin;
		subSampleRangeBegin = subSampleRangeEnd;
		subSampleRangeEnd = tmp;
	}

	if ((subSampleRangeEnd - subSampleRangeBegin) < 2)
	{
		subSampleRangeBegin = 0;
		subSampleRangeEnd = samples;
	}
	///////////////////////////////////////////////
	MStringArray context;
	generateContext(data, g_p0, g_p1, samples, context);

	// SET UP GLOBAL CONTEXT
	// Any variable name that starts with g_ is considered global.
	MString globalContext;
	for (int i = 0; i < context.length(); ++i)
	{
		if (context[i].substring(0,1) == "g_")
		{
			globalContext += context[i] +"\n";
		} 
	}
	if (globalContext.length() > 2)
	{
		// cerr << "Exec global context" << endl;
		// cerr << globalContext << endl;
		MGlobal::executePythonCommand(globalContext);
		// cerr << "-----------------------" << endl;
	}
	// RUN PRE COMMANDS
	// Global variables above may be used in the pre command.
	MString preCommand = data.inputValue(aPreCommand).asString();
	MGlobal::executePythonCommand(preCommand);
	// cerr << "preCommand:" << endl;
	// cerr << preCommand << endl;
	// cerr << "-----------------------" << endl;

	//////////////////////////////////////////////////////////////////////////

	MString expression = data.inputValue(aExpression).asString();
	MString fun = generateFunction(context, expression, "funCurveExpr");

	cerr << "fun: " << fun << endl << endl;
	// Register the function
	MGlobal::executePythonCommand(fun);
	// cerr << "-----------------------" << endl;

	MPointArray points;
	points.clear();
	// cerr << "subSampleRangeBegin: " << subSampleRangeBegin << endl;
	// cerr << "subSampleRangeEnd: " << subSampleRangeEnd << endl;
	for (unsigned i=subSampleRangeBegin ; i<subSampleRangeEnd; i++)
	{
		MString cmd ;
		double parameter = g_p0 + g_pr * i / double(samples-1);
		// cerr << "sample: " << i << " -- parameter" << parameter << endl;
		MDoubleArray xyz(3);
		cmd = "funCurveExpr("; cmd += parameter; cmd+=",";cmd+=i, cmd += ")";
		MGlobal::executePythonCommand(cmd, xyz);

		points.append(MPoint(xyz[0], xyz[1], xyz[2]));
	}
	// cerr << "create curve: " << endl;
	MObject curveObject;
	double minimumEPDistance = data.inputValue(aMinimumEPDistance).asDouble();
	funCurve::createCurve(points, curveObject, minimumEPDistance);
	// JPMDBG
	// // output
	// ////////////////////////////////////////////////////
	MDataHandle hOut = data.outputValue(aOutput);
	hOut.set(curveObject);
	// JPMDBG
	st = data.setClean(plug);

	////////////////////////////////////////////////////

	return MS::kSuccess;
}

void funCurve::generateContext(MDataBlock &data,  double g_p0, double g_p1, int samples, MStringArray &context)
{
	context.clear();
	//////////////////////////////////////////////////////////////////////////
	MArrayDataHandle ha = data.inputArrayValue(aCoefficients);
	int numInputs = ha.elementCount();
	MString entry;
	for (unsigned i = 0; i < numInputs; i++, ha.next())
	{
		MDataHandle h = ha.inputValue();
		MString name = h.child(aCoefficientName).asString() ;
		float value = h.child(aCoefficientValue).asFloat();
		entry = name;
		entry += " = ";
		entry += value;
		context.append(entry);
	}
	entry = "g_p0 = ";
	entry += g_p0;
	context.append(entry);
	entry = "g_p1 = ";
	entry += g_p1;
	context.append(entry);
	entry = "g_pr = ";
	entry += g_p1-g_p0;
	context.append(entry);
	entry = "g_s = ";
	entry += samples;
	context.append(entry);
	entry = "tn = t / g_pr";
	context.append(entry);

}
 

MString funCurve::generateFunction(const MStringArray & context, const MString & expression, const MString & functionName)
{
	MString f = "def "+functionName+"(t,i):\n";
	for (unsigned i=0; i<context.length(); i++)
	{
		// local variables from context only
		if (context[i].substring(0,1) != "g_")
		{
			f += "\t"+ context[i]+ "\n";
		}
	}

	f += "\tx=y=z=0.0\n";

	MStringArray lines;
	expression.split('\n', lines);

	for (unsigned i=0; i<lines.length(); i++)
	{
		f += "\t" + lines[i] + "\n";
	}

	f += "\treturn [x,y,z]\n";
	return f;
}

 


void funCurve::postConstructor()
{
}
