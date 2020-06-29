/* :ts=4
 *  $VER: retroSaveSprite.c $Revision$ (04-Jan-2020)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2020 LiveForIt Software.
 *  MIT License..
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
#include <stdint.h>
#include "libBase.h"

/****** retromode/main/retroSaveSprite ******************************************
*
*   NAME
*      retroSaveSprite -- Description
*
*   SYNOPSIS
*      void retroSaveSprite(void * fd, 
*          cust_fread_t cust_fread);
*
*   FUNCTION
*
*   INPUTS
*       fd - 
*       cust_fread - 
*
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

extern int _retromode_retroGetMaxPlains( struct RetroModeIFace *Self, 
	int colorMax);

extern void _retromode_retroRowToPlanar(struct RetroModeIFace *Self, 
	unsigned char mask, unsigned char * chunky, unsigned short * planar16b, int planarXSize, int width);

ULONG getMaxColors(struct retroFrameHeader *frame)
{
	int x,y;
	int maxColor =0;
	unsigned char *row = (unsigned char *) frame -> data;

	for (y=0;y<frame->height;y++)
	{
		for (x=0;x<frame->bytesPerRow;x++)
		{
			if (maxColor < row[x]) maxColor = row[x];			
		}
		row += frame->bytesPerRow;
	}

	return maxColor;
}

void _retromode_retroSaveSprite(struct RetroModeIFace *Self,
	void * fd,
	struct retroSprite *sprite,
	cust_fread_t cust_fwrite )
{
	struct retroFrameHeader *frame;
	unsigned short *planar16bRow = NULL;
	unsigned char *chunkyRow = NULL;
	int colorMax = 0;
	int colors = 0;
	int image;
	int p;
	unsigned char mask;
	int y;
	int num;
	uint16_t ECSColor;

	cust_fwrite( &sprite->number_of_frames,sizeof(sprite->number_of_frames), 1, fd );

	for (image =0 ; image < sprite->number_of_frames ; image++ )
	{
		colorMax = 0;
		frame = &sprite -> frames[ image ];

		colorMax = getMaxColors( frame );
		if (colors < colorMax) colors = colorMax;	// get largest number of colors.

		sprite->frames[ image ].numberOfPlains = _retromode_retroGetMaxPlains( Self, colorMax);

		cust_fwrite( frame, sizeof(struct retroFrameHeaderShort), 1, fd );

		if (planar16bRow) FreeVec(planar16bRow);
		planar16bRow = (unsigned short *) AllocVecTags(  frame -> planarXSize * 2, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

		for (p=0;p<sprite->frames[ image ].numberOfPlains;p++)
		{
			mask = 1<<p;

			chunkyRow = (unsigned char *) frame -> data;
			for (y=0;y<frame -> height;y++)
			{
				_retromode_retroRowToPlanar( Self, mask, chunkyRow, planar16bRow,  frame -> planarXSize, frame -> width );
				cust_fwrite( planar16bRow, frame -> planarXSize*2, 1, fd ) ;

				chunkyRow += frame -> bytesPerRow;
			}
		}
	}

	// in the doc it says 32 colors, but we are flexible.
	if (colors<32) colors = 32;	

	for (num = 0;num<colors;num++)
	{
		RGB32ColorToECS( sprite -> palette[num] , ECSColor );
		cust_fwrite( &ECSColor, 2, 1, fd );
	}

	if (planar16bRow) FreeVec(planar16bRow);
}

