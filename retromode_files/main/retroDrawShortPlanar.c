/* :ts=4
 *  $VER: retroDrawShortPlanar.c $Revision$ (08-Dec-2019)
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

/****** retromode/main/retroDrawShortPlanar ******************************************
*
*   NAME
*      retroDrawShortPlanar -- Description
*
*   SYNOPSIS
*      void retroDrawShortPlanar(struct retroScreen * screen, 
*          unsigned short data, int x, int y);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       data - 
*       x - 
*       y - 
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

void _retromode_retroPixel(struct RetroModeIFace *Self, struct retroScreen * screen, char *memory, int x, int y, unsigned char color);

void _retromode_retroDrawShortPlanar(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       unsigned short data,
       int x,
       int y)
{
	int n;
	unsigned char *mem = screen -> Memory[ screen -> double_buffer_draw_frame ];

	n=0x8000;
	do
	{
		if (n&data) _retromode_retroPixel( Self, screen, mem, x, y, 1 );
		x++;
		n=n>>1;
	} while (n>0);
}

