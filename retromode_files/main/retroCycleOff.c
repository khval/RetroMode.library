/* :ts=4
 *  $VER: retroCycleOff.c $Revision$ (11-May-2018)
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

/****** retromode/main/retroCycleOff ******************************************
*
*   NAME
*      retroCycleOff -- Description
*
*   SYNOPSIS
*      void retroCycleOff(struct retroScreen * screen);
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

void _retromode_retroCycleOff(struct RetroModeIFace *Self, struct retroScreen * screen)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroShiftColors **shift;

	for (shift = screen -> allocatedShifts ; shift < screen -> allocatedShifts_end; shift ++)
	{
		if (*shift) IExec -> FreeVec(*shift);
		*shift = NULL;
	}

	screen -> shiftsAllocated = 0;
	screen -> allocatedShifts_end = screen -> allocatedShifts;
}

