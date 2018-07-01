/* :ts=4
 *  $VER: retroRainbowColorRange.c $Revision$ (06-Nov-2017)
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

/****** retromode/main/retroRainbowColorRange ******************************************
*
*   NAME
*      retroRainbowColorRange -- Description
*
*   SYNOPSIS
*      void retroRainbowColorRange(struct retroVideo * video, 
*          int rainbowNumber, int fromLine, int fromR, int fromG, int fromB, 
*          int toLine, int toR, int toG, int toB);
*
*   FUNCTION
*
*   INPUTS
*       video - 
*       rainbowNumber - 
*       fromLine - 
*       fromR - 
*       fromG - 
*       fromB - 
*       toLine - 
*       toR - 
*       toG - 
*       toB - 
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

void _retromode_retroRainbowColorRange(struct RetroModeIFace *Self,
       struct retroVideo * video,
       int rainbowNumber,
       int fromLine,
       int fromR,
       int fromG,
       int fromB,
       int toLine,
       int toR,
       int toG,
       int toB)
{
	struct retroRGB color;
	int dr,dg,db;
	int offset;
	int deltaLine;
	int line;

	dr = toR - fromR;
	dg = toG - fromG;
	db = toB - fromB;

	if ( fromLine < 0 )  fromLine = 0;
	if ( toLine > video -> rainbow[rainbowNumber].tableSize ) toLine = video -> rainbow[ rainbowNumber ].tableSize;

	deltaLine = toLine - fromLine;

	for ( line = fromLine; line <= toLine ; line ++ )
	{
		offset = line - fromLine;
		color.r = (dr * offset / deltaLine) + fromR;
		color.g = (dg * offset / deltaLine) + fromG;
		color.b = (db * offset / deltaLine) + fromB; 
		video -> rainbow[rainbowNumber].table[line] = color;
	}
}

