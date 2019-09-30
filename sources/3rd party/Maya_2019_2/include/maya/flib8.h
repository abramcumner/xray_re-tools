#ifndef __INCLUDE_FLIB8_H__
#define __INCLUDE_FLIB8_H__

/*****************************************************************
 * (C) Copyright 2016 Autodesk, Inc.  All rights reserved.
 * Use of this software is subject to the terms of the Autodesk license
 * agreement provided at the time of installation or download, or which
 * otherwise accompanies this software in either electronic or hard copy
 * form.  
 ***************************************************************/

/*!**************************************************************************
**									     
**  File library 
**
**  Name    : flib8.h		header file for use with flib8.a
**  Author  : Nian Wu
**  Version : Beta 1.01, Wed Mar 21st 2012
**
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "maya/flib.h"

#if defined (LINUX)
	#ifndef __USE_LARGEFILE64
	#define __USE_LARGEFILE64    // for ftello64 & fseek64. Defined before stdio.h
	#endif
#endif

/*
**	some defines to use when writing chunks of unknown size.
**	For a normal (seekable) object, the writer random access the 
**	chunk's header to update the size.
*/

#define	FL_szUnknown8	0x8000000000000000
#define	FL_szFile8	0x8000000000000001
#define	FL_szFifo8	0x8000000000000002
#define	FL_szMask8	0x7ffffffffffffffc

#define	FL_szInf8	0xfffffffffffffff0
#define	FL_szSInf8	0x7ffffffffffffff0


/*
**	type definitions
**
****************************************************************************/


typedef	struct _FLchunk8
{
    FLid		id;
    __uint64_t		size;

} FLchunk8;

typedef	struct _FLgroup8
{
    FLchunk8	chunk;
    FLid		type;

} FLgroup8;

typedef	struct _FLcontext8
{
    FLnode		node;

    FLgroup8		group;
    __uint64_t		sofar;
    __uint64_t		loc;
    __int64_t			align;
    __int64_t			level;
    __uint64_t		bound;
    char *			ipath;

} FLcontext8;

typedef	struct _FLfile8
{
    FLnode		node;

    FILE *		fp;
    __int64_t			size;	/* file size								*/
    __int64_t			rwsize;	/* furthest location read or written (?)	*/

    FLcontext8 *	context;
    FLcontext8		root;
    FLparser		parser;

    char *		path;
    char *		bname;

    void *		shared;
    __uint64_t	shrwsize;
   
    FLmkey		memory;
    FLlist		marks;
    FILE *		wdelay;
    pid_t		pid;		/* pid of filter's feeder */
    void *		includes;
    FLid		userdata;
    char *		unrb;
    __int64_t		unrs;
    int			extend;
    
    // The following members are used to buffer data prior to writing it
    // to disk. This is an optimization added to reduce the number of disk
    // writes. See bug 195602.
    //
    char *		buffer;		/* the buffered data */
    int			bufsize;	/* the current amount of buffered data */
    int			bufloc;		/* the current location within the buffer */
    int			bufmaxsize;	/* the allocated size of the buffer */
    
} FLfile8;

/*
**	function prototypes
**
****************************************************************************/
extern	FND_EXPORT	int		FLfminfo8(FLfile8 *, int *);

/*	Basic file functions			*/

extern	FND_EXPORT FLfile8 *	FLopen8(const char *, const char *);
extern	FND_EXPORT FLfile8 *	FLopenCreatorType8(const char *, const char *, long, long);
extern	FND_EXPORT FLfile8 *	FLreopen8(const char *, const char *, FLfile8 *);
extern	FND_EXPORT void *		FLsopen8(const char *, const char *, uint);
extern	FND_EXPORT int			FLclose8(FLfile8 *);
extern	FND_EXPORT int			FLqclose8(FLfile8 *);
extern	FND_EXPORT int			FLflush8(FLfile8 *);
extern	FND_EXPORT void			FLflushBuffer8(FLfile8*);
extern	FND_EXPORT void			FLflushall8();
extern	FND_EXPORT int			FLseek8(FLfile8 *, __int64_t, int);
extern	FND_EXPORT __int64_t		FLtell8(const FLfile8 *);
extern	FND_EXPORT void		FLsetdelay8(int);
extern	FND_EXPORT void		FLconfig8(int, int);
extern	FND_EXPORT void			FLsettmp8(FLfile8 *, int);
extern	FND_EXPORT int			FListtyfile8(const FLfile8 *);
extern	FND_EXPORT void			FLinitializeBuffer8(FLfile8 *, int);


extern	FND_EXPORT	FLfile8 *	FLfilter8(const char *, const char *, FLfile8 *);
extern	FND_EXPORT	FLfile8 *	FLpopen8(const char *, const char *);

/*	raw IO functions			*/

extern	FND_EXPORT __int64_t	FLread8(FLfile8 *, void *, __uint64_t);
extern	FND_EXPORT __int64_t		FLunread8(FLfile8 *, const void *, __uint64_t);
extern	FND_EXPORT const void *	FLbgnread8(FLfile8 *, __uint64_t);

extern	FND_EXPORT __int64_t		FLwrite8(FLfile8 *, const void *, __uint64_t);
extern	FND_EXPORT void *	FLbgnwrite8(FLfile8 *, __uint64_t);
extern	FND_EXPORT int		FLendwrite8(FLfile8 *, __uint64_t);

/*	simple edition				*/

extern	FND_EXPORT	void *		FLinsbytes8(FLfile8 *, int);
extern	__int64_t	FLgetaux8(const char *, FLid, void **, int);
extern	int		FLputaux8(const char *, FLid, int, const void *, int);

/*	Structured IO				*/

extern	FND_EXPORT int		FLbgnget8(FLfile8 *, FLid *, __uint64_t *);
extern	FND_EXPORT __int64_t	FLget8(FLfile8 *, void *, __uint64_t);
extern	FND_EXPORT __int64_t	FLunget8(FLfile8 *, const void *, __uint64_t);
extern	FND_EXPORT int		FLendget8(FLfile8 *);
extern	FND_EXPORT const void *	FLsget8(FLfile8 *, __uint64_t);

extern	FND_EXPORT int		FLbgnput8(FLfile8 *, FLid, __uint64_t);
extern	FND_EXPORT __int64_t	FLput8(FLfile8 *, const void *, __uint64_t);
extern	FND_EXPORT int		FLendput8(FLfile8 *);


extern	FND_EXPORT void *		FLreadchunk8(FLfile8 *, FLid *, __uint64_t *);
extern	FND_EXPORT const void *	FLgetchunk8(FLfile8 *, FLid *, __uint64_t *);

extern	FND_EXPORT int		FLputchunk8(FLfile8 *, FLid, __uint64_t, const void *);
extern	FND_EXPORT int		FLputchunkTyped8(FLfile8 *, FLid, __uint64_t, const void *, __uint32_t);
extern	FND_EXPORT void *	FLbgnwbchunk8(FLfile8 *, FLid, __uint64_t);
extern	FND_EXPORT int		FLendwbchunk8(FLfile8 *, __uint64_t);

extern	FND_EXPORT int		FLbgnrgroup8(FLfile8 *, FLid *, FLid *);
extern	FND_EXPORT int		FLendrgroup8(FLfile8 *);

extern	FND_EXPORT int		FLbgnwgroup8(FLfile8 *, FLid, FLid);
extern	FND_EXPORT int		FLendwgroup8(FLfile8 *);

extern	FND_EXPORT int		FLskipgroup8(FLfile8 *);

/*	formatted IO				*/

extern	FND_EXPORT char *		FLgets8(FLfile8 *, char *, int);
extern	FND_EXPORT int			FLputs8(FLfile8 *, const char *);
extern	FND_EXPORT int			FLprintf8(FLfile8 *, const char *, ...);
extern	FND_EXPORT int			FLscanf8(FLfile8 *, const char *, ...);

/*	File finding and path control		*/
extern	FND_EXPORT const char *	FLfilename8(const FLfile8 *, char *, char *);

/*	File parsing routines			*/

extern	FND_EXPORT int		FLparse8(FLfile8 *);
extern	FND_EXPORT void		FLgetparser8(FLfile8 *, FLfunc *, FLfunc *, FLfunc *);
extern	FND_EXPORT void		FLsetparser8(FLfile8 *, FLfunc, FLfunc, FLfunc);
extern	FND_EXPORT void		FLsetform8(FLfile8 *, FLfunc);
extern	FND_EXPORT void		FLsetlist8(FLfile8 *, FLfunc);
extern	FND_EXPORT void		FLsetleaf8(FLfile8 *, FLfunc);

/*	Markers					*/

extern	FND_EXPORT int		FLsetmark8(FLfile8 *, int);
extern	FND_EXPORT int		FLdelmark8(FLfile8 *, int);
extern	FND_EXPORT int		FLjmpmark8(FLfile8 *, int);
extern	FND_EXPORT void		FLclearmarks8(FLfile8 *);

/*	Private stuff - might become public but for very
	low level readers/writers - for experts or gurus	*/

extern	FND_EXPORT	int		FLsetid8(FLfile8 *, FLid, __uint64_t);
extern	FND_EXPORT	void		FLnewcontext8(FLfile8 *);
extern	FND_EXPORT	void		FLfreecontext8(FLfile8 *);
extern	FND_EXPORT	FLcontext8 *	FLgetcontext8();
extern	FND_EXPORT	int		FLputcontext8(FLcontext8 *);

// These functions are not exported as they should only be used internally
// by the FLIB code.
//
extern				int		FLbufferedWrite8(FLfile8 * fp, const void *buf, unsigned nbyte);
extern				int		FLbufferedSeek8(FLfile8 * fp, __int64_t where, int whence);

#define	FLparent8(c)	((FLcontext8 *)(c->node.prev))


/*	really privates, used by functions or macros		*/

extern	FND_EXPORT	FLfile8 *	FLpfilter8(const char *, const char *, FLfile8 *);

extern	FND_EXPORT	int			FLmultiread8(const char *name,
	const char **filters, FLfile8 **fp, int size);

/*	support for TIFF files					*/

extern	FND_EXPORT	void *		FLopentiff8(FLfile8 *fp);

#ifdef __cplusplus
}
#endif

#endif
