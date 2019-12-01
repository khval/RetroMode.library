
#include <stdlib.h>
#include <stdio.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>
#include <math.h>
#include "libbase.h"

void draw_lowred_pixeled_color( int beamStart, struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	int display_frame = 0;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	unsigned int *video_buffer_line = video_buffer;
	struct retroScreen *screen = line -> screen;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data ; 
	unsigned char color;
	int videoWidth;
	unsigned int rgb;
	int draw_pixels;

	// check if screen is double buffer screen
	if (screen) if (screen -> Memory[1]) display_frame = 1 - screen -> double_buffer_draw_frame;
	data = line -> data[ display_frame  ] ;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if ( beamStart > 0)
	{
		// move des on postive
		video_buffer_line += beamStart ;
		videoWidth =  (line -> videoWidth - beamStart) / 2;	// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= beamStart ;	
		videoWidth =  (line -> videoWidth - beamStart) / 2;	// displayable video width;
	}

	draw_pixels = line -> pixels > videoWidth ? videoWidth :  line -> pixels;

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


void draw_lowred_emulate_color_changes( int beamStart, struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	int display_frame = 0;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	unsigned int *video_buffer_line = video_buffer;
	struct retroScreen *screen = line -> screen;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data ;
	struct retroRGB *color;
	int videoWidth;
	int draw_pixels;

	// check if screen is double buffer screen
	if (screen) if (screen -> Memory[1]) display_frame = 1 - screen -> double_buffer_draw_frame;
	data = line -> data[ display_frame  ] ;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if (beamStart > 0)
	{
		// move des on postive
		video_buffer_line += beamStart ;
		videoWidth =  (line -> videoWidth - beamStart) / 2;		// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= beamStart ;		// - & - is +
		videoWidth =  (line -> videoWidth +  beamStart)  / 2;	// displayable video width;
	}

	draw_pixels = line -> pixels > videoWidth ? videoWidth :  line -> pixels;

	for (x=0; x < draw_pixels; x++)
	{
		color = &palette[*data++];

		r = ((lr * 5) + (color->r*95)) /100;
		g = ((lg * 5) + (color->g*95)) / 100;
		b = ((lb * 5) + (color->b*95)) /100;

		// keep last
		lr = r; lg = g; lb = b;

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;

		r = ((lr * 5) + (color->r*95)) /100;
		g = ((lg * 5) + (color->g*95)) / 100;
		b = ((lb * 5) + (color->b*95)) /100;

		// keep last.
		lr = r; lg = g; lb = b;

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}

void draw_lowred_ham6( int  beamStart, struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	int display_frame = 0;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	unsigned int *video_buffer_line = video_buffer;
	struct retroScreen *screen = line -> screen;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data ;
	unsigned char color;
	int videoWidth;
	int draw_pixels;

	// check if screen is double buffer screen
	if (screen) if (screen -> Memory[1]) display_frame = 1 - screen -> double_buffer_draw_frame;
	data = line -> data[ display_frame  ] ;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if ( beamStart > 0)
	{
		// move des on postive
		video_buffer_line += beamStart ;
		videoWidth =  (line -> videoWidth - beamStart) / 2;	// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= beamStart ;		// - & - is +
		videoWidth =  (line -> videoWidth + beamStart) / 2; 	// displayable video width;
	}

	draw_pixels = line -> pixels > videoWidth ? videoWidth :  line -> pixels;

	r=0;
	g=0;
	b=0;

	for (x=0; x < draw_pixels; x++)
	{
		color = *data++;

		switch (color & 0x30)
		{
			case 0x00:
					r = palette[color].r;
					g = palette[color].g;
					b = palette[color].b;
					break;
			case 0x10:
					b = (color & 0xF) * 0x11;
					break;
			case 0x20:
					r = (color & 0xF) * 0x11;
					break;
			case 0x30:
					g = (color & 0xF) * 0x11;
					break;
		}

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}

void draw_lowred_ham8( int  beamStart, struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	int display_frame = 0;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	unsigned int *video_buffer_line = video_buffer;
	struct retroScreen *screen = line -> screen;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data ;
	unsigned char color;
	int videoWidth;
	int draw_pixels;

	// check if screen is double buffer screen
	if (screen) if (screen -> Memory[1]) display_frame = 1 - screen -> double_buffer_draw_frame;
	data = line -> data[ display_frame  ] ;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if ( beamStart > 0)
	{
		// move des on postive
		video_buffer_line += beamStart ;
		videoWidth =  (line -> videoWidth - beamStart) / 2;	// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= beamStart ;		// - & - is +
		videoWidth =  (line -> videoWidth + beamStart) / 2; 	// displayable video width;
	}

	draw_pixels = line -> pixels > videoWidth ? videoWidth :  line -> pixels;

	r=0;
	g=0;
	b=0;

	for (x=0; x < draw_pixels; x++)
	{
		color = *data++;

		switch (color & 0x3)
		{
			case 0x0:
					r = palette[color].r;
					g = palette[color].g;
					b = palette[color].b;
					break;
			case 0x1:
					b = color & 0xFC;
					break;
			case 0x2:
					g = color & 0xFC;
					break;
			case 0x3:
					r = color & 0xFC;
					break;
		}

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}


void draw_hires( int beamStart, struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	int display_frame = 0;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	unsigned int *video_buffer_line = video_buffer;
	struct retroScreen *screen = line -> screen;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data ;
	unsigned char color;
	int videoWidth;
	int draw_pixels;

	// check if screen is double buffer screen
	if (screen) if (screen -> Memory[1]) display_frame = 1 - screen -> double_buffer_draw_frame;
	data = line -> data[ display_frame  ] ;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if ( beamStart > 0)
	{
		// move des on postive
		video_buffer_line += beamStart ;
		videoWidth =  (line -> videoWidth - beamStart);		// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= beamStart ;		// - & - is +
		videoWidth =  (line -> videoWidth + beamStart) ;	// displayable video width;
	}

	draw_pixels = line -> pixels > videoWidth ? videoWidth :  line -> pixels;

	for (x=0; x < draw_pixels; x++)
	{
		color = *data++;

		r = palette[color].r;
		g = palette[color].g;
		b = palette[color].b;

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}

void draw_hires_ham6( int beamStart, struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	int display_frame = 0;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	unsigned int *video_buffer_line = video_buffer;
	struct retroScreen *screen = line -> screen;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data ;
	unsigned char color;
	int videoWidth;
	int draw_pixels;

	// check if screen is double buffer screen
	if (screen) if (screen -> Memory[1]) display_frame = 1 - screen -> double_buffer_draw_frame;
	data = line -> data[ display_frame  ] ;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if ( beamStart > 0)
	{
		// move des on postive
		video_buffer_line += beamStart ;
		videoWidth =  (line -> videoWidth - beamStart);		// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= beamStart ;		// - & - is +
		videoWidth =  (line -> videoWidth + beamStart) ;	// displayable video width;
	}

	r=0;
	g=0;
	b=0;

	draw_pixels = line -> pixels > videoWidth ? videoWidth :  line -> pixels;

	for (x=0; x < draw_pixels; x++)
	{
		color = *data++;

		switch (color & 0x30)
		{
			case 0x00:
					r = palette[color].r;
					g = palette[color].g;
					b = palette[color].b;
					break;
			case 0x10:
					b = (color & 0xF) * 0x11;
					break;
			case 0x20:
					r = (color & 0xF) * 0x11;
					break;
			case 0x30:
					g = (color & 0xF) * 0x11;
					break;
		}

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}


void draw_hires_ham8( int beamStart, struct retroScanline *line, int beamY, unsigned int *video_buffer  )
{
	int x;
	int display_frame = 0;
	unsigned short lr,lg,lb;
	unsigned short r,g,b;
	unsigned int *video_buffer_line = video_buffer;
	struct retroScreen *screen = line -> screen;
	struct retroRGB *palette = line -> rowPalette;
	unsigned char *data ;
	unsigned char color;
	int videoWidth;
	int draw_pixels;

	// check if screen is double buffer screen
	if (screen) if (screen -> Memory[1]) display_frame = 1 - screen -> double_buffer_draw_frame;
	data = line -> data[ display_frame  ] ;

	lr = 0;
	lg = 0;
	lb = 0;

	// beam emulates 8 bits per chunk.

	if ( beamStart > 0)
	{
		// move des on postive
		video_buffer_line += beamStart ;
		videoWidth =  (line -> videoWidth - beamStart);		// displayable video width;
	}
	else
	{
		// move src on nagative
		data -= beamStart ;		// - & - is +
		videoWidth =  (line -> videoWidth + beamStart) ;	// displayable video width;
	}

	r=0;
	g=0;
	b=0;

	draw_pixels = line -> pixels > videoWidth ? videoWidth :  line -> pixels;

	for (x=0; x < draw_pixels; x++)
	{
		color = *data++;

		switch (color & 0xC0)
		{
			case 0x00:
					r = palette[color].r;
					g = palette[color].g;
					b = palette[color].b;
					break;
			case 0x40:
					b = (color & 0x3F) << 2;
					break;
			case 0x80:
					r = (color & 0x3F) << 2;
					break;
			case 0xC0:
					g = (color & 0x3F) << 2;
					break;
		}

		*video_buffer_line ++ = 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}
