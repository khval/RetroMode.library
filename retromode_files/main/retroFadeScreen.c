/* :ts=4
 *  $VER: retroFadeScreen.c $Revision$ (13-May-2018)
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
#include <libbase.h>

/****** retromode/main/retroFadeScreen ******************************************
*
*   NAME
*      retroFadeScreen -- Description
*
*   SYNOPSIS
*      void retroFadeScreen(struct retroScreen * screen);
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

void _retromode_retroFadeScreen(struct RetroModeIFace *Self,
       struct retroScreen * screen)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;

	if (screen -> fade_speed)
	{
		if (screen -> fade_count < screen -> fade_speed)
		{
			screen -> fade_count++;
		}
		else
		{
			int changed_at = -1;
			int n = 0;
			struct retroRGB *opal = screen -> orgPalette;
			struct retroRGB *rpal = screen -> rowPalette;
			struct retroRGB *npal = screen -> fadePalette;

			for (n=0;n<256;n++)
			{

				if (npal -> r < 256)	// valid colour most be set.
				{
					if (npal->r > opal->r)
					{
						opal -> r +=  0x11;	
						changed_at = n | 0x1000;
					}
					else if ((npal->r < opal -> r))
					{
						opal -> r -=  0x11;	
						changed_at = n | 0x1000;
					}
				}

				if (npal -> g < 256)	// valid colour most be set.
				{
					if (npal->g > opal->g)
					{
						opal->g += 0x11;
						changed_at = n | 0x2000;
					}
					else if ((npal->g < opal -> g))
					{
						opal->g -= 0x11;
						changed_at = n | 0x2000;
					}
				}

				if (npal -> b < 256)	// valid colour most be set.
				{
					if (npal->b > opal->b)
					{
						opal->b +=  0x11;
						changed_at = n | 0x3000;
					}
					else if ((npal->b < opal->b))
					{
						opal->b -=  0x11;
						changed_at = n | 0x3000;
					}
				}

				*rpal =*opal;

				opal++;
				rpal++;
				npal++;
			}

			libBase -> IDOS -> Printf("%Fade changed at lx\n",changed_at);

			screen -> fade_count = 0;
			if (changed_at > -1)
			{
				screen -> fade_speed = 0;
				libBase -> IDOS -> Printf("screen -> fade_speed set to 0\n");
			}
		}
	}
}

