#include <stdio.h>
#include <stdlib.h>

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
struct retroModeIFace *IRetroMode = NULL;

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

#ifdef amigaos4

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

	if ( (video = alloc_retoVideo( My_Window )) == NULL ) return false;

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

#else

bool open_window(int window_width, int window_height)
{
	My_Window = (struct Window *) malloc(sizeof(struct Window));

	if (My_Window)
	{
		My_Window->RPort = (struct RastPort *) malloc(sizeof(struct RastPort));

		if (My_Window->RPort)
		{
			My_Window->RPort->BitMap = (struct BitMap *) malloc(sizeof(struct BitMap));
			My_Window->RPort->BitMap->BytePerRow = window_width * 4;
			My_Window->RPort->BitMap->height = window_height;
			My_Window->RPort->BitMap->width = window_width;
		}
	}

	return (My_Window != NULL);
}


bool init(){ 

	if (!open_window(640, 480)) return false;
	if ((video = alloc_retoVideo(My_Window)) == NULL) return false;
	return true; 
};

#endif

int main()
{
	struct retroScreen *screen = NULL;

	struct IntuiMessage *msg;
	bool running = true;

	retroRGB color;

	if (init())
	{


		for (int n= 0; n< 100; n++)
		{
			screen = retroOpenScreen(320,200);
			if (screen) retroCloseScreen(screen);
		}


		free_retroVideo(video);
	}

#ifdef amigaos4
	closedown();
#endif

	return 0;
}

