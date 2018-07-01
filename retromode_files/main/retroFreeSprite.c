/* :ts=4
 *  $VER: retroFreeSprite.c $Revision$ (11-Nov-2017)
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

/****** retromode/main/retroFreeSprite ******************************************
*
*   NAME
*      retroFreeSprite -- Description
*
*   SYNOPSIS
*      void retroFreeSprite(struct retroSprite * sprite);
*
*   FUNCTION
*
*   INPUTS
*       sprite - 
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

void _retromode_retroFreeSprite(struct RetroModeIFace *Self, struct retroSprite * sprite)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;

	if (sprite)
	{
		if (sprite->frames)
		{
			int n;

			for (n=0; n<sprite->number_of_frames; n++ )
			{

				if (sprite->frames[n].data)
				{
					libBase->IExec->FreeVec(sprite->frames[n].data);
					sprite->frames[n].data = NULL;
				}
			 }

			libBase->IExec->FreeVec(sprite->frames);
		}
		libBase->IExec->FreeVec(sprite);
	}
}

