/* :ts=4
 *  $VER: free_retroVideo.c $Revision$ (03-Oct-2017)
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

#include "libbase.h"

/****** retromode/main/retroFreeVideo ******************************************
*
*   NAME
*      retroFreeVideo -- Description
*
*   SYNOPSIS
*      void retroFreeVideo(struct retroVideo * video);
*
*   FUNCTION
*
*   INPUTS
*       video - 
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

void _retromode_retroFreeVideo(struct RetroModeIFace *Self,
       struct retroVideo * video)
{
	int c;

	Self -> retroFreeSpriteObjects( video );

	for ( c = 0; c<3 ;c++) 
	{
		if (video -> rainbow[c].table)
		{
			IExec ->FreeVec (video -> rainbow[c].table);
			video -> rainbow[c].table = NULL;
		}
	}

	if (video->Memory)
	{
		IExec ->FreeVec (video->Memory);
		video->Memory = NULL;
	}

	IExec ->FreeVec ( (void *) video );
}

