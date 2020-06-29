/* :ts=4
 *  $VER: retroFreeMask.c $Revision$ (07-Dec-2019)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2019 LiveForIt Software.
 *  MIT License..
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
#include <libbase.h>

/****** retromode/main/retroFreeMask ******************************************
*
*   NAME
*      retroFreeMask -- Description
*
*   SYNOPSIS
*      void retroFreeMask(struct retroFrameHeader * frame);
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

void _retromode_retroFreeMask(struct RetroModeIFace *Self,
       struct retroFrameHeader * frame)
{
	if (frame -> mask -> data)
	{
		FreeVec(frame  -> mask -> data);
		frame -> mask -> data = NULL;
	}
	FreeVec( frame -> mask );
	frame -> mask = NULL;
}

