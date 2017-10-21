/* :ts=4
 *  $VER: retroCircle.c $Revision$ (11-Oct-2017)
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
#include <math.h>

#include "libbase.h"

/****** retromode/main/retroCircle ******************************************
*
*   NAME
*      retroCircle -- Description
*
*   SYNOPSIS
*      void retroCircle(struct retroScreen * screen, int x, int y, int r, 
*          unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       r - 
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

void _retromode_retroCircle(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int cx,
       int cy,
       int r,
       unsigned char color)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	int x0,y0,x1,y1,_y;
	int xx;
	int r2 = r * r;
	int x,y;
	unsigned char *memory;

	y0 = cy-r;
	y1 = cy+r;

	if (y0<0) y0=0;
	if (y1>screen->height-1) y1 = screen->height-1;

	memory = screen -> Memory + (screen -> width * y0);

	libBase -> IDOS -> Printf("%08lx% - %ld, %ld\n", memory, y0, y1);

	for (y=y0;y<=y1;y++)
	{
		_y = y - cy; 

		xx = sqrt( r2 - (_y*_y));

		x0 = cx -xx;
		x1 = cx +xx;

		if (x0<0) x0 = 0;
		if (x1>screen->width-1) x1 = screen -> width-1;

		for (x = x0; x <= x1; x++)
		{
			memory[ x ] = color;
		}

		memory += screen -> width;
	}
}

