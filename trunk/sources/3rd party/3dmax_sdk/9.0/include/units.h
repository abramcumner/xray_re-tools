/**********************************************************************
 *<
	FILE: units.h

	DESCRIPTION: Include file for real-world units support

	CREATED BY: Tom Hudson

	HISTORY:

 *>	Copyright (c) 1995, All Rights Reserved.
 **********************************************************************/

#ifndef _UNITS_H_

#define _UNITS_H_
#include "maxheap.h"

// The unit types we support
#define UNITS_INCHES		0
#define UNITS_FEET			1
#define UNITS_MILES			2
#define UNITS_MILLIMETERS	3
#define UNITS_CENTIMETERS	4
#define UNITS_METERS		5
#define UNITS_KILOMETERS	6

#define MAX_UNIT_TYPES 7

// The unit display types
#define UNITDISP_GENERIC	0
#define UNITDISP_METRIC		1
#define UNITDISP_US			2
#define UNITDISP_CUSTOM		3

#define MAX_UNITDISP_TYPES 4

// Metric display options
#define UNIT_METRIC_DISP_MM	0
#define UNIT_METRIC_DISP_CM	1
#define UNIT_METRIC_DISP_M	2
#define UNIT_METRIC_DISP_KM	3

#define MAX_METRIC_DISP_TYPES 4

// US display options
#define UNIT_US_DISP_FRAC_IN	0
#define UNIT_US_DISP_DEC_IN		1
#define UNIT_US_DISP_FRAC_FT	2
#define UNIT_US_DISP_DEC_FT		3
#define UNIT_US_DISP_FT_FRAC_IN	4
#define UNIT_US_DISP_FT_DEC_IN	5

#define MAX_US_DISP_TYPES 6

// US display options
#define UNIT_FRAC_1_1	0
#define UNIT_FRAC_1_2	1
#define UNIT_FRAC_1_4	2
#define UNIT_FRAC_1_8	3
#define UNIT_FRAC_1_10	4
#define UNIT_FRAC_1_16	5
#define UNIT_FRAC_1_32	6
#define UNIT_FRAC_1_64	7
#define UNIT_FRAC_1_100	8

#define MAX_FRAC_TYPES 9

// Units designator types
#define UNIT_DESIG_MM		0
#define UNIT_DESIG_CM		1
#define UNIT_DESIG_M		2
#define UNIT_DESIG_KM		3
#define UNIT_DESIG_IN		4
#define UNIT_DESIG_FT		5
#define UNIT_DESIG_CUSTOM	6

#define UNIT_DESIG_TYPES	 7

// Display information structure

struct DispInfo: public MaxHeapOperators {
	int dispType;				// Display type	(UNITDISP_GENERIC, UNITDISP_METRIC, UNITDISP_US, UNITDISP_CUSTOM)
	int metricDisp;				// Metric display option
	int usDisp;					// US display option
	int usFrac;					// US fraction option
	TSTR customName;			// Custom unit name
	float customValue;			// Custom unit value
	int customUnit;				// Custom unit reference
	};
/*
typedef struct	{
	int lightingSystem;		// Lighting unit display system {LIGHTING_UNIT_DISPLAY_INTERNATIONAL, 
									//											LIGHTING_UNIT_DISPLAY_AMERICAN }
	TSTR luminanceUnits;		// The string describing the luminance units
	TSTR illuminanceUnits;	// The string describing the illuminance units
}	LightDispInfo;
*/
// Get the master scale in terms of the specified unit type
// i.e. GetMasterScale(UNITS_INCHES) gives number of inches per unit
// Returns -1.0 if invalid unit type supplied
CoreExport double GetMasterScale(int type);

//! \brief Computes a scaling factor for going from given System Units to others.	
/*! 
\param [in] fromType - type of System Units from which we start (a value going from UNITS_INCHES to UNITS_KILOMETERS).
\param [in] fromScale - scale of System Units from which we start.
\param [in] toType - type of System Units into which we want to rescale (a value going from UNITS_INCHES to UNITS_KILOMETERS).
\param [in] toScale - scale of System Units into which we want to rescale.
\return - The scale factor that is needed to scale from given System Units to others. 
*/
CoreExport double GetRelativeScale(int fromType, float fromScale, int toType, float toScale);

CoreExport void GetMasterUnitInfo(int *type,float *scale);
CoreExport int SetMasterUnitInfo(int type,float scale);
CoreExport void GetUnitDisplayInfo(DispInfo *info);
CoreExport int SetUnitDisplayInfo(DispInfo *info);
CoreExport int GetUnitDisplayType();
CoreExport int SetUnitDisplayType(int type);
CoreExport BOOL IsValidUnitCharacter(int ch);	// Returns TRUE if character valid for unit type

// The US unit defaults
#define US_UNIT_DEFAULT_FEET 0
#define US_UNIT_DEFAULT_INCHES 1

// Get/set the default units for US entry fields -- Inches or feet
CoreExport void SetUSDefaultUnit(int type);
CoreExport int GetUSDefaultUnit();

CoreExport TCHAR *GetFirstUnitName();
CoreExport TCHAR *GetNextUnitName();
CoreExport TCHAR *GetFirstMetricDisp();
CoreExport TCHAR *GetNextMetricDisp();
CoreExport TCHAR *GetFirstUSDisp();
CoreExport TCHAR *GetNextUSDisp();
CoreExport TCHAR *GetFirstFraction();
CoreExport TCHAR *GetNextFraction();
CoreExport TCHAR *FormatUniverseValue(float value);
CoreExport float DecodeUniverseValue(TCHAR *string, BOOL *valid = NULL);


//
// Time units
//

// Ways to display time
enum TimeDisp {
	DISPTIME_FRAMES,
	DISPTIME_SMPTE,
	DISPTIME_FRAMETICKS,
	DISPTIME_TIMETICKS
	};

// Formats a time value into a string based on the current frame rate, etc.
CoreExport void TimeToString(TimeValue t,TSTR &string);
CoreExport BOOL StringToTime(TSTR string,TimeValue &t);

CoreExport int GetFrameRate();
CoreExport void SetFrameRate(int rate);
CoreExport int GetTicksPerFrame();
CoreExport void SetTicksPerFrame(int ticks);
CoreExport TimeDisp GetTimeDisplayMode();
CoreExport void SetTimeDisplayMode(TimeDisp m);
CoreExport int LegalFrameRate(int r);

#ifdef DESIGN_VER
CoreExport TCHAR *FormatUniverseValue(TCHAR *value);
#endif

// Register a default value for a distance value.  The section and key are used
// to place the default in the .INI file.  The default has separate values for
// both Imperial and Mtric systems.  The Imperial default is given in inches
// and the Metric default is given in Meters.
CoreExport void RegisterDistanceDefault(TCHAR* section, TCHAR* key, float inchesDefault, float metersDefault);

// Restore a default value for a distance value.  The section and key are used
// to place the default in the .INI file.  The default has separate values for
// both Imperial and Metric systems.  The Imperial default is given in inches
// and the Metric default is given in Meters.
CoreExport void RestoreDistanceDefault(TCHAR* section, TCHAR* key, float inchesDefault, float metersDefault);

// Get the default for the given section and key in the defaults file.
// The system returns the default relative to the current system units.
// If the units are Imperial (inches, feet, etc.) it returns the Imperial
// version of the default, otherwise it returns the Metric version.
// In either case the default value is returned in the current system units.
CoreExport float GetDistanceDefault(TCHAR* section, TCHAR* key);

// Sets the defaults for the given section and key in the defaults file.
// The system sets the default relative to the current system units.
// If the units are Imperial (inches, feet, etc.) it sets the Imperial
// version of the default, otherwise it sets the Metric version.
CoreExport void SetDistanceDefault(TCHAR* section, TCHAR* key, float value);

#endif // _UNITS_H_
