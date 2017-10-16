/* :ts=4
 *  $VER: retroFlash.c $Revision$ (16-Oct-2017)
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

/****** retromode/main/retroFlash ******************************************
*
*   NAME
*      retroFlash -- Description
*
*   SYNOPSIS
*      struct retroFlashTable * retroFlash(struct retroScreen * screen, 
*          unsigned char color, char * data);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       color - 
*       data - 
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

struct retroFlashTable * _retromode_retroFlash(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       unsigned char color,
       char * data)
{
  return NULL;
}

