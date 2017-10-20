/* :ts=4
 *  $VER: retroDrawFrame.c $Revision$ (19-Oct-2017)
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

/****** retromode/main/retroDrawFrame ******************************************
*
*   NAME
*      retroDrawFrame -- Description
*
*   SYNOPSIS
*      void retroDrawFrame(struct retroFrame * frame, 
*          struct retroScreen * screen, int X, int Y);
*
*   FUNCTION
*
*   INPUTS
*       frame - 
*       screen - 
*       X - 
*       Y - 
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

void _retromode_retroDrawFrame(struct RetroModeIFace *Self,
       struct retroFrame * frame,
       struct retroScreen * screen,
       int X,
       int Y)
{

}

