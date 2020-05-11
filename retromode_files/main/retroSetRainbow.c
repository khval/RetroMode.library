/* :ts=4
 *  $VER: retroSetRainbow.c $Revision$ (07-Nov-2017)
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
#include "libbase.h"

/****** retromode/main/retroSetRainbow ******************************************
*
*   NAME
*      retroSetRainbow -- Description
*
*   SYNOPSIS
*      struct retroScreen * retroSetRainbow(struct retroVideo * video, 
*          int rainbowNumber, unsigned char color, int tableSize);
*
*   FUNCTION
*
*   INPUTS
*       video - 
*       rainbowNumber - 
*       color - 
*       tableSize - 
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

void _retromode_retroSetRainbow(struct RetroModeIFace *Self,
       struct retroVideo * video,
       int rainbowNumber,
       unsigned char color,
       int tableSize)
{
	if (video -> rainbow[rainbowNumber].table) IExec->FreeVec(video -> rainbow[rainbowNumber].table);

	video -> rainbow[rainbowNumber].color = color;
	video -> rainbow[rainbowNumber].tableSize = tableSize;
	video -> rainbow[rainbowNumber].table = (struct retroRGB *) IExec->AllocVecTags(sizeof(struct retroRGB)  * video -> rainbow[0].tableSize,  AVT_Type, MEMF_SHARED, TAG_END );
}

