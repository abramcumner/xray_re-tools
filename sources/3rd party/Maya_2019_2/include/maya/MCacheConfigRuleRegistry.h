#ifndef _MCacheConfigRuleRegistry
#define _MCacheConfigRuleRegistry
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MCacheConfigRuleRegistry
//
// ****************************************************************************
#include <maya/MTypes.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MCacheConfigRuleRegistry)

//! \ingroup OpenMaya
//! \brief Class for configuring the caching rules
/*!
When partitioning the evaluation graph, the Evaluation Manager uses 
a set of caching rules in order to determine what gets cached.

These rules are made of a pair of one filter and one action. The following
pseudocode highlights the role of cache configuration rules, at scheduling time:

for each node
    for each rule
        if filter(node) is True
        Then 
            call action(node)
        endif

Once registered, filters can be used to create new rules through the 
cacheEvaluator command
*/
class OPENMAYA_EXPORT MCacheConfigRuleRegistry
{
public:
    static MStatus registerFilter( const MString &filterName, MCreateCacheConfigRuleFilterFunction creatorFunction );
    static MStatus unregisterFilter( const MString &filterName);

    static const char* className();

    static void setRegisteringCallableScript();
    static bool registeringCallableScript();

private:
    MCacheConfigRuleRegistry();
    ~MCacheConfigRuleRegistry();

    static bool fRegisteringCallableScript;
};

OPENMAYA_NAMESPACE_CLOSE

#endif // MCacheConfigRuleRegistry