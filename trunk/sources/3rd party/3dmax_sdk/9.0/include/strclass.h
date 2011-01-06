/**********************************************************************
 *<
	FILE: strclass.h

	DESCRIPTION:

	CREATED BY: Dan Silva

	HISTORY:

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/

#ifndef __STRCLASS__H
#define __STRCLASS__H
#include <windows.h>
#include "maxheap.h"
#include "strbasic.h"
#include "tab.h"

/*-------------------------------------------------------------------------------
  SR NOTE64

  "Only" supports 2G strings.

-------------------------------------------------------------------------------*/ 

//-----------------------------------------------------------------------
// CStr: Simple char string class
//-----------------------------------------------------------------------
class CStr: public MaxHeapOperators {
	char *buf;
	public:
		UtilExport CStr(); 
		UtilExport CStr(const char *cs);
		UtilExport CStr(const mwchar_t *wcstr);
		UtilExport CStr(const CStr& ws);
		UtilExport ~CStr(); 
		UtilExport char *data();
		UtilExport const char *data() const;
		UtilExport operator char *();

		// realloc to nchars (padding with blanks)
		UtilExport void Resize(int nchars);

		UtilExport int Length() const;
		int length() const { return Length(); }
		BOOL isNull() const { return Length()==0?1:0; }

		UtilExport CStr & operator=(const CStr& cs);
		UtilExport CStr & operator=(const mwchar_t *wcstr);
		UtilExport CStr & operator=(const char *cs);

		// Concatenation operators.
		UtilExport CStr operator+(const CStr& cs) const;
		UtilExport CStr& operator+=(const CStr& cs); 
		CStr& Append(const CStr& cs)  { return ((*this) += cs); }
		CStr& append(const CStr& cs)  { return ((*this) += cs); }
		UtilExport CStr& remove(int pos);	// remove all chars from pos to end
		UtilExport CStr& remove(int pos, int N);	// remove N chars from pos to end

		// Substring operator
		UtilExport CStr Substr(int start, int nchars) const;
		UtilExport char& operator[](int i);
		UtilExport char operator[](int i) const;	// SR NOTE64: was const char&, which is slower but would also confuse 64 builds.

		// Char search:(return -1 if not found)
		UtilExport int first(char c) const;
		UtilExport int last(char c) const;

		// Comparison
		UtilExport int operator==(const CStr &cs) const;
		UtilExport int operator!=(const CStr &cs) const;
		UtilExport int operator<(const CStr &cs) const;
		UtilExport int operator<=(const CStr &ws) const;
		UtilExport int operator>(const CStr &ws) const;
		UtilExport int operator>=(const CStr &ws) const;

		UtilExport void toUpper();
		UtilExport void toLower();

		/**
		 * \brief Write a formatted string into this CStr.
		 *
		 * Writes the format string, filled in by the optional arguments into
		 * this CStr.  See the ISO C++ documentation for more information on 
		 * printf and format strings.
		 *
		 * \param format Specifies how to format the destination string.
		 * \param ... optional arguments to format into the destination string.
		 * \pre format is not null.
		 * \pre There are the correct number of elliptical arguments to fill the 
		 * format string.
		 * \post This string is replaced with the formatted string.
		 * \return The number of characters written to this string, not including
		 * the null terminating character, or a negative value if an error occurs.
		 */
		UtilExport int printf(const char *format, ...);
	};


//-----------------------------------------------------------------------
// WStr: Simple Wide char string class
//-----------------------------------------------------------------------
class WStr: public MaxHeapOperators {
	mwchar_t *buf;
	public:
		UtilExport WStr();
		UtilExport WStr(const char *cs);
		UtilExport WStr(const mwchar_t *wcstr);
		UtilExport WStr(const WStr& ws);
		UtilExport ~WStr();
		UtilExport mwchar_t *data();
		UtilExport const mwchar_t *data() const;
		UtilExport operator mwchar_t *();

		// realloc to nchars (padding with blanks)
		UtilExport void Resize(int nchars);
		UtilExport int Length() const;
		int length() const { return Length(); }
		BOOL isNull() const { return Length()==0?1:0; }

		UtilExport WStr & operator=(const WStr& ws);
		UtilExport WStr & operator=(const mwchar_t *wcstr);
		UtilExport WStr & operator=(const char *cstr);

		// Concatenation operators.
		UtilExport WStr operator+(const WStr& ws) const; 
		UtilExport WStr & operator+=(const WStr& ws); 
		WStr& Append(const WStr& ws) { return ((*this) += ws); }
		WStr& append(const WStr& ws)  { return ((*this) += ws); }
		UtilExport WStr& remove(int pos);	// remove chars from pos to end
		UtilExport WStr& remove(int pos, int N);	// remove N chars from pos to end

		// Substring operator
		UtilExport WStr Substr(int start, int nchars) const;
		mwchar_t& operator[](int i) {DbgAssert((buf != NULL) && (i >= 0) && (i <= Length())); return buf[i];}
		mwchar_t operator[](int i) const {DbgAssert((buf != NULL) && (i >= 0) && (i <= Length())); return buf[i];}

		// Char search:(return -1 if not found)
		UtilExport int first(mwchar_t c) const;
		UtilExport int last(mwchar_t c) const;

		// Comparison
		UtilExport int operator==(const WStr &ws) const;
		UtilExport int operator!=(const WStr &ws) const;
		UtilExport int operator<(const WStr &ws) const;
		UtilExport int operator<=(const WStr &ws) const;
		UtilExport int operator>(const WStr &ws) const;
		UtilExport int operator>=(const WStr &ws) const;

		UtilExport void toUpper();
		UtilExport void toLower();

		/**
		 * \brief Write a formatted string into this WStr.
		 *
		 * Writes the format string, filled in by the optional arguments into
		 * this WStr.  See the ISO C++ documentation for more information on 
		 * printf and format strings.
		 *
		 * \param format Specifies how to format the destination string.
		 * \param ... optional arguments to format into the destination string.
		 * \pre format is not null.
		 * \pre There are the correct number of elliptical arguments to fill the 
		 * format string.
		 * \post This string is replaced with the formatted string.
		 * \return The number of characters written to this string, not including
		 * the null terminating character, or a negative value if an error occurs.
		 */
		UtilExport int printf(const mwchar_t *format, ...);
	};					

#ifdef _UNICODE
#define TSTR WStr
#else
#define TSTR CStr
#endif

// MSTR will use the Max character type.
typedef CStr    MSTR;

//--FilterList----------------------------------------------------------------------
// A class whose sole purpose is for buildingup a  filter list to passing to
// GetSaveFileName and GetOpenFileName.  It automatically puts in the imbedded nulls
// and two terminating nulls.
//	 Example:
//
//	FilterList filterList;
//	filterList.Append( _T("Max files(*.max)"));
//	filterList.Append( _T("*.max"));
//	ofn.lpstrFilter  = filterList;
//	GetSaveFileName(&ofn)
//----------------------------------------------------------------------------------

class FilterList: public MaxHeapOperators {
    Tab<MCHAR> buf;
	public:
		UtilExport FilterList();
		UtilExport void Append(MCHAR *name);
		UtilExport void Append(FilterList& filters);
		UtilExport operator MCHAR *(); 
	};


/*------------------------------------------------ 
	Split filename "name" into 
	p  path
	f  filename
	e  extension 
-------------------------------------------------*/

UtilExport void SplitFilename(TSTR& name,TSTR* p, TSTR* f, TSTR* e);

/*--------------------------------------------------
Split filename "name" into 
	p  path
	f  filename.ext
-------------------------------------------------*/

UtilExport void SplitPathFile(TSTR& name,TSTR* p, TSTR* f);


/*--------------------------------------------------
Check to see if s matches the pattern in ptrn
-------------------------------------------------*/

UtilExport BOOL MatchPattern(TSTR &s, TSTR &ptrn, BOOL ignoreCase=TRUE);


//-------------------------------------------------------------------------
// A Case Sensitive "smart" alphanumeric compare that sorts things so that
// numerical suffices come out in numerical order.
//-------------------------------------------------------------------------
UtilExport int MaxAlphaNumComp(MCHAR *a, MCHAR *b);


//-------------------------------------------------------------------------
// A Case Insensitive "smart" alphanumeric compare that sorts things so that
// numerical suffices come out in numerical order.
//-------------------------------------------------------------------------
UtilExport int MaxAlphaNumCompI(MCHAR *a, MCHAR *b);

#endif
