/*	
 *		thread_locals.h - thread locals for each interpreter thread in MAXScript
 *
 *			Copyright © John Wainwright 1996
 *
 */

/* thread locals and initial values */

	def_thread_local( CharStream*, current_stdout,			new (GC_IN_HEAP) WindowStream(_M("Script Output")));
	def_thread_local( BOOL,		force_listener_open,		TRUE);			// whether to force listener open on output to it

	def_thread_local( Value**,	current_frame,				NULL);			// current interpreter frame (for thunk evals)
	def_thread_local( Value**,	current_scan_frame,			NULL);			// current interpreter frame (for gc scanner) 
	def_thread_local( Value**,	current_locals_frame,		NULL);			// C++ local frame
	def_thread_local( Value*,	current_result,				NULL);			// C++ current Value* function result
	def_thread_local( long,		stack_limit,				ALLOCATOR_STACK_SIZE);		// max stack size to catch recurse loops, 1Mb to start, 
																			// minus buffer to handle calls made during error handling
	def_thread_local( LONG_PTR,	stack_base,					(LONG_PTR)_alloca(sizeof(int)));	// current stack base
	def_thread_local( MSPlugin*, current_plugin,			NULL);			// current scripted plugin (for plugin thunk evals)
	def_thread_local( Struct*,	current_struct,				NULL);			// current struct (for struct member thunk evals)
	def_thread_local( Value*,	current_container,			NULL);			// current container for nested property access
	def_thread_local( int,		container_index,			0);				// current container index (if any)
	def_thread_local( Value*,	container_prop,				NULL);			// current container prop (if any)
	def_thread_local( Value*,	current_prop,				NULL);			// most recent prop access (if any)

	def_thread_local( Value*,	source_file,				NULL);			// current source file
	def_thread_local( INT_PTR,	source_pos,					0);				// current pos in source file

	def_thread_local( BOOL,		needs_redraw,				0);
	def_thread_local( BOOL,		redraw_mode,				1);				// redraw on
	def_thread_local( BOOL,		pivot_mode,					0);				// pivot off
	def_thread_local( BOOL,		undo_mode,					1);				// undo on
	def_thread_local( Value*,	current_level,				&all_objects);	// $objects
	def_thread_local( BOOL,		use_time_context,			0);				// use MAX time slider
	def_thread_local( TimeValue, current_time,				0);
	def_thread_local( Value*,	current_coordsys,			n_default);
	def_thread_local( Value*,	center_mode,				n_default);

	def_thread_local( int,		rand_accum,					0);				// for our own rand()
	def_thread_local( HANDLE,	message_event,				NULL);			// listener_message synch event
	def_thread_local( int,		stream_rand_accum,			0);				// for stream_rand()

	def_thread_local( MSZipPackage*, current_pkg,			NULL);			// currently open zip package, if any

	def_thread_local( void*,	alloc_frame,				NULL);			// top frame of allocator stack
	def_thread_local( void*,	alloc_tos,					NULL);			// top of allocator stack
	def_thread_local( void*,	alloc_stack_lim,			NULL);			// limit of allocator stack

	def_thread_local( Control*,	current_controller,			NULL);			// currently evaluating scripted controller

	def_thread_local( String*,	undo_label,					new (GC_PERMANENT) String(_M("MAXScript"))); // current undo label
	def_thread_local( BOOL,		try_mode,					0);				// try(...)
	def_thread_local( MAXScriptException*, current_exception,	NULL);		// current exception that was thrown, if any. Non-null only in catch expression

	def_thread_local( BOOL,		thread_not_active,			FALSE);			// set to TRUE when thread found not to be active any more

	def_thread_local( BOOL,		err_occurred,				FALSE);			// set to TRUE when error thrown
	def_thread_local( Value*,	err_source_file,			NULL);			// source file where error thrown
	def_thread_local( INT_PTR,	err_source_pos,				0);				// pos in source file where error thrown

	def_thread_local( Value*,	current_frame_owner,		NULL);			// owner of current stack frame while compiling
	
	def_thread_local( BOOL,		is_dubugger_thread,			FALSE);			// owner of current stack frame while compiling

	def_thread_local( DWORD,	source_flags,				0);				// current source file flags
	def_thread_local( DWORD,	err_source_flags,			0);				// source file flags for source file where error thrown
