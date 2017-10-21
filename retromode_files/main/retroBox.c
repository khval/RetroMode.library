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
       int x1,
       int y1,
       int x2,
       int y2,
       unsigned char color)
{
	int ox1 = x1;
	int oy1 = y1;
	int ox2 = x2;
	int oy2 = y2;
	int x,y;
	unsigned char *memory;

	if (x1<0) x1 = 0;
	if (x2>screen->width-1) x2 = screen -> width-1;
	if (y1<0) y1 = 0;
	if (y2>screen->height-1) y2 = screen -> height-1;

	// draw top 
	if (oy1>-1)
	{
		memory = screen -> Memory + (screen -> width * y1) + x1;
		for ( x=x1; x<=x2; x++) { *memory++ = color; }
	}

	// draw vertical left
	if (ox1>-1)
	{
		memory = screen -> Memory + (screen -> width * y1) + x1;
		for ( y=y1; y<=y2; y++) { *memory = color; memory += screen->width; }
	}

	// draw bottom
	if (oy2<screen->height)
	{
		memory = screen -> Memory + (screen -> width * y2) + x1;
		for ( x=x1; x<=x2; x++) { *memory++ = color; }
	}

	// draw vertical right	
	if (ox2<screen->width)
	{
		memory = screen -> Memory + (screen -> width * y1) + x2;
		for ( y=y1; y<=y2; y++) { *memory = color; memory += screen->width; }
	}
}

