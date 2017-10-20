/* :ts=4
 *  $VER: retroClearVideo.c $Revision$ (16-Oct-2017)
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

/****** retromode/main/retroClearVideo ******************************************
*
*   NAME
*      retroClearVideo -- Description
*
*   SYNOPSIS
*      void retroClearVideo(struct retroVideo * video);
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

void _retromode_retroClearVideo(struct RetroModeIFace *Self,
       struct retroVideo * video)
{
	int size =  video->BytesPerRow * video -> height ;
	unsigned int *mem;

	size /= 4; // get number of ints.
	size /= 4; // unroll 4.

	mem = video -> Memory;

	while (size-- )
	{
		*mem++=0;
		*mem++=0;
		*mem++=0;
		*mem++=0;
	}
}

