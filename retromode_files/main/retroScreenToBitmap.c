/* :ts=4
 *  $VER: retroScreenToBitmap.c $Revision$ (27-Mar-2019)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2019 LiveForIt Software.
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

/****** retromode/main/retroScreenToBitmap ******************************************
*
*   NAME
*      retroScreenToBitmap -- Description
*
*   SYNOPSIS
*      void retroScreenToBitmap(struct retroScreen * screen, int fromX, 
*          int fromY, int width, int height, struct BitMap * bitmap, 
*          int toX, int toY);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       fromX - 
*       fromY - 
*       width - 
*       height - 
*       bitmap - 
*       toX - 
*       toY - 
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

void _retromode_retroScreenToBitmap(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int fromX,
       int fromY,
       int width,
       int height,
       struct BitMap * bitmap,
       int toX,
       int toY)
{
	int x,y;
	APTR lock;
	unsigned char *BitMapMemory;
	uint32	BitMapBytesPerRow;
	uint32	BitMapWidth;
	uint32	BitMapHeight;
	unsigned char	*src_memory;
	unsigned char	*des_memory;

	unsigned char	*inner_src_memory;
	unsigned char	*inner_des_memory;

	BitMapWidth = IGraphics ->GetBitMapAttr( bitmap, BMA_ACTUALWIDTH );
	BitMapHeight = IGraphics ->GetBitMapAttr( bitmap, BMA_HEIGHT );

	lock = IGraphics -> LockBitMapTags( bitmap, 
			LBM_BytesPerRow, &BitMapBytesPerRow,
			LBM_BaseAddress, &BitMapMemory,
			TAG_END);

	if (lock)
	{
		// handel negative clipping I think.

		if (fromX<0)	{ toX-=fromX; width+=fromX; fromX = 0;}		// - & - is +
		if (fromY<0)	{ toY-=fromY; height+=fromY; fromY = 0;}		// - & - is +

		if (toX<0)	{ fromX-=toX; width+=toX; toX = 0; }		// - & - is +
		if (toY<0)	{ fromY-=toY; height+=toY; toY = 0; }		// - & - is +

		// make sure realWidth is inside source, and destination

		if (toX+width>BitMapWidth) width = BitMapWidth - toX;
		if (fromX+width>screen->realWidth) width = screen->realWidth - fromX;

		// make sure realHeight is inside source, and destination

		if (toY+height>BitMapHeight) height = BitMapHeight - toY;
		if (fromY+height>screen->realHeight) height = screen->realHeight - fromY;

		// we now know the limit, we can now do the job, safely.

		des_memory = BitMapMemory + (BitMapBytesPerRow * toY) + toX;
		src_memory = screen -> Memory[ screen -> double_buffer_draw_frame ] + (screen -> realWidth * fromY) + fromX;

		for(y=0;y<height;y++)
		{
			inner_src_memory = src_memory;
			inner_des_memory = des_memory;

			for(x=0;x<width;x++)
			{
				*inner_des_memory++ = *inner_src_memory++;
			}

			src_memory += screen -> realWidth;
			des_memory += BitMapBytesPerRow;
		}
		IGraphics -> UnlockBitMap( lock );
	}
}

