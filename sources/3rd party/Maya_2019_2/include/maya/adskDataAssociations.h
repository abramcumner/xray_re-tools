#ifndef adskDataAssociations_h
#define adskDataAssociations_h

#include <string>
#include <maya/adskDataChannel.h>	// For Channel object return values
#include <maya/adskCommon.h>
#include <maya/adskDebugCount.h>	// for Debug Count macro

namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};
	namespace Data {
		class AssociationsImpl;
		class AssociationsIterator;

// ****************************************************************************
/*!
	\class adsk::Data::Associations
 	\brief Class handling associations between internal and external data.

	You would use this data structure when creating something like per-component
	data; e.g. a piece of data you wish to attach to every vertex on a surface,
	every point in a cloud, every particle in a fluid simulation, etc.

	It provides a generic interface to handle lists of data streams that can be
	associated with your own data.

	Association types should be unique within the context of where this data is
	being stored. e.g. "mesh/vertex", "mesh/edge", and "mesh/face". There is no
	requirement of the naming convention. Associations are looked up by pointer
	first before string compares so using a const reference from the calling
	code is more efficient.

	The classes are designed to each be simple to use at the expense of a little
	complexity in the hierarchy. Here's how the different levels break down
	using the namespace adsk::Data

		Associations
		     |
		     |   Associates type (e.g. per-vertex data) with channel
		     |
		  Channel 
		     |
		     |   Amalgamates all data streams (e.g. per-vertex data maintained
			 |          for an external simulation) into a single entity
		     |
		   Stream
		     |
		     |   Keeps an efficient list of indexed data
			 |          (e.g. one set of simulation data per vertex)
		     |
		    Data
			     The actual data with introspection capabilities so that you
				 can find out what it is. At this level it's a single chunk
				 of data, e.g. 3 floats for color, an int and a string as a
				 version identifier, a collection of vectors and scalars for
				 current simulation state, etc.

	For example on a mesh with 8 vertices if a simulation engine wants to store
	the velocity and acceleration of every vertex the data would look like this:

		adsk::Data::Associations {
			channelCount = 1
			channel(0) = adsk::Data::Channel {
				name = "mesh/vertex"
				streamCount = 1
				stream(0) = adsk::Data::Stream {
				    indexCount = 8
					streamData = adsk::Data::Handle[8] {
						"simulationData" : compoundType {
							"velocity" : compoundType {
								"velocityX" : float
								"velocityY" : float
								"velocityZ" : float
							}
							"acceleration" : compoundType {
								"accelerationX" : float
								"accelerationY" : float
								"accelerationZ" : float
							}
						}
					}
				}
			}
		}
*/

class METADATA_EXPORT Associations
{
public:
	//----------------------------------------------------------------------
	~Associations();
	Associations();
	Associations(const Associations*);
	Associations(const Associations&);
	Associations& operator=(const Associations&);

	//----------------------------------------------------------------------
    Channel			channel         ( const std::string& );
    void			setChannel   	( Channel );
	const Channel*	findChannel 	( const std::string& ) const;
	Channel*		findChannel 	( const std::string& );
    bool        	removeChannel   ( const std::string& );
    bool        	renameChannel   ( const std::string&, const std::string& );

	//----------------------------------------------------------------------
	// To create a unique copy of this class and all owned classes.
	// Only use this if you want to create unique copies of all data
	// below this, otherwise use the similarly named method at a lower
	// level.
	bool			makeUnique		();

	//----------------------------------------------------------------------
	// Use this when creating Associations from a DLL
	static Associations*		create			();

	//----------------------------------------------------------------------
	// Support for iteration over Channels in these Associations
	//
	// For technical reasons these types live outside this class even
	// though they are a logical part of it. Declaring the typedefs lets
	// them be referenced as though they were still part of the class.
	typedef AssociationsIterator iterator;
	typedef AssociationsIterator const_iterator;

	Associations::iterator			begin	()	const;
	Associations::iterator			end		()	const;
	Associations::const_iterator	cbegin	()	const;
	Associations::const_iterator	cend	()	const;

	unsigned int					size	()	const;
	bool							empty	()	const;

	//----------------------------------------------------------------------
    // Obsolete iteration support for Channel members. Use
	// Associations::iterator instead.
	//
    unsigned int    channelCount()								const;
    Channel			channelAt	( unsigned int channelIndex )	const;
    const Channel*	operator[]	( unsigned int channelIndex )	const;

	//----------------------------------------------------------------------
	// Debug support
	static bool	Debug(const Associations* me, Debug::Print& request);
	static bool	Debug(const Associations* me, Debug::Footprint& request);
	DeclareObjectCounter( Associations );

////////////////////////////////////////////////////////////////////////

private:
	friend class AssociationsIterator;
	AssociationsImpl*		fImpl;					//! Implementation class
};

	} // namespace Data
} // namespace adsk

//=========================================================
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // adskDataAssociations_h
