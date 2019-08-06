/* :ts=4
 *  $VER: retroGetBlock.c $Revision$ (07-May-2019)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2019 LiveForIt Software.
 *  MIT License.
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
#include "libBase.h"

/****** retromode/main/retroGetBlock ******************************************
*
*   NAME
*      retroGetBlock -- Description
*
*   SYNOPSIS
*      void retroGetBlock(struct retroScreen * screen, 
*          struct retroBlock * block, int x, int y);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       block - 
*       x - 
*       y - 
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

void _retromode_retroGetBlock(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	int buffer,
	struct retroBlock * block,
	int x,
	int y)
{
	int _x,_y;
	int sx,sy;
	unsigned char *mem = screen->Memory[screen -> double_buffer_draw_frame];
	unsigned char *sslice,*dslice;

	for (_y=0;_y<block->h;_y++)
	{
		sy = _y+y;

		if ((sy>=0)&&(sy<screen->realHeight))
		{
			sslice = mem + (screen->bytesPerRow*sy);
			dslice = block->mem + (block->w * _y);

			for (_x=0;_x<block->w;_x++)
			{
				sx = _x+x;
				if ((sx>=0)&&(sx<screen->realWidth))
				{
					dslice[_x]= sslice[sx];
				}
				else dslice[_x] = 0;
			}
		}
		else
		{
			dslice = block->mem + (block->w * _y);

			for (_x=0;_x<block->w;_x++)
			{
				dslice[_x]= 0;
			}
		}
	}
}

