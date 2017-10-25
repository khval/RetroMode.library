#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef amigaos4
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/layers.h>
#include <proto/retroMode.h>
#else
#include "os4_emu.h"
#endif

int scrolled_x;
int scroll_speed = 2;
int scroll_char = 0;

char *scroll_text = "Small scroll text demo..... have fun playing with this thing..... ";

#define IDCMP_COMMON IDCMP_MOUSEBUTTONS | IDCMP_INACTIVEWINDOW | IDCMP_ACTIVEWINDOW  | \
	IDCMP_CHANGEWINDOW | IDCMP_MOUSEMOVE | IDCMP_REFRESHWINDOW | IDCMP_RAWKEY | \
	IDCMP_EXTENDEDMOUSE | IDCMP_CLOSEWINDOW | IDCMP_NEWSIZE | IDCMP_INTUITICKS

struct retroVideo *video = NULL;
struct Window *My_Window = NULL;

struct Library * IntuitionBase = NULL;
struct IntuitionIFace *IIntuition = NULL;

struct Library * GraphicsBase = NULL;
struct GraphicsIFace *IGraphics = NULL;

struct Library * RetroModeBase = NULL;
struct RetroModeIFace *IRetroMode = NULL;

struct Library * LayersBase = NULL;
struct LayersIFace *ILayers = NULL;

struct XYSTW_Vertex3D { 
float x, y; 
float s, t, w; 
}; 

typedef struct CompositeHookData_s {
	struct BitMap *srcBitMap; // The source bitmap
	int32 srcWidth, srcHeight; // The source dimensions
	int32 offsetX, offsetY; // The offsets to the destination area relative to the window's origin
	int32 scaleX, scaleY; // The scale factors
	uint32 retCode; // The return code from CompositeTags()
} CompositeHookData;

#ifdef amigaos4
void draw_comp_bitmap(struct BitMap *the_bitmap,struct BitMap *the_bitmap_dest, int width,int height, int wx,int wy,int ww, int wh)
{
	#define STEP(a,xx,yy,ss,tt,ww)   P[a].x= xx; P[a].y= yy; P[a].s= ss; P[a].t= tt; P[a].w= ww;  

	int error;
	struct XYSTW_Vertex3D P[6];

	STEP(0, wx, wy ,0 ,0 ,1);
	STEP(1, wx+ww,wy,width,0,1);
	STEP(2, wx+ww,wy+wh,width,height,1);

	STEP(3, wx,wy, 0,0,1);
	STEP(4, wx+ww,wy+wh,width,height,1);
	STEP(5, wx, wy+wh ,0 ,height ,1);

	if (the_bitmap)
	{

		error = CompositeTags(COMPOSITE_Src, 
			the_bitmap, the_bitmap_dest,

			COMPTAG_VertexArray, P, 
			COMPTAG_VertexFormat,COMPVF_STW0_Present,
		    	COMPTAG_NumTriangles,2,

			COMPTAG_SrcAlpha, (uint32) (0x0010000 ),
			COMPTAG_Flags, COMPFLAG_SrcAlphaOverride | COMPFLAG_HardwareOnly | COMPFLAG_SrcFilter ,
			TAG_DONE);
	}
}
#endif


static ULONG compositeHookFunc(struct Hook *hook, struct RastPort *rastPort, struct BackFillMessage *msg) {

	struct Window *the_win = video -> window;	

#ifdef amigaos4

	draw_comp_bitmap(video->rp.BitMap, the_win->RPort -> BitMap, video -> width, video -> height,
		the_win->BorderLeft ,
		the_win->BorderTop ,
		the_win->Width - the_win->BorderLeft - the_win->BorderRight,
		the_win->Height -  the_win->BorderTop - the_win->BorderBottom);

#endif

	return 0;
}

static CompositeHookData hookData;

static struct Rectangle rect;
static struct Hook hook;

static void set_target_hookData( void )
{
 	rect.MinX = My_Window->BorderLeft;
 	rect.MinY = My_Window->BorderTop;
 	rect.MaxX = My_Window->Width - My_Window->BorderRight - 1;
 	rect.MaxY = My_Window->Height - My_Window->BorderBottom - 1;

 	float destWidth = rect.MaxX - rect.MinX + 1;
 	float destHeight = rect.MaxY - rect.MinY + 1;
 	float scaleX = (destWidth + 0.5f) / (double) video -> width;
 	float scaleY = (destHeight + 0.5f) / (double) video -> height;

	hookData.srcWidth = video -> width;
	hookData.srcHeight = video -> height;
	hookData.offsetX = video -> window->BorderLeft;
	hookData.offsetY = video -> window->BorderTop;
	hookData.scaleX = COMP_FLOAT_TO_FIX(scaleX);
	hookData.scaleY = COMP_FLOAT_TO_FIX(scaleY);
	hookData.retCode = COMPERR_Success;

	hook.h_Entry = (HOOKFUNC)compositeHookFunc;
	hook.h_Data = &hookData;
}

static void BackFill_Func(struct RastPort *ArgRP, struct BackFillArgs *MyArgs)
{
	set_target_hookData();

//	LockLayer(0,video -> window -> RPort -> Layer);
	DoHookClipRects(&hook, video -> window -> RPort, &rect);
//	UnlockLayer(video -> window -> RPort -> Layer);
}


BOOL open_lib( const char *name, int ver , const char *iname, int iver, struct Library **base, struct Interface **interface)
{
	*interface = NULL;
	*base = OpenLibrary( name , ver);
	if (*base)
	{
		 *interface = GetInterface( *base,  iname , iver, TAG_END );
		if (!*interface) printf("Unable to getInterface %s for %s %ld!\n",iname,name,ver);
	}
	else
	{
	   	printf("Unable to open the %s %ld!\n",name,ver);
	}
	return (*interface) ? TRUE : FALSE;
}



bool open_window( int window_width, int window_height )
{
		My_Window = OpenWindowTags( NULL,

#ifdef __amigaos4__
					WA_Title,         "retroVideo window mode",
#endif
			WA_InnerWidth,      window_width,
			WA_InnerHeight,     window_height,

			WA_SimpleRefresh,		TRUE,
			WA_CloseGadget,     TRUE,
			WA_DepthGadget,     TRUE,

			WA_DragBar,         TRUE,
			WA_Borderless,      FALSE,
			WA_SizeGadget,      TRUE,
			WA_SizeBBottom,	TRUE,

			WA_IDCMP,           IDCMP_COMMON,
			WA_Flags,           WFLG_REPORTMOUSE,
			TAG_DONE);

	return (My_Window != NULL) ;
}


bool init()
{
	if ( ! open_lib( "intuition.library", 51L , "main", 1, &IntuitionBase, (struct Interface **) &IIntuition  ) ) return FALSE;
	if ( ! open_lib( "graphics.library", 54L , "main", 1, &GraphicsBase, (struct Interface **) &IGraphics  ) ) return FALSE;
	if ( ! open_lib( "layers.library", 54L , "main", 1, &LayersBase, (struct Interface **) &ILayers  ) ) return FALSE;
	if ( ! open_lib( "retromode.library", 1L , "main", 1, &RetroModeBase, (struct Interface **) &IRetroMode  ) ) return FALSE;

	if ( ! open_window(640,480) ) return false;

	if ( (video = retroAllocVideo( My_Window )) == NULL ) return false;

	return TRUE;
}

void closedown()
{
	if (My_Window) CloseWindow(My_Window);

	if (IntuitionBase) CloseLibrary(IntuitionBase); IntuitionBase = 0;
	if (IIntuition) DropInterface((struct Interface*) IIntuition); IIntuition = 0;

	if (LayersBase) CloseLibrary(LayersBase); LayersBase = 0;
	if (ILayers) DropInterface((struct Interface*) ILayers); ILayers = 0;

	if (GraphicsBase) CloseLibrary(GraphicsBase); GraphicsBase = 0;
	if (IGraphics) DropInterface((struct Interface*) IGraphics); IGraphics = 0;

	if (RetroModeBase) CloseLibrary(RetroModeBase); RetroModeBase = 0;
	if (IRetroMode) DropInterface((struct Interface*) IRetroMode); IRetroMode = 0;
}

struct p3 { int x ; int y ; int z; };

void retroBoingOutline( struct retroScreen *screen, int rx, int ry, int r, int t, unsigned char color )
{
	int x0,y0,x1,y1;
	int xx;
	int rr;
	int r2 = r * r;
	int x,y;

	x0 = rx -r;
	y0 = ry-r;
	x1 = rx+r;
	y1 = ry+r;

	for (y=-r;y<=r;y++)
	{
		xx = sqrt( r2 - (y*y));

		for (x = -xx; x<xx;x++)
		{
			rr = sqrt( (x*x) + (y*y) );

			if (rr<=r)
			{
				retroPixel( screen, x + rx, y + ry, rr<r-t ? 2 : color );
			}
		}
	}
}

void retroBoing( struct retroScreen *screen, int rx, int ry, int r, unsigned char color )
{
	int x0,y0,x1,y1;
	int xx;
	int rr;
	int r2 = r * r;
	int width, height;
	int bx,by;
	int x,y;

	x0 = rx -r;
	y0 = ry-r;
	x1 = rx+r;
	y1 = ry+r;

	height = y1-y0+1;

	for (y=-r;y<=r;y++)
	{
		xx = sqrt( r2 - (y*y));
		width = xx*2+1;

		by = ((y + r) * 6 / height) & 1;

		for (x = -xx; x<xx;x++)
		{
			bx = ((x + xx) * 6 / width) & 1;

			retroPixel( screen, x + rx, y + ry, (bx + by) & 1 ? color : 3 );
		}
	}
}


struct greate_ball_of_fire
{
	int x;
	int y;
	int speed;
	double a;
	double r;
	int color;
};

void init_ball( struct greate_ball_of_fire *ball )
{
	ball -> x = rand() % 320;
	ball -> y = 0;
	ball -> a = (2.0f*M_PI * 360.0f / (double) (rand() % 360) );
	ball -> r = rand() % 50;
	ball -> speed = (rand() % 4) + 1;
	ball -> color =  8 << (rand() % 3);
}

int main()
{
	struct retroScreen *screen = NULL;
	struct RastPort scroll_rp;

	struct IntuiMessage *msg;
	bool running = true;

	retroRGB color;
	double p = 0;
	double start_sync;

	int n;
	#define balls 20

 	struct greate_ball_of_fire ball[balls];


	if (init())		// libs open her.
	{

		for (n=0;n<balls;n++)
		{
			init_ball( &ball[ n ] );
		}


		InitRastPort(&scroll_rp);
		scroll_rp.BitMap = AllocBitMapTags( 320, 200, 256, 
				BMATags_PixelFormat, PIXF_CLUT, 
				BMATags_Clear, true,
				BMATags_Displayable, false,
				TAG_END);

		scroll_rp.Font =  My_Window -> RPort -> Font;
		SetBPen( &scroll_rp, 0 );

		retroClearVideo(video);
		
		// start set rainbow
		video -> rainbow[0].color = 0;
		video -> rainbow[0].tableSize = 1000;
		video -> rainbow[0].table = (struct retroRGB *) AllocVecTags(sizeof(struct retroRGB)  * video -> rainbow[0].tableSize,  AVT_Type, MEMF_SHARED, TAG_END );
		// end set rainbow

		// start rainbow
		video -> rainbow[0].verticalOffset = 100;	
		video -> rainbow[0].height = 300;
		// end rainbow

		// start rain
		{
			struct retroRGB color;

			for (int scanline = 0; scanline < video -> rainbow[0].tableSize ; scanline ++ )
			{
				// sacnline to ECS color.

				color.r =(scanline & 0xF00) >> 4;
				color.g =(scanline & 0x0F0);
				color.b =(scanline & 0x00F) << 4;				

				video -> rainbow[0].table[scanline] = color;
			}
		}
		//  end rain

		screen = retroOpenScreen(320,200,retroLowres);

		if (screen)
		{
			int x;

			retroScreenColor( screen, 0, 255, 100, 50 );
			retroScreenColor( screen, 1, 255, 255, 255 );
			retroScreenColor( screen, 2, 0, 0, 0 );
			retroScreenColor( screen, 3, 255, 0, 0 );

			retroScreenColor( screen, 4, 255, 255, 255 );
			retroScreenColor( screen, 5, 255, 255, 255 );
			retroScreenColor( screen, 6, 0, 0, 0 );
			retroScreenColor( screen, 7, 255, 0, 0 );

			retroScreenColor( screen, 8, 255, 0, 0 );
			retroScreenColor( screen, 8 | 16, 255, 255, 0 );
			retroScreenColor( screen, 8 | 32, 255, 0, 255 );

			retroScreenColor( screen, 16, 0, 255, 0 );
			retroScreenColor( screen, 16 | 32, 0, 0, 255 );

			retroScreenColor( screen, 32, 0, 0, 255 );

			retroScreenColor( screen, 8 | 16 | 32, 255, 255, 255 );


//			retroCircle( screen, 50, 40, 25, 1 );
//			retroOrCircle( screen, 70, 50, 25, 2 );

			retroBoingOutline( screen,  50,  40,  25+4, 3, 1 );
			retroBoing( screen, 50, 40, 25, 1 );

			retroBoingOutline( screen,  295,  35,  10+3, 2, 1 );
			retroBoing( screen, 295, 35, 10, 1 );
		}

		if (screen)	retroApplyScreen( screen, video, 0, 0,320,200 );

		while (running)
		{
			while (msg = (IntuiMessage *) GetMsg( video -> window -> UserPort) )
			{
				if (msg -> Class == IDCMP_CLOSEWINDOW) running = false;
				ReplyMsg( (Message*) msg );
			}

			video -> rainbow[0].offset ++;

			scrolled_x+=scroll_speed;			
			
			if (scrolled_x>15)
			{
				SetAPen( &scroll_rp, 4 );
				Move( &scroll_rp, 300,10 );
				Text( &scroll_rp, scroll_text+scroll_char,1 );

				scroll_char = (scroll_char + 1) % strlen(scroll_text) ;	// next char

				scrolled_x = 0;
			}


			retroAndClear(screen, 0,0,screen->width,screen->height, ~(4+8+16+32));

			ScrollRaster( &scroll_rp, scroll_speed, 0, 0, 0, 320, 200);

			p = 0;
			{
				int x;
				double y;

				for (x=0;x<320;x++)
				{
					y = sin(p)*10.0f+20.0f;
					retroOrBitmapBlit( scroll_rp.BitMap, x,0,1,30, screen, x , y);
					p+=0.05f;
				}
			 }


			retroTriangle(screen,50,100,150,120,100,150,8);

			retroOrTriangle(screen,90,100,180,120,140,150,16);

			retroXorCircle(screen,110,100,30,16);

			start_sync += 0.1f;
			if (start_sync>2*M_PI) start_sync =0.0f;

			retroClearVideo( video );
			retroDrawVideo( video );

			retroDmaVideo(video);

			WaitTOF();
			BackFill_Func(NULL, NULL );
		}

		if (screen) retroCloseScreen(screen);

		if (scroll_rp.BitMap) FreeBitMap(scroll_rp.BitMap);

		retroFreeVideo(video);
	}


#ifdef amigaos4
	closedown();
#endif

	return 0;
}

