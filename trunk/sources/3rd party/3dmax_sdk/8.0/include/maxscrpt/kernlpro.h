/*	
 *		kernel_protocol.h - def_generics for the MAXScript kernel protocol
 *
 *		see def_abstract_generics.h for more info.
 *
 *	
 *			Copyright © John Wainwright 1996
 *
 */

	def_mapped_generic_debug_ok		( print,			"print");

	def_generic						( eq,				"==");
	def_generic						( ne,				"!=");

	def_generic						( coerce,			"coerce");

	def_visible_generic_debug_ok	( classOf,			"classOf");
	def_visible_generic_debug_ok	( superClassOf,		"superClassOf");
	def_visible_generic_debug_ok	( isKindOf,			"isKindOf");
