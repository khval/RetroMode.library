/* :ts=4
 *  $VER: retroZoom.c $Revision$ (05-Nov-2017)
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

/****** retromode/main/retroZoom ******************************************
*
*   NAME
*      retroZoom -- Description
*
*   SYNOPSIS
*      void retroZoom(struct retroScreen * fromScreen, int x0, int y0, 
*          int x1, int y1, struct retroScreen * toScreen, int x2, int y2, 
*          int x3, int y3);
*
*   FUNCTION
*
*   INPUTS
*       fromScreen - 
*       x0 - 
*       y0 - 
*       x1 - 
*       y1 - 
*       toScreen - 
*       x2 - 
*       y2 - 
*       x3 - 
*       y3 - 
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

void _retromode_retroZoom(struct RetroModeIFace *Self,
       struct retroScreen * fromScreen, int x0, int y0, int x1, int y1,
       struct retroScreen * toScreen, int x2, int y2, int x3, int y3)
{
	int sx,sy,dx,dy;
	int sdx, sdy;
	int ddx, ddy;
	unsigned char *fromMemory,*toMemory;

	sdx=x1-x0+1;
	sdy=y1-y0+1;
	ddx=x3-x2+1;
	ddy=y3-y2+1;

	fromMemory = fromScreen -> Memory;
	toMemory = toScreen -> Memory;

	for (dy=0;dy<ddy;dy++)
	{
		sy = (dy * sdy / ddy) + y0;
		if (  (sy>-1)&&(sy<fromScreen->realHeight)  &&  (dy>-1)&&(dy<toScreen->realHeight)  )
		{
			for (dx=0;dx<ddx;dx++)
			{
				sx = (dx * sdx / ddx) + x0;

				if (  (sx>-1)&&(sx<fromScreen->realWidth)  &&  (dx>-1)&&(dx<toScreen->realWidth)  )
				{
					toMemory[ ((dy+y2) * toScreen -> realWidth) + (dx+x2) ] = fromMemory[ (sy * fromScreen -> realWidth) + sx ];
				}
			}
		}
	}
}

