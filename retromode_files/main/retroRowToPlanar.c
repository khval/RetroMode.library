/* :ts=4
 *  $VER: retroRowToPlanar.c $Revision$ (04-Jan-2020)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2020 LiveForIt Software.
 *  MIT License..
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

/****** retromode/main/retroRowToPlanar ******************************************
*
*   NAME
*      retroRowToPlanar -- Description
*
*   SYNOPSIS
*      void retroRowToPlanar(unsigned char mask, unsigned char * chunky, 
*          unsigned short * planar16b, int planarXSize, int width);
*
*   FUNCTION
*
*   INPUTS
*       mask - 
*       chunky - 
*       planar16b - 
*       planarXSize - 
*       width - 
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

void _retromode_retroRowToPlanar(struct RetroModeIFace *Self,
       unsigned char mask,
       unsigned char * chunky,
       unsigned short * planar16b,
       int planarXSize,
       int width)
{
	int w;
	int x,n;
	unsigned short data;

	x = 0;
	data =0;
	for (w=0;w<planarXSize;w++)
	{
		data = 0;		

		for (n=0;n<16;n++)
		{
			if (x<width)
			{
				data = (data <<1) | ((*chunky & mask) ? 1: 0);
				chunky++;	// next
			}
			x++;
		}

		*planar16b = data;
		planar16b++;	// next
	}
}

