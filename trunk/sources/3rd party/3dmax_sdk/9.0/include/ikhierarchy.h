/**********************************************************************
 *<
	FILE: IKHierarchy.h

	DESCRIPTION:  Geometrical representation of the ik problem. Note that
				  this file should not dependent on Max SDK, except for
				  some math classes, such as Matrix3, Point3, etc.

	CREATED BY: Jianmin Zhao

	HISTORY: created 16 March 2000

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/
#ifndef __IKHierarchy__H
#define __IKHierarchy__H

#include "maxheap.h"
#include "coreexp.h"

class IIKChainControl;

namespace IKSys {
//--------------
  class ZeroPlaneMap : public MaxHeapOperators {
  public:
	virtual Point3 operator()(const Point3& EEAxis) const =0;
	virtual ~ZeroPlaneMap() {}
  };

const Interface_ID kGoalID(0x53937e2, 0x2be92941);
const Interface_ID kHIIKGoalID(0x2497c0e, 0x376f602a);
const Interface_ID kSplineIKGoalID(0x4ee7cd9, 0x68a54886);

//! Id for ISplineIKGoal2 interface
const Interface_ID IID_SPLINE_IKGOAL2(0x163a2f78, 0xb3027de);

enum SAParentSpace {
	kSAInGoal,
	kSAInStartJoint,
	kSANotApplicable = 1000
};

// A LinkChain consists of a RootLink and a number of Links.
// A RootLink consists of a rotation plus a rigidExtend. It transforms
// like this:
//	To_Coordinate_Frame = rigidExtend * rotXYZ * From_Coordinate_Frame.
// where rotXYZ = Rot_x(rotXYZ[0]) * Rot_y(rotXYZ[1]) * Rot_z(rotXYZ[2]).
// 
// * Note that not all the x, y, and z, are degrees of freedom. Only
// Active() ones are. We put the whole rotation here so that some
// solver may choose to use it as a full rotation and then clamp the
// result to the permissible range.
// 
// * LinkMatrix(bool include_rot) returns rigidExtend if include_rot is
// false and returns the whole matrix from the From_Coordinate_Fram to
// To_Coordinate_Frame, i.e., rigidExtend*rotXYZ.rotXYZ are not all degrees of freedom. Only the active ones are. 
//
// * Matrix3& ApplyLinkMatrix(Matrix3& mat, bool) applies the LinkMatrix() to
// the input matrix from the left, i.e., mat = LinkMatrix(bool)*mat,
// and returns the reference to the input matrix.
//
// * Matrix3& RotateByAxis(Matrix3&, unsigned i) pre-applies the
// rotation about x, y, or z (corresponding to i=0,1,or 2).
// Therefore, starting with the identity matrix, mat,
//	ApplyLinkMatrix(
//		RotateByAxis(
//			RotateByAxis(
//				RotateByAxis(mat, 2),
//				1),
//			0),
//		false)
//  should equal to LinkMatrix(true).
//
  class RootLink : public MaxHeapOperators {
  public:
	RootLink():flags(7){} // x,y,z, are all active. No joint limits.
	Point3		rotXYZ;
	Point3		initXYZ;
	Point3		llimits;
	Point3		ulimits;
	Matrix3		rigidExtend;
	bool		GetActive(unsigned i) const { return flags&(1<<i)?true:false;}
	bool		GetLLimited(unsigned i) const { return flags&(1<<(i+3))?true:false;}
	bool		GetULimited(unsigned i) const { return flags&(1<<(i+6))?true:false;}
	
	CoreExport Matrix3&	RotateByAxis(Matrix3& mat, unsigned i) const;
	CoreExport Matrix3	LinkMatrix(bool include_rot) const;
	CoreExport Matrix3&	ApplyLinkMatrix(Matrix3& mat, bool include_rot) const;
	// Set methods:
	//
	CoreExport void	SetActive(unsigned i, bool s);
	CoreExport void	SetLLimited(unsigned i, bool s);
	CoreExport void	SetULimited(unsigned i, bool s);
  private:
	unsigned	flags;
  };

// A Link is a 1-dof rotation followed by a rigidExtend. The dof
// axis is specified by dofAxis. It is always active.
// 
// * LinkMatrix(true) == rigidExtend * Rotation(dofAxis, dofValue).
//   LinkMatrix(false) == rigidExtend.
//
// * Matrix3& ApplyLinkMatrix(Matrix3& mat, bool) pre-applies the
// LinkMatrix(bool) to the input matrix, mat.
//
// * A typical 3-dof (xyz) joint is decomposed into three links. z and
// y dofs don't have rigid extension, called NullLink(). Let's use
//		++o
// to denote NullLink() and
//		---o
// to denote !NullLink(). Then, a 3-dof joint will be decomposed into
// three Links, as:
//		---o++o++o
//         x  y  z
//
// * For an xyz rotation joint, if y is not active (Active unchecked),
// then y will be absorbed into the z-link, as:
//		---o---o
//         x   z
// In this case, the z-link is not NullLink(). But its length is
// zero. It is called ZeroLengh() link.
//
  class Link : public MaxHeapOperators {
  public:
	Link():rigidExtend(0),dofAxis(RotZ){}
	CoreExport ~Link();
	enum DofAxis {
	  TransX,
	  TransY,
	  TransZ,
	  RotX,
	  RotY,
	  RotZ
	};
	DofAxis		dofAxis;
	float		dofValue;
	float		initValue;
	Point2		limits;
	bool		NullLink() const {return rigidExtend?false:true;}
	bool		ZeroLength() const {
	  return NullLink() ? true :
		(rigidExtend->GetIdentFlags() & POS_IDENT) ? true : false; }
	bool		LLimited() const { return llimited?true:false; }
	bool		ULimited() const { return ulimited?true:false; }
	
	CoreExport	Matrix3		DofMatrix() const;
	CoreExport	Matrix3&	DofMatrix(Matrix3& mat) const;	
	CoreExport	Matrix3		LinkMatrix(bool include_dof =true) const;
	CoreExport	Matrix3&	ApplyLinkMatrix(Matrix3& mat, bool include_dof =true) const;
	
	// Set methods:
	//
	void		SetLLimited(bool s) { llimited = s?1:0; }
	void		SetULimited(bool s) { ulimited = s?1:0; }
	
	CoreExport	void		SetRigidExtend(const Matrix3& mat);

  private:
	Matrix3*	rigidExtend;
	byte		llimited : 1;
	byte		ulimited : 1;
  };

// A LinkChain consists of a RootLink and LinkCount() of Links.
// 
// * parentMatrix is where the root joint starts with respect to the
// world. It should not concern the solver. Solvers should derive their
// solutions in the parent space.
//
// * goal is represented in the parent space, i.e.,
//		goal_in_world = goal * parentMatrix
//
// * Bone(): The Link of index i may be a NullLink(). Bone(i) gives
// the index j so that j >= i and LinkOf(j).NullLink() is false. If j
// >= LinkCount() means that the chain ends up with NullLink().
//
// * PreBone(i) gives the index, j, so that j < i and LinkOf(j) is not
// NullLink(). For the following 3-dof joint:
//		---o++o++o---o
//            i
// Bone(i) == i+1, and PreBone(i) == i-2. Therefore, degrees of
// freedom of LinkOf(i) == Bone(i) - PreBone(i).
// 
// * A typical two bone chain with elbow being a ball joint has this
// structure:
//		---o++o++o---O
//         2  1  0   rootLink
// It has 3 links in addition to the root link.
//
// * A two-bone chain with the elbow being a hinge joint has this
// structure:
//		---o---O
//         0   rootLink
// It has one link. Geometrically, the axis of LinkOf(0) should be
// perpendicular to the two bones.
//
// * The matrix at the end effector is
//		End_Effector_matrix == LinkOf(n-1).LinkMatrix(true) * ... *
//			LinkOf(0).LinkMatrix(true) * rootLink.LinkMatrix(true).
//
// * swivelAngle, chainNormal, and defaultZeroMap concerns solvers that
// answer true to IKSolver::UseSwivelAngle().
//
// * chainNormal is the normal to the plane that is intrinsic to the
// chain when it is constructed. It is represented in the object space
// of the root joint.
//
// * A zero-map is a map that maps the end effector axis (EEA) to a
// plane normal perpendicular to the EEA. The IK System will provide a
// default one to the solver. However, a solver may choose to use its
// own.
//
// * Given the swivelAngle, the solver is asked to adjust the rotation
// at the root joint, root_joint_rotation, so that:
// (A)  EEA stays fixed
// (B)	chainNormal * root_joint_rotation
//		== zeroMap(EEA) * RotationAboutEEA(swivelAngle)
// By definition, zeroMap(EEA) is always perpendicular to EEA. At the
// initial pose, chainNormal is also guarranteed to be perpendicular 
// to zeroMap(EEA). When it is not, root_joint_rotation has to
// maintain (A) absolutely and satisfy (B) as good as it is possible.
//
  class LinkChain : public MaxHeapOperators {
  public:
	LinkChain():links(0),linkCount(0) {}
	CoreExport  LinkChain(unsigned lc);
	CoreExport  virtual		~LinkChain();
	virtual void* GetInterface(ULONG i) const { return NULL; }
	Matrix3		parentMatrix;
	RootLink	rootLink;
	const Link&	LinkOf(unsigned i) const {return links[i];}
	Link&		LinkOf(unsigned i) {return links[i];}
	unsigned	LinkCount() const { return linkCount; }
	CoreExport	int			PreBone(unsigned i) const;
	CoreExport	unsigned	Bone(unsigned i) const;

	BaseInterface* GetIKGoal() { return ikGoal; }
	void SetIKGoal(BaseInterface* ikgoal) { ikGoal = ikgoal; }
	CoreExport	void ReleaseIKGoal();
  protected:
	CoreExport	void SetLinkCount(unsigned lc);
  private:
	Link*		links;
	unsigned	linkCount;
	BaseInterface* ikGoal;
  };

//
// A convenience class that helps to iterate over the LinkChain on
// the basis of joint.
// Joint is defined as follows.
// (A) The RootLink is a rotaional joint.
// (B) A consecutive series of Link's of the same type
// (sliding v. rotational) of which only the last Link may have
// NullLink() being false.
// Steps to iterate, supposing linkChain is a LinkChain:
// * IterJoint iter(linkChain); -- make an iterator.
// * iter.InitJointAngles(); -- Set all the link variables to initial values.
// * iter.SetSkipSliding(true); -- If you want to skip sliding joints.
//		If your solver does not use sliding joint, they won't be present
// 		in the linkChain and there is no need to call it.
// * iter.Begin(true/false); -- Begin the iteration and this is the first
// 		joint. Pass true as the argument if you want the first joint to be
//		place in the world, according to linkChain.parentMatrix.
// * iter.GetJointType(); -- Is this joint rotational or sliding?
// * iter.DofCount(); -- How many degrees of freedom does it have?
// * iter.GetJointAxes(); -- Axes of each degree of freedom. It is represented
//		as char[3]. The usual xyz joint is "xyz". If this 1D of y, it is
//		"__y", 2D of xy as "_xy", etc.
// * iter.ProximalFrame(); -- The base reference frame of this joint. The
//		joint axes are relative to it.
// * iter.DistalFrame(); -- The reference frame that the joint brings the
//		base frame to.
// * iter.Pivot() -- The pivot of this joint.
// * iter.DistalEnd() -- The end point the rigid link attached to this joint.
//		If this is not the last joint, it is the pivot of the next joint.
// * iter.SetJointAngles(ang); -- Assign "ang" to joint angles. ang is a
//		Point3. If this is not a 3D joint, values are retrieved from ang
//		by name. For "_zx" joint, for instance, ang.z will be assigned to
//		the z-axis, and ang.x will be assigned to the x-axis.
//		After it is called, DistalFrame() and DistalEnd() will be updated.
// * iter.Next(); -- This brings to the next joint, if returns true. It is
//		the last joint if returns false.
// 
class IterJoint : public MaxHeapOperators {
public:
	struct JointAxes : public MaxHeapOperators {
		char& operator[](int i) { return mAxes[i]; }
		char mAxes[3];
	};
	enum JointType {
		SlidingJoint,
		RotationalJoint
	};

	IterJoint(LinkChain& lc)
		: mLinkChain(lc)
		, mSkipSlide(false)
	{}
	CoreExport	void			InitJointAngles();
				void			SetSkipSliding(bool skip) { mSkipSlide = skip; }
	CoreExport	void			Begin(bool in_world);

	CoreExport	JointType		GetJointType() const;
				int				DofCount() const { return mBegin==-1 ? 3: mEnd-mBegin; }
	CoreExport	JointAxes		GetJointAxes() const;
				const Matrix3&	ProximalFrame() const { return mMat0; }
				const Matrix3&	DistalFrame() const { return mMat; }
				Point3			Pivot() const { return mMat0.GetTrans(); }
				Point3			DistalEnd() const { return mMat.GetTrans(); }
	CoreExport	void			SetJointAngles(const Point3&);
    CoreExport	Point3			GetJointAngles() const;

	CoreExport	bool			Next();

protected:
	CoreExport	void			SkipSliding();

private:
	Matrix3		mMat0;
	Matrix3		mMat;
	LinkChain&	mLinkChain;
	bool		mSkipSlide;
	short		mBegin;
	short		mEnd;
	short		mNext;
};

// IK Goals:
//
class IIKGoal : public BaseInterface {
public:
	BaseInterface* GetInterface(Interface_ID id) { if (id == kGoalID) return this; else return BaseInterface::GetInterface(id); }
	Interface_ID	GetID() { return kGoalID; }
	LifetimeType	LifetimeControl() { return wantsRelease; }
	CoreExport void	ReleaseInterface();
	virtual			~IIKGoal() {}
};

class IHIIKGoal: public IIKGoal {
public:
	// Due to BaseInterface
	BaseInterface* GetInterface(Interface_ID id) { if (id == kHIIKGoalID) return this; else return IIKGoal::GetInterface(id); }
	Interface_ID GetID() { return kHIIKGoalID; }

	virtual bool		UseVHTarget() const =0;
	virtual float	 	SwivelAngle() const =0;
	virtual const Point3& VHTarget() const =0;
	virtual SAParentSpace SwivelAngleParent() const =0;
	virtual const Point3& ChainNormal() const =0;
	virtual const ZeroPlaneMap* DefaultZeroMap() const =0;
	virtual Matrix3& Goal() =0;
};


class ISplineIKGoal: public IIKGoal {
public:
	// Due to BaseInterface
	BaseInterface* GetInterface(Interface_ID id) { if (id == kSplineIKGoalID) return this; else return IIKGoal::GetInterface(id); }
	Interface_ID GetID() { return kSplineIKGoalID; }

	virtual float				StartParam()					const = 0;
	virtual float				EndParam()						const = 0;
	//watje
	virtual Point3				SplinePosAt(float, BOOL, BOOL = FALSE)		const = 0;
	virtual Point3				SplineTangentAt(float, BOOL)	const = 0;
	virtual const Matrix3&		Goal()							const = 0;
	virtual INode*				GetGoalNode()					const = 0;
	virtual IIKChainControl*	GetChainControl()					  = 0;
	virtual float				GetSplineLength()				const = 0;
	virtual float				TwistHStartAngle()				const = 0;
	virtual float				TwistHEndAngle()				const = 0;
	virtual INode*				StartJoint()					const =	0;
	virtual INode*				EndJoint()						const =	0;
	virtual const ZeroPlaneMap* DefaultZeroMap()				const =	0;
	virtual BOOL				IsClosed()						const = 0;
	virtual const Matrix3&		TwistParent()					const = 0;
};

//! \brief Extension to ISplineIKGoal, the interface of a spline IK goal object.

//! Interface ISplineIKGoal2 allows access to the data of a spline IK goal for 
//! one solve.  It extends interface ISplineIKGoal
// Supported by class SplineIKGoal
class ISplineIKGoal2: public ISplineIKGoal {
public:
	Interface_ID GetID() { return IID_SPLINE_IKGOAL2; }

	//! \brief Get the solve time of the IK goal 
	//! \return - the time at which the IK is being solved
	virtual const TimeValue&	GetSolveTime()	const = 0;
};

CoreExport IterJoint::JointType DofType(Link::DofAxis axis); 

}; // namespace IKSys
#endif __IKHierarchy__H
