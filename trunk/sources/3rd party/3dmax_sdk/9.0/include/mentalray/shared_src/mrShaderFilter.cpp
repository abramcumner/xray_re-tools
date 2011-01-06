/*==============================================================================

  file:     mrShaderFilter.cpp

  author:   Daniel Levesque

  created:  21feb2003

  description:

    The filter for the material/map browser, which also doubles as a PBValidator.

  modified:	


© 2003 Autodesk
==============================================================================*/

#include "mrShaderFilter.h"

#include <mentalray\imrShaderClassDesc.h>
#include <mentalray\imrPreferences.h>
#include <mentalray\imrShaderTranslation.h>

//==============================================================================
// class mrShaderFilter
//==============================================================================

mrShaderFilter::mrShaderFilter(
	unsigned int applyTypes,	// Accepted apply types, a combination of imrShaderClassDesc::ApplyFlags
	ParamType2 paramType,		// Accepted return types. Pass TYPE_MAX_TYPE to accept all types.
	bool acceptStructs			// Should shaders returning structures be accepted? (the structure has to contain a member with a valid type)
)
: m_enabled(true),
  m_applyTypes(applyTypes),
  m_paramType(paramType),
  m_acceptStructs(acceptStructs)
{
}

mrShaderFilter::~mrShaderFilter() {

}

const TCHAR* mrShaderFilter::FilterName() {

	return _T("mental ray: Shader Filter");
}

bool mrShaderFilter::Enabled() {

	return m_enabled;
}

void mrShaderFilter::Enable(bool enable) {

	m_enabled = enable;
}

void mrShaderFilter::Registered() {

	// do nothing
}

void mrShaderFilter::Unregistered() {

	// do nothing
}

bool mrShaderFilter::Include(MtlBase& mtlBase, DWORD flags) {

	// Validate using the class ID
	ClassDesc* classDesc = GetCOREInterface()->GetDllDir().ClassDir().FindClass(mtlBase.SuperClassID(), mtlBase.ClassID());
	DbgAssert(classDesc != NULL);
	if(classDesc != NULL) {
		return Include(*classDesc, flags);
	}
	else {
		return true;
	}
}

bool mrShaderFilter::Include(ClassDesc& classDesc, DWORD flags) {

	// [dl | 20may2003] Disregard the 'mr extensions active' flag.
	/*
	// If this is a mental ray shader, then never include it if the mental ray
	// custom attributes are not active.
	imrShaderClassDesc* shaderClassDesc = Get_mrShaderClassDesc(&classDesc);
	imrShaderTranslation_ClassInfo* customClassInfo = Get_imrShaderTranslation_ClassInfo(classDesc);
	if((shaderClassDesc != NULL) || (customClassInfo != NULL)) {
		imrPreferences* prefs = GetMRPreferences();
		if((prefs != NULL) && !prefs->GetMRExtensionsActive()) {
			return false;
		}
	}
	*/

	return (ValidateApplyType(classDesc) && ValidateReturnType(classDesc));
}

BOOL mrShaderFilter::Validate(PB2Value& v) {

	ReferenceTarget* refTarg = v.r;
	if((refTarg != NULL) && IsMtlBase(refTarg)) {
		MtlBase* mtlBase = static_cast<MtlBase*>(refTarg);
		return Include(*mtlBase, 0);
	}
	else {
		// Null values are acceptable
		return TRUE;
	}
}

BOOL mrShaderFilter::Validate(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex) {

	return Validate(v);
}

BaseInterface* mrShaderFilter::GetInterface(Interface_ID id) {

	if(id == MRSHADERFILTER_INTERFACEID) {
		return this;
	}
	else if(id == IMTLBROWSERFILTER_INTERFACEID) {
		return static_cast<IMtlBrowserFilter*>(this);
	}
	else {
		return PBValidator::GetInterface(id);
	}
}

bool mrShaderFilter::ValidateApplyType(ClassDesc& classDesc) {

	SClass_ID superClassID = classDesc.SuperClassID();

	if(superClassID == MATERIAL_CLASS_ID) {

		// Validate if materials are accepted
		return ((m_applyTypes & imrShaderClassDesc::kApplyFlag_MtlPhen) != 0);
	}
	else if(superClassID == TEXMAP_CLASS_ID) {

		// First, query for a possible custom apply type
		imrShaderTranslation_ClassInfo* customClassInfo = Get_imrShaderTranslation_ClassInfo(classDesc);
		if(customClassInfo != NULL) {

			unsigned int customApplyType = customClassInfo->GetApplyTypes();
			return ((customApplyType & m_applyTypes) != 0);
		}
		else {
			imrShaderClassDesc* shaderClassDesc = Get_mrShaderClassDesc(&classDesc);

			if(shaderClassDesc == NULL) {
				// Not a mr shader. Validate if texture shaders are accepted.
				return ((m_applyTypes & imrShaderClassDesc::kApplyFlag_Texture) != 0);
			}
			else {

				// Depends on the apply type of the shader
				unsigned int shaderApplyTypes = shaderClassDesc->GetApplyTypes();
				return ((shaderApplyTypes & m_applyTypes) != 0);
			}
		}
	}
	else {
		// Shouldn't occur
		DbgAssert(false);
		return false;
	}
}

bool mrShaderFilter::ValidateReturnType(ClassDesc& classDesc) {


	SClass_ID superClassID = classDesc.SuperClassID();

	if(superClassID == MATERIAL_CLASS_ID) {

		// Validate if we want a TYPE_MTL return type
		//return ValidType(TYPE_MTL);
		
		// Do not validate types for materials. Only use the apply type for validatoin.
		return true;
	}
	else if(superClassID == TEXMAP_CLASS_ID) {

		imrShaderClassDesc* shaderClassDesc = Get_mrShaderClassDesc(&classDesc);

		if(shaderClassDesc == NULL) {
			// Assume that the texture returns a color
			return ValidType(static_cast<ParamType2>(TYPE_RGBA));
		}
		else {

			// Go through the results structure
			ParamBlockDesc2* pbDesc = shaderClassDesc->GetResultPBDesc();
			if((pbDesc != NULL) && ValidateReturnType(*pbDesc)) {
				return true;
			}

			// Go through the sub-structures, is allowed
			if(m_acceptStructs) {
				Tab<ParamBlockDesc2*>& resultDescs = shaderClassDesc->GetResultPBDescs();
				int count = resultDescs.Count();
				for(int i = 0; i < count; ++i) {
					ParamBlockDesc2* pbDesc = resultDescs[i];
					if((pbDesc != NULL) && ValidateReturnType(*pbDesc)) {
						return true;
					}
				}
			}			

			return false;
		}
	}
	else {
		// Shouldn't occur
		DbgAssert(false);
		return false;
	}
}

bool mrShaderFilter::ValidType(ParamType2 type) {

	switch(m_paramType) {
	case TYPE_MAX_TYPE:
		return true;

	// Casts to more complex types are acceptable
#ifndef MAX51_MENTALRAY	// TYPE_FRGBA and TYPE_POINT4 don't exist in 5.1
	case TYPE_RGBA:
	case TYPE_FRGBA:
		return ((type == TYPE_RGBA) || (type == TYPE_FRGBA));
#else
	case TYPE_RGBA:
		return (type == TYPE_RGBA);
#endif

#ifndef MAX51_MENTALRAY	// TYPE_FRGBA and TYPE_POINT4 don't exist in 5.1
	case TYPE_POINT3:
	case TYPE_POINT4:
		return ((type == TYPE_RGBA)
			|| (type == TYPE_FRGBA)
			|| (type == TYPE_POINT3)
			|| (type == TYPE_POINT4));
	case TYPE_FLOAT:
		return ((type == TYPE_RGBA)
			|| (type == TYPE_FRGBA)
			|| (type == TYPE_POINT3)
			|| (type == TYPE_FLOAT));
#else
	case TYPE_POINT3:
		return ((type == TYPE_RGBA)
			|| (type == TYPE_POINT3));
	case TYPE_FLOAT:
		return ((type == TYPE_RGBA)
			|| (type == TYPE_POINT3)
			|| (type == TYPE_FLOAT));
#endif
		
	default:
		return (type == m_paramType);
	}
}

bool mrShaderFilter::ValidateReturnType(ParamBlockDesc2& pbDesc) {

	// Go through all the parameters of the block
	int count = pbDesc.Count();
	for(int i = 0; i < count; ++i) {
		const ParamDef& paramDef = pbDesc.GetParamDef(pbDesc.IndextoID(i));
		
		if(ValidType(paramDef.type))
			return true;
	}

	return false;
}

void mrShaderFilter::DeleteThis() {

	delete this;
}