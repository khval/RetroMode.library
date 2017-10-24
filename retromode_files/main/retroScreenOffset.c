/* :ts=4
 *  $VER: retroScreenOffset.c $Revision$ (24-Oct-2017)
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

/****** retromode/main/retroScreenOffset ******************************************
*
*   NAME
*      retroScreenOffset -- Description
*
*   SYNOPSIS
*      void retroScreenOffset(struct retroScreen * screen, int offsetx, 
*          int offsety);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       offsetx - 
*       offsety - 
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

void _retromode_retroScreenOffset(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int offsetx,
       int offsety)
{
}

