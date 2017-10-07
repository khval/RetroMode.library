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

/****** retromode/main/retroBAR ******************************************
*
*   NAME
*      retroBAR -- Description
*
*   SYNOPSIS
*      void retroBAR(struct retroScreen * screen, int x0, int y0, int x1, 
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

void _retromode_retroBAR(struct retromodeIFace *Self,
       struct retroScreen * screen,
       int x0,
       int y0,
       int x1,
       int y1,
       unsigned char color)
{
	int x,y;

	for(y=y0;y<=y1;y++)
	{
		for(x=x0;x<=x1;x++)
		{
			screen -> Memory[ (screen -> width * y) + x ] = color;
		}
	}
}

