/* :ts=4
 *  $VER: dma_retroVideo.c $Revision$ (03-Oct-2017)
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

/****** retromode/main/dma_retroVideo ******************************************
*
*   NAME
*      dma_retroVideo -- Description
*
*   SYNOPSIS
*      void dma_retroVideo(struct retroVideo * video);
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

void _retromode_dma_retroVideo(struct retromodeIFace *Self,
       struct retroVideo * video)
{
	struct RetroLibrary *libBase = (struct _Library *) Self -> Data.LibBase;

	// move gfx from system mem to video mem.

	libBase -> IGraphics -> WritePixelArray( (uint8 * ) video->Memory, 0, 0, video->BytesPerRow, PIXF_A8R8G8B8, 
			&video->rp, 0,0, video->width,video->height);

}

