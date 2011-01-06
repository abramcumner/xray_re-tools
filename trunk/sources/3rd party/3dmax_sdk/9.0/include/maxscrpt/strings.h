/*	
 *		Strings.h - string family for MAXScript
 *
 *			Copyright © John Wainwright 1996
 *
 */

#ifndef _H_STRING
#define _H_STRING

#include "streams.h"

visible_class_debug_ok (String)

// Collectable::flags3 - bit 0 set if string is a literal
class String : public Value
{
	MCHAR*		string;

public:
 ScripterExport String(const MCHAR *init_string);
			   ~String();

				classof_methods (String, Value);
#	define		is_string(o) ((o)->tag == class_tag(String))
	void		collect() { delete this; }
	ScripterExport void		sprin1(CharStream* s);

	Value*		append(MCHAR* str);
	Value*		append(Value* str_val) { return append(str_val->to_string()); }

#include "defimpfn.h"
#	include "strngpro.h"

	def_property( count );

	MCHAR*		to_string();
	MSTR		to_filename();
	void		to_fpvalue(FPValue& v) { v.s = to_string(); v.type = TYPE_STRING; }
	Value*		eval(); // returns new string if string is a literal

	// scene I/O 
	IOResult Save(ISave* isave);
	static Value* Load(ILoad* iload, USHORT chunkID, ValueLoader* vload);

	friend class StringStream;
};

applyable_class_debug_ok (StringStream)

class StringStream : public CharStream
{
public:
	MCHAR*	content_string;		/* the content string				*/
	MCHAR*	cursor;				/* current read/write cursor		*/
	size_t	buf_len;			/* allocated buffer length			*/
 	int		ungetchar_count;
 	Parser* reader;

	ScripterExport StringStream();
	ScripterExport ~StringStream();
	ScripterExport StringStream(MCHAR* init_string);
	ScripterExport StringStream(int ilen);
	ScripterExport StringStream(Value* init_string_value);
	ScripterExport void init(MCHAR* init_string);

			classof_methods (StringStream, CharStream);
#	define	is_stringstream(o) ((o)->tag == class_tag(StringStream))
	void	collect() { delete this; }
	void	gc_trace();
	ScripterExport void	sprin1(CharStream* s);

 #include "defimpfn.h"
 #	include "streampr.h"
 	use_generic( coerce,  "coerce");
	use_generic( copy,    "copy");

	ScripterExport MCHAR	get_char();
	ScripterExport void		unget_char(MCHAR c);
	ScripterExport MCHAR	peek_char();
 	ScripterExport int		pos();
				   void		seek(int pos);
	ScripterExport int		at_eos();
	ScripterExport void		rewind();
	ScripterExport void		set_size(int size);
	ScripterExport void		flush_to_eol();
	ScripterExport void		flush_to_eobuf();
	ScripterExport void		undo_lookahead();

	ScripterExport	MCHAR*	puts(MCHAR* str);
	ScripterExport	MCHAR	putch(MCHAR c);
	ScripterExport	int		printf(const MCHAR *format, ...);

	MCHAR*	to_string();
	void	to_fpvalue(FPValue& v) { v.s = to_string(); v.type = TYPE_STRING; }
};

ScripterExport MSTR expand_file_name(MCHAR* file_name);
ScripterExport MSTR expand_file_dir(MCHAR* dir);

#endif
