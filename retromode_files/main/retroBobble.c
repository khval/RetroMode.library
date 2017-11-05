/* :ts=4
 *  $VER: retroBobble.c $Revision$ (05-Nov-2017)
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

/****** retromode/main/retroBobble ******************************************
*
*   NAME
*      retroBobble -- Description
*
*   SYNOPSIS
*      void retroBobble(struct retroScreen * fromScreen, int fromX, int fromY, 
*          int fromR,struct retroScreen *toScreen,  int toX, int toY, int toR);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       fromX - 
*       fromY - 
*       fromR - 
*       screen - 
*       toX - 
*       toY - 
*       toR - 
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

unsigned char _retromode_retroPoint(struct RetroModeIFace *Self,  struct retroScreen * screen, int x, int y);
void _retromode_retroPixel(struct RetroModeIFace *Self, struct retroScreen * screen, int x, int y, unsigned char color);

void _retromode_retroBobble(struct RetroModeIFace *Self,
       struct retroScreen * fromScreen,
       int fromX,
       int fromY,
       int fromR,
       struct retroScreen * toScreen,
       int toX,
       int toY,
       int toR)
{
	int sourceX, sourceY;
	int destinationX, destinationY;
	char color;
	double r;

	for (destinationY = 0 ; destinationY < toR ; destinationY++)
	{
		sourceY = sin( (M_PI/2.0f) * destinationY / toR ) * fromR;

		for (destinationX = 0 ; destinationX < toR ; destinationX++)
		{
			r = sqrt( (destinationX*destinationX) + (destinationY*destinationY));

			if (r<toR)
			{
				sourceX = sin( (M_PI/2.0f) * destinationX / toR ) * fromR ;

				color = _retromode_retroPoint( Self, fromScreen, sourceX + fromX, -sourceY + fromY ) ;
				_retromode_retroPixel( Self, toScreen, destinationX + toX, -destinationY +toY, color);

				color = _retromode_retroPoint( Self, fromScreen, sourceX + fromX, sourceY + fromY );
				_retromode_retroPixel( Self, toScreen, destinationX + toX, destinationY +toY, color);

				color = _retromode_retroPoint( Self, fromScreen, -sourceX + fromX, -sourceY + fromY ) ;
				_retromode_retroPixel( Self, toScreen, -destinationX + toX, -destinationY +toY, color);

				color = _retromode_retroPoint( Self, fromScreen, -sourceX + fromX, sourceY + fromY ) ;
				_retromode_retroPixel( Self, toScreen, -destinationX + toX, destinationY +toY, color);

			}
		}
	}
}

