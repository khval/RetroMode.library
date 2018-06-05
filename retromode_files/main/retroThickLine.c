/* :ts=4
 *  $VER: retroThickLine.c $Revision$ (04-Nov-2017)
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


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>
#include <math.h>

/****** retromode/main/retroThickLine ******************************************
*
*   NAME
*      retroThickLine -- Description
*
*   SYNOPSIS
*      void retroThickLine(struct retroScreen * screen, int x0, int y0, 
*          int x1, int y1, int thickness, unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x0 - 
*       y0 - 
*       x1 - 
*       y1 - 
*       thickness - 
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
       struct retroScreen * screen, int x1,int y1, int x2, int y2, int x3, int y3, unsigned char color);

void _retromode_retroThickLine(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x0,
       int y0,
       int x1,
       int y1,
       int thickness,
       unsigned char color)
{
	int xa,ya;
	int xb,yb;

	double a ;
	double dx,dy;
	double z;

	dx = (x0<x1) ? x1-x0+1 : x0-x1+1 ;
	dy = (y0<y1) ? y1-y0+1 : y0-y1+1 ;

	thickness /= 2;

	z = sqrt( (dx*dx) + (dy*dy) );
	if (z>0)
	{
		a = asin( dy / z );
		if (y1<y0) { if (x1>x0) { a+=M_PI; } else { a =  -a; } }	else	{ if (x1>x0) a = M_PI-a;	}
	}

	a= (M_PI / 2.0f);

	ya = sin(a) * thickness;
	xa = -cos(a) * thickness;

	yb = sin(a + M_PI) * thickness;
	xb = -cos(a + M_PI) * thickness;

	_retromode_retroTriangle( Self, screen,  
			x0 + xa ,y0 + ya, 
			x0 + xb ,y0 + yb, 
			x1 + xb ,y1 + yb , 
			color );

	_retromode_retroTriangle( Self, screen,  
			x1 + xa ,y1 + ya, 
			x1 + xb ,y1 + yb, 
			x0 + xa ,y0 + ya , 
			color );
}

