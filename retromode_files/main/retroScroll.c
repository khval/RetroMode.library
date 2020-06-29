/* :ts=4
 *  $VER: retroScroll.c $Revision$ (28-Jun-2020)
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

#define __USE_INLINE__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <exec/exec.h>
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/retromode.h>
#include <libraries/retromode.h>

/****** retromode/main/retroScroll ******************************************
*
*   NAME
*      retroScroll -- Description
*
*   SYNOPSIS
*      void retroScroll(struct retroScreen * screen, int buffer, int x0, 
*          int y0, int x1, int y1, int dx, int dy);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       buffer - 
*       x0 - 
*       y0 - 
*       x1 - 
*       y1 - 
*       dx - 
*       dy - 
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

#if 0
	#define set(adr,v) *(adr)=v
	#define get(adr) *(adr)
#else

	#define set(adr,v) if ((( (char *) adr)>=(set_min)) && (( (char *) adr)<=(set_max))) { *(adr)=(v); } else { Printf("set out of range, min %08lx value %08lx max %08lx - ypos %ld\n",set_min,adr,set_max, y); }
	#define get(adr) __get( ((( (char *) adr)>=(get_min)) &&(( (char *) adr)<=(get_max))) , adr, screen, y )

	#define set_min ((char *) mem)
	#define set_max ((char *) mem + (screen -> bytesPerRow * screen -> realHeight))
	#define get_min ((char *) mem)
	#define get_max ((char *) mem + (screen -> bytesPerRow * screen -> realHeight))
	#define get_error_code 0

	unsigned char __get( bool in_limit, unsigned char *mem, struct retroScreen *screen, int y )
	{
		if (in_limit)
		{
			return *mem;
		}
		else
		{
			Printf("Get out of range, min %08lx value %08lx max %08lx - ypos %ld\n",get_min,mem,get_max, y);
		}
	}

#endif

extern struct Interface			*INewlib;

void _retromode_retroScroll(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int buffer,
       int x0,
       int y0,
       int x1,
       int y1,
       int dx,
       int dy)
{
	int bytesPerRow = screen -> bytesPerRow;
	unsigned char *mem = screen -> Memory[ buffer ];
	unsigned char *src;
	unsigned char *des;
	unsigned char *row_src;
	unsigned char *row_dst;

	int x,y;

	Printf("************ %0l8x\n", INewlib);

	if (dy>0)
	{
		y1 -= dy;

		if (dx>0)
		{
			x1 -=dx;

			printf("%s:%d retro %d,%d to %d,%d\n",__FUNCTION__,__LINE__,x0,y0,x1,y1);

			row_src = mem + x1 + (bytesPerRow * (y1+dy)) ;	
			row_dst = mem + x1 + (bytesPerRow * y1) +dx ;

			for (y=y1;y>=y0;y--)
			{
				src = row_src;	des = row_dst;
				for (x=x1;x>=x0;x--)
				{
					set(des,get(src));
					des--; src--;
				}
				row_src-=bytesPerRow;
				row_dst-=bytesPerRow;
			}
		}
		else
		{
			x0 += -dx;

			printf("%s:%d retro %d,%d to %d,%d\n",__FUNCTION__,__LINE__,x0,y0,x1,y1);

			row_src = mem + x0 + (bytesPerRow * (y1+dy)) ;	
			row_dst = mem + x0 + (bytesPerRow * y1) +dx ;

			for (y=y1;y>=y0;y--)
			{
				src = row_src;	des = row_dst;
				for (x=x0;x<=x1;x++)
				{
					set(des,get(src));
					des++; src++;
				}
				row_src-=bytesPerRow;
				row_dst-=bytesPerRow;
			}
		}
	}
	else
	{
		y0 += -dy;

		if (dx>0)
		{
			x1 -=dx;

			row_src = mem + x1 + (bytesPerRow * (y0-dy)) ;	// dy is negative number... (-  and - is +)
			row_dst = mem + x1 + (bytesPerRow * y0) +dx ;

			for (y=y0;y<=y1;y++)
			{
				src = row_src;	des = row_dst;
				for (x=x1;x>=x0;x--)
				{
					set(des,get(src));
					des--; src--;
				}
				row_src+=bytesPerRow;
				row_dst+=bytesPerRow;
			}
		}
		else
		{
			x0 += -dx;								// dx is negative number... (-  and - is +)

			row_src = mem + x0 + (bytesPerRow * (y0-dy)) ;	// dy is negative number... (-  and - is +)
			row_dst = mem + x0 + (bytesPerRow * y0) +dx ;

			for (y=y0;y<=y1;y++)
			{
				src = row_src;	des = row_dst;
				for (x=x0;x<=x1;x++)
				{
					set(des,get(src));
					des++; src++;
				}
				row_src+=bytesPerRow;
				row_dst+=bytesPerRow;
			}
		}
	}
}

