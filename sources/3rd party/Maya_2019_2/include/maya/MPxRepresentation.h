#ifndef _MPxRepresentation
#define _MPxRepresentation
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MString.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxRepresentation)

//! \ingroup OpenMaya MPx
//! \brief Abstract base class for user defined representations. 
/*!
  MPxRepresentation is an abstract base class that can be used to provide
  an interface and services for user defined representations.

  Representations are owned by a scene assembly node (see
  MPxAssembly).  A scene assembly node will activate one of 
  its representations; representations must therefore support
  activate and inactivate operations.

  This class can be used to implement new kinds of representations
  within Maya that behave in a similar manner to the representations
  included in the scene assembly reference Maya plugin, which uses
  MPxRepresentation as a base class for its representations.

  Note that use of this class to implement representations is not
  mandatory: it provides an interface that is convenient for dealing
  consistently with representations, as well as an inactivate()
  implementation that is widely useful (clear out the assembly).  Also
  note that MPxRepresentation is not associated with a corresponding
  Maya DAG or DG node.

  For use of scene assembly nodes, see function set MFnAssembly.
*/
class OPENMAYA_EXPORT MPxRepresentation
{
public: 

   /*----- member functions -----*/

   virtual ~MPxRepresentation();

   virtual bool activate() = 0;
   virtual bool inactivate();

   virtual MString getType() const = 0;

   MString getName() const;

   virtual bool canApplyEdits() const;

   virtual void getExternalContent( MExternalContentInfoTable& table ) const;
   virtual void setExternalContent(
      const MExternalContentLocationTable& table
   );

   // SCRIPT USE ONLY
   MPxAssembly* _getAssembly() const { return getAssembly(); }
   //

protected: 

   /*----- member functions -----*/

   MPxRepresentation(MPxAssembly* assembly, const MString& name);

	//!	USE _getAssembly() IN SCRIPT
   MPxAssembly* getAssembly() const;

private:

   /*----- member functions -----*/

   // Assignment operator prohibited, left undefined.
   MPxRepresentation& operator=(const MPxRepresentation&);

   /*----- data members -----*/

   MPxAssembly* const fAssembly;
   const MString    fName;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxRepresentation */



