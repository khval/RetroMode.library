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
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	int c;

	Self -> retroFreeSpriteObjects( video );

	if (video->rp.BitMap)
	{
		libBase-> IGraphics->FreeBitMap( video-> rp.BitMap );
		video -> rp.BitMap = NULL;
	}

	for ( c = 0; c<3 ;c++) 
	{
		if (video -> rainbow[c].table)
		{
			libBase -> IExec ->FreeVec (video -> rainbow[c].table);
			video -> rainbow[c].table = NULL;
		}
	}

	if (video->Memory)
	{
		libBase -> IExec ->FreeVec (video->Memory);
		video->Memory = NULL;
	}

	libBase -> IExec ->FreeVec ( (void *) video );
}

