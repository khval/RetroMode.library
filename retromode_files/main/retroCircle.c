/* :ts=4
 *  $VER: retroCircle.c $Revision$ (11-Oct-2017)
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
#include <math.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>
#include <math.h>

#include "libbase.h"

/****** retromode/main/retroCircle ******************************************
*
*   NAME
*      retroCircle -- Description
*
*   SYNOPSIS
*      void retroCircle(struct retroScreen * screen, int x, int y, int r, 
*          unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       r - 
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

void _retromode_retroPixel(struct RetroModeIFace *Self, struct retroScreen * screen, int x, int y, unsigned char color);

#define putpixel(x,y) _retromode_retroPixel(Self, screen, x,y, color)

void _retromode_retroCircle(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x0,
       int y0,
       int radius,
       unsigned char color)
{

   int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        putpixel(x0 + x, y0 + y);
        putpixel(x0 + y, y0 + x);
        putpixel(x0 - y, y0 + x);
        putpixel(x0 - x, y0 + y);
        putpixel(x0 - x, y0 - y);
        putpixel(x0 - y, y0 - x);
        putpixel(x0 + y, y0 - x);
        putpixel(x0 + x, y0 - y);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }

}

