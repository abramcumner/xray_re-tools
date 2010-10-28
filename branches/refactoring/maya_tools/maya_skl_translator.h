#pragma once
#ifndef __MAYA_SKL_TRANSLATOR_H__
#define __MAYA_SKL_TRANSLATOR_H__

#include "maya_object_translator.h"

class maya_skl_translator: public maya_object_translator {
public:
	static MStatus		initialize(MFnPlugin& plugin_fn);
	static MStatus		uninitialize(MFnPlugin& plugin_fn);

	virtual MStatus		reader(const MFileObject& file, const MString& options, FileAccessMode mode);
	virtual MStatus		writer(const MFileObject& file, const MString& options, FileAccessMode mode);
	virtual MString		defaultExtension() const;
	virtual MString		filter() const;
	virtual bool		canBeOpened() const;

private:
	static void*		creator();

private:
	static const MString	k_name;
	static char		k_options_script[];
	static char		k_default_options[];
};

#endif
