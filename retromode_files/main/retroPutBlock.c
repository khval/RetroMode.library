/* :ts=4
 *  $VER: retroPutBlock.c $Revision$ (07-Aug-2019)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2019 LiveForIt Software.
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
#include "libBase.h"

/****** retromode/main/retroPutBlock ******************************************
*
*   NAME
*      retroPutBlock -- Description
*
*   SYNOPSIS
*      void retroPutBlock(struct retroScreen * screen, int buffer, 
*          struct retroBlock * block, int x, int y, unsigned char bitmask);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       buffer - 
*       block - 
*       x - 
*       y - 
*       bitmask - 
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

void putRowBlock(int w, unsigned char *sslice,unsigned char *dslice, int x, int sw, unsigned char bitmask)
{
	int _x, dx; 

	for (_x=0;_x<w;_x++)
	{
		dx = _x+x;
		if ((dx>=0)&&(dx<sw))
		{
			dslice[dx]= sslice[_x] & bitmask;
		}
	}
}

void putRowBlockMask(int w, unsigned char *sslice,unsigned char *dslice, int x,int sw, unsigned char bitmask)
{
	int _x, dx; 

	for (_x=0;_x<w;_x++)
	{
		dx = _x+x;
		if ((dx>=0)&&(dx<sw))
		{
			if (sslice[_x]) dslice[dx]= sslice[_x] & bitmask;
		}
	}
}

void _retromode_retroPutBlock(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int buffer,
       struct retroBlock * block,
       int x,
       int y,
       unsigned char bitmask)
{
	unsigned char *sslice,*dslice;
	int _y,dy;

	void (*putRow) (int w, unsigned char *sslice,unsigned char *dslice, int x, int sw, unsigned char bitmask);

	if (block->mask)
	{
		putRow = putRowBlockMask;
	}
	else
	{
		putRow = putRowBlock;
	}

	if ( block -> flag & flag_block_vrev )
	{
		for (_y=block->h-1;_y>=0;_y--)
		{
			dy = _y+y;
			if ((dy>=0)&&(dy<screen->realHeight))
			{
				dslice = screen->Memory[ buffer ] + (screen->bytesPerRow*dy);
				sslice = block->mem + (block->w * _y);
				putRow( block -> w, sslice,dslice, x, screen -> realWidth, bitmask);
			}
		}
	}
	else
	{
		for (_y=0;_y<block->h;_y++)
		{
			dy = _y+y;
			if ((dy>=0)&&(dy<screen->realHeight))
			{
				dslice = screen->Memory[ buffer ] + (screen->bytesPerRow*dy);
				sslice = block->mem + (block->w * _y);
				putRow( block -> w, sslice,dslice, x, screen -> realWidth, bitmask);
			}
		}
	}
}

