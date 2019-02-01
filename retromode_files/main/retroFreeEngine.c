/* :ts=4
 *  $VER: retroFreeEngine.c $Revision$ (30-Jan-2019)
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
#include "libbase.h"

/****** retromode/main/retroFreeEngine ******************************************
*
*   NAME
*      retroFreeEngine -- Description
*
*   SYNOPSIS
*      void retroFreeEngine(struct retroEngine * engine);
*
*   FUNCTION
*
*   INPUTS
*       engine - 
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

void _retromode_retroFreeEngine(struct RetroModeIFace *Self,
       struct retroEngine * engine)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;

	if (engine->rp.BitMap)
	{
		libBase-> IGraphics->FreeBitMap( engine-> rp.BitMap );
		engine -> rp.BitMap = NULL;
	}
}

