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

#define __USE_INLINE__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>
#include <math.h>
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


void resetScanlines(struct retroVideo * video)
{
	struct retroParallax *line = video -> scanlines;
	int beamY;

	for (beamY=0; beamY < video-> height; beamY++)
	{
		line -> scanline[0].mode = NULL;
		line -> scanline[1].mode = NULL;
		line ++;
	}
}

extern void draw_lowred_pixeled_color( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_lowred_emulate_color_changes( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_lowred_ham6( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_lowred_ham8( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_hires( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_hires_ham6( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_hires_ham8( struct retroScanline *line, int beamY, unsigned int *video_buffer  );

extern void draw_transparent_lowred_pixeled_color( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_transparent_lowred_emulate_color_changes( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_transparent_lowred_ham6(  struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_transparent_lowred_ham8( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
extern void draw_transparent_hires( struct retroScanline *line, int beamY, unsigned int *video_buffer  );

static void do_all_screen_color_effects( struct retroScreen *screen)
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

			switch (_shift -> flags)
			{
				case 0:
					for (color = to_color; color > from_color; color -- ) screen->rowPalette[color] = screen->rowPalette[color-1];
					break;

				case 1:

					temp = screen -> rowPalette[to_color];
					for (color = to_color; color > from_color; color -- ) screen->rowPalette[color] = screen->rowPalette[color-1];
					screen -> rowPalette[ from_color ] = temp;
					break;

				case 2:
					for (color = from_color+1; color <= to_color; color ++ ) screen->rowPalette[color-1] = screen->rowPalette[color];
					break;

				case 3:
					temp = screen -> rowPalette[from_color];
					for (color = from_color+1; color <= to_color; color ++ ) screen->rowPalette[color-1] = screen->rowPalette[color];
					screen -> rowPalette[ to_color ] = temp;
					break;
			}
		}
	}
}

static void copper_to_scanline( struct retroRainbow *rainbow, int rainbow_scanline, struct retroScanline *scanline )
{
	scanline->rowPalette[rainbow ->color] = rainbow->table[ (rainbow_scanline + rainbow -> offset) % rainbow->tableSize];
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

void set_scanline( int n, struct retroParallax *line,struct retroScreen * screen, struct retroVideo * video, int offset)
{
	int videomode = screen -> videomode & ~retroInterlaced;

	struct retroScanline *scanline = &line -> scanline[n];

	scanline -> beamStart = screen -> scanline_x;
	scanline -> videoWidth = video -> width;
	scanline -> screen = screen;
	scanline -> pixels = screen -> displayWidth;

	if (screen -> Memory[1])
	{
		scanline -> data[0] = screen -> Memory[0] + offset;
		scanline -> data[1] = screen -> Memory[1] + offset;
	}
	else
	{
		scanline -> data[0] = screen -> Memory[0] + offset;
		scanline -> data[1] = NULL;
	}

	scanline -> mode = NULL;
	scanline -> rowPalette = screen -> rowPalette;
	scanline -> orgPalette = screen -> orgPalette;


	if (screen -> dualScreen)	// has dualScreen, so this need to be transparent.
	{
		switch (videomode) 
		{
			case retroLowres:
				scanline -> mode = draw_transparent_lowred_emulate_color_changes;
				break;

			case retroLowres|retroHam6:
				scanline -> mode = draw_transparent_lowred_ham6;
				break;

			case retroLowres|retroHam8:
				scanline -> mode = draw_transparent_lowred_ham8;
				break;

			case retroLowres_pixeld:
				scanline -> mode = draw_transparent_lowred_pixeled_color;
				break;

			case retroLowres_pixeld|retroHam6:
				scanline -> mode = draw_transparent_lowred_ham6;
				break;

			case retroLowres_pixeld|retroHam8:
				scanline -> mode = draw_transparent_lowred_ham8;
				break;

			case retroHires:
				scanline -> mode = draw_transparent_hires;
				break;

			case retroHires|retroHam6:
				scanline -> mode = draw_transparent_hires;
				break;

			case retroHires|retroHam8:
				scanline -> mode = draw_transparent_hires;
				break;
		}
	}
	else
	{
		switch (videomode) 
		{
			case retroLowres:
				scanline -> mode = draw_lowred_emulate_color_changes;
				break;

			case retroLowres|retroHam6:
				scanline -> mode = draw_lowred_ham6;
				break;

			case retroLowres|retroHam8:
				scanline -> mode = draw_lowred_ham8;
				break;

			case retroLowres_pixeld:
				scanline -> mode = draw_lowred_pixeled_color;
				break;

			case retroLowres_pixeld|retroHam6:
				scanline -> mode = draw_lowred_ham6;
				break;

			case retroLowres_pixeld|retroHam8:
				scanline -> mode = draw_lowred_ham8;
				break;

			case retroHires:
				scanline -> mode = draw_hires;
				break;

			case retroHires|retroHam6:
				scanline -> mode = draw_hires_ham6;
				break;

			case retroHires|retroHam8:
				scanline -> mode = draw_hires_ham8;
				break;
		}
	}
}

// do not change, its not a option.
#define is_displayed 0

void Screen_To_Scanlines( struct retroScreen * screen, struct retroVideo * video )
{
	int n;
	int scanline_x,scanline_y,screen_y ;
	int hw_start ;
	int hw_end ;
	int hw_y ;
	int offset;
	int displayed ;
	int physical_vfacor ;

	if (screen -> flags & retroscreen_flag_hide) 	return;

	scanline_x = screen -> scanline_x;
	scanline_y = screen -> scanline_y;

	for (n=0;n<=1;n++)	// dual screen.
	{
		screen_y = 0;
		hw_start = 0;
		hw_end = 0;
		hw_y = 0;
		displayed = (screen ->videomode & retroInterlaced) ? 0 : 1;
		physical_vfacor = (screen ->videomode & retroInterlaced) ? 1 : 2;

		hw_start = scanline_y;
		hw_end = hw_start + (screen -> displayHeight * physical_vfacor );

		if (hw_end<0)	return;						// outside of scope.
		if (hw_start> ((int) video->height))	return;		// outside of scope.

		if (hw_start<0)
		{
			screen_y = -hw_start / physical_vfacor;
			hw_start = 0;
		}

		if (hw_end>video->height) hw_end = video->height;

		screen_y += screen -> offset_y;

		for (hw_y = hw_start; hw_y<hw_end; hw_y++)
		{
			if ((hw_y & displayed) == is_displayed)
			{
				if ((screen_y>=0) && (screen_y <= screen -> realHeight))
				{
					offset = (screen -> bytesPerRow * screen_y) + screen -> offset_x;

					video -> scanlines[ hw_y ].beamStart = scanline_x;
					set_scanline(  n, &video -> scanlines[ hw_y ], screen, video, offset );
				}

				screen_y ++;
			}
		}

		if (screen -> dualScreen == NULL) 	break;

		screen = screen -> dualScreen;
	}
}

bool inside_screen(struct retroScreen *screen, int scanline_y)
{
	int physical_vfacor = (screen ->videomode & retroInterlaced) ? 1 : 2;
	int hw_start = screen -> scanline_y;
	int hw_end = hw_start + (screen -> displayHeight * physical_vfacor );

	if (screen -> flags & retroscreen_flag_hide) 	return false;		// if screen is hidden then we do not need to worry about it..
	if ((hw_start<=scanline_y) && (hw_end>=scanline_y)) return true;

	return false;
}

bool inside_screens( struct retroVideo * video, struct retroScreen *this_screen, int scanline_y)
{
	bool this_screen_found = false;
	struct retroScreen **screen_item;

	for (screen_item = video -> attachedScreens; screen_item < video -> attachedScreens_end; screen_item++)
	{
		if (this_screen_found == false)
		{
			if (*screen_item == this_screen ) this_screen_found = true;	
		}
		else	// we only need to check if screen after is inside.. (we are checking if we render on top...)
		{
			if (inside_screen(*screen_item, scanline_y)) return true;
		}
	}
	return false;
}

void refresh_screen_scanlines( struct retroScreen * screen, struct retroVideo * video )
{
	int n;
	int scanline_x,scanline_y,screen_y ;
	int hw_start ;
	int hw_end ;
	int hw_y ;
	int offset;
	int displayed ;
	int physical_vfacor ;

	if (screen -> flags & retroscreen_flag_hide) 	return;

	scanline_x = screen -> scanline_x;
	scanline_y = screen -> scanline_y;

	for (n=0;n<=1;n++)
	{
		screen_y = 0;
		hw_start = 0;
		hw_end = 0;
		hw_y = 0;
		displayed = (screen ->videomode & retroInterlaced) ? 0 : 1;
		physical_vfacor = (screen ->videomode & retroInterlaced) ? 1 : 2;

		hw_start = scanline_y;
		hw_end = hw_start + (screen -> displayHeight * physical_vfacor );

		if (hw_end<0)	return;						// outside of scope.
		if (hw_start> ((int) video->height))	return;		// outside of scope.

		if (hw_start<0)
		{
			screen_y = -hw_start / physical_vfacor;
			hw_start = 0;
		}

		if (hw_end>video->height) hw_end = video->height;

		screen_y += screen -> offset_y;

		for (hw_y = hw_start; hw_y<hw_end; hw_y++)
		{
			if ((hw_y & displayed) == is_displayed)
			{
				if (inside_screens( video, screen, hw_y ) == false )
				{
					if ((screen_y>=0) && (screen_y <= screen -> realHeight))
					{
						offset = (screen -> bytesPerRow * screen_y) + screen -> offset_x;

						video -> scanlines[ hw_y ].beamStart = scanline_x;
						set_scanline( n, &video -> scanlines[ hw_y ], screen, video, offset );
					}
				}

				screen_y ++;
			}
		}

		if (screen -> dualScreen == NULL) 	break;

		screen = screen -> dualScreen;
	}
}

void update_all_scanlines( struct retroVideo * video )
{
	struct retroScreen **screen_item;

	for (screen_item = video -> attachedScreens; screen_item < video -> attachedScreens_end; screen_item++)
	{
		Screen_To_Scanlines( *screen_item, video );
	}
}

void update_some_scanlines( struct retroVideo * video )
{
	struct retroScreen **screen_item;

	for (screen_item = video -> attachedScreens; screen_item < video -> attachedScreens_end; screen_item++)
	{
		if ( (*screen_item) -> refreshScanlines == TRUE)
		{
			refresh_screen_scanlines( *screen_item, video );

			(*screen_item) -> refreshScanlines = FALSE;
		}
	}
}

void _retromode_retroDrawVideo(struct RetroModeIFace *Self, struct retroVideo * video)
{
	struct retroParallax *line;
	struct retroScreen **screen_item;
	unsigned int *video_buffer;
	int beamY;
	int intsPerRow = video -> BytesPerRow / 4;
	struct retroRainbow *compressed_rainbow_table[4];
	struct retroRainbow **compressed_rainbow_table_end;
	struct retroRainbow **rainbow_ptr;
	int n;
	BOOL coopered = FALSE;
	BOOL *coopered_last = NULL;
	int rainStart, rainEnd;

	if (video -> refreshAllScanlines == TRUE)
	{
		video -> refreshAllScanlines = FALSE;
		resetScanlines(video);
		update_all_scanlines( video);
	}

	if (video -> refreshSomeScanlines == TRUE)
	{
		video -> refreshSomeScanlines = FALSE;
		update_some_scanlines( video);
	}

	// only allocated rainbow tables are in the compressed table
	// allowing me skip validating in main loop.

	compressed_rainbow_table_end = compressed_rainbow_table;

	for ( n = 0; n <= 3; n++)
	{
		if (video->rainbow[n].table)
		{
			*compressed_rainbow_table_end++ = &video->rainbow[n];
		}
	}

	for ( n=1; n>=0; n-- )
	{
		line = video -> scanlines;
		video_buffer = video -> Memory;

		for (beamY=0; beamY < video-> height; beamY++)
		{
			if (line -> scanline[n].mode != NULL) 
			{
				coopered = FALSE;

				for ( rainbow_ptr = compressed_rainbow_table; rainbow_ptr < compressed_rainbow_table_end; rainbow_ptr++)
				{
					rainStart = ((*rainbow_ptr) -> verticalOffset * 2);
					rainEnd =  rainStart + ((*rainbow_ptr) -> height * 2) ;

					if (( beamY > rainStart) && ( beamY < rainEnd ))
					{
						copper_to_scanline( *rainbow_ptr, (beamY - rainStart) / 2 ,  &line -> scanline[0] );
						coopered = TRUE;
					}
				}

				coopered_last = line -> scanline[n].screen ? &(line ->scanline[n].screen -> coopered_last) : NULL;
				if (coopered_last)		// have pointer...
				{
					if ((coopered == FALSE)&&( *coopered_last == TRUE))
					{
						color_reset( video, &line -> scanline[n] );
					}

					line -> scanline[n].mode( &line -> scanline[n],  beamY, video_buffer  );	
					*coopered_last = coopered;
				}
			}

			video_buffer += intsPerRow;	// next line
			line ++;
		}
	}

	for (screen_item = video -> attachedScreens; screen_item < video -> attachedScreens_end; screen_item++)
	{
		do_all_screen_color_effects(*screen_item);
	}
}

