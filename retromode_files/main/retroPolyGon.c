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

//#include <libbase.h>

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

extern void _retromode_retroPolyGonArray(struct RetroModeIFace *Self,
	struct retroScreen * screen,
	unsigned char color, 
	int array_size,
	int * array);


void _retromode_retroPolyGon(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       unsigned char color,
        ...)
{
	va_list list;
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	int array_size;
	int array[100*2];

	va_start(list, color );

	{
		int x,n;

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
			array[n] = va_arg(list,int);
			n++;
		} while (( array[n-1] != retroEnd) ) ;
		va_end(list);

		array_size = n-1;
	}

	_retromode_retroPolyGonArray(Self, screen, color, array_size, array);

}

