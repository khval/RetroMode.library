/* :ts=4
 *  $VER: retroStar.c $Revision$ (26-Oct-2017)
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
#include <math.h>

/****** retromode/main/retroStar ******************************************
*
*   NAME
*      retroStar -- Description
*
*   SYNOPSIS
*      void retroStar(struct retroScreen * screen, int x, int y, int n, 
*          int r0, int r1, unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       n - 
*       r0 - 
*       r1 - 
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


void _retromode_retroTriangle(struct RetroModeIFace *Self,
       struct retroScreen * screen, int x1,int y1, int x2, int y2, int x3,int y3, unsigned char color);

#define get_xy( vector,  r ) \
	v ## vector ## x = (-cos(angel_ ## vector) * r) + x; \
	v ## vector ## y = (sin(angel_ ## vector) * r) + y; 
 
void _retromode_retroStar(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	int x,
	int y,
	int n,
	int r0,
	int r1,
	double g,
	unsigned char color)
{
	int foot;
	double g0, g1, g2;
	double g00,g11,g22;

	for (foot=0;foot<n*2; foot+=2)
	{
		g0 = M_PI * 2.0f * foot / (n*2);
		g1 = M_PI * 2.0f * (foot + 1) / (n*2);
		g2 = M_PI * 2.0f * (foot + 2) / (n*2);

		g00 = g0 +g;
		g11 = g1 +g;
		g22 = g2 +g;

		_retromode_retroTriangle(Self, screen,
				(sin(g00) * r0)+x,(cos(g00) * r0)+y,
				(sin(g11) * r1)+x,(cos(g11) * r1)+y,
				(sin(g22) * r0)+x,(cos(g22) * r0)+y,color);

		_retromode_retroTriangle(Self, screen,
				(sin(g00) * r0)+x,(cos(g00) * r0)+y,
				x,y,
				(sin(g22) * r0)+x,(cos(g22) * r0)+y,color);
	}
}

