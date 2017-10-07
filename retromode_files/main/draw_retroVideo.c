/* :ts=4
 *  $VER: draw_retroVideo.c $Revision$ (03-Oct-2017)
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

/****** retromode/main/draw_retroVideo ******************************************
*
*   NAME
*      draw_retroVideo -- Description
*
*   SYNOPSIS
*      void draw_retroVideo(struct retroVideo * video);
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

void _retromode_draw_retroVideo(struct retromodeIFace *Self,
       struct retroVideo * video)
{
	struct retroScanline *scanline = video -> scanlines;
	unsigned int *video_buffer = video -> Memory;
	unsigned int beamY;
	int intsPerRow = video -> BytesPerRow / 4;

	for (beamY=0; beamY < video-> height; beamY++)
	{
		if (scanline->mode != NULL) scanline -> mode( scanline,  beamY, video_buffer  );
		video_buffer += intsPerRow;	// next line
		scanline ++;
	}
}

