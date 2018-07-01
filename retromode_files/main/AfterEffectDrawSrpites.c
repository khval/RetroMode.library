/* :ts=4
 *  $VER: afterEffectDrawSrpites.c $Revision$ (12-Nov-2017)
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

#include <stdlib.h>
#include <stdio.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>
#include <libbase.h>

/****** retromode/main/afterEffectDrawSrpites ******************************************
*
*   NAME
*      afterEffectDrawSrpites -- Description
*
*   SYNOPSIS
*      void afterEffectDrawSrpites(struct retroVideo * video);
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

void retroDrawSprite(struct RetroLibrary *libBase,
       struct retroVideo *video,
       struct retroFrameHeader *frame,
       int x,
       int y)
{
	struct retroRGB *rgb;
	int width;
	int height;
	int ypos;
	int color;
	int scanlinenumber;
	int source_x0 = 0,source_y0 = 0;
	unsigned int *destination_row_ptr;
	unsigned int *destination_row_ptr2;
	unsigned int *destination_row_start;
	char *source_row_start;
	char *source_row_ptr;
	char *source_row_end ;

	width = frame -> Width;
	height = frame -> Height;

	x -= frame -> XHotSpot;
	y -= frame -> YHotSpot;	

	if ((y+height)*2>video->height) height = (video->height - (y*2)) / 2;
	if ((x+width)*2>video->width) width = (video->width - (x*2))/2;

	// insanity check

	if ( width > frame -> Width ) return ;
	if ( height > frame -> Height ) return ;

	// things look ok.

	if (y<0) { source_y0 = -y * 4; y = 0;   }
	if (x<0) { source_x0 = -x; x = 0; width-= source_x0; }

	destination_row_start = video -> Memory + ((video -> BytesPerRow/4) * (y*2)) + (x*2);

	source_row_start = frame -> data + (source_y0 * frame -> bytesPerRow  ) + source_x0;
	source_row_end = source_row_start + width;

	scanlinenumber = y * 2;

	for ( ypos = source_y0; ypos < height; ypos++ )
	{
		destination_row_ptr = destination_row_start;
		destination_row_ptr2 = destination_row_start + (video -> BytesPerRow / 4);

		if (scanlinenumber<video -> height-1)
		{
			for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
			{
				if (video -> scanlines[ scanlinenumber ].mode)
				{
					color = *source_row_ptr;

					if (color>0)
					{
						rgb = &(video -> scanlines[ scanlinenumber ].orgPalette[ color ]);
						color = (rgb->r << 16) | (rgb->g << 8 | rgb -> b) ;

						*destination_row_ptr++ = color;
						*destination_row_ptr++ = color;

						*destination_row_ptr2++ = color;
						*destination_row_ptr2++ = color;
					}
					else 
					{
						destination_row_ptr+=2;
						destination_row_ptr2+=2;
					}
				}
			}

		}

		destination_row_start += video -> BytesPerRow / 2 ;	// lowres 
		source_row_start += frame -> bytesPerRow;
		source_row_end += frame -> bytesPerRow;
		scanlinenumber +=2;
	}
}


void _retromode_AfterEffectDrawSrpites(struct RetroModeIFace *Self,
       struct retroVideo * video)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroSpriteObject *sprite;

	for ( sprite= video -> sprites; sprite < video -> sprites_end ; sprite ++ )
	{
		if (sprite -> frame)
		{
		
//			libBase -> IDOS->Printf("sprite x %ld, y %ld\n", sprite->x, sprite->y);
//			libBase -> IDOS->Printf("frame %lx\n", sprite -> frame);

			retroDrawSprite( libBase, video, sprite -> frame, sprite->x, sprite->y);
		}
	}
}

