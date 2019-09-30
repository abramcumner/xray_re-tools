#ifndef _MUniqueString
#define _MUniqueString
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

//
// CLASS:    MUniqueString
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS

// ****************************************************************************
// CLASS DECLARATION (MUniqueString)

//! \ingroup OpenMaya
//! \brief A non-mutable string with O(1) comparison operators.
/*!
  MUniqueString is a non-mutable string class which provides an O(1) equality 
  operator, comparison operators and hash function.

  Two MUniqueString instances are equal if and only if their underlying string
  values are equal.

  operator>, operator>=, operator< and operator<= do not compare the underlying
  string values of the MUniqueStrings.  The ordering produced by MUniqueString
  comparison methods does not necessarily match the ordering produced by typical
  string comparison operators.

  MUniqueString is not order-preserving. MString A < MString B does not
  imply MUniqueString::intern(A) < MUniqueString::intern(B).

  The hash value is not deterministically computed from the underlying string
  value.  The hash value of all <b>existing</b> MUniqueString instances are
  guaranteed to match if the underlying string values of the instances are
  equal.  It is not safe to store the hash value of an MUniqueString after it
  has been deleted.  The hash value of of a deleted MUniqueString is not
  guaranteed to match a newly created MUniqueString with the same underlying
  string value as the original deleted MUniqueString instance.
*/
class OPENMAYA_EXPORT MUniqueString
{

public:
    // These functions are slow.  They may perform synchronization.
    MUniqueString();
    MUniqueString(const MUniqueString&);
    static MUniqueString intern(const MString& s);
    ~MUniqueString();
    
    // These functions execute in constant time.
    MUniqueString(MUniqueString&&);
    MUniqueString& operator=(MUniqueString&&);
    const MString& string() const;
    size_t      hash() const;
    
    bool        operator == (const MUniqueString& other) const;
    bool        operator != (const MUniqueString& other) const;
    bool        operator >  (const MUniqueString& other) const;
    bool        operator >= (const MUniqueString& other) const;
    bool        operator <  (const MUniqueString& other) const;
    bool        operator <= (const MUniqueString& other) const;

    // These functions are linear in the number of characters in the MUniqueString.
	bool		operator == ( const MString& other ) const;
	bool		operator == ( const char * other ) const;
	bool		operator != ( const MString& other ) const;
	bool		operator != ( const char * other ) const;
	
private:
    MUniqueString(void* impl);
    void* fImpl;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MUniqueString */
