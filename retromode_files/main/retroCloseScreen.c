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

void _retromode_retroCloseScreen(struct RetroModeIFace *Self, struct retroScreen * screen)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroFlashTable * flash;
	int idx;

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	if (screen)
	{
		// look for existing
		for (idx = 0; idx<256; idx++)
		{
			if (flash = screen->allocatedFlashs[idx]  )
			{
				if (flash -> table)
				{
					libBase -> IExec -> FreeVec(flash -> table);
					flash -> table = NULL;
				}

				libBase -> IExec -> FreeVec(flash);
				screen->allocatedFlashs[idx] = NULL;	
			}
		}
		
		if (screen -> Memory)  libBase -> IExec -> FreeVec(screen -> Memory);
		screen -> Memory = NULL;

		libBase -> IExec -> FreeVec(screen);
	}
}

