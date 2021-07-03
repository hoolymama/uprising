#ifndef particleCache_H
#define particleCache_H
#include <list>
#include <maya/MObject.h>
 
#include <maya/MFnDependencyNode.h>
 

#include <maya/MDataBlock.h>

#include <maya/MFnIntArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFloatPointArray.h>
#include <maya/MColorArray.h>
 
#include <maya/MVectorArray.h>
#include "errorMacros.h"

class particleTrail {

public:
	particleTrail()
		: m_positions(), m_colors() {  }

	particleTrail(MFloatPointArray p, MColorArray c)
		: m_positions(p),  m_colors(c) {	}


	void append(const MFloatPoint &p, const MColor &c)
	{
		m_positions.append(p);
		m_colors.append(c);
	}

private:
	MFloatPointArray m_positions;
	MColorArray m_colors;

	friend ostream &operator<<(ostream &os, const particleTrail &p)
	{
		os << "[" << p.m_positions << ", " << p.m_colors << "]" ;
		return os;
	}

};




class particleCache{

public:

	particleCache();

	~particleCache();

	void clear();
 
	MStatus  add(
		double dt ,
		const  MObject &node,
		MDataBlock &data
	);

	const std::vector<particleTrail>& particlesTrails() const;

private:

	MIntArray getIntArray( const  MString & name, const MObject &node,MDataBlock& data, unsigned len, MStatus *st=0) const;
	MDoubleArray getDoubleArray( const  MString & name, const MObject &node,MDataBlock& data, unsigned len, MStatus *st=0) const;
	MVectorArray getVectorArray( const  MString & name, const MObject &node,MDataBlock& data, unsigned len, MStatus *st=0) const;
	
	std::vector<particleTrail> m_particleTrails;
 
};


inline  MIntArray particleCache::getIntArray(
	const  MString & name,
	const MObject &node,
	MDataBlock& data,
	unsigned len,
	MStatus * st) const
{
	MFnDependencyNode depNode(node, st); 
	if (*st != MS::kSuccess) return MIntArray();
	MObject att = depNode.attribute(name, st);
	if (*st != MS::kSuccess) return MIntArray();
	MIntArray arr =  MFnIntArrayData(data.inputValue(att).data()).array();
	if (arr.length() != len) {
		*st = MS::kUnknownParameter;
		return MIntArray();
	}
	*st = MS::kSuccess;
	return arr;
}


inline  MDoubleArray particleCache::getDoubleArray(
	const  MString & name,
	const MObject &node,
	MDataBlock& data,
	unsigned len,
	MStatus * st) const
{
	MFnDependencyNode depNode(node, st); 
	if (*st != MS::kSuccess) return MDoubleArray(len, 0.0);
	MObject att = depNode.attribute(name, st);
	if (*st != MS::kSuccess) return MDoubleArray(len, 0.0);
	MDoubleArray arr =  MFnDoubleArrayData(data.inputValue(att).data()).array();
	if (arr.length() != len) {
		*st = MS::kUnknownParameter;
		return MDoubleArray(len, 0.0);
	}
	*st = MS::kSuccess;
	return arr;
}


inline  MVectorArray particleCache::getVectorArray(
	const  MString & name,
	const MObject &node,
	MDataBlock& data,
	unsigned len,
	MStatus * st) const
{
	MFnDependencyNode depNode(node, st); 
	if (*st != MS::kSuccess) return MVectorArray(len, MVector::zero);
	MObject att = depNode.attribute(name, st);
	if (*st != MS::kSuccess) return MVectorArray(len, MVector::zero);
	MVectorArray arr =  MFnVectorArrayData(data.inputValue(att).data()).array();
	// if (name == "leftFootFeed" || name == "rightFootFeed") {
	// 	cerr << "NAME: "<< name << " -- LEN: " << arr.length() << endl;
	// }
	if (arr.length() != len) {
		*st = MS::kUnknownParameter;
		return MVectorArray(len, MVector::zero);
	}
	*st = MS::kSuccess;
	return arr;
}





#endif

