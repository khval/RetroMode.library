/* :ts=4
 *  $VER: retroScreenColor.c $Revision$ (03-Oct-2017)
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

/****** retromode/main/retroScreenColor ******************************************
*
*   NAME
*      retroScreenColor -- Description
*
*   SYNOPSIS
*      void retroScreenColor(struct retroScreen * screen, int color, 
*          unsigned char r, unsigned char g, unsigned char b);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       color - 
*       r - 
*       g - 
*       b - 
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

void _retromode_retroScreenColor(struct retromodeIFace *Self,
       struct retroScreen * screen,
       int color,
       unsigned char r,
       unsigned char g,
       unsigned char b)
{
	screen -> palette[color].r = r;
	screen -> palette[color].g = g;
	screen -> palette[color].b = b;
}

