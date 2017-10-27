/* :ts=4
 *  $VER: retroApplyScreen.c $Revision$ (03-Oct-2017)
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

#include "libbase.h"


/****** retromode/main/retroApplyScreen ******************************************
*
*   NAME
*      retroApplyScreen -- Description
*
*   SYNOPSIS
*      void retroApplyScreen(struct retroScreen * screen, 
*          struct retroVideo * video, int offsetx, int oppsety, 
*          int videomode);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       video - 
*       offsetx - 
*       oppsety - 
*       videomode - 
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

extern void _retromode_retroSetVideoColor(struct RetroModeIFace *Self,
       struct retroVideo * video,
       struct retroRGB * RGB,
       int color,
       int from,
       int to);



void _retromode_retroApplyScreen(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	struct retroVideo * video,
	int scanline_x,
	int scanline_y,
	int display_width,
	int display_height)
{
	int found;
	int n;

	if (display_width>screen->realWidth)  display_width = screen -> realWidth;
	if (display_height>screen->realHeight) display_height = screen -> realHeight;

	found = -1;
	for (n=0;n<video -> screensAttached;n++)
	{
		if ( video -> attachedScreens[n] == screen )
		{				
			found = n;
			break;
		}
	}

	if (found == -1)	// not found so we add it,
	{
		if (video -> screensAttached<256)
		{
			screen -> attachedToVideo = video;
			screen -> refreshScanlines = TRUE;

			video -> attachedScreens[ video -> screensAttached ] = screen;
			video -> screensAttached++;
			video -> attachedScreens_end = video -> attachedScreens + video -> screensAttached;
			video -> refreshAllScanlines = TRUE;
		}
	}
	else
	{
		// detect what type of change that is needed.
		if (
			(screen -> displayWidth != display_width) ||
			(screen -> scanline_x != scanline_x))
		{
			video -> refreshSomeScanlines = TRUE;
			screen -> refreshScanlines = TRUE;
		}

		// moveing in the y direction will need to lots of changes
		if (
			(screen -> displayHeight != display_height) ||
			(screen -> scanline_y != scanline_y))
		{
			video -> refreshAllScanlines = TRUE;
			video -> refreshSomeScanlines = FALSE;
			screen -> refreshScanlines = TRUE;
		}
	}

	// all ok, lets set some new values.

	screen -> displayWidth = display_width;
	screen -> displayHeight = display_height; 
	screen -> scanline_x = scanline_x;
	screen -> scanline_y = scanline_y;
}

