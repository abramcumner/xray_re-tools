#ifndef _MColor
#define _MColor
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
// CLASS:    MColor
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MIOStreamFwd.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MColor)

//! \ingroup OpenMaya
//! \brief A color math class. 
/*!
This class is used to store values of color attributes. Right now, this
class provides no more functionality than the MFltVector class, but it is
used because Rendering plans to employ more advanced color models in the
future.  In these models, colors cannot necessarily be represented as an
RGB triple.
*/
class OPENMAYA_EXPORT MColor
{
public:
	//! Defines color space
	enum MColorType {
		kRGB,			//!< Red, Green, Blue
		kHSV,			//!< Hue, Saturation, Value
		kCMY,			//!< Cyan, Magenta, Yellow
		kCMYK			//!< Cyan, Magenta, Yellow, Black
	};

	//! Default class contructor
					MColor();
	//! Copy contructor
					MColor( const MColor& src);
	//! RGB contructor
					MColor( float rr, float gg, float bb=0.0, float aa=1.0 );
	//! RGB contructor
					MColor( const float d[3] );
	//! Alpha contructor
					MColor( float alpha );
	//! Color space contructor
					MColor( MColorType colorModel,
							unsigned char c1, unsigned char c2, unsigned char c3,
							unsigned char alpha=255U );
	//! Color space contructor
					MColor( MColorType colorModel,
							unsigned short c1, unsigned short c2, unsigned short c3,
							unsigned short alpha=65535U );
	//! Color space contructor
					MColor( MColorType colorModel, float c1, float c2, float c3,
							float alpha=1.0 );
	//! Color space contructor
					MColor( MColorType colorModel, double c1, double c2, double c3,
							double alpha=1.0 );

	//! Class destructor.
					~MColor();

	//! Assignment operator.
 	MColor&		    operator= ( const MColor& src );

	//! Index operator.
 	float   		operator()( unsigned int i ) const;

	//! Index operator.
	float			operator[]( unsigned int i )const;

	//! In place multiply operator.
 	MColor& 		operator*=( float scalar );
	//! Multiply operator.
 	MColor   		operator*( float scalar ) const;
	//! In place multiply operator.
	MColor&         operator*=( const MColor& other );
	//! Multiply operator.
 	MColor          operator*( const MColor& other ) const;

	//! In place divide operator.
 	MColor&   		operator/=( float scalar );
	//! Divide operator.
 	MColor     	    operator/( float scalar ) const;

	//! In place add operator.
	MColor&		    operator+=( const MColor& other );
	//! Add operator.
 	MColor   		operator+( const MColor& other) const;

	//! Unary minus operator.
 	MColor   		operator-() const;

	//! Substraction operator.
 	MColor   		operator-( const MColor& other ) const;

	//! Equality operator.
 	bool           	operator==( const MColor& other ) const;
	//! Inequality operator.
 	bool          	operator!=( const MColor& other ) const;

	//! Color component accessor
	bool			get( float d[3] ) const;
	//! Color component accessor
	bool			get( MColorType colorModel, float&, float&, float& ) const;
	//! Color component accessor
	bool			get( MColorType colorModel, float&, float&, float&, float& alpha ) const;
	//! Color component assigment
	bool			set( MColorType colorModel, float, float, float, float alpha = 1.0);

BEGIN_NO_SCRIPT_SUPPORT:

	//!	Index operator. NO SCRIPT SUPPORT
 	float&      	operator()( unsigned int i );

	//!	Index operator. NO SCRIPT SUPPORT
 	float&      	operator[]( unsigned int i );

	//!	Multiply operator. NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MColor	operator*( float scalar, const MColor& other);
	//!	Print operator. NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream& operator<<(std::ostream& os, const MColor& c);

END_NO_SCRIPT_SUPPORT:

	//! Opaque black color.
	static const MColor	kOpaqueBlack;

	float r;			//!< The red component of the color
	float g;			//!< The green component of the color
	float b;			//!< The blue component of the color
	float a;			//!< The alpha component of the color
};

/*!
	Initializes the object to a black opaque color.
*/
inline MColor::MColor()
 	: r(0.0)
	, g(0.0)
	, b(0.0)
	, a(1.0)
{
}

/*!
	Create a new color and initialize it to the same values as the
	given color.

	\param[in] src		The color object to copy.
*/
inline MColor::MColor(const MColor& src)
 	: r(src.r)
	, g(src.g)
	, b(src.b)
	, a(src.a)
{
}

/*!
	Initializes the color with the explicit r, g, b, and a values
	provided as arguments.

	\param[in] rr		The r component of the color.
	\param[in] gg		The g component of the color.
	\param[in] bb		The b component of the color. Defaults to 0.0.
	\param[in] aa		The a component of the color. Defaults to 1.0.
*/
inline MColor::MColor(float rr, float gg, float bb, float aa )
 	: r(rr)
	, g(gg)
	, b(bb)
	, a(aa)
{
}

/*!
	Initializes the color with the explicit red, green and blue values
	provided in the given float array. The alpha value will be 1.0:
	The color is opaque.

	\param[in] d		The 3 element array containing the initial red,
	green, and blue values.
*/
inline MColor::MColor( const float d[3] )
 	: r(d[0])
	, g(d[1])
	, b(d[2])
	, a(1.0)
{
}

/*!
	Initializes a black color with transparency. The r, g, b
	components are set to 0.

	\param[in] alpha	The transparency component of the color.
*/
inline MColor::MColor(float alpha )
 	: r(0.0)
	, g(0.0)
	, b(0.0)
	, a(alpha)
{
}

/*!
  Copies a color.

  \param[in] src		The color object to copy.

  \return
  A reference to the object whose value was set.
*/
inline MColor& MColor::operator= (const MColor& src)
{
	r = src.r;
	g = src.g;
	b = src.b;
	a = src.a;
	return (*this);
}

/*!
  Accessor to one component of the color.

  \param[in] i		Value indicating which component to return.

  \return
  A reference to the appropriate component of the color:
	\li Reference to the red channel when argument is 0.
	\li Reference to the green channel when argument is 1.
	\li Reference to the blue channel when argument is 2.
	\li Reference to the alpha channel otherwise.
*/
inline float& MColor::operator()( unsigned int i )
{
	//	switch is more efficient than else-if and can be better optimized
	//	by compilers.
	//
	switch (i) {
		default:
		case 3:		return a;
		case 2:		return b;
		case 1:		return g;
		case 0:		return r;
	}
}

/*!
  Accessor to one component of the color.

  \param[in] i		Value indicating which component to return.

  \return
  A reference to the appropriate component of the color:
	\li Reference to the red channel when argument is 0.
	\li Reference to the green channel when argument is 1.
	\li Reference to the blue channel when argument is 2.
	\li Reference to the alpha channel otherwise.
*/
inline float MColor::operator()( unsigned int i ) const
{
	switch (i) {
		default:
		case 3:		return a;
		case 2:		return b;
		case 1:		return g;
		case 0:		return r;
	}
}

/*!
  Accessor to one component of the color.

  \param[in] i		Value indicating which component to return.

  \return
  A reference to the appropriate component of the color:
	\li Reference to the red channel when argument is 0.
	\li Reference to the green channel when argument is 1.
	\li Reference to the blue channel when argument is 2.
	\li Reference to the alpha channel otherwise.
*/
inline float& MColor::operator[]( unsigned int i )
{
	switch (i) {
		default:
		case 3:		return a;
		case 2:		return b;
		case 1:		return g;
		case 0:		return r;
	}
}

/*!
  Accessor to one component of the color.

  \param[in] i		Value indicating which component to return.

  \return
  A reference to the appropriate component of the color:
	\li Reference to the red channel when argument is 0.
	\li Reference to the green channel when argument is 1.
	\li Reference to the blue channel when argument is 2.
	\li Reference to the alpha channel otherwise.
*/
inline float MColor::operator[]( unsigned int i ) const
{
	switch (i) {
		default:
		case 3:		return a;
		case 2:		return b;
		case 1:		return g;
		case 0:		return r;
	}
}


/*!
	Extracts the r, g, and b components of the color and places them
	in elements 0, 1, and 2 of the float array passed.

	\param[out] d	The array of 3 floats into which the results are placed.

	\return
	Always returns bool true.
*/
inline bool MColor::get( float d[3] ) const
{
	d[0] = r;
	d[1] = g;
	d[2] = b;
	return true;
}

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MColor */
