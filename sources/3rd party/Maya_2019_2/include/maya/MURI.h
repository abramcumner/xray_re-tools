#ifndef _MURI
#define _MURI
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
// CLASS:    MURI
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MURI)

//! \ingroup OpenMaya
//! \brief Manipulating Uniform Resource Identifiers.
/*!
The MURI class implements the W3 URI standard. It can parse and and construct URIs (Uniform Resource Identifiers).  
It is based on URI specification RFC 3986 (Uniform Resource Identifier: Generic Syntax).

\par
"The generic URI syntax consists of a hierarchical sequence of components referred to as the scheme, authority, 
path, query, and fragment." -Section 3 of <a href="http://www.ietf.org/rfc/rfc3986.txt">Network Working Group RFC 3986</a>
\par

\code
   foo://username:password@example.com:8042/over/there/index.dtb?type=animal;name=narwhal#nose
   \_/   \_______________/ \_________/ \__/            \___/ \_/ \______________________/ \__/
    |           |               |       |                |    |            |                |
    |       userinfo         hostname  port              |    |          query          fragment
    |    \________________________________/\_____________|____|/
   scheme                |                          |    |    |
                     authority                    path   |    |
                                                         |    |
                                           interpretable as filename
                                                              |
                                                              |
                                                   interpretable as extension

\endcode

The scheme and path components are required, although the path may be empty (no characters). 
When the authority is present, the path must either be empty or begin with a slash ("/") character. 
When the authority is not present, the path cannot begin with two slash characters ("//"). 


URI objects are often initialized using URI string values.  An MURI object can be constructed from an MString value, or set from a string using the setURI() method. 

A URI can also be constructed from its individual components using methods setScheme(), setUserName(), 
setPassword(), setHost(), setPort(), setPath(), addQueryItem() and setFragment().

Convenience methods such as setAuthority() and setUserInfo() are also available to initialize groups of related 
components.  Methods for configuring the query components of the URI are also provided: setQueryItems(), addQueryItem(), removeQueryItem() and setQueryDelimiters().

The string representation of the URI object is obtained by calling asString(). 
Individual components of the URI are accessed by calling the corresponding get methods.  
    
The MURI can be validated at any point by calling the isValid() method. 
In addition, a static method isValidURI() is available to determine if an MString represents a valid URI. 


*/
class OPENMAYA_EXPORT MURI
{

public:
                                        MURI();
                                        MURI( const MString & URI);            
                                        MURI( const MURI & other);            
                                        ~MURI();
    MURI&                               operator=(const MURI & rhs);
    MURI&                               operator=(const MString & URI);
    bool                                operator==(const MURI & rhs) const;
    bool                                operator!=(const MURI & rhs) const;


    MStatus                             copy(const MURI &other);             
	MStatus                             setURI(const MString & URI);             
    MStatus                             setScheme(const MString & scheme);
    MStatus                             setPath(const MString & path);  
    MStatus                             setFragment(const MString & fragment);
    MStatus                             setFileName(const MString & fileName);
    MStatus                             setDirectory(const MString & directory);
    //authority part
    MStatus                             setAuthority(const MString & authority);  
    MStatus                             setUserInfo(const MString & userInfo);  
    MStatus                             setUserName(const MString & userName);  
    MStatus                             setPassword(const MString & password);  
    MStatus                             setHost(const MString & host);  
    MStatus                             setPort(int port);  
    //query part
    MStatus                             addQueryItem(const MString & key, const MString & value);                
    MStatus                             setQueryDelimiters(char valueDelimiter, char pairDelimiter);                        

    MString                             asString() const;	            
    MString                             getScheme() const;
    MString                             getPath() const;
    MString                             getFragment() const;
    MString                             getFileName(bool includeExtension = true) const;
    MString                             getDirectory() const;
    //authority part
    MString                             getAuthority() const;
    MString                             getUserInfo() const;
    MString                             getUserName() const;
    MString                             getPassword() const;
    MString                             getHost() const;
    int                                 getPort() const;
    //query part    
    MStringArray                        getAllQueryItemKeys() const;
    MString                             getQueryItemValue(const MString & key) const;
    MStringArray                        getAllQueryItemValues(const MString & key) const;
    char                                getQueryValueDelimiter() const;
    char                                getQueryPairDelimiter() const;
    MStatus                             removeQueryItem(const MString & key);
    MStatus                             removeAllQueryItems(const MString & key);			

    bool                                isEmpty() const;
    bool                                isValid() const;

    MStatus                             clear();

    static bool                         isValidURI(const MString & URI);

	static const char*	className();

private:
    void * fPtr;
};

OPENMAYA_NAMESPACE_CLOSE


#endif /* __cplusplus */
#endif /* _MURI */
