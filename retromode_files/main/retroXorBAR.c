/* :ts=4
 *  $VER: retroXorBAR.c $Revision$ (11-Oct-2017)
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

/****** retromode/main/retroXorBAR ******************************************
*
*   NAME
*      retroXorBAR -- Description
*
*   SYNOPSIS
*      void retroXorBAR(struct retroScreen * screen, int x0, int y0, int x1, 
*          int y1, unsigned char xor_mask);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x0 - 
*       y0 - 
*       x1 - 
*       y1 - 
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

void _retromode_retroXorBAR(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x0,
       int y0,
       int x1,
       int y1,
       unsigned char xor_mask)
{
	int x,y;
	unsigned char *memory;

	if (x0<0) x0 = 0;
	if (y0<0) y0 = 0;
	if (x1>screen->realWidth-1) x1 = screen->realWidth -1;
	if (y1>screen->realHeight-1) y1 = screen->realHeight-1;

	memory = screen -> Memory[screen -> double_buffer_draw_frame] + ( screen -> realWidth * y0 );

	for(y=y0;y<=y1;y++)
	{

		for(x=x0;x<=x1;x++)
		{
			memory[ x ] ^= xor_mask;
		}

		memory += screen -> realWidth;
	}
}

