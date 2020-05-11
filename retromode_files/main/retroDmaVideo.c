/* :ts=4
 *  $VER: retroDmaVideo.c $Revision$ (16-Oct-2017)
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

/****** retromode/main/retroDmaVideo ******************************************
*
*   NAME
*      retroDmaVideo -- Description
*
*   SYNOPSIS
*      void retroDmaVideo(struct retroVideo * video);
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

void _retromode_retroDmaVideo(struct RetroModeIFace *Self,
       struct retroVideo * video, struct retroEngine * Engine)
{
	if (video->Memory)
	{
		// move gfx from system mem to video mem.
		IGraphics -> WritePixelArray( (uint8 * ) video->Memory, 0, 0, video->BytesPerRow, PIXF_A8R8G8B8, 
			&Engine->rp, 0,0, video->width,video->height);
	}
	else
	{
		IDOS -> Printf("video has no memory\n");
	}
}

