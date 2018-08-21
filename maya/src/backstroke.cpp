
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

void BackStroke::appendTargets(MMatrixArray &result) const {
	std::vector<Target>::const_reverse_iterator citer;
	unsigned i = 0;
	for (citer = m_targets.rbegin() ; citer != m_targets.rend(); citer++, i++) {
		result.append(citer->matrix(m_planeNormal, m_profile[i], true, m_follow));
	}
}

void BackStroke::getApproachTargets(MMatrix &startApproach, MMatrix &endApproach) const {
	unsigned num = m_profile.length();
	double startHeight =  m_profile[0] + m_approachDistStart;
	double endHeight =  m_profile[(num - 1)] + m_approachDistEnd;
	std::vector<Target>::const_reverse_iterator citer = m_targets.rbegin();
	startApproach = citer->matrix(m_planeNormal, startHeight, false, m_follow);
	endApproach = m_targets.front().matrix(m_planeNormal, endHeight, false, m_follow);
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

short BackStroke::direction() const {
	return -1;
}

