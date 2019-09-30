#ifndef _MCacheFormatDescription
#define _MCacheFormatDescription
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
// CLASS:    MCacheFormatDescription
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MString.h>
#include <maya/MTime.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MCacheFormatDescription)

//! \ingroup OpenMaya
//! \brief Cache format description. 
/*!
  The MCacheFormatDescription is a utility class used by MPxCacheFormat when
  implementing custom cache plug-ins that define their own handling of 
  the cache format (usually provided by Maya's own xml format).
*/
class OPENMAYA_EXPORT MCacheFormatDescription
{
public:
	//! Cache file distribution types.
	typedef enum {
		kNoFile,			//!< just memory cache
        kOneFile,			//!< one file for all frames
		kOneFilePerFrame	//!< one file per frame
    } CacheFileDistribution;        

	//! Cache file data types.
	typedef enum {
		kUnknownData,		//!< unknown data
		kDouble,			//!< a single double value
		kDoubleArray,		//!< an array of double values
		kDoubleVectorArray,	//!< an array of double vector values
		kInt32Array,		//!< an array of int32 values
		kFloatArray,		//!< an array of float values
		kFloatVectorArray	//!< an array of float vector values
	} CacheDataType;

	//! Cache file sampling types.
    typedef enum {
		kRegular,			//!< regular sampling
        kIrregular			//!< irregular sampling
    } CacheSamplingType;

	//! Set up the cache format when reading the description file.
	void				setDistribution( CacheFileDistribution distribution );
	void				setTimePerFrame( const MTime& timePerFrame );
	void				addDescriptionInfo( const MString& info );
	unsigned int		addChannel( const MString& channelName,
									const MString& interpretation,
									CacheDataType dataType,
									CacheSamplingType samplingType,
									const MTime& samplingRate,
									const MTime& startTime,
									const MTime& endTime,
									MStatus* status=NULL );

	//! Get information about the format in order to write the description file.
	CacheFileDistribution	getDistribution() const;
	MTime					getTimePerFrame() const;
	void					getStartAndEndTimes( MTime& startTime, MTime& endTime ) const;
BEGIN_NO_SCRIPT_SUPPORT:
	//! \noscript
	void					getDescriptionInfo( MStringArray& info ) const;
END_NO_SCRIPT_SUPPORT:
	MStringArray			getDescriptionInfo() const;
	unsigned int			getNumChannels() const;
	MString					getChannelName( unsigned int channelIndex ) const;
	MString					getChannelInterpretation( unsigned int channelIndex ) const;
	CacheDataType			getChannelDataType( unsigned int channelIndex ) const;
	CacheSamplingType		getChannelSamplingType( unsigned int channelIndex ) const;
	MTime					getChannelSamplingRate( unsigned int channelIndex ) const;
	MTime					getChannelStartTime( unsigned int channelIndex ) const;
	MTime					getChannelEndTime( unsigned int channelIndex ) const;

	static const char* className();

private:
	// This class should never be instantiated directly.
	MCacheFormatDescription();
	virtual ~MCacheFormatDescription();

	void *		fData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MCacheFormatDescription */
