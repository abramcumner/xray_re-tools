#ifndef _MManipData
#define _MManipData
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MManipData
//
// ****************************************************************************
//
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MManipData)

//! \ingroup OpenMayaUI
//! \brief Manipulator Data 
/*!
This class encapulates manipulator data which is returned from the
manipulator conversion functions.  MManipData is used to represent
data that is either simple or complex.  Simple data is used to
represent bool, short, int, unsigned int, float, and double types.
Complex data is used to represent MObjects created by MFnData, or
classes derived from MFnData.
*/
class OPENMAYAUI_EXPORT MManipData
{
public:
	MManipData();
	~MManipData();
	MManipData(const MManipData &);
	MManipData(bool);
	MManipData(short);
	MManipData(int);
	MManipData(unsigned int);
	MManipData(float);
	MManipData(double);
	MManipData(const MObject &);

	MManipData &	operator=(const MManipData &);
	MManipData &	operator=(bool);
	MManipData &	operator=(short);
	MManipData &	operator=(int);
	MManipData &	operator=(unsigned int);
	MManipData &	operator=(float);
	MManipData &	operator=(double);
	MManipData &	operator=(const MObject &);

	bool			isSimple()		const;

	bool			asBool()		const;
	short			asShort()		const;
	int			asLong()		const;
	unsigned int		asUnsigned()	const;
	float			asFloat()		const;
	double			asDouble()		const;
	MObject			asMObject()		const;

	static const char*	className();

private:
	bool		fIsSimple;
	double		fSimpleData;
	MObject		fComplexData;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MManipData */
