#ifndef _MCommonRenderSettingsData
#define _MCommonRenderSettingsData
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
// CLASS:    MCommonRenderSettingsData
//
// ****************************************************************************

#if defined __cplusplus


// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MTime.h>
#include <maya/MString.h>
#include <maya/MObject.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MCommonRenderSettingsData)

/*! \ingroup OpenMayaRender
\brief Data container for common rendering settings

This class is a container that encapsulates the data for common
rendering globals. The data is intended to be accessed using the
following method:

\code
MCommonRenderSettingsData data;
MRenderUtil::getCommonRenderSettings(data);
\endcode
*/
class OPENMAYARENDER_EXPORT MCommonRenderSettingsData
{
	public:
	friend class MRenderUtil;

	//! Defines the types file paths
	enum MpathType{
			kRelativePath,		//!< Relative to the project
			kFullPathImage,		//!< Full path
			kFullPathTmp		//!< Full path in the temporary directory
	};

	//! Default contructor
	MCommonRenderSettingsData();

	//! Destructor
	~MCommonRenderSettingsData();

	//! Determines if there is animation.
	bool isAnimated() const;

	//! Determines if the output format is a single movie file.
    bool isMovieFormat() const;

	//! Used with getImageName. Sets the pass name for later use.
	void			setPassName		(const MString & pass);

	//! Used with getImageName. Sets the field name for later use.
	void			setFieldName	(const MString & field);

	//! Obtains the image name with the proper subdirectory structure.
	const MString	getImageName(
						MpathType		pathType,
						double			frameNumber,
						const MString &	sceneName,
						const MString &	cameraName,
						const MString &	fileFormat,
						const MObject	layer,
						const bool		createDirectory = true,
						MStatus *		ReturnStatus = NULL) const;

	//! Obtains the image name with the proper subdirectory structure (supports custom tokens).
	const MString	 getImageName(
						MpathType		pathType,
						double			frameNumber,
						const MString&	sceneName,
						const MString&	cameraName,
						const MString&	fileFormat,
						const MObject	layer,
						const MString& customTokenString,
						const bool		createDirectory = true,
						MStatus *		ReturnStatus = NULL) const;

	//! Indicates whether render the frame or not. If true, render it. If false, skip it.
	bool			shouldRenderFrameAtTime(
						MpathType		pathType,
						double			frameNumber,
						const MString&	sceneName,
						const MString&	cameraName,
						const MString&	fileFormat,
						const MObject	layer,
						const MString& customTokenString) const;

	/*! 
        Get image buffer name
    */
	const MString getBufferName(
        const MObject&   renderPass,
        const MObject&   layer,
        const MString&   cameraName,
        const MString& customTokenString = "",
        const bool leaveUnmatchedTokens = false,
        MStatus *	ReturnStatus = NULL
		)const;

    /*
      Return the preRenderFrame MEL commands.  This command is executed
      before each frame is rendered, immediately before the
      user-specified pre render MEL command.
     */
    static MString getPreRenderFrameCmd();
    /*
      Return the postRenderFrame MEL commands.  This command is executed
      after each frame is rendered, immediately after the user-specified
      post render MEL command.
     */
    static MString getPostRenderFrameCmd();

	// The actual data

	//! Base name for the rendered image
	MString			name;
	//! File naming scheme.
	unsigned int 	namingScheme;
	//! Number of padding zeros for the frame number
	unsigned int 	framePadding;
	//! Single string that defines the whole naming convention
	MString			namePattern;

	//! Predefined supported image format
	unsigned int	imageFormat;
	//! Custom image format name
	MString			customImageFormat;

	//! First frame number
	MTime			frameStart;
	//! Last frame number
	MTime			frameEnd;
	//! Frame number step
	float			frameBy;
	
	//! When true, skip existing frames
	bool			skipExistingFrames;

	//! When false, only the selected objects should be rendered
	bool			renderAll;

	//! When true, overrides the default image format extension
	bool			useCustomExt;
	//! Custom format extension, when useCustomExt is true
	MString			customExt;

	/*! \brief When true, images need to be renumbered. This only
	  affects the image names.
	*/
	bool			renumberFrames;
	//! First image number, when renumbering
	float			renumberStart;
	//! Image number step, when renumbering
	float			renumberBy;

	//! Image width in pixels
	unsigned int	width;
	//! Image height in pixels
	unsigned int	height;
	//! Image aspect ratio. This is equal to pixelAspect * width / height
	float			deviceAspectRatio;
	//! Pixel aspect ratio
	float			pixelAspectRatio;
	//! Dot per Inch
	float			dotPerInch;

	/*! \brief When on, a default light should be created when no light is
	  available in the scene.
	*/
	bool			enableDefaultLight;

	//! The mel string to be executed before a scene is rendered
	MString			preMel;
	//! The mel string to be executed after a scene is rendered
	MString			postMel;
	//! The mel string to be executed before a render layer is rendered
	MString			preRenderLayerMel;
	//! The mel string to be executed after a render layer is rendered
	MString			postRenderLayerMel;
	//! The mel string to be executed before a frame is rendered
	MString			preRenderMel;
	//! The mel string to be executed after a frame is rendered
	MString			postRenderMel;

	static const char* className();

	private:
	//! \internal Pointer to the Maya internal data
	const void*		internalData;
};
OPENMAYA_NAMESPACE_CLOSE

#endif
#endif // _MCommonRenderSettingsData
