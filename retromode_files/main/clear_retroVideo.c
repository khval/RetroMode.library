/* :ts=4
 *  $VER: clear_retroVideo.c $Revision$ (03-Oct-2017)
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

#include "libbase.h"

/****** retromode/main/clear_retroVideo ******************************************
*
*   NAME
*      clear_retroVideo -- Description
*
*   SYNOPSIS
*      void clear_retroVideo(struct retroVideo * video);
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

void _retromode_clear_retroVideo(struct retromodeIFace *Self,
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

