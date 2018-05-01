/* :ts=4
 *  $VER: retroAllocSprite.c $Revision$ (10-Nov-2017)
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

/****** retromode/main/retroAllocSprite ******************************************
*
*   NAME
*      retroAllocSprite -- Description
*
*   SYNOPSIS
*      void retroAllocSprite(struct retroVideo * video, int numberOfSprites);
*
*   FUNCTION
*
*   INPUTS
*       video - 
*       numberOfSprites - 
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



void _retromode_retroAllocSprite(struct RetroModeIFace *Self, struct retroVideo * video, int numberOfSprites)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;

	Self -> FreeSprites( video );

	video -> sprites = (struct retroSpriteObject **) AllocVecTags( sizeof(struct retroSpriteObject) * numberOfSprites, 
						AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0 , TAG_END );

	if (video -> sprites)
	{
		video -> sprites_end = video -> sprites + numberOfSprites;
		video -> spritesAllocated = numberOfSprites;
	}
}

