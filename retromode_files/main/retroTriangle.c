/* :ts=4
 *  $VER: retroTriangle.c $Revision$ (14-Oct-2017)
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

/****** retromode/main/retroTriangle ******************************************
*
*   NAME
*      retroTriangle -- Description
*
*   SYNOPSIS
*      void retroTriangle(struct retroScreen * screen, int x1, int y1, 
*          int x2, int y2, int x3, int y3, unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x1 - 
*       y1 - 
*       x2 - 
*       y2 - 
*       x3 - 
*       y3 - 
*       color - 
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

void draw_hline_fast(struct retroScreen *screen, unsigned char *scr_mem, int x,int y,int w, unsigned char c)
{
	unsigned char *memory;
	int x2 = x + w ;

	if (y<0) return;
	if (y > screen->realHeight-1) return ;

	if (x<0) x = 0;
	if (x2>screen -> realWidth-1) x2 = screen->realWidth-1;

	memory = scr_mem + (screen -> realWidth * y);
	for ( ; x <= x2 ; x++) memory[x] = c;
}


void _retromode_retroTriangle(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x1,
       int y1,
       int x2,
       int y2,
       int x3,
       int y3,
       unsigned char color)
{
	unsigned char *src_mem = screen -> Memory[ screen -> double_buffer_draw_frame ];
	struct p p1 = {x1,y1};
	struct p p2 = {x2,y2};
	struct p p3 = {x3,y3};
	struct p *temp;
	struct p *pt[3] ;
	int xa,xb,y;
	int dxa,dya;
	int dxb,dyb;
	int my1,my3;

	pt[2] = &p3;

	// sort first two
	if (y1 < y2)  { pt[0] = &p1;  pt[1] = &p2; } else { pt[1] = &p1;  pt[0] = &p2; } 

	// sort 2en and 3rd
	if ( pt[1] -> y > pt[2] -> y ) {	temp = pt[2];	pt[2] = pt[1];	pt[1] = temp; 	};

	// sort 1en and 2rd
	if ( pt[0] -> y > pt[1] -> y ) {	temp = pt[1];	pt[1] = pt[0];	pt[0] = temp; 	};

	// we are done whit pointers!!!
	x1 = pt[0] -> x;	y1 = pt[0] -> y;
	x2 = pt[1] -> x;	y2 = pt[1] -> y;
	x3 = pt[2] -> x;	y3 = pt[2] -> y;

	xa = x1;
	xb = x1;

	dyb = y2 - y1+1;
	dxb = x2 - x1;
	dxb = (dxb >= 0 ? dxb+1 : dxb-1 );

	dya = y3 - y1+1;
	dxa = x3 - x1;
	dxa = (dxa >= 0 ? dxa+1 : dxa-1 );

	my1 = y1>=0 ? y1 : 0;
	my3 = y3>=0 ? y3 : 0;

	my1 = my1< 640 ? my1 : 480-1;
	my3 = my3< 640 ? my3 : 480-1;


	if (y1<y3)
	for (y= my1 ; y<= my3;y++)
	{

		xa = (dya ? (y-y1) *  dxa / dya : dxa) + x1;

		if (y<y2)
		{
			xb = (dyb ? (y-y1) *  dxb / dyb : dxb) + x1;
		}
		else 	if (y==y2)
		{
			dxb = x3 - x2;
			dxb = (dxb >= 0 ? dxb+1 : dxb-1 );
			dyb = y3 - y2+1;
			xb = x2;
		}
		else
		{
			xb = (dyb ? (y-y2) *  dxb / dyb : dxb) + x2;
		}

		draw_hline_fast(screen,src_mem,xa,y,xb-xa,color);
		draw_hline_fast(screen,src_mem,xb,y,xa-xb,color);
	}
}

