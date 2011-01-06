/*	
 *		Name.h - Name class for MAXScript
 *
 *			Copyright © John Wainwright 1996
 *
 */

#ifndef _H_NAME
#define _H_NAME

#include "HashTab.h"

visible_class_debug_ok (Name)

class Name : public Value
{
public:
	MCHAR*		string;
	static		HashTable* intern_table;
				Name(MCHAR *init_string);
	ScripterExport ~Name();

#	define		is_name(o) ((o)->tag == class_tag(Name))
	static void	setup();
	static ScripterExport Value* intern(MCHAR* str);
	static ScripterExport Value* find_intern(MCHAR* str);
				classof_methods (Name, Value);
	
	ScripterExport void sprin1(CharStream* s);
	void		collect() { delete this; }
	MCHAR*		to_string();
	MSTR		to_filename();
	void		to_fpvalue(FPValue& v) { v.s = to_string(); v.type = TYPE_NAME; }

#include "defimpfn.h"
	use_generic( coerce,	"coerce");
	use_generic( gt,		">");
	use_generic( lt,		"<");
	use_generic( ge,		">=");
	use_generic( le,		"<=");
	use_generic( copy,		"copy");

	// scene I/O 
	IOResult Save(ISave* isave);
	static Value* Load(ILoad* iload, USHORT chunkID, ValueLoader* vload);

};

/* core interned names */

#include "defextfn.h"
#	include "corename.h"


#endif
