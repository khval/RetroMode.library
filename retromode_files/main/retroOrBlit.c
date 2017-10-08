/* :ts=4
 *  $VER: retroBAR.c $Revision$ (03-Oct-2017)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2017 LiveForIt Software.
 *  MIT License
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
#include <proto/graphics.h>
#include <stdarg.h>

#include "libbase.h"


/****** retromode/main/retroBAR ******************************************
*
*   NAME
*      retroBAR -- Description
*
*   SYNOPSIS
*      void retroOrBlit(struct Bitmap * bitmap, int fromX, int fromY, int width, 
*          int height, struct retroScreen *screen, int toX, int toY);
*
*   FUNCTION
*	This is 8bit function do not use with 32bit bitmaps!!!
*
*   INPUTS
*       bitmap - source bitmap
*       fromX - source pos X
*       fromY - source pos Y
*       retroScreen - dest retro screen
*       toX -	dest X
*       toY -  	dest Y
*
*   RESULT
*       This function does not return a result
*
*   EXAMPLE
*
*   NOTES
*
*   BUGS
*	only works with 8bit bitmaps!!!
*
*   SEE ALSO
*
*****************************************************************************
*
*/

void _retromode_retroOrBlit(struct RetroModeIFace *Self,struct BitMap *bitmap,int fromX,int fromY,int width,int height,struct retroScreen * screen,int toX,int toY)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	int x,y;
	APTR lock;
	unsigned char *BitMapMemory;
	uint32	BitMapBytesPerRow;
	uint32	BitMapWidth;
	uint32	BitMapHeight;

	BitMapWidth = libBase -> IGraphics ->GetBitMapAttr( bitmap, BMA_ACTUALWIDTH );
	BitMapHeight = libBase -> IGraphics ->GetBitMapAttr( bitmap, BMA_HEIGHT );

	// adjust to upper limit of bitmap

	if (fromX+width>BitMapWidth) width = BitMapWidth - fromX;
	if (fromY+height>BitMapHeight) height = BitMapHeight - fromY;

	// adjust to upper limit of screen

	if (toX+width>screen -> width) width = screen -> width - toX;
	if (toY+height>screen -> height) height = screen -> height - toY;

	if ((width < 0) || (height <0)) return;

	lock = libBase -> IGraphics -> LockBitMapTags( bitmap, 
			LBM_BytesPerRow, &BitMapBytesPerRow,
			LBM_BaseAddress, &BitMapMemory,
			TAG_END);

	if (lock)
	{
		int _fromX,_toX;

		// handel negative clipping I think.

		if (fromX<0)	{ toX-=fromX; width+=fromX; fromX = 0;}		// - & - is +
		if (fromY<0)	{ toY-=fromY; height+=fromY; fromY = 0;}		// - & - is +

		if (toX<0)	{ fromX-=toX; width+=toX; toX = 0; }		// - & - is +
		if (toY<0)	{ fromY-=toY; height+=toY; toY = 0; }		// - & - is +

		_fromX = fromX;
		_toX = toX;

		for(y=0;y<height;y++)
		{
			fromX = _fromX;
			toX = _toX;

			for(x=0;x<width;x++)
			{
				screen -> Memory[ (screen -> width * toY) + toX ] |= BitMapMemory[ (BitMapBytesPerRow * fromY) +fromX ];

				fromX++;
				toX++;
			}
	
			fromY++;
			toY++;
		}
		libBase -> IGraphics -> UnlockBitMap( lock );
	}
}

