/* :ts=4
 *  $VER: AfterEffectScanline.c $Revision$ (11-Oct-2017)
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

/****** retromode/main/AfterEffectScanline ******************************************
*
*   NAME
*      AfterEffectScanline -- Description
*
*   SYNOPSIS
*      void AfterEffectScanline(struct retroVideo * video);
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

void _retromode_AfterEffectScanline(struct RetroModeIFace *Self,
       struct retroVideo * video)
{
	int ybeam=0;
	int display_frame = 0;
	int x;
	unsigned int *src_mem = video -> Memory;
	unsigned int *des_ptr;
	unsigned int *src_ptr;
	int intsPerRow = video -> BytesPerRow / 4;
	struct retroScanline *scanlineLast;
	struct retroScanline *scanline;
//	BOOL copied ;

	src_mem += intsPerRow;	// next line

	for ( ybeam = 1 ; ybeam < video->height ; ybeam++ )
	{
		scanlineLast = &video -> scanlines[ybeam-1];
		scanline = &video -> scanlines[ybeam];

//		copied = FALSE;
		if (scanlineLast -> screen)
		{
			if (scanline -> data[0] == NULL)
			{
				if (scanlineLast -> screen -> Memory[1]) 
				{
					display_frame = 1 - scanlineLast -> screen -> double_buffer_draw_frame;
				}
				else display_frame = 0;

				if ( scanlineLast -> data[ display_frame ] )		// if pre line has some data.
				{
					src_ptr = src_mem- intsPerRow;
					des_ptr = src_mem ;

					for (x=0;x<video->width;x++)
					{
						*des_ptr++= *src_ptr++ | 0xFF110011;

//						*des_ptr++=*src_ptr++;
//						*des_ptr++=(((*src_ptr++) & 0xFCFCFC) >> 1) | 0xFF000000;
					}

				}
			}
		}


		src_mem += intsPerRow;	// next line
	}
}

