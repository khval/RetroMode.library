/* :ts=4
 *  $VER: retroDrawMask.c $Revision$ (08-Dec-2019)
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

/****** retromode/main/retroDrawMask ******************************************
*
*   NAME
*      retroDrawMask -- Description
*
*   SYNOPSIS
*      void retroDrawMask(struct retroScreen * screen, 
*          struct retroMask * mask, int x, int y);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       mask - 
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

void _retromode_drawShortPlanar ( struct RetroModeIFace *Self,struct retroScreen *screen, unsigned short ptr,int x, int y );

void _retromode_retroDrawMask(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       struct retroMask * mask,
       int x,
       int y)
{
	int xx,yy;
	unsigned short *ptr;

	for (yy=0;yy<mask->height;yy++)
	{
		for (xx=0;xx < mask->int16PerRow;xx++)
		{
			ptr = mask -> data + (yy*mask->int16PerRow) + xx;
			_retromode_retroDrawShortPlanar( Self, screen ,*ptr,xx*16+x,y);
		}
		y++;
	}
}

