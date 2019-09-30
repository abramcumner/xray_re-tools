#ifndef _MFnFluid
#define _MFnFluid
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
// CLASS:    MFnFluid
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MFnDagNode.h>
#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnFluid)

//! \ingroup OpenMayaFX MFn
//! \brief Fluid node function set
/*!
  This is the function set for fluid objects.

  A fluid object is a node in the dependency graph that contains a
  grid which is made up of cells. Each cell has a variety of values
  assigned to it such as density, velocity, temperature, and color.
  The grid may be 2D or 3D. See the methods below for full details.

  Fluid objects may be used for simulation purposes. This function
  set allows read and write access to the values in the cells of the
  fluid object.
*/
class OPENMAYAFX_EXPORT MFnFluid : public MFnDagNode
{
	declareDagMFn(MFnFluid, MFnDagNode);

    //! Defines how voxel values are computed for most types of fluid data 
    enum FluidMethod {
        kZero,         //!< All values in grid are zero
        kStaticGrid,   //!< Values in the grid are static
        kDynamicGrid,  //!< Values in the grid come from a dynamic solver
        kGradient      //!< Ramps the value based on the gradient setting

    };

    //! Defines the orientation of the gradient
    enum FluidGradient {
        kConstant,        //!< Value is set to one across the volume
        kXGradient,       //!< Ramp the value from zero to one along the X axis
        kYGradient,       //!< Ramp the value from zero to one along the Y axis
        kZGradient,       //!< Ramp the value from zero to one along the Z axis
        kNegXGradient,    //!< Ramp the value from one to zero along the X axis
        kNegYGradient,    //!< Ramp the value from one to zero along the Y axis
        kNegZGradient,    //!< Ramp the value from one to zero along the Z axis
        kCenterGradient   //!< Ramps the value from one at the center to zero at the edges
    };

    //! Falloff data only supports a subset of the available fluid methods
	enum FalloffMethod {
		kNoFalloffGrid,		//!< No falloff grid should be defined.
		kStaticFalloffGrid	//!< Values in the falloff grid are static.
	};

    //! Color data has its own set of methods for computing voxel data
    enum ColorMethod {
        kUseShadingColor,        //!< Off, use shading color instead
        kStaticColorGrid,        //!< Values in the grid are static
        kDynamicColorGrid        //!< Values in the grid come from a dynamic solver 
    };

    //! Coordinate data has its own set of methods for computing voxel data
    enum CoordinateMethod {
        kFixed,   //!< Values are equal the object space coordinates
        kGrid     //!< Coordinate values will be moved using the current density solver
    };

    // Create 2D and 3D fluids
    MObject create3D			( unsigned int Xres, unsigned int Yres, unsigned int Zres,
							      double Xdim, double Ydim, double Zdim,
								  MObject parentOrOwner = MObject::kNullObj,
								  MStatus * ReturnStatus = NULL );

    MObject	create2D			( unsigned int Xres,  unsigned int Yres,
							      double Xdim, double Ydim,
							      MObject parentOrOwner = MObject::kNullObj,
							      MStatus * ReturnStatus = NULL );

    // get and set resolution, dimensions
    MStatus getResolution		( unsigned int &Xres, unsigned int &Yres,
            					  unsigned int &Zres ) const;
    MStatus getResolution		( unsigned int &Xres, unsigned int &Yres ) const;
    MStatus getDimensions		( double &Xdim, double &Ydim,
            					  double &Zdim ) const;
    MStatus setSize				( unsigned int Xres,  unsigned int Yres, unsigned int Zres,
								  double Xdim,  double Ydim, double Zdim,
            					  bool resample );
    MStatus setSize				( unsigned int Xres,  unsigned int Yres,
								  double Xdim,  double Ydim,
            					  bool resample );
    unsigned int gridSize			( MStatus * ReturnStatus = NULL );
    MStatus velocityGridSizes	( int & xsize, int & ysize, int & zsize );

    // get pointers to actual data area
    // users may get and set data into these arrays,
	float * falloff				( MStatus * ReturnStatus = NULL );
    float * density				( MStatus * ReturnStatus = NULL );
    MStatus getVelocity			( float *& Xvel, float *& Yvel, float *& Zvel );
    float * pressure			( MStatus * ReturnStatus = NULL );
    float * temperature			( MStatus * ReturnStatus = NULL );
    float * fuel				( MStatus * ReturnStatus = NULL );
    MStatus getCoordinates		( float *& u, float *& v, float *& w );
    MStatus getColors			( float *& r, float *& g, float *& b );

    // Set Data Modes
	MStatus setFalloffMode		( FalloffMethod mode );
	MStatus getFalloffMode		( FalloffMethod &mode );

    MStatus setDensityMode		( FluidMethod mode, FluidGradient gradient );
    MStatus getDensityMode		( FluidMethod &mode, FluidGradient &gradient );

    MStatus setVelocityMode		( FluidMethod mode, FluidGradient gradient );
    MStatus getVelocityMode		( FluidMethod &mode, FluidGradient &gradient );

    MStatus setTemperatureMode	( FluidMethod mode, FluidGradient gradient );
    MStatus getTemperatureMode	( FluidMethod &mode, FluidGradient &gradient );

    MStatus setFuelMode			( FluidMethod mode, FluidGradient gradient );
    MStatus getFuelMode			( FluidMethod &mode, FluidGradient &gradient );

    MStatus setCoordinateMode	( CoordinateMethod mode ) ;
    MStatus getCoordinateMode	( CoordinateMethod & mode );

    MStatus setColorMode		( ColorMethod mode );
    MStatus getColorMode		( ColorMethod & mode );

    MStatus getForceAtPoint(const MPointArray&   point,
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
							double deltaTime = 1.0 / 24.0 );
    MStatus getForceAtPoint(const MVectorArray&  point,
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
							double deltaTime = 1.0 / 24.0 );
	bool	toGridIndex( const MPoint& objectSpacePoint,
						 int3& gridCoords,
						 MStatus *status = NULL );
	MStatus voxelCenterPosition(int xi,
								int yi,
								int zi,
								MPoint& objectSpacePoint);

	MStatus	updateGrid			();

	void emitIntoArrays( float val,
						 int x,
						 int y,
						 int z,
						 float density,
						 float heat,
						 float fuel,
						 bool doColor,
						 const MColor& emitColor );


    // convert to/from single index
    int     index				( int xi, int yi );
    int     index				( int xi, int yi, int zi );
    void    index				( int ai, int & xi, int & yi, int & zi );
    int     index				( int xi, int yi, int zi,
								  int xres, int yres, int zres );
    void    index				( int ai, int xres, int yres, int zres,
								  int & xi, int & yi, int & zi );

    bool    isAutoResize();
    bool    isResizeToEmitter();
    MStatus expandToInclude(const MPoint &min, const MPoint &max);
BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnFluid, MFnDagNode );

END_NO_SCRIPT_SUPPORT:

protected:

  	virtual bool	objectChanged		( MFn::Type, MStatus * );

	void * 			updateGeomPtr		();
	void *			updateConstGeomPtr	() const;

private:
    MObject		  	create				( MObject parent, bool is2d,
        								  MStatus * );

	unsigned int fNx, fNy, fNz;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnFluid */
