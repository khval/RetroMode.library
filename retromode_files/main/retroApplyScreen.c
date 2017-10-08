/* :ts=4
 *  $VER: retroApplyScreen.c $Revision$ (03-Oct-2017)
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
#include <stdarg.h>

#include "libbase.h"


/****** retromode/main/retroApplyScreen ******************************************
*
*   NAME
*      retroApplyScreen -- Description
*
*   SYNOPSIS
*      void retroApplyScreen(struct retroScreen * screen, 
*          struct retroVideo * video, int offsetx, int oppsety, 
*          int videomode);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       video - 
*       offsetx - 
*       oppsety - 
*       videomode - 
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

extern void _retromode_set_retroVideoColor(struct RetroModeIFace *Self,
       struct retroVideo * video,
       struct retroRGB * RGB,
       int color,
       int from,
       int to);

void draw_lowred_emulate_color_changes(  struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	int beamX = 0;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	struct retroRGB *palette = line -> palette;
	unsigned char *data = line -> data;
	unsigned int *video_buffer_line = video_buffer;
	unsigned char color;

	int draw_pixels;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	video_buffer_line += (line -> beamStart >> 3);
	beamX =line -> beamStart >> 3;

	draw_pixels = line -> pixels - (line -> beamStart >> 3);

	if (draw_pixels * 2 > line -> videoWidth ) draw_pixels = (line -> videoWidth / 2) - (line -> beamStart >> 3);


	for (x=0; x < draw_pixels; x++)
	{

		color = *data++;

		r = ((lr * 5) + (palette[color].r*95)) /100;
		g = ((lg * 5) + (palette[color].g*95)) / 100;
		b = ((lb * 5) + (palette[color].b*95)) /100;

		// keep last
		lr = r; lg = g; lb = b;

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;

		r = ((lr * 5) + (palette[color].r*95)) /100;
		g = ((lg * 5) + (palette[color].g*95)) / 100;
		b = ((lb * 5) + (palette[color].b*95)) /100;

		// keep last.
		lr = r; lg = g; lb = b;

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}

void draw_hires(  struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	int beamX = 0;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	struct retroRGB *palette = line -> palette;
	unsigned char *data = line -> data;
	unsigned int *video_buffer_line = video_buffer;
	unsigned char color;

	int draw_pixels;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	video_buffer_line += (line -> beamStart >> 3);
	beamX =line -> beamStart >> 3;

	draw_pixels = line -> pixels - (line -> beamStart >> 3);

	if (draw_pixels > line -> videoWidth ) draw_pixels = (line -> videoWidth / 2) - (line -> beamStart >> 3);

	for (x=0; x < draw_pixels; x++)
	{
		color = *data++;

		r = palette[color].r;
		g = palette[color].g;
		b = palette[color].b;

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}


void _retromode_retroApplyScreen(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       struct retroVideo * video,
       int offsetx,
       int offsety,
       int videomode)
{
	int color;
	int y;
	int dest_y;
	int scan_line_from;
	int scan_line_to;

	scan_line_from = offsety;
	dest_y = offsety;

	for ( y = 0 ; y < screen -> height; y++ )
	{
		if ((dest_y > -1) && (dest_y<video->height))
		{
			video -> scanlines[ dest_y ].beamStart = offsetx;
			video -> scanlines[ dest_y ].videoWidth = video -> width;
			video -> scanlines[ dest_y ].pixels = screen -> width;
			video -> scanlines[ dest_y ].data = screen -> Memory + (screen -> width * y);
			video -> scanlines[ dest_y ].mode = NULL;

			if (videomode & retroLowres )
			{
				video -> scanlines[ dest_y ].mode = draw_lowred_emulate_color_changes;
			}

			if (videomode & retroHires )
			{
				video -> scanlines[ dest_y ].mode = draw_hires;
			}

			dest_y ++;
		}

		if ( ! (videomode & retroInterlaced) )
		{
			if ((dest_y > -1) && (dest_y<video->height))
			{
				video -> scanlines[ dest_y ].beamStart = 0;
				video -> scanlines[ dest_y ].pixels = 0;
				video -> scanlines[ dest_y ].data = NULL;
				video -> scanlines[ dest_y ].mode = NULL;
				dest_y ++;
			}
		}
	}

	if  (videomode & retroInterlaced)
	{
		scan_line_to = offsety + screen -> height;
	}
	else
	{
		scan_line_to = offsety + (screen -> height*2);
	}

	if (scan_line_from < 0) scan_line_from = 0;
	if (scan_line_from > video -> height) scan_line_from = video -> height;
	if (scan_line_to > video -> height) scan_line_to = video -> height;
	if (scan_line_to<scan_line_from) scan_line_to = scan_line_from;

	for ( color=0; color<256; color++)
	{
		 Self -> set_retroVideoColor( video, screen -> palette +color,  color, scan_line_from , scan_line_to );
	}

}

