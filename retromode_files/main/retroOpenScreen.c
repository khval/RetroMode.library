/* :ts=4
 *  $VER: retroOpenScreen.c $Revision$ (03-Oct-2017)
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

/****** retromode/main/retroOpenScreen ******************************************
*
*   NAME
*      retroOpenScreen -- Description
*
*   SYNOPSIS
*      struct retroScreen * retroOpenScreen(int width, int height, int videomode);
*
*   FUNCTION
*
*   INPUTS
*       width - 
*       height - 
*       videomode - 
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

void _retromode_retroCloseScreen(struct RetroModeIFace *Self, struct retroScreen **screen);

struct retroScreen * _retromode_retroOpenScreen(struct RetroModeIFace *Self,
      	int width,
	int height,
	int videomode)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroScreen *screen;

	screen = (struct retroScreen *) libBase -> IExec -> AllocVecTags( 
						sizeof(struct retroScreen),  
						AVT_Type, MEMF_SHARED,
						AVT_ClearWithValue, 0 ,
						TAG_END	);

	if (screen)
	{
		screen -> clones = 0;
		screen -> cloneOfScreen = NULL;
		screen -> realWidth = width;
		screen -> realHeight = height;
		screen -> videomode = videomode;
		screen -> autoback = 2;
		screen -> fade_speed = 0;
		screen -> fade_count = 0;
		screen -> pen = 2;
		screen -> paper = 1;
		screen -> Memory[0] = (unsigned char *) libBase -> IExec -> AllocVecTags(  
								screen -> bytesPerRow * (screen -> realHeight+1) ,
								AVT_Type, MEMF_SHARED,
								AVT_ClearWithValue, 0 ,
								TAG_END	);

		screen -> Memory[1] = NULL;

		// if fail
		if (screen -> Memory[0] == NULL)
		{
			_retromode_retroCloseScreen( Self, &screen );
			return NULL;
		}
	}

	return screen;
}

