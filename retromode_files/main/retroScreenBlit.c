/* :ts=4
 *  $VER: retroScreenBlit.c $Revision$ (19-Oct-2017)
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
#include <libBase.h>

/****** retromode/main/retroScreenBlit ******************************************
*
*   NAME
*      retroScreenBlit -- Description
*
*   SYNOPSIS
*      void retroScreenBlit(struct retroScreen * screen, int fromX, int fromY, 
*          int width, int heigh, struct retroScreen * screen, int toX, 
*          int toY);
*
*   FUNCTION
*
*   INPUTS
*       bitmap - 
*       fromMode - logical or physical screen
*       fromX - 
*       fromY - 
*       realWidth - 
*       heigh - 
*       screen - 
*       toMode - logical or physical screen
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

void top_down_left_right(
	struct retroScreen *source,
	int formBuffer,
	int fromX,
	int fromY,
	int width,
	int height,
	struct retroScreen *destination,
	 int toBuffer,
	int toX,
	int toY)
{
	// range
	unsigned char *src_vertical_ptr;
	unsigned char *src_vertical_end;
	unsigned char *src_horizontal_ptr;
	unsigned char *src_horizontal_end;

	unsigned char *destination_memory;
	unsigned char *destination_horizontal_ptr;

	src_vertical_ptr = source -> Memory[formBuffer] + (source -> bytesPerRow * fromY) + fromX;
	src_vertical_end = src_vertical_ptr + (height * source -> bytesPerRow);

	destination_memory = destination -> Memory[toBuffer] + (destination -> realWidth * toY) + toX;
	for(;src_vertical_ptr<src_vertical_end;src_vertical_ptr += source -> bytesPerRow)
	{
		destination_horizontal_ptr = destination_memory;
		src_horizontal_end =src_vertical_ptr+width;

		for(src_horizontal_ptr=src_vertical_ptr;src_horizontal_ptr<src_horizontal_end;src_horizontal_ptr++)
		{
			*destination_horizontal_ptr++ = *src_horizontal_ptr;
		}
		destination_memory += destination -> realWidth;
	}
}

void down_up_left_right(
	struct retroScreen *source,
	int formBuffer,
	int fromX,
	int fromY,
	int width,
	int height,
	struct retroScreen *destination,
	 int toBuffer,
	int toX,
	int toY)
{
	// range
	unsigned char *src_vertical_start;
	unsigned char *src_vertical_ptr;
	unsigned char *src_vertical_end;

	unsigned char *src_horizontal_ptr;
	unsigned char *src_horizontal_end;

	unsigned char *destination_memory;
	unsigned char *destination_horizontal_ptr;

	src_vertical_start = source -> Memory[formBuffer] + (source -> bytesPerRow * fromY) + fromX;
	src_vertical_end = src_vertical_start + (height * source -> bytesPerRow);
	src_vertical_ptr =  src_vertical_end - source -> bytesPerRow;

	destination_memory = destination -> Memory[toBuffer] + (destination -> realWidth * (toY + height -1) ) + toX;

	for(;src_vertical_ptr>=src_vertical_start;src_vertical_ptr -= source -> bytesPerRow)
	{
		destination_horizontal_ptr = destination_memory;
		src_horizontal_end =src_vertical_ptr+width;

		for(src_horizontal_ptr=src_vertical_ptr;src_horizontal_ptr<src_horizontal_end;src_horizontal_ptr++)
		{
			*destination_horizontal_ptr++ = *src_horizontal_ptr;
		}

		destination_memory -= destination -> realWidth;
	}
}

void _retromode_retroScreenBlit(struct RetroModeIFace *Self,
	struct retroScreen *source,
	int formBuffer,
	int fromX,
	int fromY,
	int width,
	int height,
	struct retroScreen *destination,
	int toBuffer,
	int toX,
	int toY)
{

	// limit to 0,0

	if (fromX<0)	{ toX-=fromX; width+=fromX; fromX = 0;}		// - & - is +
	if (fromY<0)	{ toY-=fromY; height+=fromY; fromY = 0;}		// - & - is +

	if (toX<0)	{ fromX-=toX; width+=toX; toX = 0; }		// - & - is +
	if (toY<0)	{ fromY-=toY; height+=toY; toY = 0; }		// - & - is +

	// make sure realWidth is inside source, and destination

	if (fromX+width>source->realWidth) width = source->realWidth - fromX;
	if (toX+width>destination->realWidth) width = destination->realWidth - toX;

	// make sure realHeight is inside source, and destination

	if (fromY+height>source->realHeight) height = source->realHeight - fromY;
	if (toY+height>destination->realHeight) height = destination->realHeight - toY;

	// we now know the limit, we can now do the job, safely.
	
	if (toY<fromY)
	{
	 	top_down_left_right(source, formBuffer, fromX, fromY, width, height, destination, toBuffer, toX, toY);
	}
	else
	{
	 	down_up_left_right(source, formBuffer, fromX, fromY, width, height, destination, toBuffer, toX, toY);
	}
	
}

