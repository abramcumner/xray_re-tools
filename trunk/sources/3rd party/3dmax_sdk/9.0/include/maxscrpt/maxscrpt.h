/*		MAXScript.h - main include for MAXScript sources
 *
 *		Copyright (c) John Wainwright, 1996
 *		
 *
 */

#ifndef _H_MAXSCRIPT
#define _H_MAXSCRIPT

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500	// windows 2000 or higher
#endif

#ifndef STRICT
#define STRICT					// strict type-checking - conformance with MAX SDK libs
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// trims win32 includes
#endif

#ifdef BLD_MAXSCRIPT
#	define ScripterExport __declspec( dllexport )
#else
#	define ScripterExport __declspec( dllimport )
#	define IMPORTING
#endif
#include <export.h>


#include <stdlib.h> 
#include <stdio.h>
#include <windows.h>
#include <float.h>

#include <Max.h>
#include <strbasic.h>

#define NOT_SUPPORTED_BY_PRODUCT(fn)	\
    throw RuntimeError (_M("Feature not available: "), _M(#fn)); 

/* utility defines */

#define END				NULL					// null varargs arg list terminator

class Value;
class CharStream;
class Rollout;
class MAXScriptException;
class MSPlugin;
class Struct;
class MSZipPackage;
class String;
class IMXSDebugger;

#define MAXSCRIPT_UTILITY_CLASS_ID	Class_ID(0x4d64858, 0x16d1751d)
#define MAX_SCRIPT_DIR				_M("scripts")
#define SCRIPT_AUTOLOAD_DIR			_M("Startup\\")

// check whether we are UNICODE or Code page 0 (==> no mbcs code)
#ifdef _UNICODE
#	define	no_mb_chars		TRUE
#	define	bytelen(s)		(sizeof(mwchar_t) * wcslen(s))
#else
#	define	no_mb_chars     (MB_CUR_MAX == 1)
#	define	bytelen(s)		strlen(s)
#endif

inline double EPS(double v) { return _isnan(v) ? (v) : fabs(v) < FLT_EPSILON ? 0.0 : (v); }  // small number round down for %g float printing

/* MAXScript-specific window messages */

#define MXS_ADD_ROLLOUT_PAGE		(WM_USER + 0x100)
#define MXS_DELETE_ROLLOUT_PAGE		(WM_USER + 0x101)
#define MXS_REDRAW_VIEWS			(WM_USER + 0x102)
#define MXS_EDIT_SCRIPT				(WM_USER + 0x103)
#define MXS_NEW_SCRIPT				(WM_USER + 0x104)
#define MXS_DISPLAY_BITMAP			(WM_USER + 0x105)
#define MXS_ERROR_MESSAGE_BOX		(WM_USER + 0x106)
#define MXS_PRINT_STRING			(WM_USER + 0x107)
#define MXS_LISTENER_EVAL			(WM_USER + 0x108)
#define MXS_MESSAGE_BOX				(WM_USER + 0x109)
#define MXS_INITIALIZE_MAXSCRIPT	(WM_USER + 0x10A)
#define MXS_KEYBOARD_INPUT			(WM_USER + 0x10B)
#define MXS_SHOW_SOURCE				(WM_USER + 0x10C)
#define MXS_TAKE_FOCUS				(WM_USER + 0x10D)
#define MXS_STOP_CREATING			(WM_USER + 0x10E)
#define MXS_CLOSE_DOWN				(WM_USER + 0x10F)
#define MXS_STOP_EDITING			(WM_USER + 0x110)
#define MXS_LOAD_STARTUP_SCRIPTS	(WM_USER + 0x111)
#define MXS_EXECUTE_MACRO			(WM_USER + 0x112)
#define MXS_RESTART_EDITING			(WM_USER + 0x113)
#define MXS_NOTIFY_REF_DEPENDENTS	(WM_USER + 0x114)
#define MSPLUGINCLASS_STOPEDITING	(WM_USER + 0x115)
#define MSPLUGINCLASS_RESTARTEDITING	(WM_USER + 0x116)

typedef struct			// LPARAM for MXS_MESSAGE_BOX contains a pointer to this structure
{
	MCHAR* title;
	MCHAR* message;
	BOOL beep;
	int flags;
	BOOL result;
} message_box_data;

class mxs_notify_ref_data			// LPARAM for MXS_NOTIFY_REF_DEPENDENTS contains a pointer to this class
{
public:
	RefTargetHandle hSender;
	Interval changeInt;
	PartID partID;
	RefMessage message;
	SClass_ID sclass;
	BOOL propagate;
	RefTargetHandle hTarg;
	RefResult result;
	
	mxs_notify_ref_data(RefTargetHandle hSender, PartID partID = PART_ALL, RefMessage message = REFMSG_CHANGE) : 
		changeInt(FOREVER), partID(partID), message(message), sclass(NOTIFY_ALL), propagate(TRUE),
		hTarg(NULL), hSender(hSender), result(REF_SUCCEED)
		{
			DbgAssert(hSender);
		}
};

/* thread-local storage struct decl & access macros */

typedef struct MAXScript_TLS MAXScript_TLS;
struct MAXScript_TLS
{
	MAXScript_TLS*	next;				/* links...							*/
	MAXScript_TLS*	prev;
	HANDLE			my_thread;			/* thread that owns this TLS struct	*/
	DWORD			my_thread_id;

#undef def_thread_local
#define def_thread_local(type, lcl, init_val) type lcl
#	include "thrdlcls.h"

};

#define thread_local(x)			(((MAXScript_TLS*)TlsGetValue(thread_locals_index))->x)

/* index, tls struct list globals */

extern ScripterExport int thread_locals_index;
extern int thread_id_index;
extern MAXScript_TLS* MAXScript_TLS_list;

#define needs_redraw_set()			thread_local(needs_redraw) = 1
#define needs_complete_redraw_set()	thread_local(needs_redraw) = 2
#define needs_redraw_clear()		thread_local(needs_redraw) = 0

#define MAXScript_time()	\
	(thread_local(use_time_context) ? thread_local(current_time) : MAXScript_interface->GetTime())

			   void alloc_thread_locals();
ScripterExport void init_thread_locals();
			   void free_thread_locals();
			   void copy_thread_locals(MAXScript_TLS* source, MAXScript_TLS* target);

/* error handlers */

void out_of_memory();
void bad_delete();

/* arg count check & keyword arg accessors (assume conventional names for arg_list & count in using function) */

extern ScripterExport Value* _get_key_arg(Value** arg_list, int count, Value* key_name);
extern ScripterExport Value* _get_key_arg_or_default(Value** arg_list, int count, Value* key_name, Value* def);

#define key_arg(key)					_get_key_arg(arg_list, count, n_##key)
#define key_arg_or_default(key, def)	_get_key_arg_or_default(arg_list, count, n_##key##, def)
#define int_key_arg(key, var, def)		((var = _get_key_arg(arg_list, count, n_##key)) == &unsupplied ? def : var->to_int())
#define float_key_arg(key, var, def)	((var = _get_key_arg(arg_list, count, n_##key)) == &unsupplied ? def : var->to_float())
#define bool_key_arg(key, var, def)		((var = _get_key_arg(arg_list, count, n_##key)) == &unsupplied ? def : var->to_bool())
#define interval_key_arg(key, var, def)		((var = _get_key_arg(arg_list, count, n_##key)) == &unsupplied ? def : var->to_interval())
#define timevalue_key_arg(key, var, def)	((var = _get_key_arg(arg_list, count, n_##key)) == &unsupplied ? def : var->to_timevalue())
#define node_key_arg(key, var, def)			((var = _get_key_arg(arg_list, count, n_##key)) == &unsupplied ? def : var->to_node())
#define string_key_arg(key, var, def)		((var = _get_key_arg(arg_list, count, n_##key)) == &unsupplied ? def : var->to_string())

#define check_arg_count(fn, w, g)			if ((w) != (g)) throw ArgCountError (_M(#fn), w, g)
#define check_gen_arg_count(fn, w, g)		if ((w) != (g + 1)) throw ArgCountError (_M(#fn), w, g + 1)
#define check_arg_count_with_keys(fn, w, g) if (!(g == w || (g > w && arg_list[w] == &keyarg_marker))) throw ArgCountError (_M(#fn), w, count_with_keys())
#define check_gen_arg_count_with_keys(fn, w, g) if (!(g == w || (g > w && arg_list[w-1] == &keyarg_marker))) throw ArgCountError (_M(#fn), w, count_with_keys() + 1)
#define count_with_keys()					_count_with_keys(arg_list, count)

/* for functions that return a boolean */

#define bool_result(val)					((val) ? &true_value : &false_value)

/* for testing a value to ensure that it is within range */

// following only valid for integer and float range checking
#define MXS_range_check(_val, _lowerLimit, _upperLimit, _desc)				\
	if (_val < _lowerLimit || _val > _upperLimit) {							\
		MCHAR buf[256];														\
		MCHAR buf2[128];													\
		_tcscpy(buf,_desc);													\
		_tcscat(buf,_M(" < "));												\
		_sntprintf(buf2, 128, _M("%g"), EPS(_lowerLimit));					\
		_tcscat(buf,buf2);													\
		_tcscat(buf,_M(" or > "));											\
		_sntprintf(buf2, 128, _M("%g"), EPS(_upperLimit));					\
		_tcscat(buf,buf2);													\
		_tcscat(buf,_M(": "));												\
		_sntprintf(buf2, 128, _M("%g"), EPS(_val));							\
		_tcscat(buf,buf2);													\
		throw RuntimeError (buf);											\
	}


/* value local macros - for managing C local variable references to Value*'s for the collector - see Collectable.cpp */

#define one_value_local(n1)											\
	struct { int count; Value** link; Value* n1; } vl =				\
		{ 1, NULL, NULL };											\
	vl.link = thread_local(current_locals_frame);					\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define one_typed_value_local(n1)									\
	struct { int count; Value** link; n1; } vl =					\
		{ 1, NULL, NULL };											\
	vl.link = thread_local(current_locals_frame);					\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define two_value_locals(n1, n2)									\
	struct { int count; Value** link; Value *n1, *n2; } vl =		\
		{ 2, NULL, NULL, NULL };									\
	vl.link = thread_local(current_locals_frame);					\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define two_typed_value_locals(n1, n2)								\
	struct { int count; Value** link; n1; n2; } vl =				\
		{ 2, NULL, NULL, NULL };									\
	vl.link = thread_local(current_locals_frame);					\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define three_value_locals(n1, n2, n3)								\
	struct { int count; Value** link; Value *n1, *n2, *n3; } vl =	\
		{ 3, NULL, NULL, NULL, NULL };								\
	vl.link = thread_local(current_locals_frame);					\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define three_typed_value_locals(n1, n2, n3)						\
	struct { int count; Value** link; n1; n2; n3; } vl =			\
		{ 3, NULL, NULL, NULL, NULL };								\
	vl.link = thread_local(current_locals_frame);					\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define four_value_locals(n1, n2, n3, n4)								\
	struct { int count; Value** link; Value *n1, *n2, *n3, *n4; } vl =	\
		{ 4, NULL, NULL, NULL, NULL, NULL };							\
	vl.link = thread_local(current_locals_frame);						\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define four_typed_value_locals(n1, n2, n3, n4)						\
	struct { int count; Value** link; n1; n2; n3; n4; } vl =		\
		{ 4, NULL, NULL, NULL, NULL, NULL };						\
	vl.link = thread_local(current_locals_frame);					\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define five_value_locals(n1, n2, n3, n4, n5)								\
	struct { int count; Value** link; Value *n1, *n2, *n3, *n4, *n5; } vl =	\
		{ 5, NULL, NULL, NULL, NULL, NULL, NULL };						    \
	vl.link = thread_local(current_locals_frame);						    \
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define five_typed_value_locals(n1, n2, n3, n4, n5)							\
	struct { int count; Value** link; n1; n2; n3; n4; n5; } vl =			\
		{ 5, NULL, NULL, NULL, NULL, NULL, NULL };						    \
	vl.link = thread_local(current_locals_frame);						    \
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define six_value_locals(n1, n2, n3, n4, n5, n6)								 \
	struct { int count; Value** link; Value *n1, *n2, *n3, *n4, *n5, *n6; } vl = \
		{ 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL };						 \
	vl.link = thread_local(current_locals_frame);								 \
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define six_typed_value_locals(n1, n2, n3, n4, n5, n6)						\
	struct { int count; Value** link; n1; n2; n3; n4; n5; n6; } vl =		\
		{ 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL };					\
	vl.link = thread_local(current_locals_frame);							\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define seven_value_locals(n1, n2, n3, n4, n5, n6, n7)								  \
	struct { int count; Value** link; Value *n1, *n2, *n3, *n4, *n5, *n6, *n7; } vl = \
		{ 7, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };						  \
	vl.link = thread_local(current_locals_frame);									  \
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define seven_typed_value_locals(n1, n2, n3, n4, n5, n6, n7)				\
	struct { int count; Value** link; n1; n2; n3; n4; n5; n6; n7; } vl =	\
		{ 7, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };				\
	vl.link = thread_local(current_locals_frame);							\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define eight_value_locals(n1, n2, n3, n4, n5, n6, n7, n8)								   \
	struct { int count; Value** link; Value *n1, *n2, *n3, *n4, *n5, *n6, *n7, *n8; } vl = \
		{ 8, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };					   \
	vl.link = thread_local(current_locals_frame);										   \
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define eight_typed_value_locals(n1, n2, n3, n4, n5, n6, n7, n8)				\
	struct { int count; Value** link; n1; n2; n3; n4; n5; n6; n7; n8; } vl =	\
		{ 8, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };			\
	vl.link = thread_local(current_locals_frame);								\
	thread_local(current_locals_frame) = (Value**)&vl;

// LAM - 6/07/02 - added new defines - no SDK impact	
#define nine_value_locals(n1, n2, n3, n4, n5, n6, n7, n8, n9)									\
	struct { int count; Value** link; Value *n1, *n2, *n3, *n4, *n5, *n6, *n7, *n8, *n9; } vl = \
		{ 9, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };						\
	vl.link = thread_local(current_locals_frame);												\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define nine_typed_value_locals(n1, n2, n3, n4, n5, n6, n7, n8, n9)					\
	struct { int count; Value** link; n1; n2; n3; n4; n5; n6; n7; n8; n9; } vl =	\
		{ 9, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };			\
	vl.link = thread_local(current_locals_frame);									\
	thread_local(current_locals_frame) = (Value**)&vl;
	
#define ten_value_locals(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)									\
struct { int count; Value** link; Value *n1, *n2, *n3, *n4, *n5, *n6, *n7, *n8, *n9, *n10; } vl =	\
		{ 10, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };					\
		vl.link = thread_local(current_locals_frame);												\
		thread_local(current_locals_frame) = (Value**)&vl;

#define ten_typed_value_locals(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)				\
struct { int count; Value** link; n1; n2; n3; n4; n5; n6; n7; n8; n9; n10; } vl =	\
		{ 10, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };	\
		vl.link = thread_local(current_locals_frame);								\
		thread_local(current_locals_frame) = (Value**)&vl;

#define value_local_array(var, count) {								\
	var = &((Value**)_alloca(((count) + 2) * sizeof(Value*)))[2];	\
	memset(var, 0, (count) * sizeof(Value*));						\
   var[-2] = (Value*)(static_cast<INT_PTR>(count));            \
	var[-1] = (Value*)thread_local(current_locals_frame);			\
	thread_local(current_locals_frame) = &var[-2]; }

#define pop_value_local_array(var)									\
	thread_local(current_locals_frame) = (Value**)var[-1];
	
#define value_temp_array(var, count)	{							\
	var = &((Value**)ms_malloc(((count) + 2) * sizeof(Value*)))[2];	\
	memset(var, 0, (count) * sizeof(Value*));						\
   var[-2] = (Value*)(static_cast<INT_PTR>(count));            \
   var[-1] = (Value*)thread_local(current_locals_frame);       \
   thread_local(current_locals_frame) = &var[-2];}

// SR NOTE64: If your 'count' is the constant zero, then use this version instead otherwise
// you may end up with "warning C4318: passing constant zero as the length to memset"
// ie: value_temp_array(var, 0) should be value_temp_array0(var)
#define value_temp_array0(var)   {                              \
   var = &((Value**)ms_malloc(2 * sizeof(Value*)))[2];   \
   var[-2] = 0; \
	var[-1] = (Value*)thread_local(current_locals_frame);			\
	thread_local(current_locals_frame) = &var[-2];}

// LAM - 6/07/02 - fix for when multiple value_temp_arrays in same frame
#define realloc_value_temp_array(var, count, old_count)	{						\
	Value **oldPointer = &var[-2];												\
	Value **sframe = thread_local(current_locals_frame);						\
	var = &((Value**)ms_realloc(&var[-2], ((count) + 2) * sizeof(Value*)))[2];		\
	if ((count) > (old_count))													\
		memset(&var[(old_count)], 0, ((count) - (old_count)) * sizeof(Value*));	\
   var[-2] = (Value*)(static_cast<INT_PTR>(count));               \
	Value **newPointer = &var[-2];												\
	if (sframe == oldPointer)													\
	{	thread_local(current_locals_frame) = newPointer;						\
		DbgAssert( newPointer != (Value**)newPointer[1] );						\
	}																			\
	else																		\
	{	Value **frame;															\
		for (frame = sframe; frame && (Value**)frame[1] != oldPointer && frame != (Value**)frame[1]; frame = (Value**)frame[1]);	\
		if (frame) {															\
			DbgAssert( frame != (Value**)frame[1] );							\
			frame[1] = (Value*)newPointer;										\
		}																		\
	}																			\
	}

// LAM - 6/07/02 - fix for when multiple value_temp_arrays in same frame
#define pop_value_temp_array(var)	{							\
	Value **oldPointer = &var[-2];								\
/*	Value **sframe = thread_local(current_locals_frame); */		\
	Value **head = (Value**)var[-1];							\
	thread_local(current_locals_frame) = head;					\
	ms_free(oldPointer);											\
	}
	
#define return_value(r)	{										\
	thread_local(current_result) = r;							\
	thread_local(current_locals_frame) = vl.link;				\
	return r; }
	 
#define return_value_no_pop(r) {								\
	thread_local(current_result) = r;							\
	return r; }

#define return_protected(r) {									\
	thread_local(current_result) = r;							\
	return r; }

#define pop_value_locals()										\
	thread_local(current_locals_frame) = vl.link;

#define reset_locals_frame()									\
	thread_local(current_locals_frame) = (Value**)&vl;

#define reset_locals_array_frame(var)							\
	thread_local(current_locals_frame) = &var[-2]; 

#define clear_current_frames() {								\
	thread_local(current_locals_frame) = NULL;					\
	thread_local(current_frame) = NULL;}

#define save_current_frames()									\
	Value** _sclf = thread_local(current_locals_frame);			\
	Value** _scsf = thread_local(current_scan_frame);			\
	Value** _scf = thread_local(current_frame);
	
#define restore_current_frames() {								\
	thread_local(current_locals_frame) = _sclf;					\
	thread_local(current_scan_frame) = _scsf;					\
	thread_local(current_frame) = _scf;}

#define save_current_source()									\
	Value* _save_source = thread_local(source_file);			\
	MSZipPackage* _save_pkg = thread_local(current_pkg);		\
	int _save_pos = thread_local(source_pos);					\
	DWORD _save_flags = thread_local(source_flags);

#define restore_current_source() {								\
	thread_local(source_file) = _save_source;					\
	thread_local(current_pkg) = _save_pkg;						\
	thread_local(source_pos) = _save_pos;						\
	thread_local(source_flags) = _save_flags;}

#define save_current_source_no_pos()							\
	Value* _save_source = thread_local(source_file);			\
	MSZipPackage* _save_pkg = thread_local(current_pkg);		\
	DWORD _save_flags = thread_local(source_flags);

#define restore_current_source_no_pos() {						\
	thread_local(source_file) = _save_source;					\
	thread_local(current_pkg) = _save_pkg;						\
	thread_local(source_flags) = _save_flags;}

#define save_current_source_to_frame() {						\
	Value** _frame = thread_local(current_scan_frame);			\
	if (_frame) _frame[3] = thread_local(source_file);			\
	if (_frame) _frame[4] = (Value*)thread_local(source_pos);	\
   if (_frame) _frame[7] = (Value*)static_cast<DWORD_PTR>(thread_local(source_flags));}

#define save_current_source_to_frame_no_pos() {					\
	Value** _frame = thread_local(current_scan_frame);			\
	if (_frame) _frame[3] = thread_local(source_file);			\
   if (_frame) _frame[7] = (Value*)static_cast<DWORD_PTR>(thread_local(source_flags));}

#define save_current_source_to_frame_no_file() {				\
	Value** _frame = thread_local(current_scan_frame);			\
	if (_frame) _frame[4] = (Value*)thread_local(source_pos);}

#define push_control(_c)										\
	Control* _save_cc = thread_local(current_controller);		\
	thread_local(current_controller) = _c;

#define pop_control()											\
	thread_local(current_controller) = _save_cc;

#define save_error_source_data() {								\
	thread_local(err_source_file) = thread_local(source_file);	\
	thread_local(err_source_pos) = thread_local(source_pos);	\
	thread_local(err_source_flags) = thread_local(source_flags);\
	thread_local(err_occurred) = TRUE;}

#define clear_error_source_data() {								\
	thread_local(err_source_file) = NULL;						\
	thread_local(err_source_pos) = 0;							\
	thread_local(err_source_flags) = 0;							\
	thread_local(err_occurred) = FALSE;}

/* general utilities */

ScripterExport MCHAR* save_string(const MCHAR* str);
MCHAR  wputch(HWND w, MCHAR* buf, MCHAR* bufp, const MCHAR c);					/* edit window output... */
MCHAR* wputs(HWND w, MCHAR* buf, MCHAR* bufp, const MCHAR *str);		
int    wprintf(HWND w, MCHAR* buf, MCHAR* bufp, const MCHAR *format, ...);
void   wflush(HWND w, MCHAR* buf, MCHAR* bufp);		
#define mputs	thread_local(current_stdout)->puts	/* current MAXScript stdout output... */
#define mprintf	thread_local(current_stdout)->printf
#define mflush	thread_local(current_stdout)->flush
extern MCHAR *GetString(int id);
extern ScripterExport Object *Get_Object_Or_XRef_BaseObject(Object *obj);

 
class Rollout;
class HashTable;
ScripterExport void  install_utility_page(Rollout* rollout);

class Value;
typedef Value* (Value::*value_vf)(Value**, int);
typedef Value* (*value_cf)(Value**, int);
typedef Value* (Value::*getter_vf)(Value**, int);
typedef Value* (Value::*setter_vf)(Value**, int);
typedef Value* (*max_getter_cf)(ReferenceTarget*, Value*, TimeValue, Interval&);
typedef void   (*max_setter_cf)(ReferenceTarget*, Value*, TimeValue, Value*);

/* MAXScript signal flags */

#define INTERRUPT_EVAL		0x0001
#define EXIT_LISTENER		0x0002

extern ScripterExport Interface* MAXScript_interface;
extern ScripterExport Interface7* MAXScript_interface7;
extern ScripterExport Interface8* MAXScript_interface8;
extern ScripterExport Interface9* MAXScript_interface9;
extern ScripterExport int		MAXScript_signals;
extern ScripterExport BOOL		escape_enabled; // RK: 05/20/02, 5.0 or later only
extern ScripterExport BOOL		check_maxscript_interrupt;
extern ScripterExport void		escape_checker();
extern ScripterExport BOOL		MAXScript_detaching;
extern ScripterExport int		mxs_rand(); // resolution is 0 - 2^31-1
extern ScripterExport INT64		mxs_rand64(); // resolution is 0 - 2^63-1
extern ScripterExport void		mxs_seed(int);
extern ScripterExport int		random_range(int from, int to);
extern ScripterExport INT64		random_range(INT64 from, INT64 to);
extern ScripterExport float		random_range(float from, float to);
extern ScripterExport double	random_range(double from, double to);
extern ScripterExport void		dlx_detaching(HINSTANCE hinstance);
extern ScripterExport void		define_system_global(MCHAR* name, Value* (*getter)(), Value* (*setter)(Value*));
// LAM 4/1/00 - added following to be able to overwrite existing global value in hash table.
extern ScripterExport void		define_system_global_replace(MCHAR* name, Value* (*getter)(), Value* (*setter)(Value*));
extern ScripterExport void		define_struct_global(MCHAR* name, MCHAR* struct_name, Value* (*getter)(), Value* (*setter)(Value*));
extern ScripterExport HashTable* english_to_local;
extern ScripterExport HashTable* local_to_english;
extern ScripterExport BOOL		non_english_numerics;
extern ScripterExport void	    printable_name(MSTR& name);
extern ScripterExport void		show_source_pos();
extern ScripterExport void		show_listener();

extern ScripterExport void		init_MAXScript();
extern ScripterExport BOOL		MAXScript_running;
extern ScripterExport HWND		main_thread_window;
extern ScripterExport BOOL		progress_bar_up;
extern ScripterExport BOOL		trace_back_active;
extern ScripterExport BOOL		disable_trace_back;
extern ScripterExport int		trace_back_levels;
typedef void (*utility_installer)(Rollout* ro);
extern ScripterExport void		set_utility_installer(utility_installer ui);
extern ScripterExport void		reset_utility_installer();
extern ScripterExport void		error_message_box(MAXScriptException& e, MCHAR* caption);
typedef Value* (*autocad_point_reader)(MCHAR* str);
extern ScripterExport void		set_autocad_point_reader(autocad_point_reader apr);

// LAM - 4/28/03
// Returns TRUE if script was executed successfully. If quietErrors == false and net rendering, errors are logged to LogSys. 
// If not net rendering, errors are logged to Listener. Return value from script stored in fpv, if specified.
extern ScripterExport BOOL		ExecuteMAXScriptScript(MCHAR *s, BOOL quietErrors = FALSE, FPValue *fpv = NULL);

// LAM - 8/19/05
// Returns Value* resulting from evaluating source. If res is non-NULL, will contain TRUE on return if script was executed 
// successfully, FALSE if not. Evaluation is within a try/catch, so this function will not throw.
extern ScripterExport Value*	ExecuteScript(CharStream* source, bool *res);

// LAM - 6/24/03
// Method for processing input value or array for default action value. Current recognized values are:
// #logMsg, #logToFile, #abort, and integer values. Actions are converted to DWORD where bit 0 is log to Default Action 
// system log, bit 1 is log to log file, and bit 2 is abort/cancel (if applicable).
extern ScripterExport DWORD		ProcessDefaultActionVal (Value* inpActionVal, DWORD defaultAction = DEFAULTACTIONS_LOGMSG);

// LAM - 3/25/05 - the MXS debugger interface. A Singleton.
extern ScripterExport IMXSDebugger* theMXSDebugger;

#define check_interrupts()	if (check_maxscript_interrupt) escape_checker(); if (MAXScript_signals) throw SignalException()

#define type_check(val, cl, where) if (val->tag != class_tag(cl)) throw TypeError (where, val, &cl##_class);

// macros for setting numeric printing to English locale and back again - all numeric output in MAXScript is English
#define set_english_numerics()						\
	MCHAR* locale;									\
	MCHAR slocale[256];								\
	if (non_english_numerics != NULL)				\
	{												\
		locale = setlocale(LC_NUMERIC, NULL);		\
		_tcsncpy(slocale, locale, sizeof(slocale));	\
		setlocale(LC_NUMERIC, "C");					\
	}

#define reset_numerics()							\
	if (non_english_numerics != NULL)				\
		setlocale(LC_NUMERIC, slocale);	

#define SOURCE_STREAM_ENCRYPTED_FLAG 1 /* flag set in thread local source_flags when source is encrypted  */

#include "Excepts.h"
#include "iFnPub.h"
#include "Value.h"
#include "Streams.h"
#include "SceneIO.h"
#include "IParamm2.h"

class HashTable;
class Listener;
extern ScripterExport HashTable* globals;
extern ScripterExport HashTable* persistents;
extern ScripterExport Listener*  the_listener;
extern ScripterExport HWND		 the_listener_window;
extern                HINSTANCE  hInstance;
extern ScripterExport void       listener_message(UINT iMsg, WPARAM wParam, LPARAM lParam, BOOL block_flag);

class RandGenerator;
extern ScripterExport RandGenerator *ClassIDRandGenerator;

inline int _count_with_keys(Value** arg_list, int count)
{
	// compute # args before any key-args
	for (int i = 0; i < count; i++)
		if (arg_list[i] == (Value*)&keyarg_marker)
			return i;
	return count;
}

extern ScripterExport BOOL GetPrintAllElements(); // get whether to print all elements of arrays, meshselection, BigMatrix, etc.
extern ScripterExport BOOL SetPrintAllElements(BOOL); // set whether to print all elements of arrays, meshselection, BigMatrix, etc. Returns old value

extern ScripterExport bool CanChangeGroupFlags(INode* node); // Since grouping break the node hierarchy used to represent ALOs, we don't allow it

// get MXS path to object. This is the path shown in macro recorder. If resolved, returns TRUE and places path in name
extern ScripterExport BOOL Find_MXS_Name_For_Obj(Animatable* obj, MSTR &name, BOOL explicitName = TRUE); 

// methods to convert LF strings to CR/LF strings and back
extern ScripterExport void Replace_LF_with_CRLF(MSTR &string);
extern ScripterExport void Replace_CRLF_with_LF(MSTR &string);

extern ScripterExport void checkFileOpenModeValidity(MCHAR* mode); // checks validity of mode for fopen commands, and throws error if not valid

extern ScripterExport BOOL max_name_match(MCHAR* max_name, MCHAR* pattern, bool caseSensitive = false);// returns true if name matches pattern. Will test original and translated name.

// MAXScript preferences.  An instance of this class lives in CORE and is accessible to both CORE and MAXScript
class MAXScriptPrefs
{
public:
	int		loadStartupScripts;
	int		loadSaveSceneScripts;
	int		loadSavePersistentGlobals;
	MSTR	font;
	int		fontSize;
	int		autoOpenListener;
	float	initialHeapSize;
	int		enableMacroRecorder;
	int		showCommandPanelSwitch;
	int		showToolSelections;
	int		showMenuSelections;
	int		absoluteSceneNames;
	int		absoluteSubObjects;
	int		absoluteTransforms;
	int		explicitCoordinates;
	int		useFastNodeNameLookup;
	int		showGCStatus;
	int		showEditorPath;

	MAXScriptPrefs() { Reset(); }
	void	Reset()
	{
		// MAXScript preference defaults
		loadStartupScripts =		TRUE;
		loadSaveSceneScripts =		TRUE;
		loadSavePersistentGlobals = TRUE;
		font =						_M("Courier New");
		fontSize =					12;
		initialHeapSize =			7.5;
		autoOpenListener =			FALSE;
		enableMacroRecorder =		FALSE;
		showCommandPanelSwitch =	FALSE;
		showToolSelections =		FALSE;
		showMenuSelections =		FALSE;
		absoluteSceneNames =		FALSE;
		absoluteSubObjects =		FALSE;
		absoluteTransforms =		FALSE;
		explicitCoordinates =		FALSE;
		useFastNodeNameLookup =		TRUE;
		showGCStatus =				TRUE;
		showEditorPath =			TRUE;
	}

	virtual void LoadMAXScriptPreferences();
	virtual void SaveMAXScriptPreferences();
};

// temporarily adds license for various activeX controls
// ctor registers the license if needed, dtor removes them
class ActiveXLicensor
{
	DWORD licenseAdded; // acts as bitfield. Each bit signifies whether a specifc license was added.
public:
	ScripterExport ActiveXLicensor();
	ScripterExport ~ActiveXLicensor();
};

// Exception safe helper class for controlling temporary changing of quiet mode. ctor sets quiet mode to 
// specified state, Restore resets to initial state, and dtor resets to initial state if Restore has not been run.
class TempQuietMode
{
public:
	TempQuietMode (BOOL newState) : needsRestore(true)
	{
		oldState = MAXScript_interface->SetQuietMode(newState);
	}
	~TempQuietMode()
	{
		if (needsRestore)
			MAXScript_interface->SetQuietMode(oldState);
	}
	void Restore()
	{
		if (needsRestore)
		{
			MAXScript_interface->SetQuietMode(oldState);
			needsRestore = false;
		}
	}
private:
	BOOL oldState;
	bool needsRestore;
};

#endif
