/* :ts=4
 *  $VER: retroAllocDoubleBuffer.c $Revision$ (06-Jun-2018)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2018 LiveForIt Software.
 *  MIT License.
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

#include "libbase.h"

/****** retromode/main/retroAllocDoubleBuffer ******************************************
*
*   NAME
*      retroAllocDoubleBuffer -- Description
*
*   SYNOPSIS
*      void retroAllocDoubleBuffer(struct retroScreen * screen);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*
*   RESULT
*       This function does not return a result
*
*   EXAMPLE
*
*   NOTES
*
*   BUGS
*
*   SEE ALSO
*
*****************************************************************************
*
*/

void _retromode_retroAllocDoubleBuffer(struct RetroModeIFace *Self,
       struct retroScreen * screen)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;

	if (screen -> Memory[1] == NULL )
	{ 
		screen -> Memory[1] = (unsigned char *) libBase -> IExec -> AllocVecTags(  screen -> bytesPerRow * (screen -> realHeight+1) ,
								AVT_Type, MEMF_SHARED,
								AVT_ClearWithValue, 0 ,
								TAG_END	);
	}
}

