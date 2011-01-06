/////////////////////////////////////////////////////////////////////////
//
//
//	Render Preset Interface
//
//	Created 5/14/2003	Tom Burke
//

#ifndef	I_CUSTOM_RENDER_PRESETS_H
#define I_CUSTOM_RENDER_PRESETS_H

// includes
#include "maxheap.h"
#include "sfx.h"
#include "itargetedio.h"
#include "irenderpresets.h"

//////////////////////////////////////////////////////////////
//
//		Render Presets compatibility interface
//		used by renderers to support renderer specific presets.
//		If the renderer does not support this interface, the entire
//		renderer will be saved.
//
//////////////////////////////////////////////////////////////
class ICustomRenderPresets: public MaxHeapOperators
{
public:
	virtual int  RenderPresetsFileVersion() = 0;
	virtual BOOL RenderPresetsIsCompatible( int version ) = 0;

	virtual TCHAR * RenderPresetsMapIndexToCategory( int catIndex ) = 0;
	virtual int     RenderPresetsMapCategoryToIndex( TCHAR* category ) = 0;

	virtual int RenderPresetsPreSave( ITargetedIO * root, BitArray saveCategories ) = 0;
	virtual int RenderPresetsPostSave( ITargetedIO * root, BitArray loadCategories ) = 0;
	virtual int RenderPresetsPreLoad( ITargetedIO * root, BitArray saveCategories ) = 0;
	virtual int RenderPresetsPostLoad( ITargetedIO * root, BitArray loadCategories ) = 0;
};

#endif // I_CUSTOM_RENDER_PRESETS_H