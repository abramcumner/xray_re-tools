#ifndef _MStringResourceId
#define _MStringResourceId
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
// CLASS:    MStringResourceId
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MStringResourceId)

//! \ingroup OpenMaya
//! \brief Unique identifier for an MString Resource. 
/*!
  The MStringResourceId class implements a string lookup key to be
  used in conjunction with the MStringResource class to retrieve
  strings from externally defined string resource values.
*/
class OPENMAYA_EXPORT MStringResourceId
{

public:
	MStringResourceId  ();
	MStringResourceId  (const char* pluginId, const char *keyString, const MString &defaultValue);
	MStringResourceId  (const MStringResourceId &);           // copy constructor
	MStringResourceId& operator=(const MStringResourceId &);  // assignment operator
	bool               operator != (const MStringResourceId&) const;  // inequality operator
	bool		       operator == (const MStringResourceId&) const;  // equality operator
	~MStringResourceId ();
protected:
// No protected members

private:
	struct {
		void  *data[2];
	} api_data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MStringResourceId */
