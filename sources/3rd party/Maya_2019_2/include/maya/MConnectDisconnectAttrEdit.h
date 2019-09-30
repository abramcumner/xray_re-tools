#ifndef _MConnectDisconnectAttrEdit
#define _MConnectDisconnectAttrEdit
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
// CLASS:    MConnectDisconnectAttrEdit
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MEdit.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MConnectDisconnectAttrEdit)

//! \ingroup OpenMaya
//! \brief Class for describing connection and disconnection edits.
/*!
  
This class is used to query information about connection and disconnection
edits. Such edits occur when a file reference or assembly is loaded and
connections are made or broken to attributes within the loaded nodes.

Please note that when a reference or assembly is unloaded, plugs
related to operations cannot be returned since the plugs themselves do
not exist. Thus for unloaded references, only the plug names may be queried
successfully.

The MItEdits class may be used to iterate over all the edits on a given
reference or assembly.

*/

class OPENMAYA_EXPORT MConnectDisconnectAttrEdit : public MEdit
{
	
public:
  class Imp;
	friend class MItEdits;
	
    virtual ~MConnectDisconnectAttrEdit();

	MPlug	srcPlug(MStatus* ReturnStatus = NULL) const;
	MPlug	dstPlug(MStatus* ReturnStatus = NULL) const;	
	MString srcPlugName(MStatus* ReturnStatus = NULL) const;
	MString dstPlugName(MStatus* ReturnStatus = NULL) const;
	bool	isConnection() const;

	virtual EditType editType(MStatus* ReturnStatus = NULL) const;
	
	static const char* className();	

private:
	MConnectDisconnectAttrEdit(const void* edit, int editType, bool isConnect);

    void createImp(const void* edit, int editType, bool isConnect);

    const Imp& getImp() const;

	bool fConnect;
    void* fImp[2];
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MConnectDisconnectAttrEdit */



