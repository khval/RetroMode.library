/* :ts=4
 *  $VER: retroScreenClone.c $Revision$ (06-Nov-2017)
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
#include <libbase.h>

/****** retromode/main/retroScreenClone ******************************************
*
*   NAME
*      retroScreenClone -- Description
*
*   SYNOPSIS
*      void retroScreenClone(struct retroScreen * SourceScreen, 
*          int videomode);
*
*   FUNCTION
*
*   INPUTS
*       SourceScreen - 
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

struct retroScreen * _retromode_retroScreenClone(struct RetroModeIFace *Self,
       struct retroScreen * sourceScreen,
       int videomode)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroScreen *screen;

	if (sourceScreen == NULL) return NULL;

	screen = (struct retroScreen *) libBase -> IExec -> AllocVecTags( sizeof(struct retroScreen),  AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END	);
	if (screen)
	{
		sourceScreen -> clones ++;

		screen -> realWidth = sourceScreen -> realWidth;
		screen -> realHeight = sourceScreen -> realHeight;
		screen -> videomode = videomode;
		screen -> Memory = sourceScreen -> Memory;
		screen -> cloneOfScreen = sourceScreen;

		// if fail
		if (screen -> Memory == NULL)
		{
			Self -> retroCloseScreen( &screen );
			return NULL;
		}
	}
	return screen;
}

