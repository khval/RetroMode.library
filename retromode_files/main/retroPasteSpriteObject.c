/* :ts=4
 *  $VER: retroPasteSpriteObject.c $Revision$ (07-Jan-2020)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2020 LiveForIt Software.
 *  MIT License..
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

/****** retromode/main/retroPasteSpriteObject ******************************************
*
*   NAME
*      retroPasteSpriteObject -- Description
*
*   SYNOPSIS
*      void retroPasteSpriteObject(struct retroScreen * screen, int buffer, 
*          struct retroSpriteObject * spriteObject, struct retroSprite * sprite,int image,int flags);
*
*   FUNCTION
*	Function used to paste sprite object on the screen, 
*	unlike retroPasteSprite, this one cares about sprite object x,y limits.
*
*   INPUTS
*	screen - the screen to draw on
*	buffer - the buffer to draw on
*	spriteObject - the object to use
*	sprite - the sprite to use
*	image - the image of the sprite
*	fags - flags used.
*
*   RESULT
*	Draw a sprite/bob on the screen, if it failes it wont return a error.
*
*   EXAMPLE
*
*	retroPasteSpriteObject( screen, 0, spriteObject,  SpriteObject -> sprite, spriteObject -> image, 0);
*
*   NOTES
*	spriteObject struct has a sprite, but you don't need to use it.
*
*   BUGS
*
*   SEE ALSO
*
*****************************************************************************
*
*/

#define ROW_ARGS (unsigned char *source_row_start, unsigned char *source_row_end, unsigned char *destination_row_start, int mask)

extern void _left_right_solid_src_dest_mask ROW_ARGS;
extern void _left_right_solid_dest_mask ROW_ARGS;
extern void _left_right_solid_src_mask ROW_ARGS;
extern void _right_left_solid_src_dest_mask ROW_ARGS;
extern void _right_left_solid_dest_mask ROW_ARGS;
extern void _right_left_solid_src_mask ROW_ARGS;

extern void _left_right_alpha_src_dest_mask ROW_ARGS;
extern void _left_right_alpha_dest_mask ROW_ARGS;
extern void _left_right_alpha_src_mask ROW_ARGS;
extern void _right_left_alpha_src_dest_mask ROW_ARGS;
extern void _right_left_alpha_dest_mask ROW_ARGS;
extern void _right_left_alpha_src_mask ROW_ARGS;

extern void _left_right_alpha ROW_ARGS;
extern void _left_right_solid ROW_ARGS;
extern void _right_left_alpha ROW_ARGS;
extern void _right_left_solid ROW_ARGS;

extern void _top_down(
		struct retroFrameHeader *frame, 
		unsigned char *source_row_start,
		unsigned char *source_row_end, 
		unsigned char *destination_row_start, 
		int height, 
		int srcMask,
		int destMask,
		void (*row) ROW_ARGS,
		struct retroScreen * screen);

extern void _bottom_up(
		struct retroFrameHeader *frame, 
		unsigned char *source_row_start,
		unsigned char *source_row_end, 
		unsigned char *destination_row_start, 
		int height, 
		int srcMask,
		int destMask,
		void (*row) ROW_ARGS,
		struct retroScreen * screen);

#define pick_mode(dir)													\
		if ( destMask )	{											\
			if (frame -> alpha) {											\
				draw_mode = srcMask ? _ ## dir ## _alpha_src_dest_mask : _ ## dir ## _alpha_dest_mask;		\
			} else {													\
				draw_mode = srcMask ? _ ## dir ## _solid_src_dest_mask : _ ## dir ## _solid_dest_mask;		\
			}														\
		} else { 														\
			if (frame -> alpha) {											\
				draw_mode = srcMask ? _ ## dir ## _alpha_src_mask : _ ## dir ## _alpha;		\
			} else {													\
				draw_mode = srcMask ? _ ## dir ## _solid_src_mask : _ ## dir ## _solid;		\
			}														\
		} 															\


#define show_mode(dir)													\
		if ( destMask )	{											\
			if (frame -> alpha) {											\
				if (srcMask) { IDOS->Printf(  "%s%s%s\n", "_" , #dir , "_alpha_src_dest_mask"); } else { IDOS->Printf("%s%s%s\n", "_" , #dir , "_alpha_dest_mask"); }		\
			} else {													\
				if (srcMask) { IDOS->Printf(  "%s%s%s\n", "_" , #dir ,"_solid_src_dest_mask"); } else { IDOS->Printf("%s%s%s\n","_" , #dir , "_solid_dest_mask"); }		\
			}														\
		} else { 														\
			if (frame -> alpha) {											\
				if (srcMask) { IDOS->Printf(  "%s%s%s\n", "_" , #dir , "_alpha_src_mask"); } else { IDOS->Printf("%s%s%s\n","_" , #dir , "_alpha"); }		\
			} else {													\
				if (srcMask) { IDOS->Printf(  "%s%s%s\n", "_" , #dir , "_solid_src_mask"); } else { IDOS->Printf("%s%s%s\n","_" , #dir , "_solid"); }		\
			}														\
		} 

void _retromode_retroPasteSpriteObject(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	int buffer,
	struct retroSpriteObject * spriteObject,
	struct retroSprite *sprite,
	int image,
	int flags)
{
	int width;
	int height;
	int source_x0 = 0,source_y0 = 0;
	unsigned char *destination_row_start;
	unsigned char *source_row_start;
	unsigned char *source_row_end ;
	int x,y,srcMask,destMask;
	int maxW,maxH;

	void (*draw_mode) ROW_ARGS = NULL;

//	IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	x = spriteObject -> x;
	y = spriteObject -> y;

	destMask = spriteObject -> plains;
	srcMask = spriteObject -> plains;

	if (sprite == NULL) return;

	if ( ! sprite -> frames ) 
	{
		Printf("Sprite has no frames!\n");
		return;
	}

	if (image > sprite -> number_of_frames) return;
	if (image < 0) return;

	struct retroFrameHeader *frame = sprite -> frames + image;

	if ( ! frame -> data)
	{
		Printf("Sprite -> frame[%d].data is NULL!\n",image);
		return;
	}

	width = frame -> width;
	height = frame -> height;

	x -= frame -> XHotSpot;
	y -= frame -> YHotSpot;	

	maxW = screen->realWidth;
	maxH = screen->realHeight;

	if (y > spriteObject -> limitYmin)
	{
		// has a limit, & the limit under max width
		if (spriteObject -> limitYmax>0) if ( maxH > spriteObject -> limitYmax) maxH = spriteObject -> limitYmax;

		// is bottom over the max limit
		if (y+height> maxH) height = maxH - y;
	}
	else
	{
		source_y0 = spriteObject -> limitYmin-y; 
		y = spriteObject -> limitYmin; 
		height -= source_y0; 
	}

	if (x > spriteObject -> limitXmin)
	{
		// has a limit, & the limit under max width
		if (spriteObject -> limitXmax>0) if ( maxW > spriteObject -> limitXmax) maxW = spriteObject -> limitXmax;

		// right side is over the max limit
		if (x+width>maxW) width = maxW - x;
	}
	else
	{
		source_x0 = spriteObject -> limitXmin-x; 
		x = spriteObject -> limitXmin; 
		width -= source_x0;
	}

	source_row_start = (unsigned char *) frame -> data + (source_y0 * frame -> bytesPerRow ) + source_x0;
	source_row_end = source_row_start + width;

	switch (flags)
	{
		case 0x0000:
			pick_mode(left_right);
//			show_mode(left_right);
			destination_row_start = screen -> Memory[ buffer ]  + (screen -> bytesPerRow * y)+ x;
			_top_down(frame, source_row_start,source_row_end, destination_row_start, height,  srcMask, destMask, draw_mode,  screen  );
			break;

		case 0x4000:
			pick_mode(left_right);
//			show_mode(left_right);
			destination_row_start = screen -> Memory[ buffer ] + (screen -> bytesPerRow * (y + height - 1)) + x;
			_bottom_up(frame, source_row_start,source_row_end, destination_row_start, height,  srcMask, destMask, draw_mode,  screen  );
			break;

		case 0x8000:
			pick_mode(right_left);
//			show_mode(right_left);
			destination_row_start = screen -> Memory[ buffer ]  + (screen -> bytesPerRow * y)+ x;
			_top_down(frame, source_row_start,source_row_end, destination_row_start, height,  srcMask, destMask, draw_mode,  screen  );
			break;

		case 0xC000:
			pick_mode(right_left);
//			show_mode(right_left);
			destination_row_start = screen -> Memory[ buffer ] + (screen -> realWidth * (y + height - 1)) + x;
			_bottom_up(frame, source_row_start,source_row_end, destination_row_start, height,  srcMask, destMask, draw_mode,  screen  );
			break;

		default:
			Printf("%s:%ld unexpcted flag %08lx\n",__FUNCTION__,__LINE__,flags);
			break;
	}
}

