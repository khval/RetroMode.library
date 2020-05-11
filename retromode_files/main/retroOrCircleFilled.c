/* :ts=4
 *  $VER: retroOrCircleFilled.c $Revision$ (16-May-2018)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2018 LiveForIt Software.
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
#include <math.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>

/****** retromode/main/retroOrCircleFilled ******************************************
*
*   NAME
*      retroOrCircleFilled -- Description
*
*   SYNOPSIS
*      void retroOrCircleFilled(struct retroScreen * screen, int x, int y, 
*          int r, unsigned char or_mask);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       r - 
*       or_mask - 
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

void _retromode_retroOrCircleFilled(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int cx,
       int cy,
       int r,
       unsigned char or_mask)
{
	int x0,y0,x1,y1,_y;
	int xx;
	int r2 = r * r;
	int x,y;
	unsigned char *memory;

	y0 = cy-r;
	y1 = cy+r;

	if (y0<0) y0=0;
	if (y1>screen->realHeight-1) y1 = screen->realHeight-1;

	memory = screen -> Memory[screen -> double_buffer_draw_frame] + (screen -> realWidth * y0);

	for (y=y0;y<=y1;y++)
	{
		_y = y - cy; 

		xx = sqrt( r2 - (_y*_y));

		x0 = cx -xx;
		x1 = cx +xx;

		if (x0<0) x0 = 0;
		if (x1>screen->realWidth-1) x1 = screen -> realWidth-1;

		for (x = x0; x <= x1; x++)
		{
			memory[ x ] |= or_mask;
		}

		memory += screen -> realWidth;
	}
}

