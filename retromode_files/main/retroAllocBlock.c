/* :ts=4
 *  $VER: retroAllocBlock.c $Revision$ (07-May-2019)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2019 LiveForIt Software.
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
#include "libBase.h"

/****** retromode/main/retroAllocBlock ******************************************
*
*   NAME
*      retroAllocBlock -- Description
*
*   SYNOPSIS
*      struct retroBlock * retroAllocBlock(int width, int height);
*
*   FUNCTION
*
*   INPUTS
*       width - 
*       height - 
*
*   RESULT
*       The result ...
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

void _retromode_retroFreeBlock(struct RetroModeIFace *Self,  struct retroBlock * block);

struct retroBlock * _retromode_retroAllocBlock(struct RetroModeIFace *Self,
       int width,
       int height)
{
	struct retroBlock *block;

	block = (struct retroBlock *) AllocVecTags(  sizeof(struct retroBlock), AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

	if (block)
	{
		int _size_;
		block -> w = width;
		block -> h = height;
		block -> mem = NULL;

		_size_ = block -> w * block-> h;

		if (_size_)
		{
			int x,y;

			block -> mem = (unsigned char *) AllocVecTags(  _size_ , AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

			for (y=0;y<height;y++)
			{
				for ( x=0;x<width;x++) block -> mem[ y * width + x ] = y;
			}
		}

		if (block -> mem == NULL)
		{
			_retromode_retroFreeBlock( Self, block );
			block = NULL;
		}
	}

	return block;
}

