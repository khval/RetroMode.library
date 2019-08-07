/* :ts=4
 *  $VER: retroPixel.c $Revision$ (11-Oct-2017)
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

/****** retromode/main/retroPixel ******************************************
*
*   NAME
*      retroPixel -- Description
*
*   SYNOPSIS
*      void retroPixel(struct retroScreen * screen, int x, int y, 
*          unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       color - 
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

void _retromode_retroPixel(struct RetroModeIFace *Self, struct retroScreen * screen, char *memory, int x, int y, unsigned char color)
{
	if (x<0) return;
	if (y<0) return;
	if (x>=screen->realWidth) return;
	if (y>=screen->realHeight) return;

	 memory[ screen -> bytesPerRow * y + x ] = color;
}

