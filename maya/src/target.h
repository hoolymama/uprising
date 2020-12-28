
#ifndef _Target_
#define _Target_

#include <vector>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MFnNurbsCurve.h>

#include <maya/MPoint.h>

#include <maya/MObject.h>
#include <mayaMath.h>

class Target
{
public:
	Target();

	Target(
		const MMatrix &mat,
		const MVector &tangent,
		double strokeParam,
		double curveParam);

	Target(
		const MFnNurbsCurve &curveFn,
		double dist,
		double startDist,
		double strokeRange,
		double curveLength);

	Target(const MPoint &pt, const MVector &tangent, double strokeParam, double curveParam, double contact);

	~Target();

	void setRotation(double tilt, double bank, double twist, bool follow,
					 bool backstroke);

	void setRotation(bool follow, bool backstroke);

	void setTangent(const MVector &tangent);

	const MMatrix &matrix() const;

	const MVector &tangent() const;

	const double &param() const;

	void reverseParam();

	const double &curveParam() const;

	void offsetBy(const MVector &offset);

	void offsetLocalZ(double dist);

	double distanceTo(const Target &other) const;

	MMatrix directionMatrix(bool backstroke) const;

	MPoint position(const MMatrix &space = MMatrix::identity) const;

	void setPosition(const MPoint &rhs);

	void setMatrix(const MMatrix &rhs);

	void rotate(const MPoint &pivot, const MMatrix &rotation);

	MVector rotation(
		MTransformationMatrix::RotationOrder order,
		MAngle::Unit unit,
		const MMatrix &space = MMatrix::identity) const;

	MVector transform(const MVector &rhs) const;

	void getBorderPoints(
		MPoint &left,
		MPoint &right,
		double width,
		bool flat = false,
		bool displayContactWidth = true) const;

	void setContact(double contact);

	const double &contact() const;

	void setUV(const MMatrix &inversePlaneMatrix);

	void appendUVsTo(MFloatArray &uVals, MFloatArray &vVals) const;

	void applyGlobalTilt(const MFloatVector &gradient);

	void applyGlobalAim(const MPoint &point);

	void setColor(const MColor &rgba);
	
	const MColor & color() const ;



private:
	MVector m_tangent; // tangent on the curve
	MMatrix m_matrix;
	double m_param;		 // normalised length in stroke
	double m_curveParam; //normalised length in original curve
	double m_contact;	// fraction of brush tip
	double m_tilt;		 // store these when setting rotation, just in case we have to recalc
	double m_bank;		 // store these when setting rotation, just in case we have to recalc
	double m_twist;		 // store these when setting rotation, just in case we have to recalc
	float m_u;
	float m_v;
	MColor m_color;
};

#endif
