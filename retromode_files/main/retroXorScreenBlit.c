/* :ts=4
 *  $VER: retroXorScreenBlit.c $Revision$ (19-Oct-2017)
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

/****** retromode/main/retroXorScreenBlit ******************************************
*
*   NAME
*      retroXorScreenBlit -- Description
*
*   SYNOPSIS
*      void retroXorScreenBlit(struct BitMap * bitmap, int fromX, int fromY, 
*          int realWidth, int heigh, struct retroScreen * screen, int toX, 
*          int toY);
*
*   FUNCTION
*
*   INPUTS
*       bitmap - 
*       fromX - 
*       fromY - 
*       realWidth - 
*       heigh - 
*       screen - 
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

void _retromode_retroXorScreenBlit(struct RetroModeIFace *Self,
       struct retroScreen * source,
       int fromX,
       int fromY,
       int realWidth,
       int realHeight,
       struct retroScreen * destination,
       int toX,
       int toY)
{
	// pointers
	unsigned char *destination_horizontal_ptr;

	// range
	unsigned char *src_vertical_ptr;
	unsigned char *src_vertical_end;

	unsigned char *src_horizontal_ptr;
	unsigned char *src_horizontal_end;

	unsigned char *destination_memory;

	// limit to 0,0

	if (fromX<0)	{ toX-=fromX; realWidth+=fromX; fromX = 0;}		// - & - is +
	if (fromY<0)	{ toY-=fromY; realHeight+=fromY; fromY = 0;}		// - & - is +

	if (toX<0)	{ fromX-=toX; realWidth+=toX; toX = 0; }		// - & - is +
	if (toY<0)	{ fromY-=toY; realHeight+=toY; toY = 0; }		// - & - is +

	// make sure realWidth is inside source, and destination

	if (fromX+realWidth>source->realWidth) realWidth = source->realWidth - fromX;
	if (toX+realWidth>destination->realWidth) realWidth = destination->realWidth - toX;

	// make sure realHeight is inside source, and destination

	if (fromY+realHeight>source->realHeight) realHeight = source->realHeight - fromY;
	if (toY+realHeight>destination->realHeight) realHeight = destination->realHeight - toY;

	// we now know the limit, we can now do job, safely.

	src_vertical_ptr = source -> Memory + (source -> realHeight * fromY) + fromX;
	src_vertical_end = src_vertical_ptr + (source -> realHeight * source -> realWidth);

	destination_memory = destination -> Memory + (destination -> realWidth * toY) + toX;

	for(;src_vertical_ptr<src_vertical_end;src_vertical_ptr += source -> realWidth)
	{
		destination_horizontal_ptr = destination_memory;
		src_horizontal_end =src_vertical_ptr+realWidth;
		for(src_horizontal_ptr=src_vertical_ptr;src_horizontal_ptr<src_horizontal_end;src_horizontal_ptr++)
		{
			*destination_horizontal_ptr++ ^= *src_horizontal_ptr;
		}
		destination_memory += destination -> realWidth;
	}
}

