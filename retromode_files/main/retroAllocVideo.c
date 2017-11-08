/* :ts=4
 *  $VER: alloc_retoVideo.c $Revision$ (03-Oct-2017)
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

/****** retromode/main/retroAllocVideo ******************************************
*
*   NAME
*      retroAllocVideo -- Description
*
*   SYNOPSIS
*      struct retroVideo * retroAllocVideo(struct Window * window);
*
*   FUNCTION
*
*   INPUTS
*       window - 
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

void config_scanline( 
		struct retroScanline *scanline,
		int pixels,
		void (*mode) ( struct retroScanline *line, int beamY, unsigned int *video_buffer  ),
		unsigned char *data
	 )
{
	scanline->pixels = pixels;
	scanline->mode = mode;
	scanline->data = data;
}


struct retroVideo * _retromode_retroAllocVideo(struct RetroModeIFace *Self,
       struct Window * window)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroVideo *new_video ;
	struct retroRGB RGB000;

	RGB000.r = 0;
	RGB000.g = 0;
	RGB000.b = 0;

	if (libBase -> IGraphics == NULL) return NULL;

	new_video = (struct retroVideo *) libBase -> IExec -> AllocVecTags( sizeof(struct retroVideo),  
					AVT_Type, MEMF_SHARED, 
					AVT_ClearWithValue, 0 ,
					TAG_END	);

	if (new_video)
	{
		int scanline;
		int c;

		new_video -> refreshAllScanlines = TRUE;
		new_video -> refreshSomeScanlines = FALSE;

		new_video -> width = 640;
		new_video -> height = 480;

		for ( scanline=0; scanline< new_video -> height ;scanline++) config_scanline( &new_video -> scanlines[scanline] , 0, NULL, NULL );

		for ( c = 0; c<3 ;c++) new_video -> rainbow[c].table = NULL;

		libBase -> IGraphics -> InitRastPort(&new_video->rp);

		new_video -> window = window;

		new_video -> rp.BitMap = libBase -> IGraphics -> AllocBitMap( new_video -> width , new_video -> height, 32, BMF_DISPLAYABLE, new_video -> window ->RPort -> BitMap);

		if (new_video -> rp.BitMap )
		{
			// We create a mirror copy in system ram.
			new_video -> BytesPerRow = libBase -> IGraphics -> GetBitMapAttr( new_video -> rp.BitMap, BMA_WIDTH)  * 4;
			new_video -> Memory = (unsigned int *)  libBase -> IExec -> AllocVecTags( new_video -> BytesPerRow * (new_video -> height  + 1), 
						AVT_Type, MEMF_SHARED, 
						AVT_ClearWithValue, 0 ,
						TAG_END	);
		}

		// if no success.

		if (( ! new_video -> Memory) || ( ! new_video -> rp.BitMap ))
		{
			Self -> retroFreeVideo( new_video );
			return NULL;
		}
	}

	return  new_video;
}

