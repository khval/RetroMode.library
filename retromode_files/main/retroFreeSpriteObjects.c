/* :ts=4
 *  $VER: retroFreeSpriteObjects.c $Revision$ (11-Nov-2017)
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

/****** retromode/main/retroFreeSpriteObjects ******************************************
*
*   NAME
*      retroFreeSpriteObjects -- Description
*
*   SYNOPSIS
*      void retroFreeSpriteObjects(struct retroVideo * video);
*
*   FUNCTION
*
*   INPUTS
*       video - 
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

void _retromode_retroFreeSpriteObject(struct RetroModeIFace *Self,
       struct retroSpriteObject * spriteObject, BOOL onlyData);

void _retromode_retroFreeSpriteObjects(struct RetroModeIFace *Self,
       struct retroVideo * video)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroSpriteObject *spriteObject;

	if (video -> sprites)
	{
		for (spriteObject = video -> sprites; spriteObject < video -> sprites_end; spriteObject++)
		{
			_retromode_retroFreeSpriteObject(Self,spriteObject, TRUE);
		}

		libBase->IExec->FreeVec(video -> sprites);
		video -> sprites = NULL;
		video -> sprites_end = NULL;
		video -> spriteObjectsAllocated = 0;
	}
}

