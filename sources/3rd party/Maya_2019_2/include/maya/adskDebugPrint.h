#ifndef _adskDebugPrint_h
#define _adskDebugPrint_h

#include <map>
#include <string>
#include <iostream>
#include <maya/adskCommon.h>

// ****************************************************************************
/*!
	\class adsk::Debug::Print
 	\brief Class implementing debug printing operation

	While most debuggers provide a way to examine the interior of objects
	oftentimes a simple structural and content analysis is insufficient to
	discern what is happening within an object.

	It's likewise more difficult to collect and compare information from
	several different locations over many iterations - dumping the data out
	to a stream provides the option of after-the-fact analysis.

	This debug request type provides a simple output mechanism including
	an arbitrary stream and automatic indenting styles to make the formatting
	of the data more readable.
*/

namespace adsk
{
	namespace Debug
	{

		class METADATA_EXPORT Print
		{
		public:
			Print			(std::ostream& cdbg);
			Print			(const Print& rhs);
			Print& operator=(const Print& rhs);
			~Print			();
			std::ostream &	operator <<		(const void *ptr);
			std::ostream &	operator <<		(const char *str);
			std::ostream &	operator <<		(char *str);
			std::ostream &	operator <<		(int x);
			std::ostream &	operator <<		(unsigned int x);
			std::ostream &	operator <<		(char ch);
			std::ostream &	operator <<		(unsigned char ch);
			std::ostream &	operator <<		(float fl);
			std::ostream &	operator <<		(double dbl);
			std::ostream &	operator <<		(int64_t longest);
			std::ostream &	operator <<		(uint64_t longest);
			std::ostream &	outputHexChars	(unsigned int size, unsigned char* ptr);

			// Sections are used to group similar output. Defaults to the
			// C codde block Style with braces and indentation.
			void			beginSection	(const char* title);
			void			endSection		();
			// Styles of sectioning to use
			enum eStyle { kCStyle, kPythonStyle };
			eStyle			setSectionStyle	(eStyle newStyle);

			// Define filters
			void			addFilter		(const std::string& filterName,
											 int				filterValue);
			void			removeFilter	(const std::string& filterName);
			bool			findFilter		(const std::string& filterName,
											 int&				filterValue);

			// Helpers, not usually called directly
			std::ostream &	doIndent		();
			bool			skipNextIndent	(bool doSkip);
			int				indent			(int relativeChange);
			int				setIndent		(int newIndent);
			std::string		setIndentString	(const std::string& newIndentString);

		private:
			class Impl;
			Impl* fImpl;
		};

	}; // namespace Debug
}; // namespace adsk

//=========================================================
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // _adskDebugPrint_h
