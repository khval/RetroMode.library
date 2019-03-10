/* :ts=4
 *  $VER: retroLine.c $Revision$ (14-Oct-2017)
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

#include <libbase.h>

/****** retromode/main/retroLine ******************************************
*
*   NAME
*      retroLine -- Description
*
*   SYNOPSIS
*      void retroLine(struct retroScreen * screen, int x1, int y1, int x2, 
*          int y2, unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x1 - 
*       y1 - 
*       x2 - 
*       y2 - 
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


//	memory = screen -> Memory[ screen -> double_buffer_draw_frame ] +  x1;

static void plot(unsigned char *memory, int x, int y, int w, int h, int bpr, char c )
{
	if ((x>-1)&&(x<w)&&(y>-1)&&(y<h))
	{
		memory[ y*bpr + x ] = c;
	}

}

void _retromode_retroLine(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x1,
       int y1,
       int x2,
       int y2,
       unsigned char color)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	unsigned char *memory;
	int x,y;
	int dx, dy;
	int adx,ady;
	int aa,p;
	int w,h,bpr;

	memory = screen -> Memory[ screen -> double_buffer_draw_frame ];
	w = screen -> realWidth;
	h = screen -> realHeight;
	bpr = screen -> bytesPerRow;

	dx = x2-x1;	 dy = y2-y1;
	if (dx<0) { adx=-dx; aa=-1; } else { adx=dx; aa=1; }
	if (dy<0) { ady=-dy; aa=-1; } else { ady=dy; aa=1; }

	if (adx>ady)
	{
		x=x1;
		y=y1;
		for (p=0;p<=adx;p++)
		{
			y=p*dy/adx;
			x=p*dx/adx;

			plot(memory, x+x1, y+y1, w, h, bpr, color );
		}
	}
	else
	{
		x=x1;
		y=y1;
		for (p=0;p<=ady;p++)
		{
			x=p*dx/ady;
			y=p*dy/ady;
			plot(memory, x+x1, y+y1, w, h, bpr, color );
			// next
		}
	}
}

