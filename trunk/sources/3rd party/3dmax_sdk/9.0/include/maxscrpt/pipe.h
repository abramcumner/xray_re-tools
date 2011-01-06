/*	
 *		Pipe.h - NT MCHAR Pipe wrapper for MAXScript
 *
 *			Copyright © John Wainwright 1996
 *
 */

#ifndef _H_PIPE
#define _H_PIPE

#include "Strings.h"
class FileStream;

#define PIPE_BUF_SIZE	512

// The undelivered data in the pipe is held in a linked list of
// buffers, pointed into by read and write cursors.  
// A side list is kept if writers supply info about sourcing files.
// This is provided to readers like the compiler to add source
// tags to generated code.  

typedef struct src_info src_info;
struct src_info						
{
	src_info*	next;			// next marker
	MCHAR*		start;			// source start character in buffer chain
	Value*		file;			// sourcing file name if any
	int			offset;			// starting offset into source
};

// SR NOTE64: Changed size_t to UINT, which in 32 bits doesn't change anything but in
// 64 bits "limits" us to 4GB.
class Pipe : public CharStream	
{
public:
	MCHAR*		write_buffer;		// pipe buffers & cursors
	MCHAR*		write_cursor;
	MCHAR*		read_buffer;
	MCHAR*		read_cursor;
	int			ungetch_count;

	CRITICAL_SECTION pipe_update;	// for syncing pipe updates
	HANDLE		pipe_event;			// for signalling data ready
	HANDLE		restart_event;		// used to restart a stopped pipe
	BOOL		waiting;			// reader is waiting for data
	BOOL		stopped;			// pipe reading is blocked

	FileStream* log;				// log stream if non-NULL

	Value*		read_source_file;	// sourcing file for reading if supplied by writer
	int			read_source_offset;	// running reader offset in source
	src_info*	markers;			// marker list...
	src_info*	marker_tail;		
	MCHAR*		next_source_start;	// upcoming marker starting character
	Value*		write_source_file;	// current write source file, used to determine source change
	int			write_source_offset;// running writer offset

				Pipe();
				~Pipe();
				
#	define		is_pipe(o) ((o)->tag == INTERNAL_PIPE_TAG)
	void		collect() { delete this; }
 	void		gc_trace();

	MCHAR		get_char();
	void		unget_char(MCHAR c);
	MCHAR		peek_char();
	int			at_eos();
	int			pos() { return read_source_offset; }
	void		rewind();
	void		flush_to_eol();
	void		flush_to_eobuf();

	void		put_char(MCHAR c, Value* source_file = NULL, int offset = 0);
	void		put_str(MCHAR* str, Value* source_file = NULL, int offset = 0);
	void		put_buf(MCHAR* str, UINT count, Value* source_file = NULL, int offset = 0);
	void		new_write_buffer();
	void		check_write_source_change(Value* file, int offset, int new_len);
	void		read_source_change();
	void		clear_source();
	void		stop();
	void		go();

	MCHAR*		puts(MCHAR* str);
	int			printf(const MCHAR *format, ...);

	void		log_to(FileStream* log);
	void		close_log();
	CharStream* get_log() { return log; }

	Value*		get_file_name();
};

#endif
