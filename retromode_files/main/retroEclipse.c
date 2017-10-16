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

/****** retromode/main/retroEclipse ******************************************
*
*   NAME
*      retroEclipse -- Description
*
*   SYNOPSIS
*      void retroEclipse(struct retroScreen * screen, int x, int y, int r1, 
*          int r2, unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       r1 - 
*       r2 - 
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

void _retromode_retroEclipse(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x,
       int y,
       int r1,
       int r2,
       unsigned char color)
{
}

