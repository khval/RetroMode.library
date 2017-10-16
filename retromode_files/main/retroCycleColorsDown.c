/* :ts=4
 *  $VER: retroCycleColorsDown.c $Revision$ (16-Oct-2017)
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

/****** retromode/main/retroCycleColorsDown ******************************************
*
*   NAME
*      retroCycleColorsDown -- Description
*
*   SYNOPSIS
*      void retroCycleColorsDown(struct retroScreen * screen, 
*          unsigned char from_color, unsigned char to_color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       from_color - 
*       to_color - 
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

void _retromode_retroCycleColorsDown(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       unsigned char from_color,
       unsigned char to_color)
{
	struct retroRGB temp;
	int color;

	temp = screen -> palette[from_color];

	for (color = from_color+1; color <= to_color; color ++ ) screen->palette[color-1] = screen->palette[color];
	screen -> palette[ to_color ] = temp;
}

