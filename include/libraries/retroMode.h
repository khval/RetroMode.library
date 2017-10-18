
#ifndef retroMode_h
#define retroMode_h

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>


//------------ video -------------

struct retroRGB
{
	unsigned short r;
	unsigned short g;
	unsigned short b;
};

struct retroRainbow
{
	int color;		// color to change.
	struct retroRGB *table;	// set Rainbow
	int tableSize;			// set Rainbow
	int offset;		// rainbow
	int verticalOffset;	// rainbow (vertical offset)
	int height;	// height of copper
	int drawpos;
};

struct retroScanline
{
	int beamStart;
	int pixels;
	int videoWidth;
	unsigned char *data;
	struct retroRGB *orgPalette;
	struct retroRGB *rowPalette;
	void (*mode) ( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
};

struct retroFlash
{
	int delay;		
	struct retroRGB rgb;
};

struct retroFlashTable
{
	int color;
	int colors;
	int index;
	int countDelay;		// counts up to delay, increments index.
	struct retroFlash	*table;
};

struct retroVideo
{
	struct retroRainbow	rainbow[3];
	struct Window *window;
	struct retroScanline scanlines[480];
	unsigned int *Memory;
	unsigned int BytesPerRow;
	unsigned int width;
	unsigned int height;
	struct RastPort rp;
};

struct p 
{ 
	double x ; double y ;
};

void draw_lowred_emulate_color_changes( struct retroScanline *line, int beamY, unsigned int *video_buffer  );
void draw_hires( struct retroScanline *line, int beamY, unsigned int *video_buffer  );

//void draw_video(struct retroVideo *context, unsigned int *video_buffer );


//--------- screen --------------

struct retroScreen
{
	int width;
	int height;
	unsigned char *Memory;
	struct retroRGB orgPalette[256];
	struct retroRGB rowPalette[256];
};

#define retroLowres 1
#define retroLowres_pixeld 2
#define retroHires 4
#define retroInterlaced 8

#endif
