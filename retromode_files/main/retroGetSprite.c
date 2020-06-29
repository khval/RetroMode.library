/* :ts=4
 *  $VER: retroGetSprite.c $Revision$ (30-Jun-2018)
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
#include <string.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>
#include "libbase.h"

/****** retromode/main/retroGetSprite ******************************************
*
*   NAME
*      retroGetSprite -- Description
*
*   SYNOPSIS
*      void retroGetSprite(struct retroScreen * screen, 
*          struct retroSprite * sprite, int image, int x0, int y0, int x1, 
*          int y1);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       sprite - 
*       image - 
*       x0 - 
*       y0 - 
*       x1 - 
*       y1 - 
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


struct retroFrame * _retromode_retroAllocFrame(struct RetroModeIFace *Self,
       struct retroScreen * screen, int x1, int y1, int x2, int y2, int hotspotX, int hotspotY);

void _retromode_retroGetSprite(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       struct retroSprite * sprite,
       int image,
       int x0,
       int y0,
       int x1,
       int y1)
{
	struct retroFrameHeader *frame;
	unsigned int sizeOfChunky;

	if (sprite == NULL) return;
	if (image < 0) return;

	if ( (image+1) > sprite -> number_of_frames)
	{
		struct retroFrameHeader *new_frames = NULL;
		int old_frames_count = sprite->number_of_frames;
		int new_frames_count  = image+1;

		Printf("old frames %ld, new frames %ld\n",old_frames_count, new_frames_count);

		new_frames = AllocVecTags(  sizeof(struct retroFrameHeader) *  new_frames_count,
							AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

		if (new_frames)
		{
			memcpy( new_frames, sprite -> frames, sizeof(struct retroFrameHeader) *  old_frames_count );
			FreeVec( sprite -> frames );
			sprite -> frames = new_frames;
			sprite -> number_of_frames = new_frames_count;
		}
		else	return;
	}

	if (sprite -> frames)
	{
		Printf("sprite has frames\n");

		frame = sprite -> frames + image;
		frame -> XHotSpot = 0;
		frame -> YHotSpot = 0;	
		frame -> width  = x1-x0+1;
		frame -> height = y1-y0+1;

		frame -> planarXSize = (frame -> width / 16) + ((frame->width & 15 ) ? 1: 0);
		frame -> bytesPerRow  = frame -> width;

		Printf("Width %ld, Height %ld\n", frame -> width, frame -> height);

		sizeOfChunky = frame -> bytesPerRow  * frame -> height;

		if ( frame -> data ) 
		{
			FreeVec( (void *) frame -> data );
			frame -> data = NULL;
		}

		frame -> data = (char *) AllocVecTags(  sizeOfChunky, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

		if ( ! frame -> data )
		{
			Printf("retroGetSprite - failed to alloc frame data, size %ld, width %ld, height %ld\n", sizeOfChunky, frame -> width, frame -> height);
			return;
		}
		else
		{

			unsigned char *source_row_ptr;
			unsigned char *source_row_start;
			unsigned char *destination_row_start;
			unsigned char *destination_row_ptr;
			unsigned char *destination_row_end ;
			int destination_x0 = 0;
			int destination_y0 = 0;
			int width = frame -> width;
			int height = frame -> height;
			int ypos;

			if (y0+height>screen->realHeight) height = screen->realHeight - y0;
			if (x0+width>screen->realWidth) width = screen->realWidth - x0;

			if (y0<0) { destination_y0 = -y0; y0 = 0; height -= destination_y0;  }
			if (x0<0) { destination_x0 = -x0; x0 = 0; width -= destination_x0; }

			source_row_start = screen -> Memory[ screen -> double_buffer_draw_frame ] + (screen -> bytesPerRow * y0) + x0;

			if ( (destination_x0 < frame -> width) && (destination_y0 < frame -> height))
			{
				destination_row_start = (unsigned char *) frame -> data + destination_x0 + (destination_y0 * frame -> bytesPerRow );
				destination_row_end = destination_row_start + width;

				for ( ypos = 0; ypos < height; ypos++ )
				{
					source_row_ptr = source_row_start;

					for ( destination_row_ptr = destination_row_start;  destination_row_ptr < destination_row_end ; destination_row_ptr++ )
					{
		//				*source_row_ptr = 0x02;
						*destination_row_ptr= *source_row_ptr;
						source_row_ptr++;
					}

					source_row_start += screen -> realWidth;
					destination_row_start += frame -> bytesPerRow;
					destination_row_end += frame -> bytesPerRow;
				}
			}
		}
	}
}

