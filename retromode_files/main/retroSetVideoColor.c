/* :ts=4
 *  $VER: retroSetVideoColor.c $Revision$ (16-Oct-2017)
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

/****** retromode/main/retroSetVideoColor ******************************************
*
*   NAME
*      retroSetVideoColor -- Description
*
*   SYNOPSIS
*      void retroSetVideoColor(struct retroVideo * video, 
*          struct retroRGB * RGB, int color, int from, int to);
*
*   FUNCTION
*
*   INPUTS
*       video - 
*       RGB - 
*       color - 
*       from - 
*       to - 
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

void _retromode_retroSetVideoColor(struct RetroModeIFace *Self,
       struct retroVideo * video,
       struct retroRGB * RGB,
       int color,
       int from,
       int to)
{
	int beamY;

	for (beamY = from; beamY< to; beamY++)
	{
		video -> scanlines[beamY].rowPalette[color] = *RGB;
	}
}

