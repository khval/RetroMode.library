/* :ts=4
 *  $VER: retroBAR.c $Revision$ (03-Oct-2017)
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

/****** retromode/main/retroBAR ******************************************
*
*   NAME
*      retroBAR -- Description
*
*   SYNOPSIS
*      void retroBAR(struct retroScreen * screen, int x0, int y0, int x1, 
*          int y1, unsigned char color);
*
*   FUNCTION
*	This is 8bit function do not use with 32bit screens!!!
*
*   INPUTS
*       screen - 
*       x0 - 
*       y0 - 
*       x1 - 
*       y1 - 
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

void _retromode_retroBAR(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x0,
       int y0,
       int x1,
       int y1,
       unsigned char color)
{
	int y;
	unsigned char *memory;

	unsigned char *from;
	unsigned char *to;
	unsigned char *ptr;
	unsigned int bytesPerRow = screen -> realWidth;

	if (x0<0) x0 = 0;
	if (y0<0) y0 = 0;
	if (x1>screen->realWidth-1) x1 = screen->realWidth -1;
	if (y1>screen->realHeight-1) y1 = screen->realHeight-1;

	memory = screen -> Memory[screen -> double_buffer_draw_frame] + ( screen -> realWidth * y0 );

	from = memory + x0;
	to = memory + x1;

	for(y=y0;y<=y1;y++)
	{
		for(ptr=from;ptr<=to;ptr++)
		{
			*ptr = color;
		}

		from += bytesPerRow;
		to += bytesPerRow;
	}
}

