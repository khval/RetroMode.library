/* :ts=4
 *  $VER: retroFreeBlock.c $Revision$ (07-May-2019)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2019 LiveForIt Software.
 *  MIT License.
 *
 * $Id$
 *
 * $Log$
 *
 *
 */


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>
#include "libBase.h"

/****** retromode/main/retroFreeBlock ******************************************
*
*   NAME
*      retroFreeBlock -- Description
*
*   SYNOPSIS
*      void retroFreeBlock(struct retroBlock * block);
*
*   FUNCTION
*
*   INPUTS
*       block - 
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

void _retromode_retroFreeBlock(struct RetroModeIFace *Self,
       struct retroBlock * block)
{
	if (block)
	{
		if (block->mem)  FreeVec(  block -> mem );
		FreeVec(  block );
	}
}

