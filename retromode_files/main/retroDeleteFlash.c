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
	struct retroFlashTable *flash = NULL;

	for (idx = 0; idx<256; idx++)
	{
		if (screen->allocatedFlashs[idx] != NULL)
		{
			if (screen->allocatedFlashs[idx] -> color == color)
			{
				flash = screen->allocatedFlashs[idx];

				if (flash -> table)
				{
					libBase -> IExec -> FreeVec(flash -> table);
					flash -> table = NULL;
				}

				libBase -> IExec -> FreeVec(screen->allocatedFlashs[idx]);	
				for ( ; idx<255;idx++)
				{
					screen->allocatedFlashs[idx] = screen->allocatedFlashs[idx+1];
				}

				screen->allocatedFlashs[255] = NULL;
				screen->flashsAllocated--;		// flash tables
				screen->allocatedFlashs_end = screen->allocatedFlashs + screen-> flashsAllocated;
				break;
			}
		}
	}

	return -1;
}

