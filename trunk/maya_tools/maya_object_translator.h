#pragma once
#ifndef __MAYA_OBJECT_TRANSLATOR_H__
#define __MAYA_OBJECT_TRANSLATOR_H__

#include <vector>
#define NOMINMAX
#include <maya/MPxFileTranslator.h>
#include "maya_fix_fpu_cw.h"
#include "maya_progress.h"

class MFnPlugin;

class maya_object_translator: public MPxFileTranslator {
public:
	virtual			~maya_object_translator();

	static MStatus		initialize(MFnPlugin& plugin_fn);
	static MStatus		uninitialize(MFnPlugin& plugin_fn);

	virtual MStatus		reader(const MFileObject& file, const MString& options, FileAccessMode mode);
	virtual MStatus		writer(const MFileObject& file, const MString& options, FileAccessMode mode);
	virtual bool		haveReadMethod() const;
	virtual bool		haveWriteMethod() const;
	virtual MString		defaultExtension() const;
	virtual MString		filter() const;
	virtual bool		canBeOpened() const;
	virtual MFileKind	identifyFile(const MFileObject& file, const char* buffer, short size) const;

protected:
	static MString		extract_extension(const MFileObject& file);

private:
	static void*		creator();

private:
	static const MString	k_name;
	static char		k_options_script[];
	static char		k_default_options[];
};

class maya_skl_object_writer: public maya_object_translator {
public:
	static MStatus		initialize(MFnPlugin& plugin_fn);
	static MStatus		uninitialize(MFnPlugin& plugin_fn);

	virtual MStatus		writer(const MFileObject& file, const MString& options, FileAccessMode mode);
	virtual bool		haveReadMethod() const;
	virtual bool		canBeOpened() const;

private:
	static void*		creator();

private:
	static const MString	k_name;
	static char		k_options_script[];
	static char		k_default_options[];
};

#endif
