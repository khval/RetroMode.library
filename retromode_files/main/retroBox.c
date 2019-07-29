/* :ts=4
 *  $VER: retroBox.c $Revision$ (11-Oct-2017)
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

/****** retromode/main/retroBox ******************************************
*
*   NAME
*      retroBox -- Description
*
*   SYNOPSIS
*      void retroBox(struct retroScreen * screen, int x0, int y0, int x1, 
*          int y1, unsigned char color);
*
*   FUNCTION
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

void _retromode_retroBox(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	int buffer,
	int x0,
	int y0,
	int x1,
	int y1,
	unsigned char color)
{
	int x,y;
	unsigned char *sc_memory = screen -> Memory[ buffer ];
	unsigned char *memory;
	int hx0, hx1;
	int vy0, vy1;

	if (x0<x1)	{ hx0 = x0; hx1 = x1; } else { hx0 = x1; hx1 = x0; }
	if (y0<y1) { vy0 = y0; vy1 = y1; } else { vy0 = y1; vy1 = y0; }

	if (hx0<screen -> clip_x0) hx0=screen -> clip_x0;
	if (hx1>screen -> clip_x1) hx1=screen -> clip_x1;

	if (vy0<screen -> clip_y0) vy0=screen -> clip_y0;
	if (vy1>screen -> clip_y1) vy1=screen -> clip_y1;

	// draw top 
	if ((y0>=screen -> clip_y0) && (y0<=screen->clip_y1))
	{
		memory = sc_memory + (screen -> realWidth * y0) + hx0;
		for ( x=hx0; x<=hx1; x++) { *memory++ = color; }
	}

	// draw bottom
	if ((y1>=screen -> clip_y0) && (y1<=screen->clip_y1))
	{
		memory = sc_memory + (screen -> realWidth * y1) + hx0;
		for ( x=hx0; x<=hx1; x++) { *memory++ = color; }
	}

	// draw vertical left
	if ((x0>=screen->clip_x0)&&(x0<=screen->clip_x1))
	{
		memory = sc_memory + (screen -> realWidth * vy0) + x0;
		for ( y=vy0; y<=vy1; y++) { *memory = color; memory += screen->realWidth; }
	}

	// draw vertical right	
	if ((x1>=screen->clip_x0)&&(x1<=screen->clip_x1))
	{
		memory = sc_memory + (screen -> realWidth * vy0) + x1;
		for ( y=vy0; y<=vy1; y++) { *memory = color; memory += screen->realWidth; }
	}
}

