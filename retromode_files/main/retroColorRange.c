/* :ts=4
 *  $VER: retroColorRange.c $Revision$ (07-Nov-2017)
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
#include <libbase.h>

/****** retromode/main/retroColorRange ******************************************
*
*   NAME
*      retroColorRange -- Description
*
*   SYNOPSIS
*      void retroColorRange(struct retroScreen * screen, int fromColor, 
*          int fromR, int fromG, int fromB, int toColor, int toR, int toG, 
*          int toB);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       fromColor - 
*       fromR - 
*       fromG - 
*       fromB - 
*       toColor - 
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

void _retromode_retroColorRange(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int fromColor, int fromR, int fromG, int fromB,
       int toColor, int toR, int toG, int toB)
{
	int dr,dg,db;
	int offset;
	int deltaColor;
	int color;
	char r,g,b;

	if (toColor <0) toColor = 0;
	if (fromColor > 255)  fromColor = 255;

	dr = toR - fromR;
	dg = toG - fromG;
	db = toB - fromB;

	deltaColor = toColor - fromColor;

	for ( color = fromColor; color <= toColor ; color ++ )
	{
		offset = color - fromColor;
		IDOS -> Printf("color %ld, offset %ld\n", color, offset);

		r = (dr * offset / deltaColor) + fromR;
		g = (dg * offset / deltaColor) + fromG;
		b = (db * offset / deltaColor) + fromB; 

		screen -> orgPalette[color].r = r;
		screen -> orgPalette[color].g = g;
		screen -> orgPalette[color].b = b;

		screen -> rowPalette[color].r = r;
		screen -> rowPalette[color].g = g;
		screen -> rowPalette[color].b = b;
	}
}

