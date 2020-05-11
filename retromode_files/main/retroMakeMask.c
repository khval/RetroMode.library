/* :ts=4
 *  $VER: retroMakeMask.c $Revision$ (07-Dec-2019)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2019 LiveForIt Software.
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

/****** retromode/main/retroMakeMask ******************************************
*
*   NAME
*      retroMakeMask -- Description
*
*   SYNOPSIS
*      void retroMakeMask(struct retroFrameHeader * frame);
*
*   FUNCTION
*
*   INPUTS
*       frame - 
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

void _retromode_retroMakeMask(struct RetroModeIFace *Self,
       struct retroFrameHeader * frame)
{
	struct retroMask *mask;
	char *row;
	unsigned short *rowMask;
	int xpos,ypos;

	if (frame -> mask) IExec -> FreeVec( frame -> mask );
	frame -> mask = (struct retroMask *) IExec -> AllocVecTags( sizeof(struct retroMask), AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );
	if (frame -> mask == NULL) return;

	mask = frame -> mask;
	mask -> int16PerRow = (frame -> width >> 4) + ((frame -> width  & 15) ? 1 : 0);
	mask -> height = frame -> height;

	if (mask -> data) IExec -> FreeVec( mask -> data );
	mask -> data = (unsigned short *) IExec -> AllocVecTags( sizeof(uint16_t) * mask -> int16PerRow * mask -> height, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );
	if (mask -> data == NULL) return;

	for ( ypos = 0; ypos < frame -> height; ypos++ )
	{
		row = frame -> data + (frame -> bytesPerRow * ypos);
		rowMask = mask -> data + (mask -> int16PerRow * ypos );

		for ( xpos = 0;  xpos < frame -> width ; xpos++ )
		{
			if (row[xpos]) rowMask[xpos >> 4] |= 0x8000 >> (xpos & 15);
		}
	}
}

