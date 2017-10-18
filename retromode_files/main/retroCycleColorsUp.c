/* :ts=4
 *  $VER: retroCycleColorsUp.c $Revision$ (16-Oct-2017)
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


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>

/****** retromode/main/retroCycleColorsUp ******************************************
*
*   NAME
*      retroCycleColorsUp -- Description
*
*   SYNOPSIS
*      void retroCycleColorsUp(struct retroScreen * screen, 
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

void _retromode_retroCycleColorsUp(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       unsigned char from_color,
       unsigned char to_color)
{
	struct retroRGB temp;
	int color;

	temp = screen -> rowPalette[to_color];

	for (color = to_color; color > from_color; color -- ) screen->rowPalette[color] = screen->rowPalette[color-1];
	screen -> rowPalette[ from_color ] = temp;
	
}

