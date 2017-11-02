/* :ts=4
 *  $VER: retroPolyGon.c $Revision$ (27-Oct-2017)
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

/****** retromode/main/retroPolyGon ******************************************
*
*   NAME
*      retroPolyGon -- Description
*
*   SYNOPSIS
*      void retroPolyGon(struct retroScreen * screen, unsigned char color, 
*           ...);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       color - 
*       ... - 
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

void _retromode_retroPolyGon(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       unsigned char color,
        ...)
{
	va_list list;
//	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;

	int lx =0, ly = 0;
	int s;
	int n;
	int x = 0,y = 0;
	int x0,x1;
	int lines;
	int max_y;
	int min_y;
	BOOL sorted;
	BOOL odd_even = FALSE;
	BOOL error = FALSE;

	struct _line_ *ll;
	struct _line_ l[100];
	struct _line_ *ss;
	struct _line_ *st[100];

	min_y = screen -> realHeight;
	max_y = 0;

	lines = 0;
	va_start(list, color );

	// --- START BUG fix
	x = va_arg(list,int);
	for (n=0;n<2;n++)
	{
		x = va_arg(list,int);
		x = va_arg(list,int);
	}
	// --- END BUG fix


	n = 0;
	do
	{
		if (n&1)
		{
			ly = y;
			y = va_arg(list,int);

			if (y>max_y) max_y = y;
			if (y<min_y) min_y = y;
		}
		else
		{
			lx = x;
			x = va_arg(list,int);
		}

		if ((n&1)&&(n>2))		// two points equals 4 axes (0 to 3)
		{
			setvec( l[ lines ],  lx,ly,  x,y );
			lines ++;
		}

		n++;
	} while (( x != retroEnd) && ( y != retroEnd) ) ;
	va_end(list);

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

