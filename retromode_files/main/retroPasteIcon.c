/* :ts=4
 *  $VER: retroPasteIcon.c $Revision$ (16-May-2019)
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

/****** retromode/main/retroPasteIcon ******************************************
*
*   NAME
*      retroPasteIcon -- Description
*
*   SYNOPSIS
*      void retroPasteIcon(struct retroScreen * screen, 
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

#define flag_solid 0
#define flag_transparent 1

void _retromode_retroPasteIcon(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	int buffer,
	struct retroSprite * sprite,
	int x,
	int y,
	int image)
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

	if ( ! sprite -> frames ) 
	{
		libBase -> IDOS -> Printf("Sprite has no frames!\n");
		return;
	}

	if (image > sprite -> number_of_frames) return;
	if (image < 0) return;

	struct retroFrameHeader *frame = sprite -> frames + image;

	if ( ! frame -> data)
	{
		libBase -> IDOS -> Printf("Sprite -> frame[%d].data is NULL!\n",image);
		return;
	}

	width = frame -> width;
	height = frame -> height;

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

	destination_row_start = screen -> Memory[ buffer ]  + (screen -> bytesPerRow * y)+ x;
	source_row_start = (unsigned char *) frame -> data + (source_y0 * frame -> bytesPerRow ) + source_x0;
	source_row_end = source_row_start + width;

	switch (frame -> retroFlag)
	{
		case flag_solid:

			for ( ypos = 0; ypos < height; ypos++ )
			{
				destination_row_ptr = destination_row_start;

				// we can unroll this one and write 32bit, insted of 8bits. (OCS chipset has 8 pixel x planes)

				for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
				{
					*destination_row_ptr= *source_row_ptr;
					destination_row_ptr++;
				}

				destination_row_start += screen -> bytesPerRow;
				source_row_start += frame -> bytesPerRow;
				source_row_end += frame -> bytesPerRow;
			}
			break;

		case flag_transparent:

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
	}
}

