/*	
 *		HashTable.h - HashTable class for MAXScript
 *
 *			Copyright © John Wainwright 1996
 *
 */

#ifndef _H_HASHTABLE
#define _H_HASHTABLE

typedef struct 
{
	void*	key;
	void*	value;
} binding;

typedef struct						/* secondary extent struct			*/
{
	UINT	size;				      /* size of secondary extent	*/	// SR NOTE64: was size_t
	binding*	bindings;			/* table of bindings				*/
} secondary;
	
#define KEY_IS_OBJECT	0x0001		/* init flags that indicate whether keys & values are full MXS collectable objects */
#define VALUE_IS_OBJECT	0x0002

ScripterExport int	default_eq_fn(void* key1, void* key2);  /* default comparator & hash fns */
ScripterExport DWORD_PTR default_hash_fn(void* key);

class HashTabMapper;

visible_class_debug_ok (HashTable)

class HashTable : public Value
{
public:
   // SR NOTE64: hash functions returned "INT_PTR" (and previously I suppose they
   // used to return 'int'; in Win64, the hash values can get pretty huge, and negative,
   // and then hash_value % size will give a negative value -- not good.  I'd change it
   // to a DWORD, but then the default hash function, which just returns the pointer
   // casted to an INT_PTR, would have to do more computations and this might slow
   // things down too much.  So I'll just change it to an unsigned value instead and
   // introduce a typedef for it.
   typedef DWORD_PTR (*hash_fn_type)(void*);
   typedef int       (*eq_fn_type)(void*, void*);

private:
	secondary	**table;			/* primary extent: tbl of second's	*/
	UINT        size;				/* table size						*/	// SR NOTE64: Was size_t
	int			n_entries;			/* no. entries in primary extent	*/
	eq_fn_type  eq_fn;            /* key equivalence function		*/
	hash_fn_type hash_fn;	      /* key hgashing function			*/
	int			cursor;				/* cursors used for sequencing...	*/
	int			secondCursor;			
	short		flags;
	HashTable*	inner;				/* links to next & prev tables when */
	HashTable*	outer;				/* used as a lexical scope table	*/
	int			level;				// scope nesting level

public:
	ScripterExport HashTable(UINT primary_size, eq_fn_type key_eq_fn, hash_fn_type key_hash_fn, int flags);
				HashTable() { init(17, default_eq_fn, default_hash_fn, KEY_IS_OBJECT + VALUE_IS_OBJECT); }
				HashTable(UINT primary_size) { init(primary_size, default_eq_fn, default_hash_fn, KEY_IS_OBJECT + VALUE_IS_OBJECT); }
	ScripterExport ~HashTable();
	ScripterExport void		init(UINT primary_size, eq_fn_type key_eq_fn, hash_fn_type key_hash_fn, int flags);

	ScripterExport static CRITICAL_SECTION hash_update;	// for syncing allocation hashtable updates

	static void	setup();

				classof_methods (HashTable, Value);
	void		collect() { delete this;}
	ScripterExport void		gc_trace();

	ScripterExport Value*	get(void* key);
	ScripterExport Value*	put(void* key, void* val);
	ScripterExport Value*	put_new(void* key, void* val);
	ScripterExport Value*	find_key(void *val);
	ScripterExport Value*	set(void* key, void* val);
	ScripterExport void		remove(void* key);
	ScripterExport void		map_keys_and_vals(void (*fn)(void* key, void* val, void* arg), void* arg);
	ScripterExport void		map_keys_and_vals(HashTabMapper* mapper);
	ScripterExport int		num_entries() { return n_entries; }


	HashTable*	enter_scope();
	HashTable*	leave_scope();
	HashTable*	next_scope();
	int			scope_level() { return level; }
};

class HashTabMapper 
{
public:
	virtual void map(void* key, void* val)=0;
};

#define SECONDARY_BUCKET	5

#endif
