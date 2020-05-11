/* :ts=4
 *  $VER: retroPolyGonArray.c $Revision$ (04-May-2018)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2018 LiveForIt Software.
 *  MIT License.
 *
 * $Id$
 *
 * $Log$
 *
 *
 */

#ifdef __win32__
#include "stdafx.h" 
#define BOOL bool
#define TRUE true
#define FALSE false
#endif 

#include <stdlib.h>
#include <stdio.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>
#include <limits.h>
#include "libBase.h"


#define experiment_sort

/****** retromode/main/retroPolyGonArray ******************************************
*
*   NAME
*      retroPolyGonArray -- Description
*
*   SYNOPSIS
*      void retroPolyGonArray(struct retroScreen * screen, 
*          unsigned char color, int * array);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       color - 
*       array - 
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

#define setvec(line,xa,ya,xb,yb) line.x0 = xa; line.y0 = ya; line.x1 = xb; line.y1 = yb; 

struct _line_
{
	int x0,y0;
	int x1,y1;
};

struct xpoint
{
	int	is_up;
	int	x;
	int	line;
};


BOOL find_x_on_line( int y, int lineNr, struct _line_ l , struct xpoint *retX )
{
	int ox,oy;	// org x,y
	int _x,_y;
	double dx,dy;
	double a;


	if (l.y0 < l.y1) {
		if ((y<l.y0) || (y>l.y1)) return FALSE;
		retX -> is_up = FALSE;
	} else {
		if ((y<l.y1) || (y>l.y0)) return FALSE;
		retX -> is_up = TRUE;
	}

	retX -> line = lineNr;

	if (l.y0 == y)
	{
		retX -> x = l.x0;
		return TRUE;
	}

	if (l.y1 == y)
	{
		retX -> x = l.x1;
		return TRUE;
	}


	// sort x and y;
	if (l.x0 > l.x1) { _x = l.x0;	_y = l.y0;	l.x0 = l.x1;l.y0 = l.y1; l.x1 = _x; l.y1 = _y; }

	ox = l.x0; oy = l.y0;		// keep orginal x,y
	_x = l.x0;	_y = l.y0;	

	y-=_y; l.x0-=_x; l.y0-=_y; l.x1-=_x;	l.y1-=_y;

	dy = l.y1 - l.y0;

	if (dy!=0.0f)
	{
		dy += (dy<0) ? -1 : 1;
		dx =l.x1+1;
		a = dx/dy;
		_x = ((double) y*a);
	}
	else
	{
		_x = 0;
	}

	retX -> x = _x + ox;
	return TRUE;
}


void _retromode_retroPixel(struct RetroModeIFace *Self,
       struct retroScreen * screen,  int x, int y, unsigned char color);


BOOL createLineArray(int lineCount, int * array, struct _line_ *l , int  *min_y, int *max_y )
{
	int lx =0, ly = 0;
	int n;
	int x = 0,y = 0;

	if (lineCount>=3)
	{
		lx = array[0];
		ly = array[1];
		if (ly<*min_y) *min_y =ly;
		if (ly>*max_y) *max_y =ly;

		for (n=1;n<=lineCount;n++)
		{
			x = array[(n<<1)+0];
			y = array[(n<<1)+1];

			setvec( l[n-1] , lx,ly, x,y );

//			libBase -> IDOS -> Printf("line %ld,%ld to %ld,%ld\n" , lx,ly, x,y );

			if (y<*min_y) *min_y =y;
			if (y>*max_y) *max_y =y;

			lx = x; ly = y;
		}

		return TRUE;
	}

	return FALSE;
}

void getXPoints(int y, struct _line_ *lines, int linesCount, struct xpoint *pointsX, int *pointsXCount)
{

	int n;
	struct xpoint t;
	struct _line_ *line;
	struct _line_ *lines_start = lines;
	struct _line_ *lines_end = lines + linesCount;
	int sorted_at = -1;

	*pointsXCount = 0;

//	for (line = lines_start; line < lines_end; line++)

	for (n=0;n<linesCount;n++)
	{
		if (find_x_on_line(y,n, lines[n], &pointsX[*pointsXCount]) == TRUE) (*pointsXCount)++;
	}

	if (*pointsXCount == 0) return;

#ifndef experiment_sort

	do
	{
		sorted = FALSE;

		for (n=0;n<(*pointsXCount)-1;n++)
		{
			if (pointsX[n].x > pointsX[n+1].x)
			{
				t= pointsX[n];
				pointsX[n]=pointsX[n+1];
				pointsX[n+1]=t;
				sorted = TRUE;
			}
		}	
	} while (sorted);
	printf("sorted\n");

#endif


#ifdef experiment_sort

	for (n = 0; n<(*pointsXCount) - 1; n++)
	{
		if (pointsX[n].x > pointsX[n + 1].x)
		{
			t = pointsX[n];
			pointsX[n] = pointsX[n + 1];
			pointsX[n + 1] = t;
			if (n)
			{
				n -= 2;
			}
			else if (sorted_at > -1)
			{
				n = sorted_at;
				sorted_at = -1;
			}
			else sorted_at = n;
		}
		else
		{
			if (sorted_at > -1) n = sorted_at;
			sorted_at = -1;
		}
	}


#endif

}


void _retromode_retroPolyGonArray(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	unsigned char color, 
	int array_size,
	int * array)
{
	struct _line_ lines[100];
	int linesCount;
	int pointsCount;	// points in array
	int pointsXCount;	// x array count for that line.
	struct xpoint pointsX[40];
	int x,y;
	int xp;
	int max_y = INT_MIN;
	int min_y = INT_MAX;
	int sum;
	int draw_pixel;
	int deltaLine;

	pointsCount = (array_size / sizeof(int)) >> 1;
	linesCount =pointsCount-1;

	IDOS -> Printf("linesCount %ld\n",linesCount);

	if (createLineArray(linesCount, array, lines, &min_y, &max_y) == FALSE) return;

	for (y=min_y;y<=max_y;y++)
	{
		getXPoints( y, lines, linesCount, pointsX, &pointsXCount );

		xp = 1;
		draw_pixel = 1;
		
		if (pointsXCount>0)
		{
			for (x = pointsX[0].x; x <= pointsX[pointsXCount-1].x ; x++ )
			{
				if (x == pointsX[xp].x)
				{
					sum = 0;
					do
					{
						deltaLine = abs( pointsX[xp-1].line - pointsX[xp].line );
						if ( ! ((pointsX[xp - 1].is_up == pointsX[xp].is_up) && ((deltaLine == 1)||(deltaLine == linesCount-1))) )  sum += pointsX[xp].is_up ? 1 : -1;

						xp++;
						if (xp==pointsXCount) break;
					}
					while (x == pointsX[xp].x );

					if (sum != 0)	draw_pixel ^= 1;
				}

				if (draw_pixel) _retromode_retroPixel( Self, screen, x, y, color ) ;
			}
		}
	}	
}

