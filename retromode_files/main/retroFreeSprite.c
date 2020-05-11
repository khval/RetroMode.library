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

void _retromode_retroFreeMask(struct RetroModeIFace *Self, struct retroFrameHeader * frame);

void _retromode_retroFreeSprite(struct RetroModeIFace *Self, struct retroSprite * sprite)
{
	if (sprite)
	{
		if (sprite->frames)
		{
			int n;

			for (n=0; n<sprite->number_of_frames; n++ )
			{
				if (sprite->frames[n].mask)
				{
					_retromode_retroFreeMask( Self, &sprite->frames[n] );
				}

				if (sprite->frames[n].data)
				{
					IExec->FreeVec(sprite->frames[n].data);
					sprite->frames[n].data = NULL;
				}
			 }

			IExec->FreeVec(sprite->frames);
		}
		IExec->FreeVec(sprite);
	}
}

