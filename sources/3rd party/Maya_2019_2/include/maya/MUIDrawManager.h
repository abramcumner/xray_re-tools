#ifndef _MUIDrawManager
#define _MUIDrawManager
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MStatus.h>
#include <maya/MStateManager.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// CLASS FORWARDS
class THmanipContainer;
class THmanip;
class THstandardContext;
class THselectContext;
class THtexContext;
class THdrRedraw;
class THhwShader;
class THhardwareShader;
class TapiCastUI;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE
namespace MHWRender
{

	// ****************************************************************************
	// DECLARATIONS
	//
	//! \ingroup OpenMayaRender
	//! \brief Main interface for drawing basic UI drawables in Viewport 2.0 and
	//!        Hardware Renderer 2.0.
	/*!
		The MUIDrawManager class provides a straight forward way to draw basic UI
		drawables like lines, icons, texts and other basic 2D/3D primitives.
	 
		MUIDrawManager is not designed for accessing at arbitrary time or arbitrary
		places. Access to the interface is restricted to certain methods on various
		API interfaces. An instance of MUIDrawManager is constructed and passed to
		these methods together with any required data and context information.

		All drawing operations, including setting state like color and font size,
		must occur between calls to beginDrawable() and endDrawable(). For example,
		to draw a pair of red lines you would do the following:

		\code
		MColor red(1.0f, 0.0f, 0.0f);

		painter.beginDrawable();
		painter.setColor( red );
		painter.line( MPoint(0, 0, 0), MPoint(1, 1, 1) );
		painter.line( MPoint(0, 0, 0), MPoint(-1, -1, 5) );
		painter.endDrawable();
		\endcode

		The call to endDrawable() resets the draw state, so if you want to continue
		drawing in red later on you would have to repeat the call to setColor().

		\code
		painter.beginDrawable();
		painter.setColor( red );
		painter.text( MPoint(0, 0, 4), "Hello, world!" );
		painter.endDrawable();
		\endcode

		Note that draw operations may not take place immediately but instead be
		queued up for later execution during the draw phase.
	 
		To reduce draw overhead due to the small batch problem, Viewport 2.0 may
		batch the same type of UI drawables created by the same instance of
		MUIDrawManager, where the type means a combination of primitive type,
		internal shader, space dimension (2D/3D) and selection support. Some types
		are ineligible to be batched, e.g. line/triangle strip, textured drawing.
	*/

	// ****************************************************************************

	class OPENMAYARENDER_EXPORT MUIDrawManager
	{
	public:

		/*!
			Font size for drawing the text.
		*/
		enum FontSize {
			//! Default font, pixel size 12 in height
			kDefaultFontSize	= 12,
			//! Small font, pixel size 9 in height
			kSmallFontSize		= 9
		};

		/*!
			Text alignment.
		*/
		enum TextAlignment {
			//! Text aligned to the left of the background box
			kLeft, 
			//! Text aligned at the middle of the background box
			kCenter, 
			//! Text aligned to the right of the background box
			kRight 
		};

        /*!
            Text incline.
            Most of the font families support well for incline normal and italic.
            Oblique incline is not supported for most of the font families.
        */
        enum TextIncline {
            //! Normal glyphs used in unstyled text
            kInclineNormal            = 0,
            //! Italic glyphs that are specifically designed for the purpose of representing italicized text
            kInclineItalic            = 1,
            //! Glyphs with an italic appearance that are typically based on the unstyled glyphs
            kInclineOblique           = 2,
        };

        /*!
            Text weight. 
            Most of the font families support well for weight light and bold.
            Weight normal/demibold/black is not supported for most of the font families.
        */
        enum TextWeight 
        {
            //! Text with light weight
            kWeightLight              = 25, 
            //! Text with normal weight
            kWeightNormal             = 50, 
            //! Text with demi bold weight
            kWeightDemiBold           = 63, 
            //! Text with bold weight
            kWeightBold               = 75, 
            //! Text with black weight
            kWeightBlack              = 87, 
        };

        /*!
            Text stretch.
        */
        enum TextStretch {
            //! Text with ultra condensed stretch
            kStretchUltraCondensed    = 50, 
            //! Text with extra condensed stretch
            kStretchExtraCondensed    = 62, 
            //! Text with condensed stretch
            kStretchCondensed         = 75, 
            //! Text with semi condensed stretch
            kStretchSemiCondensed     = 87, 
            //! Text with unstretched stretch
            kStretchUnstretched       = 100, 
            //! Text with semi expanded stretch
            kStretchSemiExpanded      = 112, 
            //! Text with expanded stretch
            kStretchExpanded          = 125, 
            //! Text with extra expanded stretch
            kStretchExtraExpanded     = 150, 
            //! Text with ultra expanded stretch
            kStretchUltraExpanded     = 200, 
        };

        /*!
            Text line.
        */
        enum TextLine {
            //! Font with no line
            kLineNone, 
            //! Font with overline
            kLineOverline, 
            //! Font with underline
            kLineUnderline, 
            //! Font with strike out line 
            kLineStrikeoutLine, 
        };

		/*!
			Line style.
		*/
		enum LineStyle {
			//! Solid line
			kSolid, 
			//! Short Dotted line
			kShortDotted,  
			//! Short dashed line
			kShortDashed,  
			//! Dashed line
			kDashed, 
			//! Dotted line
			kDotted  
		};

		/*!
			Paint style.
		*/
		enum PaintStyle {
			//! Solid
			kFlat, 
			//! Stippled 
			kStippled,  
			//! Shaded with lighting
			kShaded
		};

		/*!
			Primitive.
		*/
		enum Primitive {
			//! Point list
			kPoints, 
			//! Line list
			kLines, 
			//! Line strip
			kLineStrip, 
			//! Closed line
			kClosedLine, 
			//! Triangle list
			kTriangles, 
			//! Triangle strip
			kTriStrip 
		};

		/*!
			Selectability.
		*/
		enum Selectability {
			//! Geometries cannot be used for selection
			kNonSelectable, 
			//! Use geometries for selection
			kSelectable, 
			//! Use geometries for selection when not in manipulator context
			kAutomatic, 
		};

	public:

		void beginDrawable(Selectability selectability = kAutomatic, unsigned int selectionName = 0);
		//! obsolete, please use the one with selectability and selection name parameters
    OPENMAYA_DEPRECATED(2019, "For manipulator drawing: resets all draw state, such as color and line style, to defaults and indicates the start of a sequence of drawing operations for the specified manipulator handle name. All drawing operations must take place between calls to beginDrawable() and endDrawable(). This name can be used with MPxManipulatorNode::glActiveName() and is device independant. The name is only used for selection and will be ignored in the regular draw pass.")
		void beginDrawable(unsigned int name, bool nameIsPickable);
		void endDrawable();

		void beginDrawInXray();
		void endDrawInXray();

		void setColor( const MColor& color );
		void setColorIndex( const short index );

		void setPointSize(float value);
		void setLineWidth(float value);
		void setLineStyle(LineStyle style);
		void setLineStyle(unsigned int factor, unsigned short pattern);

		void setPaintStyle(PaintStyle style);

		unsigned int depthPriority() const;
		void setDepthPriority(unsigned int priority);

		// Basic primitive drawing methods
		void line( const MPoint& startPoint, const MPoint& endPoint );
		void line2d( const MPoint& startPoint, const MPoint& endPoint );
		MStatus lineList( const MPointArray& points, bool draw2D );
		MStatus lineStrip( const MPointArray& points, bool draw2D );
		
		void point( const MPoint& point );
		void point2d( const MPoint& point );
		MStatus points( const MPointArray& points, bool draw2D );

		void rect( const MPoint& center, const MVector& up, const MVector& normal,
			double scaleX, double scaleY, bool filled = false );
		void rect2d( const MPoint& center, const MVector& up,
			double scaleX, double scaleY, bool filled = false );

		void sphere( const MPoint& center, double radius, bool filled = false );

		void sphere( const MPoint& center, double radius, int subdivisionsAxis, int subdivisionsHeight, bool filled = false );

		void circle( const MPoint& center, const MVector& normal, double radius,
			bool filled = false );
		void circle2d( const MPoint& center, double radius,
			bool filled = false );

		void circle( const MPoint& center, const MVector& normal, double radius, int numSubdivision,
			bool filled);
		void circle2d( const MPoint& center, double radius, int numSubdivision,
			bool filled );

		void arc( const MPoint& center, const MVector& start, const MVector& end,
			const MVector& normal, double radius, bool filled = false );
		void arc2d( const MPoint& center, const MVector& start, const MVector& end,
			double radius, bool filled = false );

		void arc( const MPoint& center, const MVector& start, const MVector& end,
			const MVector& normal, double radius, int numSubdivisions, bool filled);
		void arc2d( const MPoint& center, const MVector& start, const MVector& end,
			double radius, int numSubdivisions, bool filled);

		void mesh( Primitive mode, const MPointArray& position,
			const MVectorArray* normal = NULL,
			const MColorArray* color = NULL,
			const MUintArray* index = NULL,
			const MPointArray* texcoord = NULL );

		void mesh( Primitive mode, const MFloatPointArray& position, 
			const MFloatVectorArray* normal = NULL, 
			const MColorArray* color = NULL,
			const MUintArray* index = NULL,
			const MFloatPointArray* texcoord = NULL );

		void mesh2d( Primitive mode, const MPointArray& position, 
			const MColorArray* color = NULL,
			const MUintArray* index = NULL,
			const MPointArray* texcoord = NULL );

		void cone(const MPoint& base, const MVector& direction, double radius, double height,
			bool filled = false);

		void cone(const MPoint& base, const MVector& direction, double radius, double height, int subdivisionsCap, bool filled);

		void cylinder(const MPoint& center, const MVector &up, double radius, double height, int subdivisionsAxis, 
			bool filled = false);

		void capsule(const MPoint& center, const MVector &up, double radius, double height, int subdivisionsAxis, int subdivisionsHeight,
			bool filled = false);

		void box (const MPoint& center, const MVector& up, const MVector & right, double scaleX = 1.0, double scaleY = 1.0,
			double scaleZ = 1.0, bool filled = false);

		// Text drawing		
		static unsigned int getFontList(MStringArray& list);

        void setFontIncline( const int fontIncline);
        void setFontWeight( const int fontWeight);
        void setFontStretch( const int fontStretch);
        void setFontLine( const int fontLine);
		void setFontSize( const unsigned int fontSize);
		void setFontName( const MString& faceName );
		void text(
			const MPoint& position,
			const MString& text,
			TextAlignment alignment = kLeft,
			const int *backgroundSize = NULL,
			const MColor* backgroundColor = NULL,
			bool dynamic = false);
		void text2d(
			const MPoint& position,
			const MString& text,
			TextAlignment alignment = kLeft,
			const int *backgroundSize = NULL,
			const MColor* backgroundColor = NULL,
			bool dynamic = false);

		void setTexture(MHWRender::MTexture* texture);
		MStatus setTextureSampler(MHWRender::MSamplerState::TextureFilter filter, MHWRender::MSamplerState::TextureAddress address);
		MStatus setTextureMask(MHWRender::MBlendState::ChannelMask mask);

		MStatus icon(const MPoint& position, const MString& name, float scale );
		static unsigned int getIconNames(MStringArray &names);
 
	private:
		MUIDrawManager( void* );
		~MUIDrawManager();

		void* fData;

	};


} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MUIDrawManager */

