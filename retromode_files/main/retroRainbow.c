/* :ts=4
 *  $VER: retroRainbow.c $Revision$ (07-Nov-2017)
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

#include <stdlib.h>
#include <stdio.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>

/****** retromode/main/retroRainbow ******************************************
*
*   NAME
*      retroRainbow -- Description
*
*   SYNOPSIS
*      void retroRainbow(struct retroVideo * video, 
*          int rainbowNumber, int offset, int verticalOffset, int height);
*
*   FUNCTION
*
*   INPUTS
*       video - retro mode video output
*       rainbowNumber - rainbow index number.
*       offset - Start location in the rainbow table
*       verticalOffset - y beam pos, to put the rainbow
*       height - visible height of the rainbow
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

void _retromode_retroRainbow(struct RetroModeIFace *Self,
       struct retroVideo * video,
       int rainbowNumber,
       int offset,
       int verticalOffset,
       int height)
{
	video -> rainbow[rainbowNumber].offset = offset;	
	video -> rainbow[rainbowNumber].verticalOffset = verticalOffset;	
	video -> rainbow[rainbowNumber].height = height;
}

