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

#include <stdlib.h>
#include <stdio.h>
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
		unsigned char *data0,
		unsigned char *data1
	 )
{
	scanline->pixels = pixels;
	scanline->mode = mode;
	scanline->data[0] = data0;
	scanline->data[1] = data1;
}

struct retroVideo * _retromode_retroAllocVideo(struct RetroModeIFace *Self,
       int width, int height)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroVideo *new_video ;
	struct retroRGB RGB000;

	RGB000.r = 0;
	RGB000.g = 0;
	RGB000.b = 0;


	libBase -> IDOS -> Printf("%s(%ld,%ld)\n",__FUNCTION__,width,height);

	new_video = (struct retroVideo *) libBase -> IExec -> AllocVecTags( sizeof(struct retroVideo),  
					AVT_Type, MEMF_SHARED, 
					AVT_ClearWithValue, 0 ,
					TAG_END	);

	if (new_video)
	{
		int scanline;
		int c;

		new_video -> width = width;
		new_video -> height = height;
		new_video -> BytesPerRow = width  * 4;		// size of ARGB

		new_video -> Memory = (unsigned int *)  libBase -> IExec -> AllocVecTags( new_video -> BytesPerRow * (new_video -> height  + 1), 
						AVT_Type, MEMF_SHARED, 
						AVT_ClearWithValue, 0 ,
						TAG_END	);

		new_video -> refreshAllScanlines = TRUE;
		new_video -> refreshSomeScanlines = FALSE;

		for ( scanline=0; scanline< new_video -> height ;scanline++) config_scanline( &new_video -> scanlines[scanline] , 0, NULL, NULL, NULL );

		for ( c = 0; c<3 ;c++) new_video -> rainbow[c].table = NULL;

		if ( ! new_video -> Memory) 
		{
			Self -> retroFreeVideo( new_video );
			return NULL;
		}
	}

	return  new_video;
}

