/* :ts=4
 *  $VER: retroScroll.c $Revision$ (28-Jun-2020)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2020 LiveForIt Software.
 *  MIT License..
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

/****** retromode/main/retroScroll ******************************************
*
*   NAME
*      retroScroll -- Description
*
*   SYNOPSIS
*      void retroScroll(struct retroScreen * screen, int buffer, int x0, 
*          int y0, int x1, int y1, int dx, int dy);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       buffer - 
*       x0 - 
*       y0 - 
*       x1 - 
*       y1 - 
*       dx - 
*       dy - 
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

void _retromode_retroScroll(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int buffer,
       int x0,
       int y0,
       int x1,
       int y1,
       int dx,
       int dy)
{
	int bytesPerRow = screen -> bytesPerRow;
	unsigned char *mem = screen -> Memory[ buffer ];
	unsigned char *src;
	unsigned char *des;
	int x,y;

	if (dy>0)
	{
		if (dx>0)
		{
			for (y=y1;y>=y0;y--)
			{
				src = mem + (bytesPerRow * y) + x1;	des = mem + (bytesPerRow * (y+dy)) + x1 +dx;
				for (x=x1;x>=x0;x--) *des--=*src--;		
			}
		}
		else
		{
			for (y=y1;y>=y0;y--)
			{
				src = mem + (bytesPerRow * y) + x0; 	des = mem + (bytesPerRow * (y+dy)) + x0 +dx;
				for (x=x0;x<=x1;x++) *des++=*src++;		
			}
		}
	}
	else
	{
		if (dx>0)
		{
			for (y=y0;y<=y1;y++)
			{
				src = mem + (bytesPerRow * (y-dy)) + x1;	des = mem + (bytesPerRow * y) + x1 +dx;
				for (x=x1;x>=x0;x--) *des--=*src--;		
			}
		}
		else
		{
			for (y=y0;y<=y1;y++)
			{
				src = mem + (bytesPerRow * (y-dy)) + x0; 	des = mem + (bytesPerRow * y) + x0 +dx;
				for (x=x0;x<=x1;x++) *des++=*src++;		
			}
		}
	}
}

