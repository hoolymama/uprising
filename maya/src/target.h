
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
		const MFloatMatrix &mat,
		const MFloatVector &tangent,
		float param,
		float arclength,
		float weight = 1.0);

	Target(
		const MFloatPoint &pt,
		const MFloatVector &aim,
		const MFloatVector &up,
		const MFloatVector &tangent,
		float param,
		float arclength,
		float weight = 1.0);

	~Target();

	void applyTilt(float angle);
	void applyBank(float angle);
	void applyTwist(float angle);
	void applyAxisAngleRotation(
		const MFloatVector &axis , 
		float angle);

	// void setRotation(
	// 	float tilt,
	// 	float bank,
	// 	float twist,
	// 	mayaMath::axis frontAxis,
	// 	mayaMath::axis upAxis,
	// 	bool follow=false,
	// 	bool backstroke=false);

	// void setRotation(bool follow, bool backstroke);

	void setTangent(const MFloatVector &tangent);

	const MFloatMatrix &matrix() const;

	const MFloatVector &tangent() const;
 

	// void reverseParam();

	const float &param() const;

	const float &arcLength() const;
	
	void offsetBy(const MFloatVector &offset);

	void offsetLocalZ(float dist);

	float distanceTo(const Target &other) const;

	MFloatMatrix directionMatrix(bool backstroke) const;

	MFloatPoint position(
		const MFloatMatrix &space) const;


	MFloatPoint position() const;
		
	void setPosition(const MFloatPoint &rhs);

	void setMatrix(const MFloatMatrix &rhs);

	void rotate(const MFloatPoint &pivot, const MFloatMatrix &rotation);

	MFloatVector rotation(
		MTransformationMatrix::RotationOrder order,
		MAngle::Unit unit,
		const MMatrix &space = MMatrix::identity) const;

	MFloatVector transform(const MFloatVector &rhs) const;

	void getBorderPoints(
	MFloatPoint &left,
	MFloatPoint &right,
	float width,
	bool flatBrush,
	bool displayWeightWidth = true) const;

	void setWeight(float contact);

	const float &weight() const;

	void setUV(const MFloatMatrix &inversePlaneMatrix);

	void appendUVsTo(MFloatArray &uVals, MFloatArray &vVals) const;

	// void applyGlobalTilt(const MFloatVector &gradient);

	// void applyGlobalAim(const MPoint &point);

	void setColor(const MColor &rgba);

	const MColor &color() const;

private:
	MFloatVector m_tangent; // tangent of curve that joins targets
	MFloatMatrix m_matrix;
	float m_param;	   // A parameter value for 1D  mapping purposes
	float m_arcLength; // length from start
	float m_weight;	   // used to indicate fraction of brush tip
	float m_u;		   // A parameter value for 2D  mapping purposes
	float m_v;		   // A parameter value for 2D  mapping purposes
	MColor m_color;	   // RGBA
					   // double m_tilt;		 // store these when setting rotation, just in case we have to recalc
					   // double m_bank;		 // store these when setting rotation, just in case we have to recalc
					   // double m_twist;		 // store these when setting rotation, just in case we have to recalc
};

#endif
