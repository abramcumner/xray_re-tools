#ifndef _MPxCacheFormat
#define _MPxCacheFormat
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
// CLASS:    MPxCacheFormat
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MString.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxCacheFormat)

//! \ingroup OpenMaya MPx
//! \brief CacheFormat definition. 
/*!
  The MPxCacheFormat class can be used to implement support for new cache
  file formats in Maya.
*/
class OPENMAYA_EXPORT MPxCacheFormat
{
public:
	//! Cache file access modes.
    typedef enum {
        kRead,		//!< read only
        kWrite,		//!< write only
	kReadWrite 	//!< read and write (e.g. append, edit, etc.)
    } FileAccessMode;

	MPxCacheFormat();
	virtual ~MPxCacheFormat();

	virtual MStatus		open( const MString& fileName, FileAccessMode mode);

	virtual void		close();

	virtual MStatus		isValid();

	virtual MStatus		rewind();


	virtual MString		extension();

	virtual MStatus		readHeader();

	virtual MStatus		writeHeader(const MString& version, MTime& startTime, MTime& endTime);


	virtual void		beginWriteChunk();

	virtual void		endWriteChunk();

	virtual MStatus		beginReadChunk();


	virtual void		endReadChunk();

	virtual MStatus		writeTime(MTime& time);

	virtual MStatus		readTime(MTime& time);

	virtual	MStatus		findTime(MTime& time, MTime& foundTime);

	virtual MStatus		readNextTime(MTime& foundTime);

	virtual unsigned	readArraySize();

	// Write data to the cache.
	virtual MStatus		writeDoubleArray(const MDoubleArray&);
	virtual MStatus		writeFloatArray(const MFloatArray&);
	virtual MStatus		writeIntArray(const MIntArray&);
	virtual MStatus		writeDoubleVectorArray(const MVectorArray& array);
	virtual MStatus     writeFloatVectorArray(const MFloatVectorArray& array);
	virtual MStatus		writeInt32(int);

	// Read data from the cache.
	virtual MStatus		readDoubleArray(MDoubleArray&, unsigned size);
	virtual MStatus		readFloatArray(MFloatArray&, unsigned size);
	virtual MStatus		readIntArray(MIntArray&, unsigned size);
	virtual MStatus		readDoubleVectorArray(MVectorArray&, unsigned arraySize);
	virtual MStatus		readFloatVectorArray(MFloatVectorArray& array, unsigned arraySize);
	virtual int			readInt32();

	virtual MStatus		writeChannelName(const MString & name);
	virtual MStatus		findChannelName(const MString & name);
	virtual MStatus		readChannelName(MString& name);

	// Read and write the description file.
	virtual bool		handlesDescription();
	virtual MStatus		readDescription( MCacheFormatDescription& description,
										 const MString& descriptionFileLocation,
										 const MString& baseFileName );
	virtual MStatus		writeDescription( const MCacheFormatDescription& description,
										  const MString& descriptionFileLocation,
										  const MString& baseFileName );

	static const char* className();

protected:

private:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxCacheFormat */
