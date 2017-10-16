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

/****** retromode/main/RetroShift ******************************************
*
*   NAME
*      RetroShift -- Description
*
*   SYNOPSIS
*      void RetroShift(struct retroScreen * screen, int x0, int y0, int x1, 
*          int y1, unsigned char bits);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x0 - 
*       y0 - 
*       x1 - 
*       y1 - 
*       bits - 
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

void _retromode_RetroShift(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x0,
       int y0,
       int x1,
       int y1,
       char bits)
{
	int x,y;
	unsigned char *memory;

	unsigned char *from;
	unsigned char *to;
	unsigned char *ptr;
	unsigned int bytesPerRow = screen -> width;

	if (x0<0) x0 = 0;
	if (y0<0) y0 = 0;
	if (x1>screen->width-1) x1 = screen->width -1;
	if (y1>screen->height-1) y1 = screen->height-1;

	memory = screen -> Memory + ( screen -> width * y0 );

	from = memory + x0;
	to = memory + x1;

	if (bits < 0)
	{
		bits=-bits;	// abs

		for(y=y0;y<=y1;y++)
		{
			for(ptr=from;ptr<=to;ptr++)
			{
				*ptr <<= bits;
			}

			from += bytesPerRow;
			to += bytesPerRow;
		}
	}
	else
	{
		for(y=y0;y<=y1;y++)
		{
			for(ptr=from;ptr<=to;ptr++)
			{
				*ptr >>= bits;
			}

			from += bytesPerRow;
			to += bytesPerRow;
		}
	}

}

