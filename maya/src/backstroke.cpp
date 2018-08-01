
#include <maya/MFnNurbsCurve.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <stroke.h>
#include <backstroke.h>

// #include <paint.h>
#include "errorMacros.h"

#include "mayaMath.h"

const double epsilon = 0.0001;

BackStroke::BackStroke()	{ }

BackStroke::~BackStroke() {}


void BackStroke::travelStrokeFractions(MDoubleArray &result) const {
	std::vector<Target>::const_iterator citer = m_targets.begin();
	citer++;
	for (; std::next(citer) != m_targets.end(); citer++) {
		result.append(citer->reverseStrokeFraction());
	}
}

void BackStroke::appendTargets(const MVector &planeNormal,  MMatrixArray &result) const {
	std::vector<Target>::const_reverse_iterator citer;
	for (citer = m_targets.rbegin() ; citer != m_targets.rend(); citer++) {
		result.append(citer->matrix(planeNormal, true, m_follow));
	}
}

void BackStroke::appendTangents(MVectorArray &result) const {
	std::vector<Target>::const_reverse_iterator citer;
	for (citer = m_targets.rbegin() ; citer != m_targets.rend(); citer++) {
		result.append(citer->tangent());
	}
}

void BackStroke::appendPoints(MVectorArray &result) const {
	std::vector<Target>::const_reverse_iterator citer;
	for (citer = m_targets.rbegin() ; citer != m_targets.rend(); citer++) {
		result.append(MVector(citer->curvePoint()));
	}
}

