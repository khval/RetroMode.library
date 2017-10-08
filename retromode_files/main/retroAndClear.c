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


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>

/****** retromode/main/retroBAR ******************************************
*
*   NAME
*      retroBAR -- Description
*
*   SYNOPSIS
*      void retroAndClear(struct retroScreen * screen, int x0, int y0, int x1, int y1, unsigned char and_mask);
*
*   FUNCTION
*	This is 8bit function do not use with 32bit screens!!!
*
*   INPUTS
*       screen - screen to clear parts of
*       x0 - start pos x
*       y0 - start pos y
*       x1 - end pos x
*       y1 - end pos y
*       and_mask - bits to keep.
*
*   RESULT
*       This function does not return a result
*
*   EXAMPLE
*
*	retroAndClear(screen, 0, 0,320, 200, ~(1<<2));	//	Removes bit 2, from color value
*
*	retroAndClear(screen, 0, 0,320, 200, (1<<2));	//	Keep only bit 2, clear other bits from color value
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

void _retromode_retroAndClear(struct retromodeIFace *Self,
       struct retroScreen * screen,
       int x0,
       int y0,
       int x1,
       int y1,
       unsigned char and_mask)
{
	int x,y;

	if (x0<0) x0 = 0;
	if (y0<0) y0 = 0;
	if (x1>screen->width-1) x1 = screen->width -1;
	if (y1>screen->height-1) y1 = screen->height-1;

	for(y=y0;y<=y1;y++)
	{
		for(x=x0;x<=x1;x++)
		{
			screen -> Memory[ (screen -> width * y) + x ] &= and_mask;
		}
	}
}

