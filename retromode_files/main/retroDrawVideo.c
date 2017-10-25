/* :ts=4
 *  $VER: retroDrawVideo.c $Revision$ (16-Oct-2017)
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

#include "libbase.h"

/****** retromode/main/retroDrawVideo ******************************************
*
*   NAME
*      retroDrawVideo -- Description
*
*   SYNOPSIS
*      void retroDrawVideo(struct retroVideo * video);
*
*   FUNCTION
*
*   INPUTS
*       video - 
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

void draw_lowred_pixeled_color(  struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data = line -> data;
	unsigned int *video_buffer_line = video_buffer;
	unsigned char color;
	int videoWidth;
	unsigned int rgb;
	int draw_pixels;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if (line -> beamStart > 0)
	{
		// move des on postive
		video_buffer_line += line -> beamStart ;
		videoWidth =  (line -> videoWidth - line -> beamStart) / 2;	// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= line -> beamStart ;		// - & - is +
		videoWidth =  line -> videoWidth / 2;	// displayable video width;
	}

	draw_pixels = line -> pixels - abs(line -> beamStart);

	// lowres has only half the number of pixels.

	if ( draw_pixels > videoWidth ) draw_pixels = videoWidth ;

	for (x=0; x < draw_pixels; x++)
	{
		color = *data++;

		r = palette[color].r;
		g =palette[color].g;
		b =palette[color].b;

		rgb = 0xFF000000 | (r << 16) | (g << 8) | b;

		*video_buffer_line ++ = rgb;
		*video_buffer_line ++ = rgb;
	}
}


void draw_lowred_emulate_color_changes(  struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data = line -> data;
	unsigned int *video_buffer_line = video_buffer;
	unsigned char color;
	int videoWidth;

	int draw_pixels;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if (line -> beamStart > 0)
	{
		// move des on postive
		video_buffer_line += line -> beamStart ;
		videoWidth =  (line -> videoWidth - line -> beamStart) / 2;	// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= line -> beamStart ;		// - & - is +
		videoWidth =  line -> videoWidth / 2;	// displayable video width;
	}

	draw_pixels = line -> pixels - abs(line -> beamStart);

	// lowres has only half the number of pixels.

	if ( draw_pixels > videoWidth ) draw_pixels = videoWidth ;

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
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data = line -> data;
	unsigned int *video_buffer_line = video_buffer;
	unsigned char color;
	unsigned int videoWidth;

	int draw_pixels;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if (line -> beamStart > 0)
	{
		// move des on postive
		video_buffer_line += line -> beamStart ;
		videoWidth =  (line -> videoWidth - line -> beamStart);		// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= line -> beamStart ;		// - & - is +
		videoWidth =  line -> videoWidth;	// displayable video width;
	}
	draw_pixels = line -> pixels - abs(line -> beamStart);

	if (draw_pixels  >  videoWidth ) draw_pixels = videoWidth ;

	for (x=0; x < draw_pixels; x++)
	{
		color = *data++;

		r = palette[color].r;
		g = palette[color].g;
		b = palette[color].b;

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}

static void do_all_screen_color_effects(struct retroScreen *screen)
{
	struct retroFlashTable **flash;
	struct retroFlashTable *_flash;
	struct retroShiftColors **shift;
	struct retroShiftColors *_shift;
	struct retroRGB temp;
	int color;
	int from_color,to_color;

	for (flash = screen -> allocatedFlashs ; flash < screen -> allocatedFlashs_end; flash ++)
	{
		_flash = *flash;
		if (_flash -> colors>0)
		{
			_flash -> countDelay ++;

			if (_flash -> countDelay > _flash -> table[ _flash -> index ].delay)
			{
				_flash -> countDelay = 0;
				_flash -> index = (_flash -> index+1) % _flash -> colors;
				screen -> rowPalette[ _flash -> color & 255 ] = _flash -> table[ _flash -> index ].rgb ;						
			}
		}
	}

	for (shift = screen -> allocatedShifts ; shift < screen -> allocatedShifts_end; shift ++)
	{
		_shift = *shift;
		_shift -> countDelay ++;

		if ( _shift -> countDelay > _shift -> delay )
		{
			_shift -> countDelay = 0;
			from_color = _shift -> firstColor;
			to_color = _shift -> lastColor;

			if (_shift -> flags & 2)
			{
				temp = screen -> rowPalette[from_color];
				for (color = from_color+1; color <= to_color; color ++ ) screen->rowPalette[color-1] = screen->rowPalette[color];
				screen -> rowPalette[ to_color ] = temp;
			}
			else
			{
				temp = screen -> rowPalette[to_color];
				for (color = to_color; color > from_color; color -- ) screen->rowPalette[color] = screen->rowPalette[color-1];
				screen -> rowPalette[ from_color ] = temp;
			}
		}
	}
}

static void copper_to_scanline( struct retroRainbow *rainbow,  struct retroScanline *scanline )
{
	scanline->rowPalette[rainbow ->color] = rainbow->table[rainbow -> drawpos % rainbow->tableSize];
	rainbow -> drawpos++;
}

static void color_reset( struct retroVideo * video, struct retroScanline *scanline)
{
	int n;
	for ( n=0; n<3;n++ )
	{
		if (video->rainbow[n].table)
		{
			scanline -> rowPalette[video->rainbow[n].color] = scanline -> orgPalette[video->rainbow[n].color];
		}
	}
}

void Screen_To_Scanlines( struct RetroLibrary *libBase, struct retroScreen * screen, struct retroVideo * video )
{
	int y;
	int dest_y;
	int videomode = screen -> videomode;

	dest_y = screen -> scanline_y;

//	libBase -> IDOS -> Printf("%s: screen -> scanline_y=%d\n",__FUNCTION__, screen -> scanline_y);

	for ( y = 0 ; y < screen -> height; y++ )
	{
		libBase -> IDOS -> Printf("%s: y=%ld width %ld mode %lx\n",__FUNCTION__,y, screen -> width, videomode);

		if ((dest_y > -1) && (dest_y<video->height))
		{
			video -> scanlines[ dest_y ].beamStart = screen -> scanline_x;
			video -> scanlines[ dest_y ].videoWidth = video -> width;
			video -> scanlines[ dest_y ].screen = screen;
			video -> scanlines[ dest_y ].pixels = screen -> width;
			video -> scanlines[ dest_y ].data = screen -> Memory + (screen -> width * (y + screen -> offset_y) ) + screen -> offset_x;
			video -> scanlines[ dest_y ].mode = NULL;

			video -> scanlines[ dest_y ].rowPalette = screen -> rowPalette;
			video -> scanlines[ dest_y ].orgPalette = screen -> orgPalette;

			if (videomode & retroLowres )
			{
				video -> scanlines[ dest_y ].mode = draw_lowred_emulate_color_changes;
			}

			if (videomode & retroLowres_pixeld )
			{
				video -> scanlines[ dest_y ].mode = draw_lowred_pixeled_color;
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
				video -> scanlines[ dest_y ].orgPalette = NULL;
				video -> scanlines[ dest_y ].rowPalette = NULL;
				video -> scanlines[ dest_y ].beamStart = 0;
				video -> scanlines[ dest_y ].pixels = 0;
				video -> scanlines[ dest_y ].data = NULL;
				video -> scanlines[ dest_y ].mode = NULL;
				video -> scanlines[ dest_y ].screen = NULL;
				dest_y ++;
			}
		}
	}
}

void update_all_scanlines( struct RetroLibrary *libBase, struct retroVideo * video )
{
	struct retroScreen **screen_item;

	libBase->IDOS->Printf("%s: %lx %lx\n",__FUNCTION__,video -> attachedScreens,video -> attachedScreens_end);

	for (screen_item = video -> attachedScreens; screen_item < video -> attachedScreens_end; screen_item++)
	{
		Screen_To_Scanlines( libBase, *screen_item, video );
	}
}

void _retromode_retroDrawVideo(struct RetroModeIFace *Self, struct retroVideo * video)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroScanline *scanline = video -> scanlines;
	struct retroScreen **screen_item;
	unsigned int *video_buffer = video -> Memory;
	unsigned int beamY;
	unsigned int beamcount;
	int intsPerRow = video -> BytesPerRow / 4;
	struct retroRainbow *compressed_rainbow_table[3];
	struct retroRainbow **compressed_rainbow_table_end;
	struct retroRainbow **rainbow_ptr;
	int n;
	BOOL coopered = FALSE;
	BOOL coopered_last = TRUE;

	if (video -> refreshAllScanlines == TRUE)
	{
		libBase->IDOS->Printf("%s: if (video -> refreshAllScanlines == TRUE)\n",__FUNCTION__);

		video -> refreshAllScanlines = FALSE;
		update_all_scanlines(libBase, video);
	}

	if (video -> refreshSomeScanlines == TRUE)
	{
		video -> refreshSomeScanlines = FALSE;
//		update_some_scanlines(video);
	}


	// only allocated rainbow tables are in the compressed table
	// allowing me skip validating in main loop.

	compressed_rainbow_table_end = compressed_rainbow_table;

	for ( n = 0; n < 3; n++)
	{
		if (video->rainbow[n].table)
		{
			video->rainbow[n].drawpos =video->rainbow[n].offset;
			*compressed_rainbow_table_end++ = &video->rainbow[n];
		}
	}

	beamcount = 0;
	for (beamY=0; beamY < video-> height; beamY++)
	{
		if (scanline->mode != NULL) 
		{
			coopered = FALSE;

			for ( rainbow_ptr = compressed_rainbow_table; rainbow_ptr < compressed_rainbow_table_end; rainbow_ptr++)
			{
				if (( beamcount > (*rainbow_ptr) -> verticalOffset) && ( beamcount < (*rainbow_ptr) -> verticalOffset + (*rainbow_ptr) -> height ))
				{
					copper_to_scanline( *rainbow_ptr,  scanline );
					coopered = TRUE;
				}
			}

			if ((coopered == FALSE)&&( coopered_last == TRUE))
			{
				color_reset( video, scanline );
			}

			scanline -> mode( scanline,  beamY, video_buffer  );
			beamcount ++;
			coopered_last = coopered;
		}

		video_buffer += intsPerRow;	// next line
		scanline ++;
	}

	for (screen_item = video -> attachedScreens; screen_item < video -> attachedScreens_end; screen_item++)
	{
		do_all_screen_color_effects(*screen_item);
	}
}

