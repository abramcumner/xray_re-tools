 /*	
  *		Streams.h - stream family for MAXScript
  *
  *			Copyright © John Wainwright 1996
  *
  */
 
 #ifndef _H_STREAM
 #define _H_STREAM
 
 class Listener;
 class FileStream;
 class Parser;
 
 visible_class_debug_ok (CharStream)
 
 class CharStream : public Value
 {
 public:
 					classof_methods (CharStream, Value);
 			BOOL	_is_charstream() { return 1; }
 #	define			is_charstream(o) ((o)->_is_charstream())
 
 	virtual MCHAR	get_char() = 0;
 	virtual void	unget_char(MCHAR c) = 0;
 	virtual MCHAR	peek_char() = 0;
 	virtual int		at_eos() = 0;
 	virtual void	rewind() = 0;
	virtual int		pos() { return 0; }
	virtual void	seek(int pos) { };
	virtual void	flush_to_eol() = 0;
 	virtual void	flush_to_eobuf() {}
 	virtual void	flush_whitespace();
 
 	virtual MCHAR	putch(MCHAR c) { return c; }
 	virtual MCHAR*	puts(MCHAR* str) { return str; }
 	virtual int		printf(const MCHAR *format, ...) { return 0; }
 	virtual void	close() {}
 	virtual void	flush() {}
 
 	virtual void	log_to(CharStream* log) {}
 	virtual void	close_log() {}
 	virtual Listener* get_listener() { return NULL; }
 	virtual CharStream* get_log() { return NULL; }

	virtual Value*	get_file_name() { return NULL; }
 };
 
 visible_class_debug_ok (FileStream)
 
 class FileStream : public CharStream
 {
 public:
 	Value*	file_name;
 	MCHAR*	mode;
 	FILE*	file;
 	CharStream*	log;
 	int		ungetchar_count;
	MCHAR	ungetchar_buf[8];
 	Parser* reader;
	DWORD	decrypt;	// was BOOL. russom - 10/09/2002
 
 			ScripterExport FileStream ();
 		    ScripterExport ~FileStream ();
 
 			classof_methods (FileStream, CharStream);
 #	define	is_filestream(v) ((v)->tag == class_tag(FileStream))
	void	collect() { delete this; }
 	void	gc_trace();
 	ScripterExport void	sprin1(CharStream* s);
 
 #include "defimpfn.h"
 #	include "streampr.h"
 
 	/* internal char stream protocol */
 	
 	ScripterExport MCHAR	get_char();
 	ScripterExport void		unget_char(MCHAR c);
 	ScripterExport MCHAR	peek_char();
 	ScripterExport int		at_eos();
 	ScripterExport int		pos();
 	ScripterExport void		seek(int pos);
	ScripterExport void		rewind();
 	ScripterExport void		flush_to_eol();
 
 	ScripterExport MCHAR	putch(MCHAR c);
 	ScripterExport MCHAR*	puts(MCHAR* str);
 	ScripterExport int		printf(const MCHAR *format, ...);
 
 	ScripterExport FileStream* open_decrypt(MCHAR* ifile_name, int seed);
 	ScripterExport FileStream* open(MCHAR* ifile_name, MCHAR* imode);
 	ScripterExport void	flush();
 	ScripterExport void	close();
 	void	log_to(CharStream* log);
 	void	close_log();

	Value*	get_file_name() { return file_name; }
	void	undo_lookahead();

	void	check_readable();
	void	check_writeable();
 };
 
 visible_class_debug_ok (WindowStream)
 
 class WindowStream : public CharStream
 {
 public:
 	HWND		window;
 	int			cursor;
 	MCHAR*		title;
 	Listener*	listener;
 	CharStream*	log;
	MCHAR		wputs_buf[512];			// edit control output buffer
	MCHAR*		wputs_p;
	HWND		echo;
 
 				WindowStream(HWND iwin);
 				WindowStream(MCHAR* title);		/* for background scripts; window with given title will open if output generated */
 			   ~WindowStream();
 
 				classof_methods (WindowStream, CharStream);
 	void		collect() { delete this; }
 	void		gc_trace();
 	ScripterExport void	sprin1(CharStream* s);
 
 #include "defimpfn.h"
 	def_generic(sprint,	  "sprint");
  	use_generic( coerce,  "coerce");
// 	use_generic( flush,   "flush"); // LAM - defect 307041 - uncomment when SDK can change (tested)
		
 	/* internal MCHAR stream protocol */
 	
 	MCHAR		get_char() { return 0; }
 	void		unget_char(MCHAR c) {}
 	MCHAR		peek_char() {return 0; }
 	int			at_eos() { return TRUE; }
 	void		rewind() {}
 	void		flush_to_eol() {}
 
 	ScripterExport MCHAR	putch(MCHAR c);
 	ScripterExport MCHAR*	puts(MCHAR* str);
 	ScripterExport int		printf(const MCHAR *format, ...);
 	ScripterExport void		flush();

 	void		ensure_window_open();
 
 	void		log_to(CharStream* log);
 	void		close_log();
 	Listener*	get_listener() { return listener; }
 	CharStream* get_log() { return log; }

	// edit control output primitives
	ScripterExport MCHAR*	wputs(const MCHAR *str);
	ScripterExport void		wflush();
	ScripterExport MCHAR	wputch(const MCHAR c);
	ScripterExport int		wprintf(const MCHAR *format, ...);
				   void		set_echo_window(HWND wnd) { echo = wnd; }
				   void		echo_cur_line();
 				   int		get_cur_line(MSTR& line);
};
 
 #endif
 
 
 
