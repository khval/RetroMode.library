/* :ts=4
 *  $VER: retroAllocDoubleBuffer.c $Revision$ (06-Jun-2018)
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

#include "libbase.h"

/****** retromode/main/retroAllocDoubleBuffer ******************************************
*
*   NAME
*      retroAllocDoubleBuffer -- Description
*
*   SYNOPSIS
*      void retroAllocDoubleBuffer(struct retroScreen * screen);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
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

void _retromode_retroAllocDoubleBuffer(struct RetroModeIFace *Self,  struct retroScreen * screen)
{

	if (screen -> Memory[1] == NULL )
	{ 
		int _size_ = screen -> bytesPerRow * (screen -> realHeight+1);
		unsigned char *src_ptr,*dest_ptr;
		unsigned char *mem_end;

		screen -> Memory[1] = (unsigned char *) AllocVecTags(  _size_ , AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

		src_ptr = screen -> Memory[0];
		dest_ptr=screen -> Memory[1];
		mem_end = screen -> Memory[1] + _size_;

		for(;dest_ptr<mem_end;dest_ptr++)
		{
			*dest_ptr	= *src_ptr; src_ptr++;
		}		
	}
}

