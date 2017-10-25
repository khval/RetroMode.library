/* :ts=4
 *  $VER: retroXorCircle.c $Revision$ (11-Oct-2017)
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

/****** retromode/main/retroXorCircle ******************************************
*
*   NAME
*      retroXorCircle -- Description
*
*   SYNOPSIS
*      void retroXorCircle(struct retroScreen * screen, int x, int y, int r, 
*          unsigned char xor_mask);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       r - 
*       xor_mask - 
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

void _retromode_retroXorCircle(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int cx,
       int cy,
       int r,
       unsigned char xor_mask)
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
	if (y1>screen->realHeight-1) y1 = screen->realHeight-1;

	memory = screen -> Memory + (screen -> realWidth * y0);

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
			memory[ x ] ^= xor_mask;
		}

		memory += screen -> realWidth;
	}
}

