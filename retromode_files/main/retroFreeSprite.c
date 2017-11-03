/* :ts=4
 *  $VER: retroFreeSprite.c $Revision$ (02-Nov-2017)
 *
 *  This file is part of retromode.
 *
 *  2017 LiveForIt Software.
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

#define AllocVecTags libBase->IExec->AllocVecTags
#define FreeVec libBase->IExec->FreeVec

void _retromode_retroFreeSprite(struct RetroModeIFace *Self,
       struct retroSprite * sprite)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	int n;

	if (!sprite) return;

	if (sprite->frames)
	{
		for (n=0; n<sprite->number_of_frames; n++ )
		{
			if (sprite->frames[n].data)
			{
				FreeVec( sprite->frames[n].data);
			}
		}
		FreeVec(sprite->frames);
		sprite->frames = NULL;
	}

	FreeVec(sprite);
}

