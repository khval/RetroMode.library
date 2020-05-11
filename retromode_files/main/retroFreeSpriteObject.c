/* :ts=4
 *  $VER: retroFreeSpriteObject.c $Revision$ (01-Jul-2018)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2018 LiveForIt Software.
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
#include "libbase.h"

/****** retromode/main/retroFreeSpriteObject ******************************************
*
*   NAME
*      retroFreeSpriteObject -- Description
*
*   SYNOPSIS
*      void retroFreeSpriteObject(struct retroSpriteObject * spriteObject, 
*          bool onlyData);
*
*   FUNCTION
*
*   INPUTS
*       spriteObject - 
*       onlyData - 
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
       struct retroSpriteObject * spriteObject,
       BOOL onlyData)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	int n;

	if (spriteObject)
	{
		for (n=0;n<=1;n++)
		{
			if (spriteObject -> clear[n].mem)
			{
				IExec -> FreeVec(spriteObject -> clear[n].mem);
				spriteObject -> clear[n].mem = NULL;
			}
		}

		if (onlyData==FALSE)
		{
			IExec -> FreeVec(spriteObject);
		}
	}
}

