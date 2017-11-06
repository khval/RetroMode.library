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
		screen -> Memory = (unsigned char *) libBase -> IExec -> AllocVecTags(  width * (height+1) ,
								AVT_Type, MEMF_SHARED,
								AVT_ClearWithValue, 0 ,
								TAG_END	);

		// if fail
		if (screen -> Memory == NULL)
		{
			Self -> retroCloseScreen( screen );
			return NULL;
		}
	}

	return screen;
}

