/* :ts=4
 *  $VER: retroFreeFrame.c $Revision$ (19-Oct-2017)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2017 LiveForIt Software.
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

/****** retromode/main/retroFreeFrame ******************************************
*
*   NAME
*      retroFreeFrame -- Description
*
*   SYNOPSIS
*      void retroFreeFrame(struct retroFrame * frame);
*
*   FUNCTION
*
*   INPUTS
*       frame - 
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

void _retromode_retroFreeFrame(struct RetroModeIFace *Self,
       struct retroFrame * frame)
{
	if (frame)
	{
		IExec -> FreeVec( frame );
	}
}

