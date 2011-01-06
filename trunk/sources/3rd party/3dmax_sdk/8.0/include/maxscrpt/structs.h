/*		Structs.h - the MAXSript struct definition classes
 *
 *		Copyright (c) John Wainwright, 1996
 *		
 */

#ifndef _H_STRUCT
#define _H_STRUCT

visible_class_debug_ok (StructDef)

class StructDef : public Value
{
public:
	Value*		name;						/* struct's global var name */
	Value**		member_inits;				/* member init vals			*/
	int			member_count;				/*   "    count				*/
	HashTable*	members;					/* member name to index table */

	ScripterExport StructDef(Value* name, int member_count, Value** inits, HashTable* members);
				~StructDef();
				classof_methods (StructDef, Value);
#	define		is_structdef(o) ((o)->tag == class_tag(StructDef))

	void		collect() { delete this; }
	void		gc_trace();
	ScripterExport void		sprin1(CharStream* s);

#include "defimpfn.h"
	def_generic ( get_props,	"getPropNames"); // LAM: added 4/27/00

	ScripterExport Value* apply(Value** arglist, int count, CallContext* cc=NULL);
	ScripterExport Value* get_property(Value** arg_list, int count);
	ScripterExport Value* set_property(Value** arg_list, int count);
};

class Struct : public Value
{
public:
	StructDef*	definition;					/* pointer to my struct def							*/
	Value**		member_data;				/* data elements, indexed via struct def hashtable	*/

	ScripterExport Struct(StructDef* idef, int mem_count);
				~Struct();

#	define		is_struct(o) ((o)->tag == INTERNAL_STRUCT_TAG)
	Value*		classOf_vf(Value** arg_list, int count);
	Value*		superClassOf_vf(Value** arg_list, int count);
	Value*		isKindOf_vf(Value** arg_list, int count);
	BOOL		is_kind_of(ValueMetaClass* c) { return (c == class_tag(StructDef)) ? 1 : Value::is_kind_of(c); }

	void		collect() { delete this; }
	void		gc_trace();
	ScripterExport void		sprin1(CharStream* s);

#include "defimpfn.h"
	use_generic ( copy, "copy" );
	def_generic ( get_props,	"getPropNames"); // LAM: added 4/27/00

	Value*		get_property(Value** arg_list, int count);
	Value*		set_property(Value** arg_list, int count);

	// scene I/O 
	IOResult Save(ISave* isave);
	static Value* Load(ILoad* iload, USHORT chunkID, ValueLoader* vload);

	// added 3/21/05. Used by debugger to dump locals to standard out
	void		dump_local_vars(int indentLevel);
};


#endif