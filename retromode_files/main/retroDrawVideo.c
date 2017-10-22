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

static BOOL is_found( struct retroVideo * video, struct retroScreen * screen)
{
	struct retroScreen **screen_item;

	for (screen_item = video -> attachedScreens; screen_item < video -> attachedScreens_end; screen_item++)
	{
		if (*screen_item == screen) return TRUE;
	}

	return FALSE;
}

static void update_screen_list(struct retroVideo * video)
{
	int y;

	// refersh the list
	video -> screensAttached = 0;
	for (y=0;y<video->height;y++)
	{
		if (video -> scanlines[ y ].screen)
		{
			if ( is_found( video, video -> scanlines[ y ].screen ) == FALSE )
			{
				video -> attachedScreens[ video -> screensAttached ] = video -> scanlines[ y ].screen;

				video -> screensAttached++;
				video -> attachedScreens_end = video -> attachedScreens + video -> screensAttached;	
			}
		}
	}
}

void _retromode_retroDrawVideo(struct RetroModeIFace *Self,
       struct retroVideo * video)
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

	if (video -> updateScreenList == TRUE)
	{
		video -> updateScreenList = FALSE;
		update_screen_list(video);
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

