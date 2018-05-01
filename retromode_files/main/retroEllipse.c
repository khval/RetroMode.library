/* :ts=4
 *  $VER: retroEllipse.c $Revision$ (12-Nov-2017)
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

/****** retromode/main/retroEllipse ******************************************
*
*   NAME
*      retroEllipse -- Description
*
*   SYNOPSIS
*      void retroEllipse(struct retroScreen * screen, int x, int y, int r1, 
*          int r2, double angel, unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       r1 - 
*       r2 - 
*       angel - 
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

void _retromode_retroEllipse(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x,
       int y,
       int r1,
       int r2,
       double angel,
       unsigned char color)
{
	double vxx, vxy, vyx, vyy;
	double tx, ty;
	double a;
	double ia;
	double xx,yy, lx,ly;

	// setup vector axes

	vxx = cos( angel ) ;
	vxy = -sin( angel ) ;

	vyx = cos( angel + (M_PI/2) ) ;
	vyy = -sin( angel + (M_PI/2) ) ;

	ia = (M_PI/16.0f);

	a = 0;

	tx = cos( a ) * r1;
	ty = -sin( a ) * r2;

	lx = (vxx * tx) + (vyx * ty) + x;
	ly = (vxy * tx) + (vyy * ty) + y;

	for (a=ia;a<=(M_PI*2.0f)+ia;a+=ia)
	{
		tx = cos( a ) * r1;
		ty = -sin( a ) * r2;

		xx = (vxx * tx) + (vyx * ty) + x;
		yy = (vxy * tx) + (vyy * ty) + y;

		Self->retroLine( screen, lx, ly, xx, yy, color );

		lx = xx;
		ly = yy;

	//	color++;
	}
}

