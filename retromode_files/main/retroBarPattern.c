/* :ts=4
 *  $VER: retroBarPattern.c $Revision$ (18-May-2019)
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
#include "libBase.h"

/****** retromode/main/retroBarPattern ******************************************
*
*   NAME
*      retroBarPattern -- Description
*
*   SYNOPSIS
*      void retroBarPattern(struct retroScreen * screen, int x0, int y0, 
*          int x1, int y1, struct retroSprite * sprite, int frame);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x0 - 
*       y0 - 
*       x1 - 
*       y1 - 
*       sprite - 
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

char getPixelFromFrame(struct retroFrameHeader *frame , int x, int y )
{
	int _x = x %  frame -> width;
	int _y = y % frame -> height;
	return frame -> data[ frame -> bytesPerRow * _y + _x  ];
}

int CheckMultiColors(struct retroFrameHeader *frame)
{
	int x,y;

	if (frame -> retroFlag & retro_flag_scaned)
	{
		return frame -> retroFlag & retro_flag_multicolor ? 1 : 0;
	}

	for (y=0;y<frame -> height;y++)
	{
		for (x=0;x<frame -> width;x++)
		{
			if (frame -> data[ frame -> bytesPerRow * y + x  ] > 1) 
			{				
				frame -> retroFlag = retro_flag_scaned | retro_flag_multicolor;
				return 1;
			}
		}
	}

	frame -> retroFlag = retro_flag_scaned;
	return 0;
}

void _retromode_retroBarPattern(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	int buffer,
	int x0,
	int y0,
	int x1,
	int y1,
	struct retroSprite * sprite,
	int image,
	int ink0,
	int ink1)
{
	int y;
	unsigned char *memory;

	unsigned char *from;
	unsigned char *to;
	unsigned char *ptr;
	unsigned int bytesPerRow = screen -> realWidth;
	struct retroFrameHeader *frame;

	image = image % sprite -> number_of_frames;
	frame = sprite -> frames + image;

	if (x0<screen -> clip_x0) x0 = screen -> clip_x0;
	if (y0<screen -> clip_y0) y0 = screen -> clip_y0;
	if (x1>screen -> clip_x1) x1 = screen -> clip_x1;
	if (y1>screen -> clip_y1) y1 = screen -> clip_y1;

	memory = screen -> Memory[ buffer ] + ( screen -> realWidth * y0 );

	from = memory + x0;
	to = memory + x1;

	if (CheckMultiColors( frame ))
	{
		for(y=y0;y<=y1;y++)
		{
			for(ptr=from;ptr<=to;ptr++)
			{
				*ptr = getPixelFromFrame( frame, (int) ptr, y);
			}

			from += bytesPerRow;
			to += bytesPerRow;
		}
	}
	else
	{
		for(y=y0;y<=y1;y++)
		{
			for(ptr=from;ptr<=to;ptr++)
			{
				*ptr = getPixelFromFrame( frame, (int) ptr, y) ? ink0 : ink1 ;
			}

			from += bytesPerRow;
			to += bytesPerRow;
		}
	}
}

