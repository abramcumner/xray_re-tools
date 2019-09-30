#ifndef _MPxCacheConfigRuleFilter
#define _MPxCacheConfigRuleFilter
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MPxCacheConfigRuleFilter
//
// ****************************************************************************
#include <maya/MTypes.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxCacheConfigRuleFilter)
//! \ingroup OpenMaya MPx
//! \brief Proxy class for defining a new caching rule filter
/*!
Caching rules are defined through the cacheEvaluator command. They can be 
built using filter/action pairs. This class allows defining a custom caching 
rule filter.

See also: MCacheConfigRuleRegistry
*/
class OPENMAYA_EXPORT MPxCacheConfigRuleFilter
{
public:
    virtual	~MPxCacheConfigRuleFilter() = default;

    virtual void preRulesExecution();
    virtual void postRulesExecution();
    virtual bool isMatch(const MEvaluationNode& evalNode);
};

OPENMAYA_NAMESPACE_CLOSE

#endif // MPxCacheConfigRuleFilter