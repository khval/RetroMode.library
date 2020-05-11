/* :ts=4
 *  $VER: init.c $Revision$ (03-Oct-2017)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2017 LiveForIt Software.
 *  MIT License
 *
 * $Id$
 *
 * $Log$
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>

/* Version Tag */
#include "retromode.library_rev.h"
STATIC CONST UBYTE USED verstag[] = VERSTAG;

struct Library			*newlibBase = NULL;
struct Interface			*INewlib = NULL;
struct ExecIFace		*IExec = NULL;
struct Library			*DOSBase = NULL;
struct DOSIFace		*IDOS = NULL;
struct Library			*GraphicsBase = NULL;
struct GraphicsIFace		*IGraphics = NULL;


#include "libbase.h"

/*
 * The system (and compiler) rely on a symbol named _start which marks
 * the beginning of execution of an ELF file. To prevent others from 
 * executing this library, and to keep the compiler/linker happy, we
 * define an empty _start symbol here.
 *
 * On the classic system (pre-AmigaOS 4.x) this was usually done by
 * moveq #0,d0
 * rts
 *
 */
int32 _start(void);

int32 _start(void)
{
    /* If you feel like it, open DOS and print something to the user */
    return RETURN_FAIL;
}


/* Open the library */
STATIC struct Library *libOpen(struct LibraryManagerInterface *Self, ULONG version)
{
    struct RetroLibrary *libBase = (struct RetroLibrary *)Self->Data.LibBase; 

    if (version > VERSION)
    {
        return NULL;
    }

    /* Add any specific open code here 
       Return 0 before incrementing OpenCnt to fail opening */


    /* Add up the open count */
    libBase->libNode.lib_OpenCnt++;
    return (struct Library *)libBase;

}


/* Close the library */
STATIC APTR libClose(struct LibraryManagerInterface *Self)
{
    struct RetroLibrary *libBase = (struct RetroLibrary *)Self->Data.LibBase;
    /* Make sure to undo what open did */


    /* Make the close count */
    ((struct Library *)libBase)->lib_OpenCnt--;

    return 0;
}


/* Expunge the library */
STATIC APTR libExpunge(struct LibraryManagerInterface *Self)
{
	/* If your library cannot be expunged, return 0 */

	struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;

	APTR result = (APTR)0;
	struct RetroLibrary *libBase = (struct RetroLibrary *)Self->Data.LibBase;

	if (libBase->libNode.lib_OpenCnt == 0)
	{
		result = (APTR)libBase->segList;
		/* Undo what the init code did */

        	IExec->Remove((struct Node *)libBase);
		IExec->DeleteLibrary((struct Library *)libBase);

		if (IGraphics) IExec->DropInterface( (struct Interface *) IGraphics);
		if (GraphicsBase) IExec->CloseLibrary( GraphicsBase );

		IGraphics = NULL;
		GraphicsBase = NULL;

		if (IDOS) IExec->DropInterface( (struct Interface *) IDOS);
		if (DOSBase) IExec->CloseLibrary( DOSBase );

		IDOS = NULL;
		DOSBase = NULL;

		if (INewlib) IExec->DropInterface( (struct Interface *) INewlib);
		if (newlibBase) IExec->CloseLibrary( newlibBase );

		INewlib = NULL;
		newlibBase = NULL;
	}
	else
	{
		result = (APTR)0;
		libBase->libNode.lib_Flags |= LIBF_DELEXP;
	}
	return result;
}

/* The ROMTAG Init Function */
STATIC struct Library *libInit(struct Library *LibraryBase, APTR seglist, struct Interface *exec)
{
    struct RetroLibrary *libBase = (struct RetroLibrary *)LibraryBase;
    IExec = (struct ExecIFace *)exec;

    libBase->libNode.lib_Node.ln_Type = NT_LIBRARY;
    libBase->libNode.lib_Node.ln_Pri  = 0;
    libBase->libNode.lib_Node.ln_Name = "retromode.library";
    libBase->libNode.lib_Flags        = LIBF_SUMUSED|LIBF_CHANGED;
    libBase->libNode.lib_Version      = VERSION;
    libBase->libNode.lib_Revision     = REVISION;
    libBase->libNode.lib_IdString     = VSTRING;

    libBase->segList = (BPTR)seglist;

    /* Add additional init code here if you need it. For example, to open additional
       Libraries:*/

	newlibBase = IExec->OpenLibrary("newlib.library", 53L);
	if (newlibBase)
	{
		INewlib = IExec->GetInterface(DOSBase,"main", 1, NULL);
		if (INewlib) return NULL;
	} else return NULL; 

	DOSBase = IExec->OpenLibrary("dos.library", 53L);
	if (DOSBase)
	{
		IDOS = (struct DOSIFace *)IExec->GetInterface(DOSBase,"main", 1, NULL);

		if (!IDOS) return NULL;
	} else return NULL; 

	GraphicsBase = IExec->OpenLibrary("graphics.library", 54L);
	if (GraphicsBase)
	{
		IGraphics = (struct GraphicsIFace *)IExec->GetInterface(GraphicsBase,"main", 1, NULL);

		if (!IGraphics) return NULL;
	} else return NULL; 

	return (struct Library *)libBase;
}

/* ------------------- Manager Interface ------------------------ */
/* These are generic. Replace if you need more fancy stuff */
STATIC uint32 _manager_Obtain(struct LibraryManagerInterface *Self)
{
	uint32 res;
	__asm__ __volatile__(
	"1:	lwarx	%0,0,%1\n"
	"addic	%0,%0,1\n"
	"stwcx.	%0,0,%1\n"
	"bne-	1b"
	: "=&r" (res)
	: "r" (&Self->Data.RefCount)
	: "cc", "memory");

	return res;
}

STATIC uint32 _manager_Release(struct LibraryManagerInterface *Self)
{
	uint32 res;
	__asm__ __volatile__(
	"1:	lwarx	%0,0,%1\n"
	"addic	%0,%0,-1\n"
	"stwcx.	%0,0,%1\n"
	"bne-	1b"
	: "=&r" (res)
	: "r" (&Self->Data.RefCount)
	: "cc", "memory");

	return res;
}

/* Manager interface vectors */
STATIC CONST APTR lib_manager_vectors[] =
{
	_manager_Obtain,
	_manager_Release,
	NULL,
	NULL,
	libOpen,
	libClose,
	libExpunge,
	NULL,
	(APTR)-1
};

/* "__library" interface tag list */
STATIC CONST struct TagItem lib_managerTags[] =
{
	{ MIT_Name,			(Tag)"__library"		},
	{ MIT_VectorTable,	(Tag)lib_manager_vectors},
	{ MIT_Version,		1						},
	{ TAG_DONE,			0						}
};

/* ------------------- Library Interface(s) ------------------------ */

#include "retromode_vectors.c"

/* Uncomment this line (and see below) if your library has a 68k jump table */
/* extern APTR VecTable68K[]; */

STATIC CONST struct TagItem main_v1_Tags[] =
{
	{ MIT_Name,			(Tag)"main"			},
	{ MIT_VectorTable,	(Tag)main_v1_vectors	},
	{ MIT_Version,		1					},
	{ TAG_DONE,			0					}
};

STATIC CONST CONST_APTR libInterfaces[] =
{
	lib_managerTags,
	main_v1_Tags,
	NULL
};

STATIC CONST struct TagItem libCreateTags[] =
{
	{ CLT_DataSize,		sizeof(struct RetroLibrary)	},
	{ CLT_InitFunc,		(Tag)libInit			},
	{ CLT_Interfaces,	(Tag)libInterfaces		},
	/* Uncomment the following line if you have a 68k jump table */
	/* { CLT_Vector68K, (Tag)VecTable68K }, */
	{TAG_DONE,		 0 }
};


/* ------------------- ROM Tag ------------------------ */
STATIC CONST struct Resident lib_res USED =
{
	RTC_MATCHWORD,
	(struct Resident *)&lib_res,
	(APTR)(&lib_res + 1),
	RTF_NATIVE|RTF_AUTOINIT, /* Add RTF_COLDSTART if you want to be resident */
	VERSION,
	NT_LIBRARY, /* Make this NT_DEVICE if needed */
	0, /* PRI, usually not needed unless you're resident */
	"retromode.library",
	VSTRING,
	(APTR)libCreateTags
};

