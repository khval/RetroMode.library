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

#define setvec( line , xa,ya, xb,yb ) line.x0 = xa; line.y0 = ya; line.x1 = xb; line.y1 = yb 

struct _line_
{
	int x0,y0;
	int x1,y1;
};



BOOL line_above_and_below_line( int x, int y, int lines,  struct _line_ *ll )
{
	int n;
	double fx,fy;
	double dx,dy;
	double a;
	double _y;

	BOOL above = FALSE;
	BOOL below = FALSE;

	for (n=0;n<lines;n++)
	{
		if (((x>=ll->x0)&&(x<=ll->x1)) || ((x>=ll->x1)&&(x<=ll->x0)))
		{
			if (ll->x0<ll->x1)
			{
				fx = ll -> x0;
				fy = ll -> y0;
				dx = ll->x1-ll->x0+1.0f;
				dy = ll->y1-ll->y0;
			}
			else
			{
				fx = ll->x1;
				fy = ll->y1;
				dx = ll->x0-ll->x1+1.0f;
				dy = ll->y0-ll->y1;
			}

			dy+= (dy<0) ? -1.0f : 1.0f;

			a = dy/dx;

			_y = (a * (x-fx))+fy;	

			if (_y<y)	above = TRUE;
			if (_y>y) 	below = TRUE;
		}
		ll++;
	}

	return (above && !below) || (!above && below);
}


int find_x_on_line( int y, struct _line_ l )
{
	int ox,oy;	// org x,y
	int _x,_y;
	double dx,dy;
	double a;

	if (l.y0 == y) return l.x0;
	if (l.y1 == y) return l.x1;

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

	return _x + ox;
}


void _retromode_retroPixel(struct RetroModeIFace *Self,
       struct retroScreen * screen,  int x, int y, unsigned char color);


void _retromode_retroPolyGonArray(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	unsigned char color, 
	int array_size,
	int * array)
{
//	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;

	int lx =0, ly = 0;
	int s;
	int n;
	int x = 0,y = 0;
	int x0,x1;
	int lines;
	int max_y = INT_MIN;
	int min_y = INT_MAX;
	BOOL sorted;
	BOOL odd_even = FALSE;
	BOOL error = FALSE;

	struct _line_ *ll;
	struct _line_ l[100];
	struct _line_ *ss;
	struct _line_ *st[100];

	if (array_size>=3)
	{
		lx = array[0];
		ly = array[1];
		if (ly<min_y) min_y =ly;
		if (ly>max_y) max_y =ly;

		for (n=1;n<(array_size>>1);n++)
		{
			x = array[(n<<1)+0];
			y = array[(n<<1)+1];

			setvec( l[n-1] , lx,ly, x,y );

			if (y<min_y) min_y =y;
			if (y>max_y) max_y =y;

			lx = x; ly = y;
		}
	}
	else return;



	lines = (array_size >> 1) -1;

	for ( y=min_y; y<=max_y;y++)
	{
		s = 0;

		// find interesting vectors.
		for (n=0;n<lines;n++)
		{
			ll = l + n;
			if ( ll->y0 > ll->y1 )
			{
				if ((y >= ll->y1) && (y <= ll->y0)) st[s++] = l + n;
			}
			else
			{
				if ((y >= ll->y0) && (y <= ll->y1)) st[s++] = l + n;
			}
		}

		do
		{
			sorted = FALSE;
			// sort intersting vectors
			for (n=0;n<s-1;n++)
			{
				x0 = find_x_on_line( y, *st[n] );
				x1 = find_x_on_line( y, *st[n+1] );

				if (x0>x1)
				{
					ss = st[n]; st[n] = st[n+1]; st[n+1] = ss;
					sorted = TRUE;
				}
			}
		} while (sorted);

		if (s>1)
		{
			odd_even = TRUE;
			n = 0;

			error = FALSE;
			lx = -1;
			while (n<s-1)
			{
				x0 = find_x_on_line( y, *st[n] );
				x1 = find_x_on_line( y, *st[n+1] );

				if (error)
				{
					int px;
					error = FALSE;
					px = (x1 - x0)/2+x0;
					// make a guess whats correct.
					odd_even = ! line_above_and_below_line( px, y,  lines,  l );
				}

				if (x0 != x1) 
				{
					if (odd_even)
					{
						for (x=x0;x<=x1;x++)
						{
							_retromode_retroPixel( Self, screen, x, y, color ) ;
						}
					}
					odd_even = ! odd_even;
				}
				else
				{
					 error = TRUE;
				}

				n+=1;
			}						
		}
	}

}

