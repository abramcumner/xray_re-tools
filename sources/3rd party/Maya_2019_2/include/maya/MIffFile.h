#ifndef _MIffFile
#define _MIffFile
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
// CLASS:    MIffFile
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MApiNamespace.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MIffFile)

//! \ingroup OpenMaya
//! \brief Read file images stored on disk. 
/*!
	This class provides a simple C++ interface to the IFF i/o library
	(flib). A class called MIffTag is used to create the 4 byte tags
	that are used to distinguish the different block types within an
	IFF file.
*/
class OPENMAYA_EXPORT MIffFile
{
public:

					MIffFile();
					MIffFile(const MString &fileName);
					virtual ~MIffFile();
	MStatus			open(const MString & fileName);

	MStatus			close();

	bool			isActive() const;

	MStatus			beginReadGroup(MIffTag& id, MIffTag& type);

    MStatus			endReadGroup();

    const void *	getChunk(MIffTag& chunkTag, unsigned *byteCount);

	MStatus			beginGet(MIffTag& chunkTag, unsigned *byteCount);

    MStatus			endGet();

	int				get(void *buf, unsigned byteCount, MStatus *stat = NULL);

	MStatus			iffGetShort( short int& result );

	MStatus			iffGetInt( int& result );

	MStatus			iffGetFloat( float& result );

	static const char* className();

protected:
	// No protected members

private:
	void *_fileHandler;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MIffFile */
