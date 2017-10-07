/* :ts=4
 *  $VER: applyCopper.c $Revision$ (03-Oct-2017)
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


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>

/****** retromode/main/applyCopper ******************************************
*
*   NAME
*      applyCopper -- Description
*
*   SYNOPSIS
*      void applyCopper(struct retroVideo * video);
*
*   FUNCTION
*
*   INPUTS
*       video - 
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

void _retromode_applyCopper(struct retromodeIFace *Self,
       struct retroVideo * video)
{
	int scanline_start;
	int scanline_end;
	int scanline;
	int color;
	struct retroRGB *table;
	int table_height;
	int table_pos;
	int c;

	for ( c = 0; c < 3; c++)
	{
		if (video->rainbow[c].table)
		{
			color = video->rainbow[c].color;
			table = video->rainbow[c].table;
			table_height = video ->rainbow[c].tableSize;
			table_pos = video->rainbow[c].offset;

			scanline_start = video->rainbow[c].verticalOffset;
			scanline_end = scanline_start + video->rainbow[c].height;
			if (scanline_end > video -> height) scanline_end = video->height;

			for ( scanline = scanline_start ; scanline < scanline_end ; scanline++ )
			{
				if ((scanline>0)&&(video -> scanlines[scanline].data))
				{
					video -> scanlines[scanline].palette[color] = table[table_pos % table_height];
					table_pos++;
				}
			}
		}
	}
}

