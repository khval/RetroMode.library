/* :ts=4
 *  $VER: retroSprite.c $Revision$ (12-Nov-2017)
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

/****** retromode/main/retroSprite ******************************************
*
*   NAME
*      retroSprite -- Description
*
*   SYNOPSIS
*      void retroSprite(struct retroVideo * video, int number, int x, int y, 
*          int image);
*
*   FUNCTION
*
*   INPUTS
*       video - 
*       number - 
*       x - 
*       y - 
*       image - 
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

void _retromode_retroSprite(struct RetroModeIFace *Self,
       struct retroVideo * video,
       int number,
       int x,
       int y,
       int image)
{
//	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
//	libBase->IDOS->Printf("video -> sprites[%ld].sprite = %lx\n",number, video -> sprites[number].sprite);

	if (video -> sprites[number].sprite)
	{
		if (video -> sprites[number].sprite->frames)
		{
			video -> sprites[number].frame = video -> sprites[number].sprite->frames +image ;
		}
	}

	video -> sprites[number].x = x;
	video -> sprites[number].y = y;
}

