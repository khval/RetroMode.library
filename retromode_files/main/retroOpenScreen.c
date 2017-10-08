/* :ts=4
 *  $VER: retroOpenScreen.c $Revision$ (03-Oct-2017)
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

/****** retromode/main/retroOpenScreen ******************************************
*
*   NAME
*      retroOpenScreen -- Description
*
*   SYNOPSIS
*      struct retroScreen * retroOpenScreen(int width, int height);
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

struct retroScreen * _retromode_retroOpenScreen(struct RetroModeIFace *Self,
       int width,
       int height)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroScreen *screen;

	libBase -> IDOS -> Printf("%s:%ld - START\n",__FUNCTION__,__LINE__ );

	screen = (struct retroScreen *) libBase -> IExec -> AllocVecTags( 
						sizeof(struct retroScreen),  
						AVT_Type, MEMF_SHARED,
						AVT_ClearWithValue, 0 ,
						TAG_END	);

	if (screen)
	{
		screen -> width = width;
		screen -> height = height;
		screen -> Memory = (unsigned char *) libBase -> IExec -> AllocVecTags(  width * (height+1) ,
								AVT_Type, MEMF_SHARED,
								AVT_ClearWithValue, 0 ,
								TAG_END	);

		// if fail
		if (screen -> Memory == NULL)
		{

			libBase -> IDOS -> Printf("WTF: No screen memory\n");

			Self -> retroCloseScreen( screen );
			return NULL;
		}
	}
	libBase -> IDOS -> Printf("sizeof(struct retroScreen) %ld\n",  sizeof(struct retroScreen));


	libBase -> IDOS -> Printf("size %ld,%ld\n",  screen->width,  screen->height);

	libBase -> IDOS -> Printf("screen %08lx\n",screen );
	libBase -> IDOS -> Printf("screen -> Memory %08lx\n",screen -> Memory );

	libBase -> IDOS -> Printf("%s:%ld\n",__FUNCTION__,__LINE__ );

	return screen;
}

