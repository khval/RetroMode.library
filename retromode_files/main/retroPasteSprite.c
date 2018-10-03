/* :ts=4
 *  $VER: retroPasteSprite.c $Revision$ (02-Nov-2017)
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
#include <libbase.h>

/****** retromode/main/retroPasteSprite ******************************************
*
*   NAME
*      retroPasteSprite -- Description
*
*   SYNOPSIS
*      struct retroSprite * retroPasteSprite(struct retroScreen * screen, 
*          struct retroSprite * sprite, int x, int y, int image);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       sprite - 
*       x - 
*       y - 
*       image - 
*
*   RESULT
*       The result ...
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

void _retromode_retroPasteSprite(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	struct retroSprite * sprite,
	int x,
	int y,
	int image,
	int flags)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	int width;
	int height;
	int ypos;
	int source_x0 = 0,source_y0 = 0;
	unsigned char *destination_row_ptr;
	unsigned char *destination_row_start;
	unsigned char *source_row_start;
	unsigned char *source_row_ptr;
	unsigned char *source_row_end ;

	if (image > sprite -> number_of_frames) image = sprite -> number_of_frames;
	if (image < 0) image = 0;

	struct retroFrameHeader *frame = sprite -> frames + image;

	width = frame -> Width;
	height = frame -> Height;

	x -= frame -> XHotSpot;
	y -= frame -> YHotSpot;	

	if (y>0)
	{
		if (y+height>screen->realHeight) height = screen->realHeight - y;
	}
	else
	{
		source_y0 = -y; y = 0; height -= source_y0; 
	}

	if (x>0)
	{
		if (x+width>screen->realWidth) width = screen->realWidth - x;
	}
	else
	{
		 source_x0 = -x; x = 0; width -= source_x0;
	}

	destination_row_start = screen -> Memory[ screen -> double_buffer_draw_frame ] + (screen -> bytesPerRow * y) + x;
	source_row_start = (unsigned char *) frame -> data + (source_y0 * frame -> bytesPerRow ) + source_x0;
	source_row_end = source_row_start + width;

	switch (flags)
	{
		case 0x0000:

			for ( ypos = 0; ypos < height; ypos++ )
			{
				destination_row_ptr = destination_row_start;

				for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
				{
					if (*source_row_ptr) *destination_row_ptr= *source_row_ptr;
					destination_row_ptr++;
				}

				destination_row_start += screen -> bytesPerRow;
				source_row_start += frame -> bytesPerRow;
				source_row_end += frame -> bytesPerRow;
			}
			break;
		case 0x4000:

			destination_row_start = screen -> Memory[ screen -> double_buffer_draw_frame ] + (screen -> bytesPerRow * (y + height - 1)) + x;

			for ( ypos = 0; ypos < height; ypos++ )
			{
				destination_row_ptr = destination_row_start;

				for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
				{
					if (*source_row_ptr) *destination_row_ptr= *source_row_ptr;
					destination_row_ptr++;
				}

				destination_row_start -= screen -> bytesPerRow;
				source_row_start += frame -> bytesPerRow;
				source_row_end += frame -> bytesPerRow;
			}
			break;
		case 0x8000:
			for ( ypos = 0; ypos < height; ypos++ )
			{
				destination_row_ptr = destination_row_start;

				for ( source_row_ptr = source_row_end-1;   source_row_ptr >= source_row_start  ; source_row_ptr-- )
				{
					if (*source_row_ptr) *destination_row_ptr= *source_row_ptr;
					destination_row_ptr++;
				}

				destination_row_start += screen -> bytesPerRow;
				source_row_start += frame -> bytesPerRow;
				source_row_end += frame -> bytesPerRow;
			}
			break;
		case 0xC000:

			destination_row_start = screen -> Memory[ screen -> double_buffer_draw_frame ] + (screen -> realWidth * (y + height - 1)) + x;

			for ( ypos = 0; ypos < height; ypos++ )
			{
				destination_row_ptr = destination_row_start;

				for ( source_row_ptr = source_row_end-1;   source_row_ptr >= source_row_start  ; source_row_ptr-- )
				{
					if (*source_row_ptr) *destination_row_ptr= *source_row_ptr;
					destination_row_ptr++;
				}

				destination_row_start -= screen -> bytesPerRow;
				source_row_start += frame -> bytesPerRow;
				source_row_end += frame -> bytesPerRow;
			}
			break;
	}
}

