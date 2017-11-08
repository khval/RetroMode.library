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
*      struct retroScreen * retroRainbow(struct retroVideo * video, 
*          int rainbowNumber, int verticalOffset, int height);
*
*   FUNCTION
*
*   INPUTS
*       video - 
*       rainbowNumber - 
*       verticalOffset - 
*       height - 
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

struct retroScreen * _retromode_retroRainbow(struct RetroModeIFace *Self,
       struct retroVideo * video,
       int rainbowNumber,
       int verticalOffset,
       int height)
{
	video -> rainbow[rainbowNumber].verticalOffset = verticalOffset;	
	video -> rainbow[rainbowNumber].height = height;
}

