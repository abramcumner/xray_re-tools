 /**********************************************************************
 
	FILE:           INodeMentalRayProperties.h

	DESCRIPTION:    Public interface for setting and getting a node's
                    mental ray properties.

	CREATED BY:     Daniel Levesque, Discreet

	HISTORY:        created 13 December 2004

 *>	Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/
#ifndef _INODEMENTALRAYPROPERTIES__H_
#define _INODEMENTALRAYPROPERTIES__H_

#include "ifnpub.h"

class INode;

//! The interface ID for class INodeMentalRayProperties
#define NODEMENTALRAYPROPERTIES_INTERFACE Interface_ID(0x1c763921, 0x4d542752)

//==============================================================================
// Class INodeMentalRayProperties
//
//! Extension to INode for accessing mental ray properties.
//!
//! This interface class defines a set of methods for accessing mental ray properties
//! on an INode. To get a pointer to this interface from an INode, call:
//!
//!			INodeMentalRayProperties* mr_properties = Get_INodeMentalRayProperties(node);
//!
//! The properties exposed by this class to the SDK are also exposed to MaxScript
//! through the function publishing system.
//!
class INodeMentalRayProperties : public FPMixinInterface {

public:

	enum DisplacementMethod {
		kDisplacementMethod_Fine = 0,
		kDisplacementMethod_Fine_NoSmoothing = 1
	};

	//! Use this function to retrieve a pointer to the INodeMentalRayProperties interface.
	//! \param[in] node - The node on which you want to query the interface.
	//! \return A pointer to the interface. May be NULL if the interface is not supported by this release.
	static INodeMentalRayProperties* Get_INodeMentalRayProperties(INode& node);

	//! Copy properties from another interface
	//! \param[in] source - The properties to be copied.
	virtual void CopyMentalRayPropertiesFrom(const INodeMentalRayProperties& source) = 0;

	//! Gets the value of the "Use global displacement settings" flag.
	//! \return The value of the flag.
	virtual bool MRGetDisplacementUseGlobalSettings() const = 0;

	//! Sets the value fo the "Use global displacement settings" flag.
	//! \param[in] useGlobal - The value to be set.
	virtual void MRSetDisplacementUseGlobalSettings(bool useGlobal) = 0;

	//! Gets the displacement method to be used.
	//! \return The displacement method to be used.
	virtual DisplacementMethod MRGetDisplacementMethod() const = 0;  

	//! Sets the displacement method to be used.
	//! \param[in] method - The displacement method.
	virtual void MRSetDisplacementMethod(DisplacementMethod method) = 0;

	//! Gets the value of the "View-Dependent Displacement" flag.
	//! \return The value of the flag.
	virtual bool MRGetIsViewDependentDisplacement() const = 0;

	//! Sets the value of the "View-Dependent Displacement" flag.
	//! \param[in] viewDependent - The value to be set.
	virtual void MRSetIsViewDependentDisplacement(bool viewDependent) = 0;

	//! Gets the value of the "Edge Length" property.
	//! Note: this value should be interpreted as pixels if the "view-dependent" flag is ON,
	//! and as object-space units if the "view-dependent" flag is OFF.
	//! \return The value of the "Edge Length" parameter.
	virtual float MRGetDisplacementEdgeLength() const = 0;

	//! Sets the value of the "Edge Length" property.
	//! Note: this value should be interpreted as pixels if the "view-dependent" flag is ON,
	//! and as object-space units if the "view-dependent" flag is OFF.
	//! \param[in] length - The value to be set.
	virtual void MRSetDisplacementEdgeLength(float length) = 0;

	//! Gets the value of the "Max. Displace" property.
	//! \return The value of the "Max. Displace" property.
	virtual float MRGetMaxDisplace() const = 0;

	//! Sets the value of the "Max. Displace" property.
	//! \param[in] maxDisplace - The value to be set.
	virtual void MRSetMaxDisplace(float maxDisplace) = 0;

	//! Gets the value of the "Displacement Max. Level" property.
	//! \return The value of the "Displacement Max. Level" property.
	virtual unsigned short MRGetDisplacementMaxLevel() const = 0;

	//! Sets the value of the "Displacement Max. Level" property.
	//! \param[in] maxLevel - The value to bet set.
	virtual void MRSetDisplacementMaxLevel(unsigned short maxLevel) = 0;

private:

	// This version of the method is needed by the function publishing macros.
	void MRSetDisplacementMethod(int method);

public:

	// Function publishing interface	
	enum PROPERTY_ACCESSOR_ID {
		GET_DISPLACEMENTUSEGLOBALSETTINGS,
		SET_DISPLACEMENTUSEGLOBALSETTINGS,
		GET_DISPLACEMENTMETHOD,
		SET_DISPLACEMENTMETHOD,
		GET_VIEWDEPENDENTDISPLACEMENT,
		SET_VIEWDEPENDENTDISPLACEMENT,
		GET_DISPLACEMENTEDGELENGTH,
		SET_DISPLACEMENTEDGELENGTH,
		GET_MAXDISPLACE,
		SET_MAXDISPLACE,
		GET_DISPLACEMENTMAXLEVEL,
		SET_DISPLACEMENTMAXLEVEL
	};

	BEGIN_FUNCTION_MAP
		// Property accessors
		PROP_FNS(GET_DISPLACEMENTUSEGLOBALSETTINGS, MRGetDisplacementUseGlobalSettings, SET_DISPLACEMENTUSEGLOBALSETTINGS, MRSetDisplacementUseGlobalSettings, TYPE_bool);
		PROP_FNS(GET_DISPLACEMENTMETHOD, MRGetDisplacementMethod, SET_DISPLACEMENTMETHOD, MRSetDisplacementMethod, TYPE_ENUM);
		PROP_FNS(GET_VIEWDEPENDENTDISPLACEMENT, MRGetIsViewDependentDisplacement, SET_VIEWDEPENDENTDISPLACEMENT, MRSetIsViewDependentDisplacement, TYPE_bool);
		PROP_FNS(GET_DISPLACEMENTEDGELENGTH, MRGetDisplacementEdgeLength, SET_DISPLACEMENTEDGELENGTH, MRSetDisplacementEdgeLength, TYPE_FLOAT);
		PROP_FNS(GET_MAXDISPLACE, MRGetMaxDisplace, SET_MAXDISPLACE, MRSetMaxDisplace, TYPE_WORLD);
		PROP_FNS(GET_DISPLACEMENTMAXLEVEL, MRGetDisplacementMaxLevel, SET_DISPLACEMENTMAXLEVEL, MRSetDisplacementMaxLevel, TYPE_INT);
	END_FUNCTION_MAP
};

inline void INodeMentalRayProperties::MRSetDisplacementMethod(int method) {

	return MRSetDisplacementMethod(static_cast<DisplacementMethod>(method));
}

inline INodeMentalRayProperties* INodeMentalRayProperties::Get_INodeMentalRayProperties(INode& node) {

	return static_cast<INodeMentalRayProperties*>(node.GetInterface(NODEMENTALRAYPROPERTIES_INTERFACE));
}

#endif
