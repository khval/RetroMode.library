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

#define ROW_ARGS (unsigned char *source_row_start, unsigned char *source_row_end, unsigned char *destination_row_start, int mask)

// ----- ALPHA

void _left_right_alpha ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
	{
		if (*source_row_ptr) *destination_row_ptr= *source_row_ptr;
		destination_row_ptr++;
	}
}

void _right_left_alpha ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_end-1;   source_row_ptr >= source_row_start  ; source_row_ptr-- )
	{
		if (*source_row_ptr) *destination_row_ptr= *source_row_ptr;
		destination_row_ptr++;
	}
}

void _left_right_alpha_mask ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
	{
		if (*source_row_ptr) *destination_row_ptr= mask & *source_row_ptr;
		destination_row_ptr++;
	}
}

void _right_left_alpha_mask ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_end-1;   source_row_ptr >= source_row_start  ; source_row_ptr-- )
	{
		if (*source_row_ptr) *destination_row_ptr= mask & *source_row_ptr;
		destination_row_ptr++;
	}
}


//---------------

void _left_right_solid ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
	{
		*destination_row_ptr= *source_row_ptr;
		destination_row_ptr++;
	}
}

void _right_left_solid ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_end-1;   source_row_ptr >= source_row_start  ; source_row_ptr-- )
	{
		*destination_row_ptr= *source_row_ptr;
		destination_row_ptr++;
	}
}

void _left_right_solid_mask ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
	{
		*destination_row_ptr= mask & *source_row_ptr;
		destination_row_ptr++;
	}
}

void _right_left_solid_mask ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_end-1;   source_row_ptr >= source_row_start  ; source_row_ptr-- )
	{
		*destination_row_ptr= mask & *source_row_ptr;
		destination_row_ptr++;
	}
}



//-------------

void _top_down(
		struct retroFrameHeader *frame, 
		unsigned char *source_row_start,
		unsigned char *source_row_end, 
		unsigned char *destination_row_start, 
		int height, 
		int mask,
		void (*row) ROW_ARGS,
		struct retroScreen * screen)
{
	int ypos;

	for ( ypos = 0; ypos < height; ypos++ )
	{
		row( source_row_start, source_row_end, destination_row_start, mask);

		destination_row_start += screen -> bytesPerRow;
		source_row_start += frame -> bytesPerRow;
		source_row_end += frame -> bytesPerRow;
	}
}

void _bottom_up(
		struct retroFrameHeader *frame, 
		unsigned char *source_row_start,
		unsigned char *source_row_end, 
		unsigned char *destination_row_start, 
		int height, 
		int mask,
		void (*row) ROW_ARGS,
		struct retroScreen * screen)
{
	int ypos;

	for ( ypos = 0; ypos < height; ypos++ )
	{
		row( source_row_start, source_row_end, destination_row_start, mask);

		destination_row_start -= screen -> bytesPerRow;
		source_row_start += frame -> bytesPerRow;
		source_row_end += frame -> bytesPerRow;
	}
}

#define __pick_mode(dir) draw_mode = mask ? _ ## dir ## _alpha_mask : _ ## dir ## _alpha;	

#define pick_mode(dir)													\
			if (frame -> alpha) {											\
				draw_mode = mask ? _ ## dir ## _alpha_mask : _ ## dir ## _alpha;		\
			} else {													\
				draw_mode = mask ? _ ## dir ## _solid_mask : _ ## dir ## _solid;		\
			}														\


void _retromode_retroPasteSprite(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	struct retroSprite * sprite,
	int x,
	int y,
	int image,
	int flags,
	unsigned int mask)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	int width;
	int height;
	int source_x0 = 0,source_y0 = 0;
	unsigned char *destination_row_start;
	unsigned char *source_row_start;
	unsigned char *source_row_end ;

	void (*draw_mode) ROW_ARGS = NULL;

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	if ( ! sprite -> frames ) 
	{
		libBase -> IDOS -> Printf("Sprite has no frames!\n");
		return;
	}

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	if (image > sprite -> number_of_frames) return;
	if (image < 0) return;

	struct retroFrameHeader *frame = sprite -> frames + image;

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	if ( ! frame -> data)
	{
		libBase -> IDOS -> Printf("Sprite -> frame[%d].data is NULL!\n",image);
		return;
	}

	width = frame -> width;
	height = frame -> height;

	x -= frame -> XHotSpot;
	y -= frame -> YHotSpot;	

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	if (y>0)
	{
		if (y+height>screen->realHeight) height = screen->realHeight - y;
	}
	else
	{
		source_y0 = -y; y = 0; height -= source_y0; 
	}

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	if (x>0)
	{
		if (x+width>screen->realWidth) width = screen->realWidth - x;
	}
	else
	{
		 source_x0 = -x; x = 0; width -= source_x0;
	}

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	source_row_start = (unsigned char *) frame -> data + (source_y0 * frame -> bytesPerRow ) + source_x0;
	source_row_end = source_row_start + width;

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	switch (flags)
	{
		case 0x0000:
			pick_mode(left_right);
			destination_row_start = screen -> Memory[ screen -> double_buffer_draw_frame ]  + (screen -> bytesPerRow * y)+ x;

			libBase -> IDOS -> Printf("draw_mode %08x\n",draw_mode);

			_top_down(frame, source_row_start,source_row_end, destination_row_start, height,  mask, draw_mode,  screen  );
			break;

		case 0x4000:
			pick_mode(left_right);
			destination_row_start = screen -> Memory[ screen -> double_buffer_draw_frame ] + (screen -> bytesPerRow * (y + height - 1)) + x;
			_bottom_up(frame, source_row_start,source_row_end, destination_row_start, height,  mask, draw_mode,  screen);
			break;

		case 0x8000:
			pick_mode(right_left);
			destination_row_start = screen -> Memory[ screen -> double_buffer_draw_frame ]  + (screen -> bytesPerRow * y)+ x;
			_top_down(frame, source_row_start,source_row_end, destination_row_start, height,  mask, draw_mode,  screen);
			break;

		case 0xC000:
			pick_mode(right_left);
			destination_row_start = screen -> Memory[ screen -> double_buffer_draw_frame ] + (screen -> realWidth * (y + height - 1)) + x;
			_bottom_up(frame, source_row_start,source_row_end, destination_row_start, height,  mask, draw_mode,  screen);
			break;

		default:
			libBase -> IDOS -> Printf("%s:%ld unexpcted flag %08lx\n",__FUNCTION__,__LINE__,flags);
			break;
	}

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

}

