/* :ts=4
 *  $VER: retroPoint.c $Revision$ (11-Oct-2017)
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

/****** retromode/main/retroPoint ******************************************
*
*   NAME
*      retroPoint -- Description
*
*   SYNOPSIS
*      unsigned char retroPoint(struct retroScreen * screen, int x, int y);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*
*   RESULT
*       The result ...
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

unsigned char _retromode_retroPoint(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x,
       int y)
{
	if (x<0) return 0;
	if (y<0) return 0;
	if (x>=screen->width) return 0;
	if (y>=screen->height) return 0;

  return screen -> Memory[ screen -> width * y + x ];
}

