#ifndef _MPxAssembly
#define _MPxAssembly

// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MPxNode.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxAssembly)

//! \ingroup OpenMaya MPx
//! \brief Parent class of all user defined assembly nodes. 
/*!
  MPxAssembly is the parent class of all user defined assembly nodes.
  User defined assemblies are DAG nodes.  An assembly allows
  activation of one of its representations.  The implementation of
  representations is not specified by the MPxAssembly API: for example,
  a representation can be a data structure internal to the
  assembly implementation, identified by string name, or
  it can be a DAG node.

  An assembly object is also a DAG container.  As such, operations on
  containers can be done on assemblies.

  This class can be used to implement new kinds of assembly nodes within
  Maya that integrate with Maya's scene assembly workflows.

  For use of assemblies, see function set MFnAssembly.  For use of
  container functionality on assemblies, see function set MFnContainerNode. 

  <h2>Deriving from MPxAssembly to Extend Scene Assembly</h2>

  To extend Maya's scene assembly, you can write a dependency graph
  plug-in with an assembly node that is derived from MPxAssembly.  The
  plug-in can be written either in C++ or in Python.  A minimal scene
  assembly node must maintain a list of representations, and be able
  to activate one of them, inactivating the previous representation in
  the process.

  An MPxAssembly derived node must be created with no representation
  active.  In other words, after the MPxAssembly derived class
  constructor executes, the getActive() method must return the empty string.
  This is to ensure support for workflows where nested assembly
  hierarchies are expanded a single level at a time.

  <h3>Fundamental Properties of Representations</h3>

  The fundamental properties of a representation are the following:
  <dl>
  <dt>Name</dt>
  <dd>The MPxAssembly API refers to representations by string names,
  so the plug-in must have a correspondence between these names and its
  own representation data structures.  The representation name is
  to be used in programming, should not be localized, and is not shown
  in the Maya UI.</dd>
  <dt>Label</dt>
  <dd>This is the representation property that is shown in the Maya UI
  to identify a representation to a user, and is the property that
  should be localized if desired.</dd>
  <dt>Type</dt>
  <dd>This representation property expresses commonality
  with other representations of the same type, and is used to control
  creation through createRepresentation().  It should not be localized.</dd>
  </dl>

  <h3>Virtual Method Overrides</h3>

  Here is a summary description of how to override
  MPxAssembly methods to implement a scene assembly node.
  The following MPxAssembly pure virtual methods must be overriden by even
  the most basic scene assembly node.
  <dl>
  <dt>createRepresentation()</dt>
  <dd>Use the type-dependent input, the representation type, and the
  given representation name to create a representation, and add it to
  the list of representations.</dd>
  <dt>getActive()</dt>
  <dd>Return the currently-active representation.</dd>
  <dt>getRepresentations()</dt>
  <dd>Return the list of representation names.</dd>
  <dt>getRepType()</dt>
  <dd>Return the type a representation.</dd>
  <dt>getRepLabel()</dt>
  <dd>Return the label of a representation.</dd>
  <dt>repTypes()</dt>
  <dd>Return the list of representation types that can be created.</dd>
  <dt>deleteRepresentation()</dt>
  <dd>Delete a representation from the list of representations.</dd>
  <dt>deleteAllRepresentations()</dt>
  <dd>Delete all representations.</dd>
  <dt>setRepName()</dt>
  <dd>Set a representation's name.</dd>
  <dt>setRepLabel()</dt>
  <dd>Set a representation's label.</dd>
  <dt>activateRep()</dt>
  <dd>Choose a representation to be active in the Maya scene, and
  inactivate the previously-active representation.</dd>
  </dl>
  All other virtual methods have default implementations in
  MPxAssembly.  Please refer to the documentation of these methods to
  determine if this default behavior is appropriate for your plug-in.

  <h3>Non-Destructive Edit Tracking</h3>

  Assembly nodes can track certain edits done on their members.
  These edits are non-destructive and recorded separately from the
  representation data onto which they are applied.  Please see the
  Maya documentation for more details.  Edits are a property of the
  assembly node, not of individual representations, and when switching
  active representations will be applied to a newly-activated
  representation if the representation can apply edits (see
  canRepApplyEdits()).

  A plug-in node can opt in to the Maya edit tracking system simply by
  overriding supportsEdits() to return true.

  To interact with edits once they have been created, see MItEdits.

  <h3>Namespace Support</h3>

  To avoid node name clashes on member nodes, each assembly node can
  optionally have a representation namespace, a namespace into which
  all nodes resulting from activating a representation will be placed.
  This representation namespace is always a child namespace of the
  namespace in which the assembly node itself has been placed.  In the
  case of nested assemblies, this will produce a hierarchy of namespaces
  that matches the hierarchy of assemblies.

  Without the use of namespaces, member node name clashes will be
  resolved by Maya adding a numerical suffix to clashing nodes.  If an
  assembly node is tracking edits on its members, this name clash
  resolution will cause edits to fail, as edits are applied by name.
  Therefore, the use of representation namespaces is very strongly
  recommended when tracking assembly member edits.

  <h3>Activate Support</h3>

  An MPxAssembly derived class can completely override the activation
  of a representation (and therefore inactivation of the
  previously-active representation) provided by activate().  However,
  MPxAssembly provides the performActivate() and performInactivate()
  non-virtual methods as building blocks from which a derived class
  activate() method can be implemented.  See the performActivate() and
  performInactivate() methods for a description of services that can
  be useful to derived class implementations of activate().

  <h3>Assembly Integration into Maya</h3>

  Once a derived assembly node type has been implemented, and
  registered into Maya's type system through MFnPlugin::registerNode(),
  it should be registered to the assembly command as an available
  assembly node type.  See the assembly command documentation for more
  details.

  <h3>MPxAssembly::inactivateRep() and undo</h3>

  The default representation inactivation code flushes the undo queue on
  activating a new representation.  This is done for performance reasons:
  undoable inactivation of the previously-active representation with a very
  large number of nodes and attribute connections (e.g. a large and
  deeply-nested hierarchy) would require an undoable delete, which is
  time-consuming (to record the operations to undo), and memory-consuming
  (as the undo data is stored in memory).  The consequence of this is that
  once a representation switch is done, any operations previously on the
  undo queue are lost.
  
  However, this behavior is under plugin control, by overriding
  MPxAssembly::inactivateRep().  If the current representation is known to
  be lightweight (e.g. in terms of number of Maya nodes), a plugin can
  bypass the default representation inactivation code, and implement its
  own inactivation using the normal undoable delete, and therefore preserve
  undo capability.

  <h2>Top-Level Assemblies versus Nested Assemblies</h2>

  Assembly nodes can be created inside a hierarchy of assemblies.  An
  assembly can be nested inside another assembly; the nested assembly is
  a DAG child of its parent assembly. MFnAssembly::getParentAssembly() 
  and MFnAssembly::getSubAssemblies() can be used to efficiently
  navigate the hierarchy of assembly nodes.  Nested assemblies are
  created through the activation of a representation on their parent
  assembly.

  An assembly node that does not have an assembly parent is called a
  top-level assembly.  It is created through file I/O (either opening
  a file, or importing a file at the top level), or through scripting.
  Top-level assemblies have special properties:
  <ul>
  <li>Top-level assemblies are saved as part of the Maya file being
  edited (the top-level Maya file).  Nested assemblies are not saved in the
  top-level Maya file.  They are created by their parent assembly, and
  are saved elsewhere, or created procedurally by their parent's
  activation code.
  <li>Top-level assemblies collect and store all non-destructive edits
  made to nodes under their DAG hierarchy while editing the top-level
  Maya file.  Nested assemblies can contain edits which will be
  applied to nodes under their DAG hierarchy, but these edits are
  read-only and cannot be changed or deleted, only overridden.  In
  contrast, edits on top-level assemblies can be added and deleted.
  In a hierarchy of assembly nodes, the application of edits is also
  hierarchical: edits from leaf assemblies are applied first, followed
  by parent edits, and so on, ending at the top-level assemblies.
  When edits are made to the same Maya data structure from multiple
  levels in the hierarchy, parent edits override child edits.
  <li>Top-level assemblies created through scripting are not
  initialized by Maya through postLoad().  Nested assemblies are
  always initialized by Maya through postLoad(), and top-level
  assemblies created by file I/O are initialized by Maya, but
  top-level assemblies created through scripting are not, and the
  script should call postLoad() on the assembly, if required.
  </ul>

*/
class OPENMAYA_EXPORT MPxAssembly : public MPxNode
{
public:

   MPxAssembly();
   virtual ~MPxAssembly();
   virtual MPxNode::Type type() const;

   virtual MString createRepresentation(
      const MString& input,
      const MString& type,
      const MString& representation,
      MDagModifier*  undoRedo = NULL,
      MStatus*       ReturnStatus = NULL
   ) = 0;
   
   virtual bool         activate				(const MString& rep);
   virtual MString      getActive				() const									= 0;
   virtual bool         isActive				(const MString& rep) const;
   virtual MStringArray getRepresentations		(MStatus* ReturnStatus = NULL) const		= 0;
   virtual MString      getRepType				(const MString& rep) const					= 0;
   virtual MString      getRepLabel				(const MString& rep) const					= 0;
   virtual MStringArray repTypes				() const									= 0;
   virtual MStatus      deleteRepresentation	(const MString& rep)						= 0;
   virtual MStatus      deleteAllRepresentations()											= 0;
   virtual MString      getRepNamespace			() const;
   virtual void         updateRepNamespace		(const MString& repNamespace);
   virtual MString      setRepName				(const MString& rep, const MString& newName,
		   										 MStatus* ReturnStatus = NULL)				= 0;
   virtual MStatus      setRepLabel				(const MString& rep, const MString& label ) = 0;

   virtual bool         supportsEdits			() const;
   virtual bool         supportsMemberChanges	() const;
   virtual bool         canRepApplyEdits		(const MString& representation) const;

   virtual bool     handlesAddEdits	() const;
   virtual MStatus  addEdits		();

   virtual void     beforeSave	();
   virtual void     postLoad	();

   virtual void     memberAdded		(MObject& member);
   virtual void     memberRemoved	(MObject& member);

   bool             performActivate		(const MString& representation);
   bool             performInactivate	();

   virtual bool     activateRep		(const MString& representation) = 0;
   virtual bool     inactivateRep	();

   virtual void		postActivateRep	(const MString& representation);

   virtual void     preApplyEdits	();
   virtual void     preUnapplyEdits	();

   virtual void     postApplyEdits	();
   virtual void     postUnapplyEdits();

   virtual MString  getInitialRep(const MObject &assembly, bool& hasInitialRep, MStatus* ReturnStatus=NULL) const;

   static const char* className();

   void* getInstancePtr();
   void  setInstancePtr(void* ptr);

   bool  activating() const;
	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// DO NOT OVERRIDE in your derived class
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	// The following methods were made virtual in MPxNode and overridden
	// here as a temporary workaround for an internal problem. This will be
	// removed in the very near future.
	// [07/2012]
	// 
	MTypeId		typeId		() const /*override final*/;
	MString		typeName	() const /*override final*/;
	MString		name		() const /*override final*/;
	MObject     thisMObject	() const /*override final*/;
	MStatus		setExistWithoutInConnections	( bool flag ) /*override final*/;
	bool		existWithoutInConnections		( MStatus* ReturnStatus = NULL ) const /*override final*/;
	MStatus		setExistWithoutOutConnections	( bool flag ) /*override final*/;
	bool		existWithoutOutConnections		( MStatus* ReturnStatus = NULL ) const /*override final*/;


   MStatus addSetAttrEdit( const MString& targetAssembly,
						   const MString& plugName,
						   const MString& parameters,
						   MPxEditData *editData=NULL
					     );
   MStatus addConnectAttrEdit( const MString& targetAssembly,
							   const MString& srcPlugName,
							   const MString& dstPlugName,
							   MPxEditData *editData=NULL
						     );
   MStatus addDisconnectAttrEdit( const MString& targetAssembly,
								  const MString& srcPlugName,
								  const MString& dstPlugName,
								  MPxEditData *editData=NULL
							    );
   MStatus addDeleteAttrEdit( const MString& targetAssembly,
							  const MString& nodeName,
							  const MString& attributeName,
							  MPxEditData *editData=NULL
						    );
   MStatus addAddAttrEdit( const MString& targetAssembly,
						   const MString& nodeName,
						   const MString& longAttributeName,
						   const MString& shortAttributeName,
						   const MString& parameters,
						   MPxEditData *editData=NULL
					     );
   MStatus addParentEdit( const MString& targetAssembly,
						  const MString& childNodeName,
						  const MString& parentNodeName,
						  const MString& parameters,
						  MPxEditData *editData=NULL
					    );


BEGIN_NO_SCRIPT_SUPPORT:
protected:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// DO NOT OVERRIDE in your derived class
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	// The following methods made virtual as a temporary workaround for
	// an internal problem. This will be removed in the very near future.
	// [07/2012]
	// 
	virtual MDataBlock	forceCache		() /*override final*/;
	virtual MStatus		setDoNotWrite	( bool flag ) /*override final*/;
	virtual bool		doNotWrite		( MStatus *ReturnStatus = nullptr ) const /*override final*/;
END_NO_SCRIPT_SUPPORT:

protected:

	OPENMAYA_DEPRECATED(2019, "Override schedulingType() to set thread safety instead.")
	virtual void		setMPSafe		( bool flag ) /*override final*/;

	OPENMAYA_DEPRECATED(2018, "Call getInitialRep(const MObject &, bool& hasInitialRep, MStatus*) instead.")
	virtual MString  getInitialRep(const MObject &assembly,MStatus* ReturnStatus=NULL) const;

};

OPENMAYA_NAMESPACE_CLOSE

#endif // _MPxAssembly
