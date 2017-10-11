/* :ts=4
 *  $VER: AfterEffectAdjustRGB.c $Revision$ (11-Oct-2017)
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

/****** retromode/main/AfterEffectAdjustRGB ******************************************
*
*   NAME
*      AfterEffectAdjustRGB -- Description
*
*   SYNOPSIS
*      void AfterEffectAdjustRGB(struct retroVideo * video, 
*          unsigned int red_shift, unsigned int green_shift, 
*          unsigned int blue_shift);
*
*   FUNCTION
*
*   INPUTS
*       video - 
*       red_shift - 
*       green_shift - 
*       blue_shift - 
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

void _retromode_AfterEffectAdjustRGB(struct retromodeIFace *Self,
       struct retroVideo * video,
       unsigned int red_shift,
       unsigned int green_shift,
       unsigned int blue_shift)
{
	int scanline=0;
	int x;
	unsigned int *data = NULL;
	unsigned int *src_mem = video -> Memory;
	unsigned int *des_ptr;
	unsigned int *src_ptr;
	int intsPerRow = video -> BytesPerRow / 4;


	for ( scanline = 0 ; scanline < video->height ; scanline++ )
	{
		src_ptr = src_mem + red_shift;

		if (red_shift>0)
		{
			for (x=0;x<video->width-red_shift;x++)
			{
				des_ptr=src_ptr-red_shift;

				*des_ptr= (*src_ptr & 0xFF0000) | (*des_ptr & 0xFF00FFFF );
				src_ptr++;
			}
		}

		src_ptr = src_mem + green_shift;

		if (green_shift>0)
		{
			for (x=0;x<video->width-green_shift;x++)
			{
				des_ptr=src_ptr-green_shift;

				*des_ptr= (*src_ptr & 0xFF00) | (*des_ptr & 0xFFFF00FF );
				src_ptr++;
			}
		}

		src_ptr = src_mem + blue_shift;

		if (blue_shift>0)
		{
			for (x=0;x<video->width-blue_shift;x++)
			{
				des_ptr=src_ptr-blue_shift;

				*des_ptr= (*src_ptr & 0xFF) | (*des_ptr & 0xFFFFFF00 );
				src_ptr++;
			}
		}


		src_mem += intsPerRow;	// next line
	}
}

