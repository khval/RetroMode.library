/* :ts=4
 *  $VER: retroCloseScreen.c $Revision$ (03-Oct-2017)
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

/****** retromode/main/retroCloseScreen ******************************************
*
*   NAME
*      retroCloseScreen -- Description
*
*   SYNOPSIS
*      void retroCloseScreen(struct retroScreen * screen);
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

void _retromode_retroCloseScreen(struct retromodeIFace *Self,
       struct retroScreen * screen)
{
	struct RetroLibrary *libBase = (struct _Library *) Self -> Data.LibBase;

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__);

	if (screen)
	{
		libBase -> IDOS -> Printf("size %ld,%ld\n",  screen->width,  screen->height);

		libBase -> IDOS -> Printf("screen %08lx\n",screen );
		libBase -> IDOS -> Printf("screen -> Memory %08lx\n",screen -> Memory );

		libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__ );
		
		if (screen -> Memory)  libBase -> IExec -> FreeVec(screen -> Memory);
		screen -> Memory = NULL;

		libBase -> IExec -> FreeVec(screen);
	}
}

