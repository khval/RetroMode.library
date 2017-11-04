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

struct retroSprite * _retromode_retroPasteSprite(struct RetroModeIFace *Self,
       struct retroScreen * screen,
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
	char *destination_row_ptr;
	char *destination_row_start;
	char *source_row_start;
	char *source_row_ptr;
	char *source_row_end ;

	if (image > sprite -> number_of_frames) image = sprite -> number_of_frames;
	if (image < 0) image = 0;

	struct retroFrameHeader *frame = sprite -> frames + image;

	width = frame -> Width;
	height = frame -> Height;

	x -= frame -> XHotSpot;
	y -= frame -> YHotSpot;	

	if (y+height>screen->realHeight) height = screen->realHeight - y;
	if (x+width>screen->realWidth) width = screen->realWidth - x;

	if (y<0) { source_y0 = -y; y = 0; height -= source_y0;  }
	if (x<0) { source_x0 = -x; x = 0; width -= source_x0; }

	destination_row_start = screen -> Memory + (screen -> realWidth * y) + x;

	source_row_start = frame -> data + (source_y0 * frame -> bytesPerRow );
	source_row_end = source_row_start + width;

	for ( ypos = 0; ypos < height; ypos++ )
	{
		destination_row_ptr = destination_row_start;

		for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
		{
			*destination_row_ptr++= *source_row_ptr;
		}

		destination_row_start += screen -> realWidth;
		source_row_start += frame -> bytesPerRow;
		source_row_end += frame -> bytesPerRow;
	}
}

