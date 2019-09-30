#ifndef _MString
#define _MString
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
// CLASS:    MString
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MString)

//! \ingroup OpenMaya
//! \brief Manipulate strings.
/*!
  The MString class implements a string class, and is used to pass
  all string arguments to Maya API methods.

  The MString class supports localized strings.
  Use of the MString class to store and manipulate strings largely insulates
  the code from locale-dependent processing. Code that uses the class
  and its methods will in most cases operate seamlessly
  in both multibyte and single-byte environments. Knowledge of the underlying
  string storage mechanism or encoding is not required to use the class, and
  assumptions about this should not be made by the calling code.

  Unless otherwise indicated, MString methods accepting or returning string
  contents via character buffers (char *) use the convention that the
  string contents are in the locale's native multibyte encoding.
  Methods which explicitly support other common encodings and representations
  such as UTF-8 and wchar_t are also provided.
  Code that manipulates string contents should use the multibyte-safe
  methods provided by the class.
  Refer to documentation on individual MString methods for more information.

*/
class OPENMAYA_EXPORT MString
{

public:
				MString();
				MString( const char* charString );
				MString( const wchar_t* charString );
                MString( const char* charString, int charLength );
				MString( const wchar_t* charString, int wcharLength );
				MString( const MString& other );
	        	~MString();
	MString&	operator += ( const MString& other );
	MString&	operator += ( const char * other );
	MString&	operator += ( double other );
	MString&	operator += ( int other );
	MString&	operator += ( unsigned int other );
	MString&	operator += ( float other );
	MString&	operator =  ( const MString& other );
	MString&	operator =  ( const char * other );
	MString&	operator =  ( double value );
	bool		operator == ( const MString& other ) const;
	bool		operator == ( const char * other ) const;
	bool		operator != ( const MString& other ) const;
	bool		operator != ( const char * other ) const;
	MString     operator + (const MString& other ) const;
	MString     operator + (const char * other ) const;
	MString     operator + ( double value ) const;
	MStatus		set( const char * charString );
	MStatus		setUTF8( const char *utf8String );
	MStatus		setWChar( const wchar_t * wcharString );
    MStatus     set( const char * charString, int charLength );
	MStatus		setWChar( const wchar_t * wcharString, int wcharLength );
	MStatus		set( double value );
	MStatus		set( double value, int precision );
	const char*		asChar() const;
	const char*		asChar(int &charLength) const;
	const char* 	asUTF8() const;
	const char* 	asUTF8(int &utf8Length) const;
	const wchar_t*	asWChar() const;
	const wchar_t*	asWChar(int &numChars) const;
	unsigned int	length() const;
	unsigned int    numChars() const;
	void		clear();
	int			index(char) const;
	int			indexW(char) const;
	int			indexW(wchar_t) const;
	int			indexW(const MString &) const;
	int			rindex(char) const;
	int			rindexW(char) const;
	int			rindexW(wchar_t) const;
	int			rindexW(const MString &) const;

	MStatus		split(char, MStringArray&) const;
	MStatus		split(wchar_t, MStringArray&) const;
	MString		substring(int start, int end) const;
	MString		substringW(int start, int end) const;
	MString&	toUpperCase();
	MString&	toLowerCase();
	bool		isShort() const;
	short		asShort() const;
	bool		isInt() const;
	int			asInt() const;
	bool		isUnsigned() const;
	unsigned int	asUnsigned() const;
	bool		isFloat() const;
	float		asFloat() const;
	bool		isDouble() const;
	double		asDouble() const;
	MString		expandEnvironmentVariablesAndTilde( MStatus * ReturnStatus = NULL ) const;
	MString		expandFilePath( MStatus * ReturnStatus = NULL ) const;
	MStatus     format(const MString &fmt, const MStringArray& args);
	MStatus     format(const MString &fmt,
					   const MString& arg1=MString::argUnused,
					   const MString& arg2=MString::argUnused,
					   const MString& arg3=MString::argUnused,
					   const MString& arg4=MString::argUnused,
					   const MString& arg5=MString::argUnused,
					   const MString& arg6=MString::argUnused,
					   const MString& arg7=MString::argUnused,
					   const MString& arg8=MString::argUnused,
					   const MString& arg9=MString::argUnused,
					   const MString& arg10=MString::argUnused);
	MStatus		substitute(const MString& src, const MString& target);
	MStatus		substituteFirst(const MString& src, const MString& target);

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream& operator<<(std::ostream&, const MString& );
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MString operator+(const char *, const MString& );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
	struct Tapi_data {
		std::wstring  wstringValue;
		std::string   stringValue;
	} *api_data;
	//  Placeholder argument for variable argument list
	static const MString argUnused;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MString */
