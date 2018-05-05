/* :ts=4
 *  $VER: retroDeleteFlash.c $Revision$ (21-Oct-2017)
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
#include "libbase.h"

/****** retromode/main/retroDeleteFlash ******************************************
*
*   NAME
*      retroDeleteFlash -- Description
*
*   SYNOPSIS
*      void retroDeleteFlash(struct retroScreen * screen, 
*          unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       color - 
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

int _retromode_retroDeleteFlash(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       unsigned char color)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;

	int idx = 0;
	int idx_free = -1;
	struct retroFlashTable *flash = NULL;

	// look for existing
	for (idx = 0; idx<256; idx++)
	{
		if (screen->allocatedFlashs[idx] != NULL)
		{
			if (screen->allocatedFlashs[idx] -> color == color)
			{
				idx_free = idx;
				flash = screen->allocatedFlashs[idx];

				// as we are going to create a new color table we need to kill the old one.
				if (flash -> table)
				{
					libBase -> IExec -> FreeVec(flash -> table);
					flash -> table = NULL;
				}
				break;
			}
		}
	}

	return idx_free;
}

