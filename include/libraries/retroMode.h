
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
	int color;				// color to change.
	struct retroRGB *table;	// set Rainbow
	int tableSize;			// set Rainbow
	int offset;				// rainbow
	int verticalOffset;		// rainbow (vertical offset)
	int height;			// height of copper
	int drawpos;
};

struct retroScanline
{
	int beamStart;
	int pixels;
	int videoWidth;
	unsigned char *data;
	struct retroScreen *screen;
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
	int countDelay;		// counts up to delay, increments value.
	struct retroFlash	*table;
};

struct retroShiftColors
{
	int delay;
	int countDelay;		// counts up to delay, increments value.
	unsigned char firstColor;
	unsigned char lastColor;
	unsigned char flags;
};

struct retroVideo
{
	struct retroScreen *attachedScreens[256];
	struct retroScreen **attachedScreens_end;
	int screensAttached;
	struct retroRainbow	rainbow[3];
	struct Window *window;
	struct retroScanline scanlines[480];
	unsigned int *Memory;
	unsigned int BytesPerRow;
	unsigned int width;
	unsigned int height;
	struct RastPort rp;
	BOOL updateScreenList;
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

	// color palette and copper palette
	struct retroRGB orgPalette[256];
	struct retroRGB rowPalette[256];

	// keeping track of what video its attached to.
	struct retroVideo *attachedToVideo;

	// typical classic effects flash and color shifting
	struct retroFlashTable *allocatedFlashs[256];
	struct retroFlashTable **allocatedFlashs_end;
	int flashsAllocated;
	struct retroShiftColors *allocatedShifts[256];	// you can't shift more colors then there is on the screen.
	struct retroShiftColors **allocatedShifts_end;
	int shiftsAllocated;
	// text cursor place holder	
	int locateX;		
	int locateY;
};

struct retroFrame
{
	struct retroScreen *screen;
	int x1;
	int y1;
	int x2;
	int y2;
	int hotspotX;
	int hotspotY;
};

#define retroLowres 1
#define retroLowres_pixeld 2
#define retroHires 4
#define retroInterlaced 8

#define ECSColorToRGB32( ecs, color )			\
			color.r =( ecs & 0xF00) >> 4;		\
			color.g =( ecs & 0x0F0);			\
			color.b =( ecs & 0x00F) << 4;	

#endif
