/* :ts=4
 *  $VER: retroCloseScreen.c $Revision$ (03-Oct-2017)
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

/****** retromode/main/retroCloseScreen ******************************************
*
*   NAME
*      retroCloseScreen -- Description
*
*   SYNOPSIS
*      void retroCloseScreen(struct retroScreen * screen);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
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


/*
void trunc_flash_table( struct retroScreen * screen )
{
	int n;
	BOOL sorted;

	do
	{
		sorted = FALSE;
		for (n=255;n>0;n++)
		{
			if ((screen->allocatedFlashs[n] != NULL) && (screen->allocatedFlashs[n-1] == NULL))
			{
				screen->allocatedFlashs[n-1] = screen->allocatedFlashs[n];
				sorted = TRUE;
			}
		}
	} while (sorted);
}
*/

/*
static void clean_up_video(struct retroVideo *video, struct retroScreen * screen)
{
	int y;
	for (y=0;y<video->height;y++)
	{
		if (video -> scanlines[y].screen == screen)
		{
			video -> scanlines[y].screen = NULL;
			video -> scanlines[y].data = NULL;
			video -> scanlines[y].pixels = 0;
			video -> scanlines[y].beamStart = 0;
			video -> scanlines[y].videoWidth = 0;
		}
	}
}
*/

extern void _retromode_retroScreenDetach(struct RetroModeIFace *Self, struct retroScreen * screen);

void _retromode_retroCloseScreen(struct RetroModeIFace *Self, struct retroScreen * screen)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroFlashTable * flash;
	struct retroShiftColors * shift;
	int idx;

	if (screen==NULL) return;

	_retromode_retroScreenDetach( Self, screen );

	for (idx = 0; idx<256; idx++)
	{
		flash = screen->allocatedFlashs[idx];
		if (flash)
		{
			if (flash -> table)
			{
				libBase -> IExec -> FreeVec(flash -> table);
				flash -> table = NULL;
			}
			libBase -> IExec -> FreeVec(flash);
			screen->allocatedFlashs[idx] = NULL;	
		}

		shift = screen->allocatedShifts[idx];
		if (shift)
		{
			libBase -> IExec -> FreeVec(shift);
			screen->allocatedShifts[idx] = NULL;	
		}
	}
		
	if (screen -> Memory)  libBase -> IExec -> FreeVec(screen -> Memory);
	screen -> Memory = NULL;

	libBase -> IExec -> FreeVec(screen);
}

