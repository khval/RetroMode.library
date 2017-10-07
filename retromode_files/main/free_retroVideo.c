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
#include <proto/graphics.h>
#include <proto/retromode.h>
#include <stdarg.h>

#include "libbase.h"

/****** retromode/main/free_retroVideo ******************************************
*
*   NAME
*      free_retroVideo -- Description
*
*   SYNOPSIS
*      void free_retroVideo(struct retroVideo * video);
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

void _retromode_free_retroVideo(struct retromodeIFace *Self,
       struct retroVideo * video)
{
	struct RetroLibrary *libBase = (struct _Library *) Self -> Data.LibBase;
	int c;

	libBase -> IExec -> DebugPrintF("video->rp.BitMap: %08x\n",video->rp.BitMap);

	if (video->rp.BitMap)
	{
		libBase-> IGraphics->FreeBitMap( video-> rp.BitMap );
		video -> rp.BitMap = NULL;
	}

	for ( c = 0; c<3 ;c++) 
	{
		libBase -> IExec -> DebugPrintF("video -> rainbow[%d].table %08x\n", c, video -> rainbow[c].table);

		if (video -> rainbow[c].table)
		{
			libBase -> IExec ->FreeVec (video -> rainbow[c].table);
			video -> rainbow[c].table = NULL;
		}
	}

	libBase -> IExec -> DebugPrintF("video->Memory: %08x\n",video->Memory);

	if (video->Memory)
	{
		libBase -> IExec ->FreeVec (video->Memory);
		video->Memory = NULL;
	}

	libBase -> IExec ->FreeVec ( (void *) video );
}

