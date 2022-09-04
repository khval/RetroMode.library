/* :ts=4
 *  $VER: retroPolyLine.c $Revision$ (27-Oct-2017)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2017 LiveForIt Software.
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
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>

#include <libbase.h>

/****** retromode/main/retroPolyLine ******************************************
*
*   NAME
*      retroPolyLine -- Description
*
*   SYNOPSIS
*      void retroPolyLine(struct retroScreen * screen, int buffer, unsigned char color, ... );
*
*   FUNCTION
*
*   INPUTS
*       screen - screen to draw to.
*		buffer - select buffer 0 or 1
*       color - the color to use
*       ... - list of (x,y) points, last item should be "retroEnd"
*
*   RESULT
*       This function does not return a result
*
*   EXAMPLE
*
*	retroPolyLine( screen, color,   10,10,   50,20,   50,50,   retroEnd );
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

void _retromode_retroLine(struct RetroModeIFace *Self, struct retroScreen * screen, int buffer, int x1, int y1, int x2, int y2, unsigned char color);

void _retromode_retroPolyLine(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	int buffer,
	unsigned char color,
        ...)
{
	va_list list;

	int x=0,y=0,lx=0,ly=0;
	int i = 0, count = 0;

	va_start(list,color);	// should start at color, but its not working correct!!!!

	// --- START BUG fix

	x = va_arg(list,int); // bug fix.. (screen)
	x = va_arg(list,int); // bug fix.. (buffer)
	x = va_arg(list,int); // bug fix.. (color)

	// --- END BUG fix

	x = va_arg(list,int); // bug fix..

	i = 0;
	do
	{
		if ( ! ( i &1) ) 
		{
			lx = x;
			x = va_arg(list,int);
		}
		else
		{
			ly = y;
			y = va_arg(list,int);
			count ++;

			if (count > 1 )
			{
				_retromode_retroLine(Self,  screen, buffer, lx, ly, x, y, color);
			}
		}
		i++;
		
	} while (( x != retroEnd) && ( y != retroEnd) ) ;
	va_end(list);
}

